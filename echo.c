# include "headers.h"
# include "echo.h"

int echo(char* string)
{	
	if(string != NULL)
		printf("%s", string);
	printf("\n");

	return 0;
}
