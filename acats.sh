#!/bin/bash

OUTPATH=testcases/acats
INPATH=${OUTPATH}_raw

# Download ACATS.
wget http://www.ada-auth.org/acats-files/2.6/tars/acats_26.tar.Z -P testcases
mkdir -p "$INPATH"
uncompress testcases/acats_26.tar.Z
tar -xvf testcases/acats_26.tar -C "$INPATH"
rm testcases/acats_26.tar

# Tests must be converted for use by GNAT using gnatchop.
for file in $(find "$INPATH" -type f); do
    (
      OUT=$(dirname $OUTPATH/$(realpath --relative-to="$INPATH" "$file"))
      mkdir -p "$OUT"
      $GNAT_HOME/bin/gnatchop "$file" "$OUT"
    ) &
done

wait
rm -rf "$INPATH"
echo "ACATS downloaded into testcases/acats"