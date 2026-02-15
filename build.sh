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
# Pre-flight checks: verify required tools are available
# -----------------------------------------------------------------------------
check_tool() {
    if ! command -v "$1" &> /dev/null; then
        echo "Error: '$1' is not installed or not in PATH."
        echo "Please install $1 before running this script."
        exit 1
    fi
}

check_tool conan
check_tool cmake
check_tool make

# -----------------------------------------------------------------------------
build_core() {
    echo "Building zerr core library..."
    cd core || { echo "Failed to enter 'core' directory"; exit 1; }

    conan install . --output-folder=build --build=missing
    cd build || { echo "Failed to enter 'core/build' directory"; exit 1; }

    cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
    cmake --build .

    echo "Installing zerr_core library to local lib folder..."
    make install

    cd ../.. || exit 1
}

check_core_built() {
    if [ -d "core/lib" ]; then
        return 0
    else
        return 1
    fi
}

# -----------------------------------------------------------------------------
build_puredata() {
    echo "Building Zerr* for Pure Data..."
    cd puredata || { echo "Failed to enter 'puredata' directory"; exit 1; }

    conan install . --output-folder=build --build=missing

    make

    if [ "$install" = true ]; then
        echo "Installing Pure Data build..."
        make install
    fi

    cd .. || exit 1
}



# -----------------------------------------------------------------------------
build_maxmsp() {
    echo "Building Zerr* for Max/MSP..."
    cd maxmsp || { echo "Failed to enter 'maxmsp' directory"; exit 1; }

    conan install . --output-folder=build --build=missing

    cd build || { echo "Failed to enter 'maxmsp/build' directory"; exit 1; }

    cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
    cmake --build .

    if [ "$install" = true ]; then
        echo "Installing Max/MSP build..."
        make install
    fi

    cd ../.. || exit 1

}

# -----------------------------------------------------------------------------
build_jack() {
    echo "Building Zerr* for JACK..."
    cd jack || { echo "Failed to enter 'jack' directory"; exit 1; }

    meson setup builddir --wipe 2>/dev/null || meson setup builddir
    meson compile -C builddir

    if [ "$install" = true ]; then
        echo "Installing JACK build..."
        meson install -C builddir
    fi

    cd .. || exit 1
}


# -----------------------------------------------------------------------------
clean_core() {
    echo "Cleaning core build artifacts..."
    rm -rf core/build core/lib
    echo "Core cleaned."
}

clean_puredata() {
    echo "Cleaning PureData build artifacts..."
    rm -rf puredata/build
    cd puredata && make clean 2>/dev/null; cd ..
    echo "PureData cleaned."
}

clean_maxmsp() {
    echo "Cleaning Max/MSP build artifacts..."
    rm -rf maxmsp/build
    echo "Max/MSP cleaned."
}

clean_jack() {
    echo "Cleaning JACK build artifacts..."
    rm -rf jack/builddir
    echo "JACK cleaned."
}

# -----------------------------------------------------------------------------
if [ $# -eq 0 ]; then
    echo "No targets provided. Usage: $0 [-i] [-c] <core|puredata|maxmsp|jack>"
    exit 1
fi

for target in "$@"; do
    case $target in
        core|puredata|maxmsp|jack) ;;
        *)
            echo "Invalid target: $target. Valid options are: core, puredata, maxmsp, jack"
            exit 1
            ;;
    esac
done

# Clean or build the requested targets
for target in "$@"; do
    if [ "$clean" = true ]; then
        case $target in
            core)     clean_core ;;
            puredata) clean_puredata ;;
            maxmsp)   clean_maxmsp ;;
            jack)     clean_jack ;;
        esac
    else
        case $target in
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
