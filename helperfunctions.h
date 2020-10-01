# ifndef _HELPERFUNCTIONS_H_
# define _HELPERFUNCTIONS_H_

int stringToInt(char* str);
void getProcessNameFromID(char* pid, char* name);
int countTokens(char* input, char* c);
void trim(char* str1);
void removeSpaces(char* str1);
int find(int* arr, int ele, int len);

# endif