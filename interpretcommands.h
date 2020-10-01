# ifndef _INTERPRETCOMMANDS_H_
# define _INTERPRETCOMMANDS_H_

void closePipes(int fd[][2], int pipe_num);
int pipeHandler(char* command, int background);
int interpretCommand(char* command, int background);

# endif