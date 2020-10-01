# include "headers.h"
# include "setenv.h"
# include "helperfunctions.h"

int setEnvironVar(char* command)
{
	int token_count = countTokens(command, " ");
	if(token_count < 2)
	{
		fprintf(stderr, "setenv: too few arguments\n");
		return 1;
	}
	if(token_count > 3)
	{
		fprintf(stderr, "setenv: too many arguments\n");
		return 1;
	}
	
	char *envname = strtok(command, " "), *envval;
	envname = strtok(NULL, " ");
	envval = strtok(NULL, " ");

	// if no envval is provided
	if(envval == NULL)
		envval = "\0";

	if(setenv(envname, envval, 1) == -1)
	{
		perror("setenv");
		return 1;
	}

	return 0;
}

int unsetEnvironVar(char* command)
{
	int token_count = countTokens(command, " ");
	if(token_count < 2)
	{
		fprintf(stderr, "unsetenv: missing argument\n");
		return 1;
	}
	if(token_count > 2)
	{
		fprintf(stderr, "unsetenv: too many arguments\n");
		return 1;
	}
	
	char* envname = strchr(command, ' ');

	if(unsetenv(envname+1) == -1)
	{
		perror("unsetenv");
		return 1;
	}

	return 0;
}

int getEnvironVar(char* command)
{
	int token_count = countTokens(command, " ");
	if(token_count < 2)
	{
		fprintf(stderr, "getenv: missing argument\n");
		return 1;
	}
	if(token_count > 2)
	{
		fprintf(stderr, "getenv: too many arguments\n");
		return 1;
	}
	
	char* envname = strchr(command, ' ');
	char* envval = getenv(envname+1);

	if(envval == NULL)
		fprintf(stderr, "getenv: no such environment variable\n");
	else
		printf("%s\n", envval);

	return 0;
}
