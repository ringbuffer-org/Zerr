#!/bin/bash
set -e

install=false

while getopts ":i" opt; do
  case ${opt} in
    i )
      install=true
      ;;
    \? )
      echo "Invalid Option: -$OPTARG" 1>&2
      exit 1
      ;;
  esac
done
shift $((OPTIND -1))

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

    echo "Installing zerr_core library to local lib folder..."
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
    cd maxmsp || { echo "Failed to enter 'puredata' directory"; exit 1; }

    conan install . --output-folder=build --build=missing

    cd build || { echo "Failed to enter 'maxmsp/build' directory"; exit 1; }

    cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
    cmake --build .

    echo "Installing zerr_core library to local lib folder..."
    make install

    cd ../.. || exit 1

}

# -----------------------------------------------------------------------------
build_jack() {
    echo "Building Zerr* for JACK... (placeholder)"
    # Placeholder for JACK build
}


# -----------------------------------------------------------------------------
if [ $# -eq 0 ]; then
    echo "No targets provided. Usage: $0 [-i] <puredata|jack|...>"
    exit 1
fi

# Build core if needed
if check_core_built; then
    echo "Core already built. Skipping core build."
else
    build_core
fi

# Build the requested targets
for target in "$@"; do
    case $target in
        puredata)
            build_puredata
            ;;
        maxmsp)
            build_maxmsp
            ;;
        jack)
            build_jack
            ;;
        *)
            echo "Invalid target: $target. Valid options are: puredata, jack"
            exit 1
            ;;
    esac
done