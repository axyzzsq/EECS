#!/bin/sh
#单分支if语句
read filename
if [ -d $filename ]; then
    echo $filename" is a directory"
fi
exit 0
