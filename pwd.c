# include "headers.h"
# include "pwd.h"

void setPwd(char* dir)
{
	if(getcwd(dir, PATH_MAX) == NULL)
	{
		perror("getcwd() error");
		return;
	}
}
