#include<stdio.h>
#include "headers.h"
#include "chain_check.h"
#include "redirect.h"
#include "pipe.h"
#include "read_func.h"
#include "global_var.h"
#define BUFF_LEN 1024
extern int errno;

int check_chain(char** stor_list , int arg_num)
{
	int fl = 0;
	int chained_num = 0;
	int chain_arr[256];
	for(int i=0;i<arg_num;i++)
	{
		if(!strcmp("@" , stor_list[i]) || !strcmp("$" , stor_list[i]))
		{
			fl = 1;
			chain_arr[chained_num] = i;
			chained_num++;
		}
	}

	if(!fl)
		return 1;

	
	
	int background;
	
	int lim;
	int st = 0;
	int pos;
	background = 0;
	lim = chain_arr[0];
	if(lim > 1 && !strcmp("&" , stor_list[lim-1])) //checking if background
	{
		background = 1;
		stor_list[lim-1] = '\0';
		lim--;
	}

	if(check_pipe_red(stor_list , lim))
	{
		if(check_redir(stor_list , lim))
		{
			execute_cmnds(stor_list , lim , background);
		}
	}


	for(int i=0;i<chained_num;i++)
	{
		if(!strcmp("@" , stor_list[chain_arr[i]]))
		{
			if(sad)
			{
				continue;
			}
		}
		if(!strcmp("$" , stor_list[chain_arr[i]]))
		{
			if(!sad)
			{
				continue;
			}
		}
		background=0;
		pos = chain_arr[i];

		if(i == chained_num-1)
			lim = arg_num;
		else
			lim = chain_arr[i+1];

		if(lim-(pos+1) > 1 && !strcmp("&" , stor_list[lim-1])) //checking if background
		{
			background = 1;
			stor_list[lim-1] = '\0';
			lim--;
		}

		if(check_pipe_red(stor_list+(pos+1) , lim-(pos+1)))
		{
			if(check_redir(stor_list+(pos+1) , lim - (pos+1)))
			{
				execute_cmnds(stor_list+(pos+1) , lim-(pos+1) , background);
			}
		}

	}

	return 0;
}
