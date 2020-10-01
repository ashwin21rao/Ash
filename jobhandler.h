# ifndef _JOBHANDLER_H_
# define _JOBHANDLER_H_

int jobs();
int  fg(char* command);
int bg(char* command);
int kjob(char* command);
int stopjob(char* command);

# endif