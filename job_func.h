#ifndef RED_H
#define RED_H

void list_jobs();
void signal_job(char** stor_list , int arg_num);
char* get_state(int proc_id);
void mosh_bg(char** stor_list , int arg_num);
void mosh_fg(char** stor_list , int arg_num);

#endif