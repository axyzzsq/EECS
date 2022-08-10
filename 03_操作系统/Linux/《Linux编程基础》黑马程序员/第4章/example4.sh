#!/bin/sh
#双分支if语句
read filename
if [ -d $filename ]; then
    echo $filename" is a directory"
else
    echo $filename" is't a directory"
fi
exit 0

