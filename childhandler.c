# include "headers.h"
# include "globalvars.h"
# include "childhandler.h"

void addChild(int pid, char* command)
{
	child_proc_arr[child_proc_num] = (struct child_proc_info*)malloc(sizeof(struct child_proc_info));
	child_proc_arr[child_proc_num]->pid = pid;
	child_proc_arr[child_proc_num]->job_no = ((child_proc_num == 0) ? 1 : (child_proc_arr[child_proc_num-1]->job_no + 1));
	strcpy(child_proc_arr[child_proc_num]->command, command);
	getInfoFromFileSystem(child_proc_arr[child_proc_num]);
	child_proc_num++;
}

void getInfoFromFileSystem(struct child_proc_info* child_proc)
{
	int pid = child_proc->pid;
	char status;

	char path[30];
	sprintf(path, "/proc/%d/stat", pid);
	
	FILE* fp = fopen(path, "r");
	
	// process has already been reaped
	if(fp == NULL)
		return;

	fscanf(fp, "%*s %s %c", child_proc->name, &status);
	
	if(status == 'R' || status == 'S')
		strcpy(child_proc->state, "Running");
	else if(status == 'T')
		strcpy(child_proc->state, "Stopped");
	else
		strcpy(child_proc->state, "Killed");

	fclose(fp);
}

int getChildNumberFromPid(int pid)
{
	int pos = 0;
	while(child_proc_arr[pos]->pid != pid)
	{
		pos++;

		// not a child process
		if(pos == child_proc_num)
			return -1;
	}
	return pos;
}

int getChildNumberFromJobNo(int job_no)
{
	int pos = 0;
	while(child_proc_arr[pos]->job_no != job_no)
	{
		pos++;

		// not a child process
		if(pos == child_proc_num)
			return -1;
	}
	return pos;
}

void removeChild(int child_no)
{
	// not a child process
	if(child_no < 0 || child_no > child_proc_num-1)
		return;

	for(int i=child_no+1; i<child_proc_num; i++)
	{
		*child_proc_arr[i-1] = *child_proc_arr[i];
	}

	free(child_proc_arr[child_proc_num-1]);
	child_proc_arr[child_proc_num-1] = NULL;
	child_proc_num--;
}

void updateBackgroundChildStatus(int pid, int status)
{
	int child_no = getChildNumberFromPid(pid);
	if(child_no == -1)
		return;

	if(WIFEXITED(status))
	{
		int exit_code = WEXITSTATUS(status);
		if(exit_code == 0)
			fprintf(stderr, "[%d] Exited normally (with status %d) %s [%d]\n", child_proc_arr[child_no]->job_no, exit_code, child_proc_arr[child_no]->command, pid);
		else
			fprintf(stderr, "[%d] Exited abnormally (with status %d) %s [%d]\n", child_proc_arr[child_no]->job_no, exit_code, child_proc_arr[child_no]->command, pid);

		removeChild(child_no);
	}
	else if(WIFSIGNALED(status))
	{
		fprintf(stderr, "[%d] Killed (by signal %d) %s [%d]\n", child_proc_arr[child_no]->job_no, WTERMSIG(status), child_proc_arr[child_no]->command, pid);
		removeChild(child_no);
	}
	else if(WIFSTOPPED(status))
	{
		fprintf(stderr, "[%d] Stopped %s [%d]\n", child_proc_arr[child_no]->job_no, child_proc_arr[child_no]->command, pid);
		getInfoFromFileSystem(child_proc_arr[child_no]);
	}
	else if(WIFCONTINUED(status))
	{
		fprintf(stderr, "[%d] Resumed %s [%d]\n", child_proc_arr[child_no]->job_no, child_proc_arr[child_no]->command, pid);
		getInfoFromFileSystem(child_proc_arr[child_no]);
	}
}

int waitForForegroundChild(int child_no)
{	
	int pid = child_proc_arr[child_no]->pid;
	int status, exit_status;

	// bring process to foreground and resume it if stopped
	if(tcsetpgrp(STDERR_FILENO, getpgid(pid)) == -1)
	{
		perror("shell");
		kill(pid, SIGKILL);
		removeChild(child_no);
		exit_status = 1;
	}
	else 
		kill(pid, SIGCONT);
	
	// wait for foreground process to finish
	waitpid(pid, &status, WUNTRACED);
	if(WIFEXITED(status))
	{
		removeChild(child_no);
		exit_status = WEXITSTATUS(status);
	}
	else if(WIFSIGNALED(status))
	{
		removeChild(child_no);
		exit_status = (WTERMSIG(status) == SIGINT) ? 0 : 1;	
	}
	else if(WIFSTOPPED(status))
	{
		exit_status = 1;	
		fprintf(stderr, "[%d] Stopped %s [%d]\n", child_proc_arr[child_no]->job_no, child_proc_arr[child_no]->command, pid);
		getInfoFromFileSystem(child_proc_arr[child_no]);
	}
	
	// give foreground access back to parent (shell)
	tcsetpgrp(STDERR_FILENO, getpgid(getpid()));

	// return exit status of foreground process
	return exit_status;
}

int killAllChildProcesses()
{
	int return_value = 0;
	for(int i=0; i<child_proc_num; i++)
	{
		if(kill(child_proc_arr[i]->pid, SIGKILL) == -1)
		{
			perror("shell");
			return_value = 1;
		}
	}

	return return_value;
}
