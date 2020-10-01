# include "headers.h"
# include "globalvars.h"
# include "cd.h"
# include "pwd.h"
# include "helperfunctions.h"

int cd(char* path)
{	
	char new_path[PATH_MAX];

	// handles cd with no argument
	if(path == NULL)
		strcpy(new_path, home);

	// handles cd with more than 1 argument
	else if(countTokens(path, " ") > 1)
	{
		fprintf(stderr, "cd: too many arguments\n");
		return 1;
	}

	// handle cd ~
	else if(path[0] == '~')
	{
		strcpy(new_path, home);
		strcat(new_path, path + 1);
	}

	// handle cd -
	else if(path[0] == '-')
	{
		if(strlen(path) != 1)
		{
			fprintf(stderr, "cd: invalid argument\n");
			return 1;
		}
		if(prev_wd[0] == '\0')
		{
			printf("%s\n", cwd);
			return 0;
		}
		strcpy(new_path, prev_wd);
	}

	// handle normal cd
	else
		strcpy(new_path, path);

	if(chdir(new_path) == -1)
		fprintf(stderr, "cd: %s: %s\n", new_path, strerror(errno));	
	else
	{
		strcpy(prev_wd, cwd);
		setPwd(cwd);

		if(path != NULL && path[0] == '-')
			printf("%s\n", cwd);
	}

	return 0;
}
