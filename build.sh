#!/bin/bash
set -e

# -----------------------------------------------------------------------------
# Usage: build.sh [-p|-i|-l] [-c] <deps|core|puredata|maxmsp|jack>...
#
#   (none)  build only
#   -p      package: stage a distributable in <target>/dist/ and zip it
#   -i      install by copy into the host application's user folder (implies -p)
#   -l      install by symlink to the staged package, for development (implies -p)
#   -c      clean the target's build output (and dist/) instead of building
#
# -p, -i and -l mean the same thing for every wrapper, so the Pd and Max targets
# are driven the same way; see package_<target>, install_copy and install_link.
# -----------------------------------------------------------------------------
usage() {
    cat <<USAGE
Usage: $0 [-p|-i|-l] [-c] <deps|core|puredata|maxmsp|jack>...

  (none)  build only
  -p      package: stage a distributable in <target>/dist/ and zip it
  -i      install by copy into the host app's user folder (implies -p)
            puredata -> \$PDLIBDIR/zerr        (pd-lib-builder's default: ~/Library/Pd on macOS)
            maxmsp   -> \$MAX_PACKAGES_DIR/Zerr (default: ~/Documents/Max 9/Packages)
  -l      install by symlink to the staged package, for development (implies -p)
  -c      clean the target's build output and dist/ instead of building
USAGE
}

package=false
install=false
link=false
clean=false

while getopts ":pilc" opt; do
  case ${opt} in
    p ) package=true ;;
    i ) install=true; package=true ;;
    l ) link=true;    package=true ;;
    c ) clean=true ;;
    \? )
      echo "Invalid Option: -$OPTARG" 1>&2
      usage 1>&2
      exit 1
      ;;
  esac
done
shift $((OPTIND -1))

if [ "$install" = true ] && [ "$link" = true ]; then
    echo "-i (install by copy) and -l (install by symlink) exclude each other." 1>&2
    exit 1
fi

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

    cd "$ROOT" || exit 1

    if [ "$package" = true ]; then
        package_puredata
    fi
    if [ "$install" = true ]; then
        install_copy "$ROOT/puredata/dist/zerr" "$(host_pd_dir)/zerr" puredata
    elif [ "$link" = true ]; then
        install_link "$ROOT/puredata/dist/zerr" "$(host_pd_dir)/zerr"
    fi
}

# Stages the Pd library the way `make install` lays it out (externals, help patches,
# circulation_8.yaml, zerr-meta.pd, LICENSE, README) under puredata/dist/zerr, then zips it.
package_puredata() {
    local dist="$ROOT/puredata/dist"
    echo "Assembling the Pure Data package..."
    rm -rf "$dist/zerr"
    ( cd "$ROOT/puredata" && "$MAKE" install PDLIBDIR="$dist" )

    local zip
    zip="zerr-pd-$(pd_version)-$(host_os)-$(host_arch).zip"
    rm -f "$dist"/*.zip   # also zips from a previous version, architecture or naming
    zip_dir "$dist" zerr "$zip"
    echo "Pure Data package assembled: $dist/zerr"
    echo "Pure Data package zipped:    $dist/$zip"
}

# Where Pd looks for user libraries, as pd-lib-builder resolves it for this host
# (~/Library/Pd on macOS, /usr/local/lib/pd-externals on Linux -- set PDLIBDIR to
# e.g. ~/.local/lib/pd/extra there to avoid needing root). PDLIBDIR in the
# environment wins, exactly as it does for `make install`.
host_pd_dir() {
    if [ -n "$PDLIBDIR" ]; then
        echo "$PDLIBDIR"
        return
    fi
    local dir
    dir="$(cd "$ROOT/puredata" && "$MAKE" -s vars 2>/dev/null | sed -n 's/^variable PDLIBDIR = //p')"
    if [ -z "$dir" ]; then
        echo "Could not determine Pd's library folder from pd-lib-builder; set PDLIBDIR." 1>&2
        exit 1
    fi
    echo "$dir"
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

    if [ "$package" = true ]; then
        package_maxmsp
    fi
    if [ "$install" = true ]; then
        install_copy "$ROOT/maxmsp/dist/Zerr" "$(host_max_dir)/Zerr" maxmsp
    elif [ "$link" = true ]; then
        install_link "$ROOT/maxmsp/dist/Zerr" "$(host_max_dir)/Zerr"
    fi
}

# Max's user package folder. MAX_PACKAGES_DIR in the environment wins; otherwise
# the newest "Max N/Packages" under ~/Documents.
host_max_dir() {
    if [ -n "$MAX_PACKAGES_DIR" ]; then
        echo "$MAX_PACKAGES_DIR"
        return
    fi
    local candidate
    for candidate in "$HOME/Documents/Max 9/Packages" "$HOME/Documents/Max 8/Packages"; do
        if [ -d "$candidate" ]; then
            echo "$candidate"
            return
        fi
    done
    echo "No Max Packages folder found under ~/Documents; set MAX_PACKAGES_DIR." 1>&2
    exit 1
}

# Assembles the Max package (maxmsp/dist/Zerr, see the install rules in
# maxmsp/CMakeLists.txt) and zips it the way a release would ship it. ditto keeps the
# bundle metadata and permissions that a plain zip of a .mxo can lose.
package_maxmsp() {
    local dist="$ROOT/maxmsp/dist"
    echo "Assembling the Max package..."
    cmake --install "$ROOT/maxmsp/build"

    local probe archs
    probe=$(ls -d "$dist"/Zerr/externals/*.mxo | head -n 1)
    archs=$(lipo -archs "$probe/Contents/MacOS/$(basename "$probe" .mxo)")
    case "$archs" in
        *" "*) archs=universal ;;
    esac
    # zerr-max-<ver>-macos-<arch>.zip, parallel to zerr-pd-...; the folder inside
    # stays Zerr/, the Max package name.
    local zip="zerr-max-$(max_version)-macos-${archs}.zip"

    rm -f "$dist"/*.zip   # also zips from a previous version, architecture or naming
    zip_dir "$dist" Zerr "$zip"
    echo "Max package assembled: $dist/Zerr"
    echo "Max package zipped:    $dist/$zip"
}

# -----------------------------------------------------------------------------
# Packaging and install helpers shared by the wrappers
# -----------------------------------------------------------------------------

# Each host carries its own version. Max: maxmsp/package-info.json (Max reads it,
# CMake compiles it in as ZERR_MAX_VERSION). Pd: puredata/zerr-meta.pd (the Pd libdir
# convention; puredata/Makefile compiles it in as ZERR_PD_VERSION). ci.yml reads both
# with the same two sed lines; keep the three copies identical.
max_version() {
    sed -n 's/.*"version" *: *"\([^"]*\)".*/\1/p' "$ROOT/maxmsp/package-info.json" | head -n 1
}

pd_version() {
    sed -n 's|^#X text [0-9][0-9]* [0-9][0-9]* VERSION \(.*\);|\1|p' "$ROOT/puredata/zerr-meta.pd" | head -n 1
}

host_os() {
    case "$(uname -s)" in
        Darwin)               echo "macos" ;;
        Linux)                echo "linux" ;;
        MINGW*|MSYS*|CYGWIN*) echo "windows" ;;
        *)                    uname -s | tr '[:upper:]' '[:lower:]' ;;
    esac
}

host_arch() {
    uname -m
}

# zip_dir <parent> <folder> <zip>: zips <parent>/<folder> as <parent>/<zip> with the
# folder as the top-level entry. ditto keeps the bundle permissions a plain zip of a
# .mxo can lose; --norsrc leaves out resource forks and extended attributes, which
# --sequesterRsrc would write as a __MACOSX/._* tree that Windows and Linux users see
# as junk (a code signature lives in _CodeSignature/, not in an attribute). Finder's
# .DS_Store files are dropped first. Elsewhere CMake's tar does the job.
zip_dir() {
    local parent="$1" folder="$2" zip="$3"
    find "$parent/$folder" -name .DS_Store -delete
    if command -v ditto &> /dev/null; then
        ( cd "$parent" && ditto -c -k --norsrc --keepParent "$folder" "$zip" )
    else
        ( cd "$parent" && cmake -E tar cf "$zip" --format=zip "$folder" )
    fi
}

# Does <dir> look like a Zerr package of the given kind? Guards the rm -rf in
# install_copy: we only ever replace something we put there.
is_zerr_package() {
    local dir="$1" kind="$2"
    case "$kind" in
        maxmsp)   grep -qi '"name" *: *"zerr"' "$dir/package-info.json" 2>/dev/null ;;
        puredata) ls "$dir"/zerr_*~.* >/dev/null 2>&1 ;;
        *)        return 1 ;;
    esac
}

# install_copy <staged> <destination> <kind>: copies the staged package into the
# host folder. A symlink there (a previous -l) is replaced; a real folder is
# replaced only if it is a Zerr package; anything else is left alone with an error.
install_copy() {
    local src="$1" dst="$2" kind="$3"
    mkdir -p "$(dirname "$dst")"
    if [ -L "$dst" ]; then
        rm "$dst"
    elif [ -d "$dst" ]; then
        if is_zerr_package "$dst" "$kind"; then
            rm -rf "$dst"
        else
            echo "$dst exists and does not look like a Zerr package; not replacing it." 1>&2
            exit 1
        fi
    elif [ -e "$dst" ]; then
        echo "$dst exists and is not a folder; not replacing it." 1>&2
        exit 1
    fi
    cp -R "$src" "$dst"
    echo "Installed (copy):    $dst"
}

# install_link <staged> <destination>: points the host folder at the staged
# package, so every later build is what the host loads (after a restart). Refuses
# to replace a real folder -- use -i, or remove it by hand -- so a copied install
# is never deleted by accident.
install_link() {
    local src="$1" dst="$2"
    mkdir -p "$(dirname "$dst")"
    if [ -L "$dst" ]; then
        rm "$dst"
    elif [ -e "$dst" ]; then
        echo "$dst exists and is not a symlink; remove it or use -i to replace it." 1>&2
        exit 1
    fi
    ln -s "$src" "$dst"
    echo "Installed (symlink): $dst -> $src"
}

# -----------------------------------------------------------------------------
build_jack() {
    echo "Building Zerr* for JACK..."
    cd "$ROOT/jack" || { echo "Failed to enter 'jack' directory"; exit 1; }

    meson setup builddir --wipe 2>/dev/null || meson setup builddir
    meson compile -C builddir

    if [ "$package" = true ] && [ "$install" != true ]; then
        echo "JACK has no package or symlink install; only -i (meson install) applies." 1>&2
    fi
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
    # puredata/build is a leftover from older trees (conan output now lives at the
    # repo root -- see clean_deps); puredata/dist is what -p stages.
    rm -rf "$ROOT/puredata/build" "$ROOT/puredata/dist"
    # Subshell so the cd cannot leak, and `|| true` because `set -e` would abort the
    # whole script -- skipping any remaining targets -- if `make clean` failed (it
    # needs the pd-lib-builder submodule checked out).
    ( cd "$ROOT/puredata" && "$MAKE" clean >/dev/null 2>&1 ) || true
    echo "PureData cleaned."
}

clean_maxmsp() {
    echo "Cleaning Max/MSP build artifacts..."
    rm -rf "$ROOT/maxmsp/build" "$ROOT/maxmsp/externals" "$ROOT/maxmsp/dist"
    # Hand-copied bundles beside the help patches shadow fresh builds; the
    # installed package in maxmsp/dist/Zerr is the place to test from.
    rm -rf "$ROOT"/maxmsp/help/*.mxo
    echo "Max/MSP cleaned."
}

clean_jack() {
    echo "Cleaning JACK build artifacts..."
    rm -rf "$ROOT/jack/builddir"
    echo "JACK cleaned."
}

# -----------------------------------------------------------------------------
if [ $# -eq 0 ]; then
    echo "No targets provided." 1>&2
    usage 1>&2
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
