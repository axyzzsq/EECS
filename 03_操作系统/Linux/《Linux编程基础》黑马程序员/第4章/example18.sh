#!/bin/sh
function fun(){
    local a=10
    echo "fun:a=$a"
}
a=5
echo "main:a=$a"
fun             #函数调用
echo "main:a=$a"
exit 0

