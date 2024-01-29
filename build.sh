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

find $(pwd)/ -type f -name "*.o" -delete
find $(pwd)/ -type f -name $pd_file_ext -delete

main_path=$(pwd)

cd puredata/zerr_disperser
make -f Makefile
cd $main_path

cd puredata/zerr_combinator
make -f Makefile
cd $main_path

cd puredata/zerr_envelopes
make -f Makefile
cd $main_path

cd puredata/zerr_features
make -f Makefile
cd $main_path

echo "---- Build successful!"

find $(pwd)/ -type f -name "*.o" -delete
find $(pwd)/ -type f -name $pd_file_ext -exec mv {} externals/ \;

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
