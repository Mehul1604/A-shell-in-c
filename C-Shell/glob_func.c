#include<stdio.h>
#include "headers.h"
#include "global_var.h"
#define BUFF_LEN 1024
extern int errno;


void insert_process(int proc_id , char* proc_name) //linked list insert
{
	struct p_name* q = (struct p_name*)malloc(sizeof(struct p_name));
	
	q->pid = proc_id;
	strcpy(q->name , proc_name);
	q->next = NULL;
	if(head == NULL)
	{
		head = q;
	}
	else
	{
		struct p_name* temp;
		temp = head;
		while(temp->next != NULL)
			temp = temp->next;

		temp->next = q;
	}
}

void remove_process(int proc_id) //linked list delete
{
	struct p_name* prev = (struct p_name*)malloc(sizeof(struct p_name));
	struct p_name* temp = (struct p_name*)malloc(sizeof(struct p_name));
	if(head->pid == proc_id)
	{
		temp = head;
		head = temp->next;
		temp->next = NULL;
		free(temp);
	}
	else
	{
		prev = head;
		temp = head->next;
		while(temp != NULL && temp->pid != proc_id)
		{
			prev = temp;
			temp = temp->next;
		}

		if(temp == NULL)
			return;

		prev->next = temp->next;
		temp->next = NULL;
		free(temp);
	}

}

int kill_children()
{
	struct p_name* run = head;
	while(run != NULL) //killing remaining processes
	{
		//printf("killing %d - %s\n" , run->pid , run->name);
		if(kill(run->pid, SIGKILL) < 0)
		{
			perror("shell: ");
			sad = 1;
			return 1;
		}
		run = run->next;
	}

	return 0;

}

void signal_handler(int signal)  //Signal handler for SIGCHLD
{
	int pro_id;
	int status;
	int flag = 0;
	//write(1 , "CHILD CAME\n" , 11);
	if((pro_id = waitpid(-1 , &status , WNOHANG)) > 0) //reaping zombies
	{

		if(head)
		{
			flag = 1;
			char pname[256];	
			struct p_name* run = head;
			while(run != NULL && run->pid != pro_id)
				run = run->next;

			strcpy(pname , run->name);
			remove_process(run->pid);
			p_c--;
			if(WIFEXITED(status))
			{
				sprintf(back_string , "%s with pid %d exited normally\n" , pname , pro_id);
				write(STDERR_FILENO , back_string , strlen(back_string));
			}
			else
			{
				sprintf(back_string , "%s with pid %d exited abnormally\n" , pname , pro_id);
				write(STDERR_FILENO , back_string , strlen(back_string));
			}
		}
	}

	if(flag) //reprinting prompt
	{	
		write(1 , ":')" , 3);
		write(1 , "\033[01;34m" , strlen("\033[01;34m"));
		write(1 , username , strlen(username));
		write(1 , "@" , 1);
		write(1 , "\033[01;33m" , strlen("\033[01;33m"));
		write(1 , prompt_hostname_dir , strlen(prompt_hostname_dir));
		write(1 , "\033[0m" , strlen("\033[0m"));

	}	
}

void ctrl_c_handler(int sig_num) //Signal handler for SIGINT
{ 
	if(cur_fore->pid == -1)
	{
		//printf("no foreground process , actually\n");
		write(1 , "\n" , 1);
		write(1 , ":')" , 3);
		write(1 , "\033[01;34m" , strlen("\033[01;34m"));
		write(1 , username , strlen(username));
		write(1 , "@" , 1);
		write(1 , "\033[01;33m" , strlen("\033[01;33m"));
		write(1 , prompt_hostname_dir , strlen(prompt_hostname_dir));
		write(1 , "\033[0m" , strlen("\033[0m"));
		return;
	}
	else
	{
		return;
	}
}

void ctrl_z_handler(int sig)
{
	if(cur_fore->pid == -1)
	{
		//printf("no foreground process , actually\n");

		write(1 , "\n" , 1);
		write(1 , ":')" , 3);
		write(1 , "\033[01;34m" , strlen("\033[01;34m"));
		write(1 , username , strlen(username));
		write(1 , "@" , 1);
		write(1 , "\033[01;33m" , strlen("\033[01;33m"));
		write(1 , prompt_hostname_dir , strlen(prompt_hostname_dir));
		write(1 , "\033[0m" , strlen("\033[0m"));
		return;
	}
	else
	{
		//printf("shells pid is %d and currently running foreground was %d\n" , (int)getpgid(0) , (int)getpgid(cur_fore->pid));
		
		kill(cur_fore->pid , SIGTSTP);

	}	
} 