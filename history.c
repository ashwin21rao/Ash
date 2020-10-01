# include "headers.h"
# include "globalvars.h"
# include "history.h"
# include "helperfunctions.h"

// GLOBLAL VARIABLES ACCESSED: int histsize, char* history_file_path, char** history_arr

int historySize()
{
	for(int i=0; i<MAX_HISTORY_SIZE; i++)
		if(history_arr[i] == NULL)
			return i;

	return MAX_HISTORY_SIZE;
}

void freeHistory()
{
	for(int i=0; i<histsize; i++)
		free(history_arr[i]);
}

void loadHistory()
{
	FILE* fp = fopen(history_file_path, "a+");
	if(fp == NULL)
	{
		fprintf(stderr, "shell: %s: %s", history_file_path, strerror(errno));
		return;
	}

	int pos = 0, rb;	
	char* line = NULL;
	size_t read_size = 0;
	while(1)
	{	
		// break if history file has been edited manually to have too many commands
		if(pos == MAX_HISTORY_SIZE)
			break;

		rb = getline(&line, &read_size, fp);
		if(rb == -1)
		{
			if(ferror(fp))
			{
				fprintf(stderr, "shell: %s: %s", history_file_path, strerror(errno));
				fclose(fp);
				return;
			}
			break;
		}
		history_arr[pos++] = strdup(line);
	}

	fclose(fp);
	free(line);
}

void writeHistory()
{
	FILE* fp = fopen(history_file_path, "w");
	if(fp == NULL)
	{
		fprintf(stderr, "shell: %s: %s", history_file_path, strerror(errno));
		return;
	}

	for(int i=0; i<histsize; i++)
		fprintf(fp, "%s", history_arr[i]);

	fclose(fp);
}

void updateHistory(char* command)
{	
	loadHistory();

	if(histsize != 0)
		if(strcmp(command, history_arr[0]) == 0)
			return;
	
	if(command[0] == ' ' || command[0] == '\t')
		return;

	if(histsize < MAX_HISTORY_SIZE)
		histsize++;
	
	for(int i=histsize-1; i>0; i--)
		history_arr[i] = strdup(history_arr[i-1]);
	
	history_arr[0] = strdup(command);

	writeHistory();
}

int history(char* command)
{
	int histnum;
	if(command == NULL)
		histnum = 10;
	else if(countTokens(command, " ") > 1)
	{
		fprintf(stderr, "history: too many arguments\n");
		return 1;
	}
	else if((histnum = stringToInt(command)) < 0)
	{
		fprintf(stderr, "history: %s: invalid argument\n", command);
		return 1;
	}

	histnum = ((histnum < histsize) ? histnum : histsize);

	for(int i=histnum-1; i>=0; i--)
		printf("%.2d: %s", histsize-i, history_arr[i]);

	return 0;
}
