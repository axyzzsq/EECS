//
//  main.c
//  func
//
//  Created by lichao on 2020/1/19.
//  Copyright © 2020年 lichao. All rights reserved.
//

#include <stdio.h>

int func(int a){
    printf("a=%d\n", a);
    return 0;
}

int func1(int b){
    printf("b=%d\n", b + 5);
    return 0;
}

int main(int argc, const char * argv[]) {

    int (*f)(int);
    
    f = func;
    f(2);
    
    f = func1;
    f(3);
    //func(2);
    //func1(3);
    
    return 0;
}
