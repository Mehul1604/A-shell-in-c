#include<stdio.h>
#include "headers.h"
#include "pipe.h"
#include "redirect.h"
#include "read_func.h"
#include "global_var.h"
#define BUFF_LEN 1024
extern int errno;

int check_pipe_red(char** stor_list , int arg_num)
{
	
	int fl = 0;
	//printf("the piped stuff are\n");
	for(int i=0;i<arg_num;i++)
	{
		if(!strcmp("|" , stor_list[i]))
		{
			fl = 1;
			break;
		}
	}

	if(!fl)
	{
		//printf("no pipes\n");
		return 1;
	}

	//int p_count = 0;
	int st = 0;
	int inp = 0;
	int outp;
	int frk;
	//char* sub_cmnd_list[256][BUFF_LEN];
	for(int i=0;i<arg_num;i++)
	{
		sad = 0;
		if(!strcmp("|" , stor_list[i]) || i==arg_num-1)
		{
			int lim;
			if(i == arg_num-1)
				lim = i+1;
			else
				lim = i;

			int fd[2];
			pipe(fd);
			frk = fork();
			if(frk < 0)
			{
				perror("shell: ");
				sad = 1;
				return 0;
			}
			else if(frk == 0)
			{
				dup2(inp , STDIN_FILENO);
				if(lim != arg_num)
					dup2(fd[1] , STDOUT_FILENO);

				close(fd[0]);
				if(!check_redir(stor_list+st , lim-st))
					exit(0);
				
				execute_cmnds(stor_list+st , lim-st , 0);
				if(sad == 1)
					exit(1);
				else
					exit(0);
			}
			else if(frk > 0)
			{
				int status;
				waitpid(frk , &status , WUNTRACED);
				if(WIFEXITED(status))
				{
					if(WEXITSTATUS(status))
						sad = 1;
				}
				else
					sad = 1;

				close(fd[1]);
				inp = fd[0];
				st = lim+1;
			}
		}
	}

	return 0;

}