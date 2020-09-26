# OSN Assignment 2

# **MOSH - A shell in C**

#### Mehul Mathur 2019101046

## Introduction
This is a simple shell made in C which tries to mimic the actual linux terminal shell (bash , more specifically). 
It supports basic bash commands like echo , ls , pwd , cd , etc.. and some user defined commands like pinfo and nightswatch. Other commands have been taken care of by using execvp(). The whole shell has been coded using C.


## Compiling and Running:
- Run make in the assigmment folder to compile all the files. (makefile file also present in the folder)
- Run the executable named "mosh" inside that folder from any other folder and that folder will become your home directory for the shell. (A test directory and a .secret file has been included in the folder just for testing out the ls and cd command)

## File and Functions

- ### headers.h
This is a header file that includes all the required headers for the rest of the files
- - -
- ### main.c
This file contains the main code that implements functions from all other files. This file is responsible for printing the prompt of the shell after getting username and hostname of the system. All the signal handling functions are declared in this file as well as any fork() and execvp() calls. The signal handler for handling SIGCHLD and SIGINT signals from child processes is declared in this file.

##### Additional info
- All paths containing the home directory path as their subset have it replaced by ~(tilda). 
- On entering a command that is not recognizable even by bash , "Command not found" is printed
- For specific errors in ls , cd , pwd etc. ,  a defined error messaged is printed.
- For any general error like errors in File I/O , getcwd() command etc. perror is used
- The background process names and pid are stored using a Linked list data structure
- - -
- ### read_func.c
This file has all the function bodies responsible for reading and parsing the user typed command. Fgets is used to read a line typed by the user. This line is then parsed by semicolon and then by spaces using the strtok function. The typed line is transformed into an array of strings of different commands (that were semicolon separated) and each such string is then parsed into an array of words.
##### Additional info
- If no semicolon is specified , it is treated as one command
- Typing nothing , typing only spaces or only semicolons are all treated as empty commands
- Pressing of ctrl c is handled by the SIGINT handler which kills any background processes before exiting
- All function declarations are present in the **read_func.h** file.
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

- ### Additional commands
- clear - Clears the screen
- exit - Terminates the shell after killing any left child processes

## Assumptions
- MOSH is designed for day to day use and has reasonable buffer sizes for holding any kind of command or string. So it is not meant to be stress tested.
- For all commands that read system info they do so from proc folder and therefore this shell is limited to linux.

- - -
- - -





