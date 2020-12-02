#ifndef FUNC_DEC
#define FUNC_DEC

int readCommand(char* typed_cmnd);
int parseBySpace(char* typed_cmnd , char** stor_list);
int parseBySemicolon(char* typed_cmnd , char** stor_list);
int execute_cmnds(char** stor_list , int space_args , int b_flag);

#endif
