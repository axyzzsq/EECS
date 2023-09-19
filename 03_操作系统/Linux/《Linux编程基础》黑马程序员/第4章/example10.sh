#!/bin/sh
count=1
sum=0
while [ $count -le 100 ]
do
    sum=`expr $sum + $count`
    count=`expr $count  + 1`
done
echo "sum=$sum"
exit 0

