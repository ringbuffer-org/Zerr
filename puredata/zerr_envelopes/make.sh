#!/bin/bash
set -e

kernel_name=$(uname -s)

case "$kernel_name" in
    Linux*)
        echo "Linux"
        pd_file_ext="*.pd_linux"
        ;;
    Darwin*)
        echo "macOS"
        pd_file_ext="*.pd_darwin"
        ;;
    CYGWIN* | MINGW* | MSYS*)
        echo "Windows"
        pd_file_ext="*.pd_dll"
        ;;
    *)
        echo "Unknown"
        ;;
esac

echo $pd_file_ext

mkdir -p builddir
make

echo "---- Build successful!"
echo "---- Continuing with the rest of the script."


cp $pd_file_ext ../../externals/
cp *.pd         ../../externals/


pd_name="Pd"
pd_pgid="$(pgrep ${pd_name})"

# Check if the application is running
if [ -z ${pd_pgid} ]; then
  echo "---- ${pd_name} is not running."
else
  echo "---- ${pd_name} is running with PGID: ${pd_pgid}. Closing it..."
  kill -9 "-${pd_pgid}"
  echo "---- Closed ${pd_name} successfully."
fi

# open object test patch
helpfile="zerr_envelopes~_helper.pd"
open ${helpfile}