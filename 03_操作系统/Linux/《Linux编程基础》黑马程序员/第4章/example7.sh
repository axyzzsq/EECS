#!/bin/sh
echo -e "a:\c"
read a
echo -e "b:\c"
read b
echo -e "select(+ - * /):\c"
read var 
case $var in
    '+') echo "a+b="`expr $a "+" $b`;;
	"-") echo "a-b="`expr $a "-" $b`;;
	"*") echo "a*b="`expr $a "*" $b`;;
	"/") echo "a/b="`expr $a "/" $b`;;
	*) echo "error"
esac
exit 0
