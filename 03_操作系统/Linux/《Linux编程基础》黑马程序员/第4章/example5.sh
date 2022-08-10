#!/bin/sh
read filename
if [ -d $filename ]; then
    ls $filename
elif [ -x $filename ]; then
    echo "This is a executable file."
else
    echo "This is not a executable file."
fi
exit 0

