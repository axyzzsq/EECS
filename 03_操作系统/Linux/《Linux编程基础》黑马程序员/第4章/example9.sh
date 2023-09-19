#!/bin/sh
for file in ~/itheima/*.bxg
do 
    rm $file
    echo "$file has been deleted."
done
exit 0


