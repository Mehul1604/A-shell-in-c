#include<stdio.h>
#include "headers.h"
#include "global_var.h"
#include "read_func.h"
#include "nightswatch.h"
#define BUFF_LEN 1024
#define NB_ENABLE 0
#define NB_DISABLE 1
extern int errno;

//Function to implement interrupt command of history

void mosh_nightswatch_interrupt(char** stor_list , int arg_num , int time)
{
	FILE* fp;
	fp = fopen("/proc/interrupts" , "r"); //reading from proc/interrut
	if(!fp)
	{
		perror("shell: ");
		sad = 1;
		return;
	}

	char cpu_buf[1024];
	fgets(cpu_buf , 1025 , fp);
	int len = strlen(cpu_buf);
	if(cpu_buf[len-1] == '\n')
		cpu_buf[len-1] = '\0';

	int num_cpus;
	char* cpu_array[10];

	num_cpus = parseBySpace(cpu_buf , cpu_array);

	for(int i=0;i<num_cpus;i++)  //printing the CPUs
	{
		if(i < num_cpus-1)
			printf("%s    " , cpu_array[i]);
		else
			printf("%s\n" , cpu_array[i]);
	}
	
	fclose(fp);
	while(1)  //loop to print the interrupts
	{
		
		fp = fopen("/proc/interrupts" , "r");
		if(!fp)
		{
			perror("shell: ");
			return;
		}


		char interrupt_buf[1024];
		char* interrupt_array[20];
		fgets(interrupt_buf , 1025 , fp);
		fgets(interrupt_buf , 1025 , fp);
		fgets(interrupt_buf , 1025 , fp);  //getting to the required row
		len = strlen(interrupt_buf);
		if(interrupt_buf[len-1] == '\n')
			interrupt_buf[len-1] = '\0';

		int num_words = parseBySpace(interrupt_buf , interrupt_array);
		for(int i=0;i<num_words;i++)
		{
			if(!i)
				continue;

			if(i < num_cpus)
				printf("%s    " , interrupt_array[i]);
			else
			{
				printf("%s\n" , interrupt_array[i]);
				break;
			}

		}

		sleep(time);

		fclose(fp);
	}


}

//Function to implement newborn command of nightswatch
void mosh_nightswatch_newborn(char** stor_list , int arg_num , int time)
{
	FILE* fp;
	while(1)
	{
		fp = fopen("/proc/loadavg" , "r"); //reading from proc/interrupt
		if(!fp)
		{
			perror("shell: ");
			sad = 1;
			return;
		}

		char latest_pid[12];
		while(fscanf(fp , "%s " , latest_pid) > 0); //looking for latest pid
		printf("%s\n" , latest_pid);
		sleep(time);
		fclose(fp);
	}
}


