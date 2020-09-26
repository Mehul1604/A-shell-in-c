#include<stdio.h>
#include "headers.h"
#include "global_var.h"
#include "history.h"
#define BUFF_LEN 1024
extern int errno;

// Function to implement history. It reads from the mosh_history file 
void mosh_history(char** stor_list , int arg_num , char* home_loc)
{
	// Error handling
	int n;
	if(arg_num > 2)
	{
		fprintf(stderr , "shell: history: too many arguments\n");
		sad = 1;
		return;
	}
	else
	{
		if(arg_num == 2)
		{
			if(strlen(stor_list[1]) > 2)
			{
				fprintf(stderr , "shell: history: invalid argument\n");
				sad = 1;
				return;
			}
			else
			{
				for(int i=0;i<strlen(stor_list[1]);i++)
				{
					if(!(stor_list[1][i] >= '0' && stor_list[1][i] <= '9'))
					{
						fprintf(stderr , "shell: history: invalid argument\n");
						sad = 1;
						return;
					}
				}

				n = atoi(stor_list[1]);
				if(n > 10)
				{
					fprintf(stderr , "shell: history: argument too large\n");
					sad = 1;
					return;
				}
			}
		}
		else
			n = 10;
	}

	char proc_path[256];
	sprintf(proc_path , "/proc/%d/exe" , getpid());
	char exec_path[512];
	int s_t = readlink(proc_path , exec_path , sizeof(exec_path)-1);
	if(s_t < 1)
	{
		fprintf(stderr , "shell: History file error\n");
		sad = 1;
		return;
	}

	exec_path[s_t] = '\0';
	int len = strlen(exec_path);
	for(int i=len-1;i>=0;i--)
	{
		if(exec_path[i] == '/')
			break;

		exec_path[i] = '\0';
	}

	char test_hist[1001];
	char test_temp[1001];
	sprintf(test_hist , "%smosh_history" , exec_path);  //making path for history file to be stored
	sprintf(test_temp , "%shist.temp" , exec_path);

	FILE *fp;
	fp = fopen(test_hist , "r");
	if(!fp)
	{
		perror("shell: ");
		sad = 1;
		return;
	}

	char buf[1025];
	char* list[1024];
	char big[20480] = "";
	while(fgets(buf , 1026 , fp))
	{
		strcat(big , buf);
	}

	char* token;
	token = strtok(big , "\n");

	int ptr = 0;
	while(token != NULL)  // storing the obtained commands in an array
	{

		list[ptr] = token;
		ptr++;
		token = strtok(NULL,"\n");
	}

	int low_lim;
	if(ptr - n < 0)
		low_lim = 0;
	else
		low_lim = ptr - n;


	for(int i=low_lim;i<ptr;i++) //displaying last n commands
	{
		printf("%s\n" , list[i]);
	}
}

 // Function to store a command in history file
int store_history(char* command , char* home_loc)
{
	char proc_path[256];
	sprintf(proc_path , "/proc/%d/exe" , getpid());
	char exec_path[512];
	int s_t = readlink(proc_path , exec_path , sizeof(exec_path)-1);
	if(s_t < 1)
	{
		fprintf(stderr , "shell: History file error\n");
		sad = 1;
		return 1;
	}

	exec_path[s_t] = '\0';
	int len = strlen(exec_path);
	for(int i=len-1;i>=0;i--)
	{
		if(exec_path[i] == '/')
			break;

		exec_path[i] = '\0';
	}

	char test_hist[1001];
	char test_temp[1001];
	sprintf(test_hist , "%smosh_history" , exec_path);  //making path for history file
	sprintf(test_temp , "%shist.temp" , exec_path);

	FILE *fp;

	fp = fopen(test_hist , "a+");
	if(!fp)
	{
		perror("shell: ");
		sad = 1;
		return 1;
	}

	char buf[1025];
	int cnt = 0;
	long int sec_start;
	int same = 0;
	char last_arg[1024];
	while(fgets(buf , 1026 , fp))  //counting already present commands
	{

		strcpy(last_arg , buf);
		if(!cnt)
			sec_start = ftell(fp);

		cnt++;
	}

	int l = strlen(last_arg);
	last_arg[l-1] = '\0';
	if(!strcmp(last_arg , command))
		return 0;


	if(same)
		return 0;	
	if(cnt == 20)
	{
		rewind(fp);
		fseek(fp , sec_start ,  SEEK_CUR); // if 20 commands , deleting the first and taking the rest in another file 
		FILE * ftemp;
		ftemp = fopen(test_temp , "a+");
		if(!ftemp)
		{
			perror("shell: ");
			sad = 1;
			return 1;
		}

		while(fgets(buf , 1026 , fp)) //putting in temp file
		{
			fputs(buf , ftemp);
		}

		fclose(fp);
		fclose(ftemp);

		if(remove(test_hist) != 0) //deleting old file
		{
			perror("shell: ");
			sad = 1;
			return 1;
		}

		rename(test_temp , test_hist);

		fp = fopen(test_hist , "a+");
		if(!fp)
		{
			perror("shell: ");
			sad = 1;
			return 1;
		}
	}

	fputs(command , fp);  //appending in history file
	fputs("\n" , fp);
	fclose(fp);
	return 0;
}

