#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>

//完成加减乘除四种简单运算
int ADD(int, int);
int SUB(int, int);
int MUL(int, int);
int DIV(int, int);

void cacl1(int (*caclf)(int, int));