#!/bin/bash
set -e

find $(pwd)/ -type f -name "*.o" -delete
find $(pwd)/ -type f -name "*.pd_darwin" -delete

mkdir -p builddir
make
# if [ $? -eq 2 ]; then
#   echo "---- make failed, exist"
#   exist 0
# else
  echo "---- Build successful!"
  echo "---- Continuing with the rest of the script."
# fi



find $(pwd)/ -type f -name "*.o" -delete

cp *.pd_darwin ../../externals/
cp *.pd        ../../externals/

mv *.pd_darwin builddir 
cp *.pd        builddir
cp *.yaml      builddir

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
helpfile="zerr_envelope_generator~_helper.pd"
open builddir/${helpfile}

find "$(pwd)" -name ".DS_Store" -type f -delete
echo "---- All .DS_Store files have been deleted."