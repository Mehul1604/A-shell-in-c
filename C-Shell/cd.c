#include<stdio.h>
#include "headers.h"
#include "global_var.h"
#include "cd.h"
#define BUFF_LEN 1024
extern int errno;

// Function to implement cd. The chdir() function is used
void mosh_cd(char** stor_list , int arg_num , char* home_dir)
{
	int dir_state;

	if(arg_num == 1)
	{
		chdir(home_dir);
		strcpy(prev_dir , cwd);
	}
	else
	{
		if(arg_num > 2)
		{
			fprintf(stderr , "shell: cd: too many arguments\n");
			return;
		}
		if(!strcmp(".." , stor_list[1]))
		{
			chdir("..");
			strcpy(prev_dir , cwd);
		}
		else if(!strcmp("." , stor_list[1]))
		{
			chdir(".");
			strcpy(prev_dir , cwd);
		}
		else if(!strcmp("~" , stor_list[1]))
		{
			chdir(home_dir);
			strcpy(prev_dir , cwd);
		}
		else if(!strcmp("-" , stor_list[1]))
		{
			printf("%s\n" , prev_dir);
			chdir(prev_dir);
			strcpy(prev_dir , cwd);
		}
		else
		{
			dir_state = chdir(stor_list[1]);
			if(dir_state == -1)
			{
				perror("shell");
				sad = 1;
				return;
				
			}

			strcpy(prev_dir , cwd);
		}
	}
}