#!/bin/sh
#select条件语句
echo "What do you want to study?"
select subject in "Android" "Java" "C++" "IOS"
do
    echo "You have selected $subject."
	break
done
exit 0

