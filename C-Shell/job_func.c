#include<stdio.h>
#include "headers.h"
#include "global_var.h"
#include "job_func.h"
#define BUFF_LEN 1024
extern int errno;

char* get_state(int proc_id)
{
	FILE *fptr;
	char f_path[100];
	char* ret_string = (char*)malloc(256 * sizeof(char));
	sprintf(f_path , "/proc/%d/stat" , proc_id);  //preparing proc paths
	fptr = fopen(f_path , "r"); //open proc/pid/stat
	if(!fptr)
	{
		strcpy(ret_string , "Couldnt access state");
		return ret_string;
	}

	//printf("successfully opened\n");
	int p;
	char state[3];
	char ex_letter;

	fscanf(fptr , "%d " , &p); //pid
	int to_start;
	while(fscanf(fptr , "%c" , &ex_letter) > 0) //running over the exe name
	{
		if(ex_letter == ')')
			to_start = ftell(fptr);
	}

	rewind(fptr);
	fseek(fptr , to_start+1 , SEEK_CUR);
	fscanf(fptr , "%s " , state);
	fclose(fptr);
	if(strncmp("T" , state , 1) == 0)
		strcpy(ret_string , "Stopped");
	else
		strcpy(ret_string , "Running");

	return ret_string;
}

void list_jobs()
{
	struct p_name* run = head;
	int cnt = 1;
	//printf("total jobs are %d\n" , p_c);
	while(run != NULL)
	{
		printf("[%d] %s %s [%d]\n" , cnt , get_state(run->pid) , run->name , run->pid);
		run = run->next;
		cnt++;
	}
}

void signal_job(char** stor_list , int arg_num)
{
	if(arg_num != 3)
	{
		printf("shell: kjob: invalid number of arguments\n");
		sad = 1;
		return;
	}

	for(int i=0;i<strlen(stor_list[1]);i++)
	{
		if(!(stor_list[1][i] >= '0' && stor_list[1][i] <= '9'))
		{
			printf("shell: kjob: invalid process number\n");
			sad = 1;
			return;
		}

	}

	int p_num = atoi(stor_list[1]);
	if(p_num < 1 || p_num > (p_c))
	{
		printf("shell: kjob: invalid process number\n");
		sad = 1;
		return;
	}

	for(int i=0;i<strlen(stor_list[2]);i++)
	{
		if(!(stor_list[2][i] >= '0' && stor_list[2][i] <= '9'))
		{
			printf("shell: kjob: invalid signal\n");
			sad = 1;
			return;
		}
	}
	int sig = atoi(stor_list[2]);
	struct p_name* run = head;
	int cnt = 1;
	while(run!=NULL)
	{
		if(cnt == p_num)
		{
			if(kill(run->pid , sig) < 0)
			{
				perror("shell: ");
				sad = 1;
				return;
			}

			break;
		}

		cnt++;
		run = run->next;
	}

	
}

void mosh_bg(char** stor_list , int arg_num)
{
	if(arg_num != 2)
	{
		fprintf(stderr , "shell: bg: invalid number of arguments\n");
		sad = 1;
		return;
	}

	for(int i=0;i<strlen(stor_list[1]);i++)
	{
		if(!(stor_list[1][i] >= '0' && stor_list[1][i] <= '9'))
		{
			fprintf(stderr , "shell: bg: invalid process number\n");
			sad = 1;
			return;
		}

	}

	int p_num = atoi(stor_list[1]);
	if(p_num < 1 || p_num > (p_c))
	{
		fprintf(stderr , "shell: bg: invalid process number\n");
		sad = 1;
		return;
	}

	struct p_name* run = head;
	int cnt = 1;
	while(run != NULL)
	{
		if(cnt == p_num)
		{
			if(kill(run->pid , SIGCONT) < 0)
			{
				perror("shell: ");
				sad = 1;
				return;
			}

			break;
		}

		cnt++;
		run = run->next;
	}

}

void mosh_fg(char** stor_list , int arg_num)
{
	if(arg_num != 2)
	{
		fprintf(stderr , "shell: fg: invalid number of arguments\n");
		sad = 1;
		return;
	}

	for(int i=0;i<strlen(stor_list[1]);i++)
	{
		if(!(stor_list[1][i] >= '0' && stor_list[1][i] <= '9'))
		{
			fprintf(stderr , "shell: fg: invalid process number\n");
			sad = 1;
			return;
		}

	}

	int p_num = atoi(stor_list[1]);
	if(p_num < 1 || p_num > (p_c))
	{
		fprintf(stderr , "shell: fg: invalid process number\n");
		sad = 1;
		return;
	}

	struct p_name* run = head;
	int cnt = 1;
	int target;
	char target_name[256];
	while(run != NULL)
	{
		if(cnt == p_num)
		{
			target = run->pid;
			strcpy(target_name , run->name);
			break;
		}

		cnt++;
		run = run->next;
	}
	//printf("the current terminal group is %d\n" , (int)tcgetpgrp(0));
	// int fore_group = (int)tcgetpgrp(STDIN_FILENO);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	
	tcsetpgrp(STDIN_FILENO , getpgid(target));
	if(kill(target , SIGCONT) < 0)
	{
		perror("shell:  ");
		signal(SIGTTIN, SIG_DFL);
		signal(SIGTTOU, SIG_DFL);
		sad = 1;
		return;

	}
	cur_fore->pid = target;

	strcpy(cur_fore->name , target_name);
	int status;
	waitpid(target , &status , WUNTRACED);
	

	//printf("the group of %d has the terminal\n" , (int)tcgetpgrp(0));
	tcsetpgrp(STDIN_FILENO , getpgid(shell_pid));
	signal(SIGTTIN, SIG_DFL);
	signal(SIGTTOU, SIG_DFL);
	remove_process(target);
	p_c--;

	if(WIFEXITED(status))
	{
		if(WEXITSTATUS(status))
			sad = 1;
	}
	else
		sad = 1;

	if(WIFSTOPPED(status))
	{
		//printf("process was stopped\n");
		sad = 1;
		p_c++;
		fprintf(stderr , "[%d] %d - %s\n" , p_c , cur_fore->pid , cur_fore->name);
		insert_process(cur_fore->pid , cur_fore->name);
	}

	cur_fore->pid = -1;
	strcpy(cur_fore->name , "");
	strcpy(target_name , "");
	

	
	

}