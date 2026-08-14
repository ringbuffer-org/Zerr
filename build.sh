#!/bin/bash
set -e

install=false
clean=false

while getopts ":ic" opt; do
  case ${opt} in
    i )
      install=true
      ;;
    c )
      clean=true
      ;;
    \? )
      echo "Invalid Option: -$OPTARG" 1>&2
      exit 1
      ;;
  esac
done
shift $((OPTIND -1))

# -----------------------------------------------------------------------------
# Paths. Dependencies are resolved once at the repo root (see /conanfile.txt) and
# shared by every target, so all of them link against one identical profile.
# -----------------------------------------------------------------------------
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CONAN_DIR="$ROOT/build"
TOOLCHAIN="$CONAN_DIR/conan_toolchain.cmake"

# -----------------------------------------------------------------------------
# Pre-flight checks: verify required tools are available
# -----------------------------------------------------------------------------
check_tool() {
    if ! command -v "$1" &> /dev/null; then
        echo "Error: '$1' is not installed or not in PATH."
        echo "Please install $1 before running this script."
        exit 1
    fi
}

check_tool cmake
check_tool make

# -----------------------------------------------------------------------------
# Dependencies (conan, once, at the root)
# -----------------------------------------------------------------------------
# Pick the committed profile matching the host. Each pins the settings that must
# agree between zerr_core and the wrappers; see profiles/ for the rationale.
detect_profile() {
    case "$(uname -s)" in
        Darwin)               echo "profiles/macos" ;;
        Linux)                echo "profiles/linux" ;;
        MINGW*|MSYS*|CYGWIN*) echo "profiles/mingw" ;;
        *)                    echo "default" ;;
    esac
}

# Existence is not freshness. A git pull that bumps the pinned recipe revisions in
# conanfile.txt -- the whole reason those revisions are pinned there -- leaves a
# perfectly well-formed but outdated build/ behind, and every target would keep
# linking the previously resolved packages without a word. Re-resolve whenever an
# input is newer than the output conan generated from it.
deps_installed() {
    [ -f "$TOOLCHAIN" ] && [ -f "$CONAN_DIR/conandeps.mk" ] || return 1

    local input
    for input in "$ROOT/conanfile.txt" "$ROOT/$(detect_profile)"; do
        if [ -f "$input" ] && [ "$input" -nt "$TOOLCHAIN" ]; then
            echo "$input is newer than the resolved dependencies."
            return 1
        fi
    done

    return 0
}

install_deps() {
    # conan is only required when dependencies actually need resolving, so a tree
    # with a populated build/ folder still builds on a machine without conan.
    check_tool conan

    local profile
    profile="$(detect_profile)"

    echo "Resolving dependencies with conan (profile: $profile)..."
    cd "$ROOT" || exit 1
    conan install . --output-folder=build --build=missing \
        -pr:h="$profile" -pr:b="$profile"
}

ensure_deps() {
    if deps_installed; then
        echo "Conan dependencies already present in $CONAN_DIR. Skipping resolve."
    else
        install_deps
    fi
}

# A CMakeCache left over from an earlier layout silently ignores a newly passed
# CMAKE_TOOLCHAIN_FILE -- CMake only warns "Manually-specified variables were not
# used by the project" and keeps using the cached one. The shared root toolchain,
# and the deployment target it pins, would then never take effect. Drop the cache
# whenever it disagrees with the toolchain we intend to build against.
stale_cache_reason() {
    local build_dir="$1"

    if [ -f "$build_dir/conan_toolchain.cmake" ]; then
        # Pre-consolidation tree: conan used to generate into each target's own
        # build folder, so this cache has that older toolchain baked in.
        echo "it holds a pre-consolidation conan output"
    elif [ -f "$build_dir/CMakeCache.txt" ]; then
        local cached
        cached="$(sed -n 's/^CMAKE_TOOLCHAIN_FILE:[^=]*=//p' "$build_dir/CMakeCache.txt" | head -1)"
        if [ "$cached" != "$TOOLCHAIN" ]; then
            echo "it was configured with ${cached:-<no toolchain>}"
        elif [ -f "$TOOLCHAIN" ] && [ "$TOOLCHAIN" -nt "$build_dir/CMakeCache.txt" ]; then
            # The toolchain's *contents* changed since this cache was written -- a
            # re-resolve with different profile settings, for instance. The toolchain
            # assigns with set(... CACHE ...), which never overrides an existing
            # entry, so an older cache silently wins: that is how a newly pinned
            # CMAKE_OSX_DEPLOYMENT_TARGET stayed empty and left objects at the
            # host SDK. Path equality is not enough; compare freshness too.
            echo "the toolchain is newer than this cache"
        fi
    fi
}

reconfigure_if_stale() {
    local build_dir="$1"
    [ -d "$build_dir" ] || return 0

    local reason
    reason="$(stale_cache_reason "$build_dir")"
    [ -n "$reason" ] || return 0

    echo "Discarding $build_dir: $reason."
    echo "  (generated output only — reconfiguring against $TOOLCHAIN)"
    rm -rf "$build_dir"
}

# -----------------------------------------------------------------------------
build_core() {
    ensure_deps
    reconfigure_if_stale "$ROOT/core/build"

    echo "Building zerr core library..."
    cmake -S "$ROOT/core" -B "$ROOT/core/build" \
        -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN" \
        -DCMAKE_BUILD_TYPE=Release
    cmake --build "$ROOT/core/build"

    echo "Installing zerr_core library to local lib folder..."
    cmake --install "$ROOT/core/build"
}

# What a wrapper actually links is core/lib/libzerr_core.a, so "core is already
# built" has to mean "built against the toolchain this wrapper is about to be
# configured with". Existence alone is not enough: after a re-resolve,
# reconfigure_if_stale drops the wrapper's cache so the wrapper picks up the new
# toolchain, while a surviving archive still carries the old settings. That is the
# very core/wrapper mismatch (deployment target, libcxx, arch) the shared toolchain
# exists to prevent -- it would just move from configure time to link time.
check_core_built() {
    local lib="$ROOT/core/lib/libzerr_core.a"
    [ -f "$lib" ] || return 1

    # Same freshness test reconfigure_if_stale applies to a CMakeCache.
    if [ -f "$TOOLCHAIN" ] && [ "$TOOLCHAIN" -nt "$lib" ]; then
        echo "Installed core library predates $TOOLCHAIN."
        return 1
    fi

    # The build tree that produced the archive would itself be discarded now, so
    # whatever it installed was configured against something we no longer accept.
    local reason
    reason="$(stale_cache_reason "$ROOT/core/build")"
    if [ -n "$reason" ]; then
        echo "Installed core library came from a build tree to be discarded: $reason."
        return 1
    fi

    return 0
}

# -----------------------------------------------------------------------------
build_puredata() {
    ensure_deps

    echo "Building Zerr* for Pure Data..."
    cd "$ROOT/puredata" || { echo "Failed to enter 'puredata' directory"; exit 1; }

    make

    if [ "$install" = true ]; then
        echo "Installing Pure Data build..."
        make install
    fi

    cd "$ROOT" || exit 1
}



# -----------------------------------------------------------------------------
build_maxmsp() {
    ensure_deps
    reconfigure_if_stale "$ROOT/maxmsp/build"

    echo "Building Zerr* for Max/MSP..."
    cmake -S "$ROOT/maxmsp" -B "$ROOT/maxmsp/build" \
        -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN" \
        -DCMAKE_BUILD_TYPE=Release
    cmake --build "$ROOT/maxmsp/build"

    if [ "$install" = true ]; then
        echo "Installing Max/MSP build..."
        cmake --build "$ROOT/maxmsp/build" --target install
    fi
}

# -----------------------------------------------------------------------------
build_jack() {
    echo "Building Zerr* for JACK..."
    cd "$ROOT/jack" || { echo "Failed to enter 'jack' directory"; exit 1; }

    meson setup builddir --wipe 2>/dev/null || meson setup builddir
    meson compile -C builddir

    if [ "$install" = true ]; then
        echo "Installing JACK build..."
        meson install -C builddir
    fi

    cd "$ROOT" || exit 1
}


# -----------------------------------------------------------------------------
clean_deps() {
    echo "Cleaning resolved dependencies..."
    rm -rf "$CONAN_DIR"
    echo "Dependencies cleaned."
}

clean_core() {
    echo "Cleaning core build artifacts..."
    rm -rf "$ROOT/core/build" "$ROOT/core/lib"
    echo "Core cleaned."
}

clean_puredata() {
    echo "Cleaning PureData build artifacts..."
    # puredata/build is now only an install destination (PDLIBDIR); the conan
    # output it used to hold lives at the repo root -- see clean_deps.
    rm -rf "$ROOT/puredata/build"
    # Subshell so the cd cannot leak, and `|| true` because `set -e` would abort the
    # whole script -- skipping any remaining targets -- if `make clean` failed (it
    # needs the pd-lib-builder submodule checked out).
    ( cd "$ROOT/puredata" && make clean >/dev/null 2>&1 ) || true
    echo "PureData cleaned."
}

clean_maxmsp() {
    echo "Cleaning Max/MSP build artifacts..."
    rm -rf "$ROOT/maxmsp/build"
    echo "Max/MSP cleaned."
}

clean_jack() {
    echo "Cleaning JACK build artifacts..."
    rm -rf "$ROOT/jack/builddir"
    echo "JACK cleaned."
}

# -----------------------------------------------------------------------------
if [ $# -eq 0 ]; then
    echo "No targets provided. Usage: $0 [-i] [-c] <deps|core|puredata|maxmsp|jack>"
    exit 1
fi

for target in "$@"; do
    case $target in
        deps|core|puredata|maxmsp|jack) ;;
        *)
            echo "Invalid target: $target. Valid options are: deps, core, puredata, maxmsp, jack"
            exit 1
            ;;
    esac
done

# Clean or build the requested targets
for target in "$@"; do
    if [ "$clean" = true ]; then
        case $target in
            deps)     clean_deps ;;
            core)     clean_core ;;
            puredata) clean_puredata ;;
            maxmsp)   clean_maxmsp ;;
            jack)     clean_jack ;;
        esac
    else
        case $target in
            deps)
                install_deps
                ;;
            core)
                build_core
                ;;
            puredata|maxmsp|jack)
                if check_core_built; then
                    echo "Core already built. Skipping core build."
                else
                    build_core
                fi
                case $target in
                    puredata) build_puredata ;;
                    maxmsp)   build_maxmsp ;;
                    jack)     build_jack ;;
                esac
                ;;
        esac
    fi
done
