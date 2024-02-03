#!/bin/bash

build_puredata() {
    echo "Building Zerr* for Pure Data..."
    cd puredata
    make
    if [ "$install" = true ]; then
        echo "Installing Pure Data build..."
        make install
    fi
}

build_jack() {
    echo "Building Zerr* for JACK...(placeholder)"
}

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

if [ $# -eq 0 ]; then
    echo "No arguments provided. Please specify a target to build."
    exit 1
fi

case $1 in
    puredata)
        build_puredata
        ;;
    jack)
        build_jack
        ;;
    *)
        echo "Invalid argument: $1. Please specify 'puredata' or 'jack'."
        exit 1
        ;;
esac
