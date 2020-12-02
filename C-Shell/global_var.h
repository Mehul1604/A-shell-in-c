#ifndef GLOB_H
#define GLOB_H
#define BUFF_LEN 1024

struct p_name //struct for storing process name and pid
{
	char name[256];
	int pid;
	struct p_name *next;
};

struct p_name* head;
struct p_name* cur_fore;

int shell_pid;
int p_c;
char main_dir[BUFF_LEN];
char prev_dir[BUFF_LEN];
char cwd[BUFF_LEN];
char typed_line[BUFF_LEN];
char* sentence_list[BUFF_LEN];
char username[256];
char prompt_hostname_dir[2048];
char back_string[512];
int sad;

void insert_process(int proc_id , char* proc_name);
void remove_process(int proc_id);
int kill_children();
void signal_handler(int signal);  //Signal handler for SIGCHLD
void ctrl_c_handler(int sig_num); //Signal handler for SIGINT
void ctrl_z_handler(int sig); //Signal handler for SIGSTP




#endif