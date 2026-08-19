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

# -----------------------------------------------------------------------------
# Host platform
# -----------------------------------------------------------------------------
# "Windows" here always means an MSYS2 / MinGW64 / Git Bash shell -- that is the
# only way a bash script runs on Windows, and it is what profiles/mingw targets.
is_windows() {
    case "$(uname -s)" in
        MINGW*|MSYS*|CYGWIN*) return 0 ;;
        *)                    return 1 ;;
    esac
}

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

# The MinGW-w64 toolchain ships its make as `mingw32-make`. MSYS2 additionally
# provides a plain `make` built against the MSYS runtime, which is *not* the one
# that pairs with the mingw compilers. The PureData Windows CI job runs
# mingw32-make; prefer the same binary, and fall back to `make` where it is all
# that exists.
detect_make() {
    if is_windows && command -v mingw32-make &> /dev/null; then
        echo "mingw32-make"
    else
        echo "make"
    fi
}

MAKE="$(detect_make)"
check_tool "$MAKE"

# CMake on Windows defaults to a Visual Studio generator whenever one is installed,
# which would compile with MSVC against the gcc/libstdc++ toolchain profiles/mingw
# resolves -- objects that cannot link together. The core CI job pins the generator
# for exactly this reason; match it rather than depending on what happens to be
# installed on the machine.
CMAKE_GEN_ARGS=()
CMAKE_GENERATOR_NAME=""
if is_windows; then
    CMAKE_GENERATOR_NAME="MinGW Makefiles"
    CMAKE_GEN_ARGS=(-G "$CMAKE_GENERATOR_NAME")
fi

# What CMake records in CMakeCache.txt is not the string we handed it. Under
# MSYS2 / Git Bash, $ROOT comes out of `pwd` as /c/Users/..., and the MSYS layer
# rewrites the argument to the native C:/Users/... on its way to a native CMake --
# so comparing the cached CMAKE_TOOLCHAIN_FILE against $TOOLCHAIN verbatim can
# never match, and every single run would discard the build tree and rebuild the
# core. Keep $TOOLCHAIN in POSIX form (bash still has to stat it) and compare
# against the native spelling instead.
#
# If cygpath is somehow absent this degrades to the over-eager discard rather than
# to a wrong build, which is the right way round to fail.
TOOLCHAIN_CACHE="$TOOLCHAIN"
if is_windows && command -v cygpath &> /dev/null; then
    TOOLCHAIN_CACHE="$(cygpath -m "$TOOLCHAIN")"
fi

# A Max external on Windows is a .mxe64, which Max loads with the MSVC runtime.
# profiles/mingw resolves gcc/libstdc++11 packages and builds a libzerr_core.a to
# match, and the two ABIs cannot be linked together -- so there is no way to
# produce a working .mxe64 from this script's single conan resolve.
#
# Checked up front, over the whole target list, rather than inside build_maxmsp:
# the dispatcher builds the core library before it calls the per-target function,
# so a guard down there would resolve dependencies and compile the entire core
# before announcing that it cannot proceed.
assert_target_supported() {
    if [ "$1" = "maxmsp" ] && is_windows; then
        echo "Max/MSP is not buildable on Windows from this script."
        echo "  A .mxe64 requires an MSVC toolchain and a matching conan profile;"
        echo "  profiles/mingw produces gcc/libstdc++ objects that cannot link into one."
        echo "  See docs/design/dependency-fallbacks.md section 3.7."
        exit 1
    fi
}

# -----------------------------------------------------------------------------
# Dependencies (conan, once, at the root)
# -----------------------------------------------------------------------------

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
        local cached generator
        cached="$(sed -n 's/^CMAKE_TOOLCHAIN_FILE:[^=]*=//p' "$build_dir/CMakeCache.txt" | head -1)"
        generator="$(sed -n 's/^CMAKE_GENERATOR:[^=]*=//p' "$build_dir/CMakeCache.txt" | head -1)"
        if [ "$cached" != "$TOOLCHAIN_CACHE" ]; then
            echo "it was configured with ${cached:-<no toolchain>}"
        elif [ -n "$CMAKE_GENERATOR_NAME" ] && [ "$generator" != "$CMAKE_GENERATOR_NAME" ]; then
            # Passing -G to a build directory whose cache names a different generator
            # is a hard CMake error, not a warning. The case that matters: a cache
            # written before this script pinned "MinGW Makefiles", when CMake picked
            # Visual Studio on its own. Discard it like any other disagreement rather
            # than making the user delete it by hand.
            echo "it was configured with the ${generator:-<unknown>} generator"
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
    cmake -S "$ROOT/core" -B "$ROOT/core/build" "${CMAKE_GEN_ARGS[@]}" \
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

    "$MAKE"

    if [ "$install" = true ]; then
        echo "Installing Pure Data build..."
        "$MAKE" install
    fi

    cd "$ROOT" || exit 1
}



# -----------------------------------------------------------------------------
build_maxmsp() {
    # Windows is rejected by assert_target_supported before any build starts.
    ensure_deps
    reconfigure_if_stale "$ROOT/maxmsp/build"

    echo "Building Zerr* for Max/MSP..."
    cmake -S "$ROOT/maxmsp" -B "$ROOT/maxmsp/build" "${CMAKE_GEN_ARGS[@]}" \
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
    ( cd "$ROOT/puredata" && "$MAKE" clean >/dev/null 2>&1 ) || true
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
    # Cleaning an unsupported target is still fine -- it is only ever an rm -rf.
    [ "$clean" = true ] || assert_target_supported "$target"
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
