#include<stdio.h>
#include "headers.h"
#include "echo.h"
#define BUFF_LEN 1024
extern int errno;

void mosh_echo(char** stor_list , int arg_num)
{
	for(int i=1;i<arg_num;i++)
	{
		printf("%s ", stor_list[i]);
	}

	printf("\n");
}