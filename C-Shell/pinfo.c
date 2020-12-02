#include<stdio.h>
#include "headers.h"
#include "global_var.h"
#include "pinfo.h"
#define BUFF_LEN 1024
extern int errno;

// Function to implement pinfo 
void mosh_pinfo(char** stor_list , int arg_num , char* home_dir)
{	
	int pid;
	int main_len = strlen(home_dir);
	if(arg_num == 1) //getting the pid
		pid = shell_pid;
	else
	{
		if(arg_num == 2)
			pid = atoi(stor_list[1]);
	}
	FILE *fptr;
	char f_path[100];
	char ex_path[100];
	sprintf(f_path , "/proc/%d/stat" , pid);  //preparing proc paths
	sprintf(ex_path , "/proc/%d/exe" , pid);
	fptr = fopen(f_path , "r"); //open proc/pid/stat
	if(!fptr)
	{
		perror("shell: couldnt access process details\n");
		sad = 1;
		return;
	}

	//printf("successfully opened\n");
	int p;
	char exec_name[256];
	char not_exec[100];
	char state[3];
	long int virt_mem;
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
	fscanf(fptr , "%s " , state); //getting state
	int mem_pos = 20;
	while(mem_pos--)
	{
		fscanf(fptr , "%ld " , &virt_mem); //getting virtual memory
	}

	fclose(fptr);
	int s_t = (int)readlink(ex_path , exec_name , sizeof(exec_name)-1); //executable name
	exec_name[s_t] = '\0';
	int size = strlen(exec_name);

	printf("pid -- %d\n" , p);
	printf("Process Status -- %s\n" , state);
	printf("memory -- %ld\n" , virt_mem);
	char* ptr = exec_name;
	//printf("%d\n" , (int)strncmp(home_dir , exec_name , main_len));
	if(s_t >= 1)
	{
		if(strncmp(exec_name , home_dir , main_len) != 0)
			printf("Executable Path -- %s\n" , exec_name);
		else
		{
			printf("Executable Path -- ~");
			ptr = ptr+main_len;
			printf("%s\n" , ptr);
		}
	}
	else
		printf("Couldn't access executable path\n");

}
