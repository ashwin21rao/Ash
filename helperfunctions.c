# include "headers.h"
# include "helperfunctions.h"

int stringToInt(char* str)
{
	int length = strlen(str);
	for(int i=0; i<length; i++)
	{
		if(!(str[i] >= '0' && str[i] <= '9'))
			return -1;
	}
	
	char* end;
	return (int)strtol(str, &end, 10);
}

void getProcessNameFromID(char* pid, char* name)
{
	char path[30];
	sprintf(path, "/proc/%s/comm", pid);
	
	printf("%s\n", path);
	FILE* fp = fopen(path, "r");
	if(fp == NULL)
	{
		perror("Error");
		return;
	}
	fscanf(fp, "%[^\n]s", name);
	
	fclose(fp);
}

int countTokens(char* input, char* c)
{
	char str[strlen(input)+1];
	strcpy(str, input);

	int count = 0;
	char* token = strtok(str, c);
	while(token != NULL)
		count++, token = strtok(NULL, c);

	return count;
}

void trim(char* str1)
{
	int length = strlen(str1);
	char str2[length+1];
	strcpy(str2, str1);

	int i;
	for(i=0; i<length; i++)
	{
		if(str2[i] != ' ')
			break;
	}
	for(int j=length-1; j>=0; j--)
	{
		if(str2[j] != ' ')
		{
			if(j != length-1)
				str2[j+1] = '\0';
			break;
		}
	}

	strcpy(str1, str2+i);
}

void removeSpaces(char* str1)
{
	int j = 0, flag = 0;
	int length = strlen(str1);

	char str2[length+1];
	strcpy(str2, str1);

	for(int i=0; i<length; i++)
		if(str1[i] == '\t' || str1[i] == '\n')
			str1[i] = ' ';

	for(int i=0; i<length-1; i++)
	{	
		if(!flag && str1[i] != ' ')
			flag = 1;
		if(flag && (str1[i] != ' ' || (str1[i] == ' ' && str1[i+1] != ' ')))
			str2[j++] = str1[i];
	}
	if(str1[length-1] != ' ')
		str2[j++] = str1[length-1];
	str2[j] = '\0';

	strcpy(str1, str2);
}

int find(int* arr, int ele, int len)
{
	for(int i=0; i<len; i++)
	{
		if(arr[i] == ele)
			return i;
	}
	return -1;
}
