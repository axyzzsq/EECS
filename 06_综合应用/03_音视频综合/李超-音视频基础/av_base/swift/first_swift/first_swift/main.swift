//
//  main.swift
//  first_swift
//
//  Created by lichao on 2020/1/29.
//  Copyright © 2020年 lichao. All rights reserved.
//

import Foundation

let a = 555
var b = 456

if a < 500 {
    print("the a is less than 500")
}else {
    print("the a is more than 500")
}

switch a {
case 123:
    print("the value is 123")
case 456:
    print("the value is 456")
default:
    print("the value is ", a)
}

var list = [1,2,3,4,5,6]

for i in list {
    print(i)
}

for i in 0..<100 {
    print("for==", i)
}

var loop = 0
while loop < 100 {
    print("loop=", loop)
    loop = loop + 1
}

func myfunc(a : Int)->Int{
    var b: Int = 0
    b = a + 10
    return b
}

var aa = 20
print("Hello, World!", a, b, myfunc(a: aa))

