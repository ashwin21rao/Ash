# include "headers.h"
# include "globalvars.h"
# include "externalapi.h"
# include "helperfunctions.h"
# include "childhandler.h"

int runExternalProcess(char* input_command, int background)
{
	int id = fork();
	// error while forking
	if(id < 0)
	{
		perror("fork");
		return 1;
	}

	if(id == 0)
	{
		// give child its own process group (with grp id = id)
		setpgid(0, 0);

		// reset inherited signal handlers to default
		struct sigaction sa;
		sa.sa_handler = SIG_DFL;
		sa.sa_flags = SA_RESTART;
		sigaction(SIGINT, &sa, NULL);   
		sigaction(SIGTSTP, &sa, NULL);  
		sigaction(SIGQUIT, &sa, NULL);  
		sigaction(SIGTTOU, &sa, NULL);  

		char* command = strtok(input_command, "<>>");
		char* argv[countTokens(command, " ") + 1];
		char* token = strtok(command, " ");
		int i = 0;
		while(token != NULL)
		{
			argv[i++] = token;
			token = strtok(NULL, " ");
		}
		argv[i] = NULL;

		if(execvp(argv[0], argv) < 0)
		{
			fprintf(stderr, "%s: command not found\n", command);
			exit(1);
		}
	}
	else
	{
		// give child its own process group (with grp id = id)
		setpgid(id, id);

		// give child time to reach execvp (to save name)
		// usleep(10000);

		// save child info
		addChild(id, input_command);

		if(!background)
		{
			return waitForForegroundChild(getChildNumberFromPid(id));
		}
		else
		{
			fprintf(stderr, "[%d] [%d] sent to background\n", child_proc_arr[child_proc_num-1]->job_no, id);
		}
	}

	return 0;
}

void checkForBackgroundProcesses()
{
	// detects change in state of background children - exit, stopped, continued
	int status, pid;
	while((pid = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED)) > 0)
	{
		updateBackgroundChildStatus(pid, status);
	}
}
