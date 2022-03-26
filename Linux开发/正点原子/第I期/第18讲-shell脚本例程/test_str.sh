#!/bin/bash
echo "input two string"
read -p "firststr:" firststr
read -p "secondstr:" secondstr
test $firststr == $secondstr && echo "$firststr = $secondstr" || echo "$firststr != $secondstr"
