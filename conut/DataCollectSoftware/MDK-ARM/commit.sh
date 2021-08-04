#!/bin/sh
header="#include \"pl_type.h\""
file="../User/config/commit.c"

echo $header > $file
export commit=$(git rev-parse HEAD)

commit="U8 commit[]={\"$commit\"};"
echo -e >> $file
echo -e $commit >> $file