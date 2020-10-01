# ifndef _LS_H_
# define _LS_H_

long long int getTotalBlocks(DIR* dp, int a_flag);
void printInfo(DIR* dp, char* file);
int checkIfFile(char* file);
int checkIfExecutable(char* file);
void list(char* directory, int a_flag, int l_flag, int multiple);
int ls(char* args);

# endif