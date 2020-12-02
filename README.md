

# **MOSH - A shell in C**

#### Mehul Mathur

## Introduction
This is a simple shell made in C which tries to mimic the actual linux terminal shell (bash , more specifically). 
It supports basic bash commands like echo , ls , pwd , cd , etc.. and some user defined commands like pinfo and nightswatch. Other commands have been taken care of by using execvp(). The whole shell has been coded using C.
The shell also supports Redirectiion and Piping along with handling of ctrl C and ctrl Z. Job control functions(fg , bg..) are also implemented. Chaining of commands is also supported.


## Compiling and Running:
- Run **make** in the folder **C-Shell** to compile all the files. (makefile file also present in the folder)
- Run the executable named **mosh** by typing the pathname to the executable on the command line inside that folder from any other folder and that folder will become your home directory for the shell. (A test directory and a .secret file has been included in the folder just for testing out the ls and cd command)

## File and Functions

- ### headers.h
This is a header file that includes all the required headers for the rest of the files
- - -
- ### main.c
This file contains the main code that implements functions from all other files. This file is responsible for printing the prompt of the shell after getting username and hostname of the system. 

##### Additional info
- All paths containing the home directory path as their subset have it replaced by ~(tilda). 
- On entering a command that is not recognizable even by bash , "Command not found" is printed
- For specific errors in ls , cd , pwd etc. ,  a defined error messaged is printed.
- For any general error like errors in File I/O , getcwd() command etc. perror is used
- The background process names and pid are stored using a Linked list data structure
- - -

- ### global_var.h
This file contains all the global variable declarations like - **username , home directory , background job counter , shell's process id , string of what the user typed , prev_directory string , current directory string , The process structure(Process name , process pid) and its head pointer.**
It also contains the declarations of the functions related to inserting and deleting the processes from the linked list as well as signal handlers.

##### Additional info
- This echo does not take into account inverted commas and escape sequences
- All spaces and tabs between words are reduced to one space character during printing(like bash)
- All function declarations are present in the **echo.h** file.
- - -

- ### glob_func.c
This file has all the function bodies of the - Signal handlers for Ctrl Z and Ctrl C , Linked list related functions for the background job list , Killing of background children.
##### Additional info
- The ctrl c handler sends SIGINT signal to any current running foreground process. If no such process , it simply prints a new prompt line.
- The ctrl z handler stops any running Foreground process and puts it in the background with the stopped state. If no such child process,prints new prompt line.
- All function declarations are present in the **global_var.h** file.
- - - 

- ### read_func.c
This file has all the function bodies responsible for reading and parsing the user typed command. Fgets is used to read a line typed by the user. This line is then parsed by semicolon and then by spaces using the strtok function. The typed line is transformed into an array of strings of different commands (that were semicolon separated) and each such string is then parsed into an array of words.
This file also contains the code for recognizing and execution of the commands.
The parsed command is recognized , checked for chaining , checked for piping , checked for redirection and then executed (either using execvp or defined functions in the code)
##### Additional info
- If no semicolon is specified , it is treated as one command
- Typing nothing , typing only spaces or only semicolons are all treated as empty commands
- All function declarations are present in the **read_func.h** file.
- - - 

- ### job_func.c
This file contains all the job control functions - fg , bg , jobs , kjob , overkill
**fg  [job number]**
**bg  [job number]**
**jobs**
**kjob [job number] [signal number]**
**overkill**

fg - It brings the job selected from the job list by its number from background to the foreground(gives it terminal)
bg - It changes state of a background job selected by number to running
jobs - lists out all the jobs in order they were created along with their state and pid
kjob - sends a signal number to the particular job number
overkill - kills all background jobs by giving the SIGKILL signal
##### Additional info
- The jobs are indexed starting from 1
- The job number argument should be more than equal to 1 (if atleast 1 job exists) till the last number of job
- All function declarations are present in the **job_func.h** file.
- - - 

- ### pwd.c
This file is for implementing the bash pwd functionality. It prints the path of the current working directory of the shell to stdout.

- - -




- ### echo.c
This file implements the "echo" functionality (printing user typed line to stdout followed by a newline).
**echo  [args]**

##### Additional info
- This echo does not take into account inverted commas and escape sequences
- All spaces and tabs between words are reduced to one space character during printing(like bash)
- All function declarations are present in the **echo.h** file.
- - -

- ### pwd.c
This file is for implementing the bash pwd functionality. It prints the path of the current working directory of the shell to stdout.

##### Additional info
- All function declarations are present in the **pwd.h** file.
- - -

- ### cd.c
This file contains functionality to mimic the cd command in bash. It is used to change the corrent working directory to another one using relative or absolute paths.
**cd [relative/absolute path]**

##### Additional info
-  The path "." is reserved for staying in the same directory.
-  The path ".." is reserved for going one level up.
- The path "~" is for going in the home directory.
- All function declarations are present in the **cd.h** file.
- - - 

- ### ls.c
This file implements the functionality to list all the files and folders in the specified directories. The -l and -a flags are supported. The -l flag lists them in a long form with additional info such as file size , last modified date , hard links , user name , group name etc.. The -a flag includes any hidden files too.
**ls [flags (-l or -a)] [directory names]** (order doesnt matter)

##### Additional info
- The flags and directories order can be rearranged in any order.
- Any other flag other than -l or -a is treated as invalid.
- -l and -a can be have multiple occurrences without error for eg. ls -lalalla -alla -la is valid
- The default directory is current directory and the default flag is none.
- Directories are colored dark blue , symlinks blue , executables green.
-  All function declarations are present in the **ls.h** file.
- - -

- ### pinfo.c
This file implements a user defined command called "pinfo" . It is used to get information about any process by specifying its id. The PID , Status , Virtual memory and executable path of the process is shown.
**pinfo [process pid]**

##### Additional info
- The default pid is the shell process's own pid.
- For processes with restricted access to its exe path (example init (1)) , the executable path is not shown.
-  All function declarations are present in the **pinfo.h** file.
- - -

- ### history.c
This file implements the history command used to check last typed commands. It allows storage of upto 20 latest commands which stay even when shell is terminated and started again. The number of commands to display is specified by the user or default is taken as 10.
**history [num]**

##### Additional info
- Maximum number of commands it can show is 10.
- Commands repeated one after the other are not stored again. For eg. entering cd two times consecutively stores only one.
- Even if semicolons are used to separate the commands , the whole line is stored (like in bash).
- Empty commands are not stored.
-  All function declarations are present in the **history.h** file.
- - -

- ### nightswatch.c
This file implements the user defined nightswatch function of the shell.
The interrupt command after nightswatch prints a line showing number of interrupts to all the CPUs by the keyboard controller every n seconds where n is specified.
The newborn command after nightswatch prints a line every n seconds showing the latest process pid in the system , where n is specified
**nightswatch [options] [commands**] (the options are   "-n seconds" where seconds is the amount of time after which each line is printed )

##### Additional info
- If not specified , n is equal to two.
- User has to press "q" and enter to stop the command
-  All function declarations are present in the **nightswatch.h** file.

- - -

- ### redirect.c
This file implements redirection for the shell. It alters stdout and stdin temporarily for a command to a specified file. It does so with the dup2() call.
**command1 [< , > , >> ] [filename**] (< means input redirected and > means output(overwrite) , >> means output(append) )

##### Additional info
- Multiple redirections are supported eg. cat a > b > c > d .. < f < g , only the rightmost output file and input file are considered(altho the rest are still created/checked for existence)
- For input redirection(<) , the file should exist
- Any filename after > , is truncated to 0 length.
- $1> , <> , etc.. are not supported.
-  All function declarations are present in the **redirect.h** file.

- - -

- ### pipe.c
This file implements piping for the shell. The function runs a for loop over the parsed command and looks for pipes (|). At each pipe the stdout and stdin are connected to the write end of this pipe and read end of previous pipe respectively.
**command1 | command2 | command3 ...** 

##### Additional info
- Redirections within piping is also supported (eg. wc < a | cat)
- For the first command in the pipe , the stdin is 0 and for the last command stdout is 1
-  All function declarations are present in the **pipe.h** file.

- - -

- ### chain_check.c
This file implements command chaining for the shell. The function runs a for loop over the parsed command and looks for symbols $(OR) and @(AND). At a @ , if the commands to the left of it are returning false , the command to the right of it wont execute. At a $ , if the commands to the left of it returned true , the commands to the right of it wont execute.
**command1 @ command2 $ command3 ...** 

##### Additional info
- There is no priority order is taken from left to right
- The exit value(0 or 1) of a group of commands is 1(false) if an error occurs or command is not found. Other wise its 0(false). This is represented by a sad and happy smiley face preceding the prompt.
-  All function declarations are present in the **chain_check.h** file.

- - -




- ### Additional implementations
- clear or c - Clears the screen
- q or quit - Terminates the shell after killing any left child processes
- :') or :'( is shown before the prompt to show whether the execution was erroneous or not.
- The hyphen "-" saves the previously visited directory , so cd - takes us to the previously visited directory.

## Assumptions
- MOSH is designed for day to day use and has reasonable buffer sizes for holding any kind of command or string. So it is not meant to be stress tested.
- For all commands that read system info they do so from proc folder and therefore this shell is limited to linux.

- - -
- - -





