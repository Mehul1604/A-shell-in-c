#include<stdio.h>
#include "headers.h"
#include "global_var.h"
#include "echo.h"
#include "pwd.h"
#include "cd.h"
#include "ls.h"
#include "pinfo.h"
#include "history.h"
#include "nightswatch.h"
#include "job_func.h"
#include "read_func.h"
#define BUFF_LEN 1024
extern int errno;


// Function to read user command
int readCommand(char* typed_cmnd)
{
	if(!fgets(typed_cmnd , BUFF_LEN+1 , stdin))
		return 0;

	int length = strlen(typed_cmnd);
	if(typed_cmnd[length - 1] == '\n')
		typed_cmnd[length - 1] = '\0';

	
	return 1;
}

// parsing by space
int parseBySpace(char* typed_cmnd , char** stor_list)
{

	char* token;
	token = strtok(typed_cmnd , " \t");

	int ptr = 0;
	while(token != NULL)
	{

		stor_list[ptr] = token;
		//printf("token number %d is %s\n", ptr+1,token);
		ptr++;
		token = strtok(NULL," \t");
	}


	return ptr;
}

// parsing by semicolon
int parseBySemicolon(char* typed_cmnd , char** stor_list)
{

	char* token;
	token = strtok(typed_cmnd , ";");
	int ptr = 0;
	while(token != NULL)
	{

		stor_list[ptr] = token;
		//printf("sentence number %d is %s\n", ptr+1,token);
		ptr++;
		token = strtok(NULL,";");
	}


	return ptr;
}

int execute_cmnds(char** stor_list , int space_args , int b_flag)
{
	

	//Checking the command
	if(!strcmp("echo" , stor_list[0]))
		mosh_echo(stor_list , space_args);

	else if(!strcmp("pwd" , stor_list[0]))
		mosh_pwd(space_args);

	else if(!strcmp("cd" , stor_list[0]))
		mosh_cd(stor_list , space_args , main_dir);

	else if(!strcmp("ls" , stor_list[0]))
		mosh_ls_process(stor_list , space_args , main_dir);

	else if(!strcmp("pinfo" , stor_list[0]))
	{
		mosh_pinfo(stor_list , space_args , main_dir);
	}

	else if (!strcmp("history" , stor_list[0]))
	{
		mosh_history(stor_list , space_args , main_dir);
	}

	else if(!strcmp("nightswatch" , stor_list[0]))
	{
		int secs , inter , born;
		inter = 0;
		born = 0;
		if(space_args < 2)
		{
			fprintf(stderr , "shell: nightswatch: too few arguments\n");
			sad = 1;
			return 0;
		}

		if(space_args > 4)
		{
			fprintf(stderr , "shell: nightswatch: too many arguments\n");
			sad = 1;
			return 0;
		}

		if(space_args >= 2)
		{
			if(space_args == 3)
			{
				fprintf(stderr , "shell: nightswatch: invalid number of arguments\n");
				sad = 1;
				return 0;
			}

			if(space_args == 2)
			{
				if(!strcmp("interrupt" , stor_list[1]))
				{
					inter = 1;
					secs = 2;
					born = 0;
				}
				else if(!strcmp("newborn" , stor_list[1]))
				{
					born = 1;
					inter = 0;
					secs = 2;
				}
				else
				{
					printf("shell: nightswatch: invalid command\n");
					sad = 1;
					return 0;
				}	
			}

			if(space_args == 4)
			{
				if(strcmp("-n" , stor_list[1]))
				{
					fprintf(stderr , "shell: nightswatch: invalid flag\n");
					sad = 1;
					return 0;
				}

				int early = 0;
				for(int j=0;j<strlen(stor_list[2]);j++)
				{
					if(!(stor_list[2][j] >= '0' && stor_list[2][j] <= '9'))
					{
						fprintf(stderr , "shell: nightswatch: incorrect seconds format\n");
						sad = 1;
						early = 1;
						break;
					}
				}

				if(early)
					return 0;

				secs = atoi(stor_list[2]);
				if(!strcmp("interrupt" , stor_list[3]))
				{
					inter = 1;
					born = 0;
				}
				else if(!strcmp("newborn" , stor_list[3]))
				{
					born = 1;
					inter = 0;
				}
				else
				{
					fprintf(stderr , "shell: nightswatch: invalid command\n");
					sad = 1;
					return 0;
				}	
			}

			int loc_rc = fork();
			if(loc_rc == -1)
			{
				fprintf(stderr , "shell: forking error\n");
				sad = 1;
			}

			if(loc_rc == 0)
			{
				if(inter)
					mosh_nightswatch_interrupt(stor_list , space_args , secs);
				else
					mosh_nightswatch_newborn(stor_list , space_args , secs);

				exit(1);
			}
			else
			{
				char c;
				while(1)
				{
					c = getchar();
					fflush(stdin);
					if(c == 'q')
					{
						kill(loc_rc , SIGKILL);
						break;
					}
				}

			}
		}

		
	}

	else if(!strcmp("clear" , stor_list[0]) || !strcmp("c" , stor_list[0]))
	{
		if(space_args > 1)
		{
			fprintf(stderr , "shell: clear: too many arguments\n");
			sad = 1;
			return 0;
		}
		else
			printf("\e[1;1H\e[2J");
	}

	else if(!strcmp("setenv" , stor_list[0]))
	{
		if(space_args < 2 || space_args >3)
		{
			fprintf(stderr , "shell: setenv: invalid number of arguments\n");
			sad = 1;
			return 0;
		}

		if(space_args == 3)
		{
			if(setenv(stor_list[1] , stor_list[2] , 1) < 0)
			{
				perror("shell: ");
				sad = 1;
				return 0;
			}
		}

		if(space_args == 2)
		{
			if(setenv(stor_list[1] , "" , 1) < 0)
			{
				perror("shell: ");
				sad = 1;
				return 0;
			}
		}

	}

	else if(!strcmp("unsetenv" , stor_list[0]))
	{
		if(space_args != 2)
		{
			fprintf(stderr , "shell: unsetenv: invalid number of arguments\n");
			sad = 1;
			return 0;
		}


		
		if(unsetenv(stor_list[1]) < 0)
		{
			perror("shell: ");
			sad = 1;
			return 0;
		}
		
		
	}



	else if(!strcmp("jobs" , stor_list[0]))
	{
		if(space_args > 1)
		{
			fprintf(stderr , "shell: jobs: too many arguments\n");
			sad = 1;
			return 0;
		}
		else
			list_jobs();
	}

	else if(!strcmp("kjob" , stor_list[0]))
	{
		signal_job(stor_list , space_args);
	}

	else if(!strcmp("overkill" , stor_list[0]))
	{
		if(space_args > 1)
		{
			fprintf(stderr , "shell: overkill: too many arguments\n");
			sad = 1;
			return 0;
		}

		kill_children();
	}

	else if(!strcmp("bg" , stor_list[0]))
	{
		mosh_bg(stor_list , space_args);
	}

	else if(!strcmp("fg" , stor_list[0]))
	{
		mosh_fg(stor_list , space_args);
	}

	else if(!strcmp("quit" , stor_list[0]) || !strcmp("q" , stor_list[0]))
	{
		if(space_args > 1)
		{
			fprintf(stderr , "shell: quit: too many arguments\n");
			sad = 1;
		}
		else
		{
			return 1;
		}
		
	}

	else //for commands given to execvp()
	{
		char* args[256];
		for(int j=0;j<space_args;j++)
		{
			if(j==0)
			{
				if(strncmp("~" , stor_list[j] , 1) == 0)
				{
					char newpath[2000];
					stor_list[j]++;
					sprintf(newpath , "%s%s" , main_dir , stor_list[j]);
					args[j] = newpath;
				}
				else
					args[j] = stor_list[j];

			}
			else
				args[j] = stor_list[j];

		}
		args[space_args] = NULL;
		int rc = fork();  //forking and creating a child
		if(rc == -1)
		{
			fprintf(stderr , "shell: forking error\n");
			sad = 1;
		}
		int cpid;
		if(rc == 0)  //CHILD
		{

			if(b_flag) //changing group id of child
				setpgid(0, 0);
			
			
			int rere = execvp(args[0] , args);
			if(rere == -1)
			{
				if(!b_flag)
					fprintf(stderr , "shell: Command not found\n");
				else
					fprintf(stderr , "\nshell: Command not found\n");
				exit(1);
			}
		}
		else
		{
			if(rc > 0) //PARENT
			{

				if(!b_flag)
				{
					cur_fore->pid = rc;
					// char fore_ins[512];
					// for(int i=0;i<space_args;i++)
					// {
					// 	strcat(fore_ins , stor_list[i]);
					// 	if(i < (space_args-1))
					// 		strcat(fore_ins , " ");
					//}
					strcpy(cur_fore->name , args[0]);
					// strcpy(fore_ins , "");
					int status_code;
					cpid = waitpid(rc , &status_code , WUNTRACED);
					//usleep(100000);
					if(WIFEXITED(status_code))
					{
						if(WEXITSTATUS(status_code))
							sad = 1;
					}
					else
						sad = 1;


					if(WIFSTOPPED(status_code))
					{
						sad = 1;
						//printf("process was stopped\n");
						p_c++;
						fprintf(stderr , "[%d] %d - %s\n" , p_c , cur_fore->pid , cur_fore->name);
						insert_process(cur_fore->pid , cur_fore->name);
					}
					cur_fore->pid = -1;
					strcpy(cur_fore->name , "");
				}
				else
				{
					setpgid(rc,rc);
					p_c++;
					//printf("new insert\n");
					// char name_ins[512];
					// for(int i=0;i<space_args;i++)
					// {
					// 	strcat(name_ins , stor_list[i]);
					// 	if(i < (space_args-1))
					// 		strcat(name_ins , " ");
					// }
					fprintf(stderr , "[%d] %d - %s\n" , p_c , rc , args[0]);
					insert_process(rc , args[0]); //inserting process into linked list
					//strcpy(name_ins , "");
					
					
				}
			}
		}
	}

	return 0;

}




