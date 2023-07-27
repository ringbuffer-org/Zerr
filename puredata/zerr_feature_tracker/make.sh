#!/bin/bash

mkdir -p builddir
make

mv *.pd_darwin builddir 
cp *.pd        builddir

pd_name="Pd"
pd_pgid="$(pgrep ${pd_name})"

# Check if Pd is running
if [ -z ${pd_pgid} ]; then
  echo "${pd_name} is not running."
else
  echo "${pd_name} is running with PGID: ${pd_pgid}. Closing it..."
  kill -9 "-${pd_pgid}"
  echo "Close ${pd_pgid} successfully."
fi

echo "Opening zerr_feature_tracker~_helper.pd..."
helpfile="zerr_feature_tracker~_helper.pd"
open builddir/${helpfile}
echo "Done!"
