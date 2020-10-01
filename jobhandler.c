# include "headers.h"
# include "globalvars.h"
# include "jobhandler.h"
# include "helperfunctions.h"
# include "childhandler.h"

int jobs()
{
	for(int i=0; i<child_proc_num; i++)
	{
		printf("[%d] %s %s [%d]\n", child_proc_arr[i]->job_no, 
								    child_proc_arr[i]->state,
								    child_proc_arr[i]->command,
								    child_proc_arr[i]->pid);
	}

	return 0;
}

int fg(char* command)
{
	int token_count = countTokens(command, " ");
	if(token_count < 2)
	{
		fprintf(stderr, "fg: missing argument\n");
		return 1;
	}
	if(token_count > 2)
	{
		fprintf(stderr, "fg: too many arguments\n");
		return 1;
	}

	int job_no = stringToInt(strchr(command, ' ') + 1);
	if(job_no == -1)
	{
		fprintf(stderr, "fg: invalid argument\n");
		return 1;
	}

	int child_no = getChildNumberFromJobNo(job_no);
	if(child_no == -1)
	{
		fprintf(stderr, "fg: no such process\n");
		return 1;
	}

	return waitForForegroundChild(child_no);
}

int bg(char* command)
{
	int token_count = countTokens(command, " ");
	if(token_count < 2)
	{
		fprintf(stderr, "bg: missing argument\n");
		return 1;
	}
	if(token_count > 2)
	{
		fprintf(stderr, "bg: too many arguments\n");
		return 1;
	}

	int job_no = stringToInt(strchr(command, ' ') + 1);
	if(job_no == -1)
	{
		fprintf(stderr, "bg: invalid argument\n");
		return 1;
	}

	int child_no = getChildNumberFromJobNo(job_no);
	if(child_no == -1)
	{
		fprintf(stderr, "bg: no such process\n");
		return 1;
	}

	// continue process in background
	if(kill(child_proc_arr[child_no]->pid, SIGCONT) == -1)
	{
		perror("bg");
		return 1;
	}

	return 0;
}

int kjob(char* command)
{
	int token_count = countTokens(command, " ");
	if(token_count < 3)
	{
		fprintf(stderr, "kjob: too few arguments\n");
		return 1;
	}
	if(token_count > 3)
	{
		fprintf(stderr, "kjob: too many arguments\n");
		return 1;
	}

	char* str1 = strtok(command, " "), *str2;
	str1 = strtok(NULL, " ");
	str2 = strtok(NULL, " ");

	int job_no = stringToInt(str1);
	int sig_no = stringToInt(str2);

	if(job_no == -1 || sig_no == -1)
	{
		fprintf(stderr, "kjob: invalid argument\n");
		return 1;
	}

	int child_no = getChildNumberFromJobNo(job_no);
	if(child_no == -1)
	{
		fprintf(stderr, "kjob: no such process\n");
		return 1;
	}

	if(kill(child_proc_arr[child_no]->pid, sig_no) == -1)
	{
		perror("kjob");
		return 1;
	}

	return 0;
}

int stopjob(char* command)
{
	int token_count = countTokens(command, " ");
	if(token_count < 2)
	{
		fprintf(stderr, "stopjob: too few arguments\n");
		return 1;
	}
	if(token_count > 2)
	{
		fprintf(stderr, "stopjob: too many arguments\n");
		return 1;
	}

	int job_no = stringToInt(strchr(command, ' ') + 1);
	if(job_no == -1)
	{
		fprintf(stderr, "stopjob: invalid argument\n");
		return 1;
	}

	int child_no = getChildNumberFromJobNo(job_no);
	if(child_no == -1)
	{
		fprintf(stderr, "stopjob: no such process\n");
		return 1;
	}

	// stop background process
	if(kill(child_proc_arr[child_no]->pid, SIGTSTP) == -1)
	{
		perror("stopjob");
		return 1;
	}

	return 0;
}
