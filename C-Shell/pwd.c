#include<stdio.h>
#include "headers.h"
#include "global_var.h"
#include "pwd.h"
#define BUFF_LEN 1024
extern int errno;

//Function to implement pwd
void mosh_pwd(int arg_num)
{
	if(arg_num > 1)
	{
		fprintf(stderr, "shell: pwd: too many arguments\n");
		return;
	}
	char wd[BUFF_LEN];
	if(!(getcwd(wd , sizeof(wd)+1)))
	{
		perror("shell: ");
		sad = 1;
		return;
	}

	printf("%s\n",wd);
}