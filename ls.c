#include<stdio.h>
#include "headers.h"
#include "global_var.h"
#include "ls.h"
#define BUFF_LEN 1024
extern int errno;


// Function to execute ls with flags
void mosh_ls_execute(int lflag , int aflag , char** dir_list , int num_dir)
{
	for(int i=0;i<num_dir;i++) //reading all directories in a loop
	{

		struct stat init;
		stat(dir_list[i] , &init);
		if(S_ISREG(init.st_mode))
		{
			printf("%s\n" , dir_list[i]);
			continue;
		}
		if(num_dir!=1)
			printf("%s:\n" , dir_list[i]);
		DIR *dir = opendir(dir_list[i]);
		if(!dir)
		{
			perror("shell: ");
			sad = 1;
			return;
		}

		struct dirent *file;
		struct stat st;
		int cnt;

		if(!lflag) // if no l flag
		{
			while((file = readdir(dir)) != NULL) // going through directory contents
			{
				char buffer[1024];
				sprintf(buffer , "%s/%s" ,dir_list[i] ,  file->d_name);
				stat(buffer , &st);
				if(aflag || (file->d_name[0] != '.' && strcmp(".." , file->d_name))) //checking for a flag
				{	
					if(S_ISDIR(st.st_mode))
						printf("\033[0;34m");

					printf("%s\n" , file->d_name);  //name
					printf("\033[0m"); 

				}
			}

		}
		else //if l flag is on
		{
			while((file = readdir(dir)) != NULL) //going through directory contents
			{
				char buffer[1024];
				sprintf(buffer , "%s/%s" ,dir_list[i] ,  file->d_name);
				lstat(buffer , &st);
				int plain = 0;
				if(aflag || file->d_name[0] != '.')
				{	
					if(S_ISDIR(st.st_mode))  //checking type of file
						printf("d");
					else if(S_ISLNK(st.st_mode))
						printf("l");
					else if(S_ISFIFO(st.st_mode))
						printf("p");
					else if(S_ISSOCK(st.st_mode))
						printf("s");
					else if(S_ISCHR(st.st_mode))
						printf("c");
					else if(S_ISBLK(st.st_mode))
						printf("b");
					else
					{
						plain = 1;
						printf("-");
					}
				    printf( (st.st_mode & S_IRUSR) ? "r" : "-");  //permissions
				    printf( (st.st_mode & S_IWUSR) ? "w" : "-");
				    printf( (st.st_mode & S_IXUSR) ? "x" : "-");
				    printf( (st.st_mode & S_IRGRP) ? "r" : "-");
				    printf( (st.st_mode & S_IWGRP) ? "w" : "-");
				    printf( (st.st_mode & S_IXGRP) ? "x" : "-");
				    printf( (st.st_mode & S_IROTH) ? "r" : "-");
				    printf( (st.st_mode & S_IWOTH) ? "w" : "-");
				    printf( (st.st_mode & S_IXOTH) ? "x" : "-");
				    printf(" ");
				    printf("%d " , (int)st.st_nlink); //hardlinks
				    struct passwd *pw = getpwuid(st.st_uid);
				    struct group  *gr = getgrgid(st.st_gid);
				    printf("%10s %10s " , pw->pw_name , gr -> gr_name);  //user name and group name
				    int wr;
				    wr = printf("%5zu ",st.st_size);  //file size

				    struct tm *tm_ptr;
				    char buf_time[256];
				    strftime(buf_time , 257 , "%b %d %H:%M" , localtime(&st.st_mtime));
				    printf("%s " , buf_time);  //last modified time

					if(S_ISDIR(st.st_mode))
						printf("\033[0;34m");

					if(S_ISLNK(st.st_mode))
						printf("\033[1;36m");

					if(st.st_mode & S_IXUSR && plain)
						printf("\033[1;32m");


					printf("%s\n" , file->d_name);
					printf("\033[0m"); 

				}
			}
		}

		closedir(dir);
		if(i < (num_dir-1))
			printf("\n");
	}
}


// Function to process input and determine flags and directories
void mosh_ls_process(char** stor_list , int arg_num , char* home_dir)
{
	int l_flag = 0;
	int a_flag = 0;
	int specfic_dir = 0;
	char* dirname[256];
	int dir_num = 0;
	if(arg_num == 1) // if no other args
	{	
		dirname[0] = ".";
		dir_num++;
		mosh_ls_execute(0 , 0 , dirname , dir_num);
	}
	else
	{
		for(int i=1;i<arg_num;i++)
		{
			if(stor_list[i][0] == '-') //looking for l and a flags
			{
				if(strlen(stor_list[i]) == 1)
				{
					specfic_dir = 1;
					dirname[dir_num] = prev_dir;
					dir_num++;
				}
				else
				{
					for(int j=1;j<strlen(stor_list[i]);j++) 
					{
						if(stor_list[i][j] == 'l')
							l_flag = 1;
						else
						{
							if(stor_list[i][j] == 'a')
								a_flag = 1;
							else
							{
								fprintf(stderr , "shell: ls: invalid flag(s)\n");
								sad = 1;
								return;
							}
						}
					}
				}
			}
			else  //looking for directories
			{
				specfic_dir = 1;
				if(!strcmp("~" , stor_list[i]))
					dirname[dir_num] = home_dir;
				else
					dirname[dir_num] = stor_list[i];

				dir_num++;
			}
		}

		if(!specfic_dir)  // if no directory specified
		{
			dirname[0] = ".";	
			dir_num++;
		}

		mosh_ls_execute(l_flag , a_flag , dirname , dir_num);
	}

}