# include "headers.h"

int pinfo(char* str_id)
{
	int pid;

	if(str_id == NULL)
		pid = getpid();
	else
		pid = atoi(str_id);

	char path[30];
	sprintf(path, "/proc/%d/stat", pid);
	
	FILE* fp = fopen(path, "r");
	if(fp == NULL)
	{
		perror("pinfo");
		return 1;
	}

	printf("PID -- %d\n", pid);

	char info[PATH_MAX] = "";
	for(int i=0; i<23; i++)
	{
		fscanf(fp, "%s", info);
		if(i == 2)
			printf("Process Status -- %s\n", info);
		else if(i == 22)
			printf("Virtual Memory -- %s\n", info);
	}

	sprintf(path, "/proc/%d/exe", pid);
	
	memset(info, '\0', 100);
	if(readlink(path, info, PATH_MAX) == -1)
	{
		perror("pinfo");
		fclose(fp);
		return 1;
	}

	printf("Executable Path -- %s\n", info);

	fclose(fp);
	return 0;
}