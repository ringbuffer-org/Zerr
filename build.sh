#!/bin/bash
set -e

find $(pwd)/ -type f -name "*.o" -delete
find $(pwd)/ -type f -name "*.pd_darwin" -delete

main_path=$(pwd)
cd puredata/zerr_audio_disperser
make -f Makefile

cd $main_path
cd puredata/zerr_envelope_combinator
make -f Makefile

cd $main_path
cd puredata/zerr_envelope_generator
make -f Makefile

cd $main_path
cd puredata/zerr_feature_tracker
make -f Makefile

echo "---- Build successful!"
cd $main_path

find $(pwd)/ -type f -name "*.o" -delete
find $(pwd)/ -type f -name "*.pd_darwin" -exec cp {} externals/ \;

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
helpfile="externals/zerr~_helper.pd"
open ${helpfile}