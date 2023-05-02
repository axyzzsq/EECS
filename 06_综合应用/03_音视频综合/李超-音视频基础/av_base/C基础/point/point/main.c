//
//  main.c
//  point
//
//  Created by lichao on 2020/1/18.
//  Copyright © 2020年 lichao. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char * argv[]) {
 
    char* p = (char*)malloc(10);
    *p = 'a';
    *(p+1) = 'b';
    *(p+2) = 'c';
    free(p);
    p = NULL;
    *p='d';
    printf("%s\n", p);
    
    return 0;
}
