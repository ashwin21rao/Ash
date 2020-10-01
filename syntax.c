# include "headers.h"
# include "syntax.h"
# include "helperfunctions.h"

int checkSyntaxErrors(char* input_command)
{
    char command[strlen(input_command)+1];
    strcpy(command, input_command);

    removeSpaces(command);
    int length = strlen(command);

    int delim_chars[] = {';', '&', '|', '<', '>'};
    int delim_num = sizeof(delim_chars) / sizeof(int);

    int delim_flags[] = {1, 3, 2, 1, 2};
    int error_flag = 1;

    // command can end only with ; or & (but not &&)
    if(find(delim_chars, command[length-1], delim_num) != -1)
    {
        if(command[length-1] != '&' && command[length-1] != ';')
            return 1;
        if(command[length-2] == '&')
            return 1;
    }

    for(int i=0; i<length; i++)
    {   
        // handles non delim characters
        if(find(delim_chars, command[i], delim_num) == -1)
        {
            if(command[i] != ' ')
            {
                error_flag = 0;
                for(int i=0; i<delim_num; i++)
                    delim_flags[i] = 0;
            }
            continue;
        }

        // handles >> which is valid
        if(command[i] == '>' && delim_flags[4] == 1)
        {
            if(command[i-1] == '>')
            {
                delim_flags[4]++;
                continue;
            }
        }

        // handles & && which is valid
        if(command[i] == '&')
        {
            // handles &&
            if(delim_flags[1] == 1 && command[i-1] == '&')
            {
                delim_flags[1]++;
                continue;
            }

            // handles & &&
            if(delim_flags[1] == 1 && command[i-1] == ' ' && command[i+1] == '&')
            {
                delim_flags[1]++;
                continue;   
            }
            if(delim_flags[1] == 2 && command[i-1] == '&' && command[i-2] == ' ')
            {
                delim_flags[1]++;
                continue;
            }
        }

        // handles & || which is valid
        if(command[i] == '|')
        {
            // handles ||
            if(delim_flags[2] == 1 && command[i-1] == '|')
            {
                delim_flags[2]++;
                continue;
            }

            // handles & ||
            if(delim_flags[1] == 1 && command[i-1] == ' ' && command[i+1] == '|')
            {
                delim_flags[2]++;
                continue;
            }
        }

        // handles successive delim characters 
        if(error_flag)
            return 1;

        error_flag = 0;
        for(int j=0; j<delim_num; j++)
        {
            if(command[i] == delim_chars[j])
                delim_flags[j] = 1;
            error_flag = (error_flag || delim_flags[j]); 
        }
    }
    return 0;
}
