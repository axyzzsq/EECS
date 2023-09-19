#!/bin/sh
#until
i=1
until [ $i -gt 3 ] 
do
    echo "the number is $i."
    i=`expr $i + 1`
done
exit 0

