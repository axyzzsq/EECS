#!/bin/sh
#function
function _choice(){
    echo "Your chioce is $1."
}
#main
case $1 in
    "C++" ) _choice C++ ;;
    "Android" ) _choice Android ;;
    "Python" ) _choice Python;;
    *) echo "$0:please select in (C++/Android/Python)"
esac
exit 0

