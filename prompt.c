# include "headers.h"
# include "globalvars.h"
# include "getcommands.h"
# include "history.h"
# include "externalapi.h"
# include "childhandler.h"

// GLOBLAL VARIABLES ACCESSED: int histsize, char* history_file_path

int histsize;
char history_file_path[PATH_MAX]; 
char* history_arr[MAX_HISTORY_SIZE];

void printShellPrompt(char* username, char* hostname)
{
	printf(RED "%s@%s" RESET ":", username, hostname);

	if(strcmp(cwd, home) == 0)
	{
		printf(CYAN "~" RESET "$ ");
		return;
	}

	char* home_ptr = strstr(cwd, home);
	if(home_ptr == NULL)
		printf(CYAN "%s" RESET "$ ", cwd);
	else
	{
		if(cwd[strlen(home)] == '/')
			printf(CYAN "~%s" RESET "$ ", cwd + strlen(home));
		else
			printf(CYAN "%s" RESET "$ ", cwd);
	}
}

void printExitStatus(int exit_status)
{
	if(exit_status == 0)
		printf(CYAN ":) ");
	else
		printf(CYAN ":( ");
}

void prompt()
{
	// get user name
	struct passwd *p = getpwuid(getuid());
	if(p == NULL)
	{
		perror("getpwuid() error");
		exit(1);
	}

	// get system name
	char hostname[HOST_NAME_MAX];
	if(gethostname(hostname, sizeof(hostname)) == -1)
	{
		perror("gethostname() error");
		exit(1);
	}

	// load shell history
	for(int i=0; i<MAX_HISTORY_SIZE; i++)
		history_arr[i] = NULL;

	sprintf(history_file_path, "/home/%s/.shell_history", p->pw_name);
	loadHistory();
	histsize = historySize();

	// ignore signals to shell
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);   // ctrl+C.
	sigaction(SIGTSTP, &sa, NULL);  // ctrl+Z.
	sigaction(SIGQUIT, &sa, NULL);  // ctrl+\.
	sigaction(SIGTTOU, &sa, NULL);  // lets shell regain fg control

	int exit_status = 0;
	while(1)
	{
		checkForBackgroundProcesses();
		printExitStatus(exit_status);
		printShellPrompt(p->pw_name, hostname);
		exit_status = getCommand();
	}	
}
