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
#include "redirect.h"
#include "pipe.h"
#include "job_func.h"
#include "chain_check.h"
#include "read_func.h"
#define BUFF_LEN 1024
extern int errno;


int main(int arc, char **argv)
{
	
	head = NULL;
	p_c = 0;
	shell_pid = getpid();
	cur_fore = (struct p_name*)malloc(sizeof(struct p_name));
	cur_fore->pid = -1;
	strcpy(cur_fore->name , "");
	sad = 0;


	signal(SIGCHLD , signal_handler); //resgistering the handlers
	signal(SIGINT, ctrl_c_handler); 
	signal(SIGTSTP, ctrl_z_handler); 
	char hostname[256];
	int user_ret;
	int host_ret;
	user_ret = getlogin_r(username , 257); //getting username and hostname
	host_ret = gethostname(hostname , 257);
	getcwd(main_dir , sizeof(main_dir)+1);
	strcpy(prev_dir , main_dir);
	int main_len = strlen(main_dir);
	printf("\e[1;1H\e[2J");
	printf("                    									*************************\n");
	printf("                    									*  *                 *  *\n");
	printf("                    									* *                   * *\n");
	printf("                    									**  WELCOME TO MOSH!   **\n");
	printf("                    									* *                   * *\n");
	printf("                    									*  *                 *  *\n");
	printf("                    									*************************\n");


	


	while(1)
	{

		getcwd(cwd , sizeof(cwd) + 1);
		//printf("previous working directory is %s\n" , prev_dir);
		if(sad)
			printf(":'(");
		else
			printf(":')");

		printf("\033[01;34m");  //printing prompt
		

		printf("%s@" , username);
		printf("\033[01;33m");
		char* p = cwd;

		if(strncmp(cwd , main_dir , main_len) != 0)
		{
			printf("%s:%s> " , hostname , cwd);
			sprintf(prompt_hostname_dir , "%s:%s> " , hostname , cwd);
		}
		else
		{
			
			printf("%s:~" , hostname);
			p = p+main_len;
			printf("%s> " , p);
			sprintf(prompt_hostname_dir , "%s:~%s> "  , hostname , p);
		}

		printf("\033[0m"); 
			
		sad = 0;
		if(!readCommand(typed_line)) //reading command
			break;

		int can_store = 0;
		for(int j=0;j<strlen(typed_line);j++)
		{
			if(typed_line[j] != ' ')
			{
				can_store = 1;
				break;
			}
		}

		if(can_store) //storing in history
		{
			if(store_history(typed_line , main_dir))
				continue;
		}

		int err = 0;
		int sent_num = parseBySemicolon(typed_line , sentence_list);


		for(int i=0;i<sent_num;i++) //running through commands separated by semicolon
		{
			sad = 0;
			char* parsed_list[BUFF_LEN];
			int spaceargs = parseBySpace(sentence_list[i] , parsed_list);  //parsing the command
			if(spaceargs == 0)
				return 0;

			if(!check_chain(parsed_list , spaceargs))
				continue;

			if(!check_pipe_red(parsed_list , spaceargs))
				continue;
			
			int red_ret = check_redir(parsed_list , spaceargs);
			if(!red_ret)
				continue;

			//printf("no redirection\n");

			int background = 0;
			if(spaceargs > 1 && !strcmp("&" , parsed_list[spaceargs-1])) //checking if background
			{
				background = 1;
				parsed_list[spaceargs-1] = '\0';
				spaceargs--;
				
			}

			err = execute_cmnds(parsed_list , spaceargs , background);
			if(err == 1)
				break;
			
		}

		if(err)
		{
			if(!kill_children())
				break;
		}

	}
}