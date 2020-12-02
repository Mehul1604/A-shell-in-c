#include<stdio.h>
#include "headers.h"
#include "redirect.h"
#include "read_func.h"
#include "global_var.h"
#define BUFF_LEN 1024
extern int errno;

int check_redir(char** stor_list , int arg_num)
{
	
	
	int out_red = 0 , in_red = 0 , appnd = 0;
	char out_file[256];
	char in_file[256];
	int found = arg_num;
	for(int i=0;i<arg_num;i++)
	{
		if(!strcmp(stor_list[i] , ">") || !strcmp(stor_list[i] , ">>"))
		{
			if(i == arg_num-1 || !strcmp(stor_list[i+1] , ">") || !strcmp(stor_list[i+1] , "<") || !strcmp(stor_list[i+1] , ">>"))
			{
				printf("shell: invalid redirection\n");
				sad = 1;
				return 0;
			}

			out_red = 1;
			int fd_out;
			if(!strcmp(stor_list[i] , ">"))
			{
				appnd = 0;
				fd_out = open(stor_list[i+1] , O_WRONLY | O_TRUNC | O_CREAT , 0644);
			}
			else if(!strcmp(stor_list[i] , ">>"))
			{
				appnd = 1;
				fd_out = open(stor_list[i+1] , O_WRONLY |  O_CREAT | O_APPEND , 0644);

			}
			if(fd_out < 0)
			{
				perror("shell: ");
				sad = 1;
				return 0;
			}
			strcpy(out_file , stor_list[i+1]);
			if(found == arg_num)
				found = i;

			i++;
			
		}

		if(!strcmp(stor_list[i] , "<"))
		{
			if(i == arg_num-1 || !strcmp(stor_list[i+1] , "<") || !strcmp(stor_list[i+1] , ">") || !strcmp(stor_list[i+1] , ">>"))
			{
				printf("shell: invalid redirection\n");
				sad = 1;
				return 0;
			}

			in_red = 1;

			int fd_in = open(stor_list[i+1] , O_RDONLY);
			if(fd_in < 0)
			{
				perror("shell: ");
				sad = 1;
				return 0;
			}
			strcpy(in_file , stor_list[i+1]);
			if(found == arg_num)
				found = i;

			i++;
			
		}
	}

	if(!(out_red || in_red))
	{
		return 1;
	}

	int b_flag = 0;
	if(arg_num > 1 && !strcmp("&" , stor_list[arg_num-1])) //checking if background
	{
		b_flag = 1;
		stor_list[arg_num-1] = '\0';
		arg_num--;
		
	}

	int frk = fork();
	if(frk == -1)
	{
		printf("shell: forking error\n");
		sad = 1;
		return 0;
	}

	if(frk == 0)
	{
		if(in_red)
		{
			int new_in = open(in_file , O_RDONLY);
			dup2(new_in , STDIN_FILENO);
			//printf("%d\n", new_in);
			close(new_in);
			in_red = 0;

		}

		if(out_red)
		{
			int new_out;
			if(!appnd)
			{
				//printf("no append normal\n");
				new_out = open(out_file , O_WRONLY | O_TRUNC | O_CREAT , 0644);
			}
			else
			{
				//printf("appending in it\n");
				new_out = open(out_file , O_WRONLY | O_CREAT | O_APPEND , 0644);
			}

			dup2(new_out , STDOUT_FILENO);
			close(new_out);
			out_red = 0;
		}

		int err;
		execute_cmnds(stor_list , found , b_flag);
		exit(0);

	}
	else if(frk > 0)
	{
		waitpid(frk , NULL , WUNTRACED);
	}

	return 0;
}