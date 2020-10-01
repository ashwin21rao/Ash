# include "headers.h"
# include "globalvars.h"
# include "getcommands.h"
# include "helperfunctions.h"
# include "interpretcommands.h"
# include "history.h"
# include "childhandler.h"
# include "syntax.h"

int logicalChain(char* command)
{
    char *token;
    char *cmd_ptr = command;
        
    char *subtoken, *tok_ptr, *tok_copy;

    int background;
    int EXIT_STATUS = 0;
    char *delim_ptr1 = cmd_ptr, *delim_ptr2 = cmd_ptr;
    char *end_ptr, *prev_ptr = cmd_ptr;

    // split into commands delimited by && (AND) or || (OR)
    // AND: execute next command if previous command is successful
    // OR: execute next command if previous command is unsuccessful
    while(1)
    {
        delim_ptr1 = strstr(cmd_ptr, "&&");
        delim_ptr2 = strstr(cmd_ptr, "||");

        if(delim_ptr1 != NULL && delim_ptr2 != NULL)
        {
            if(delim_ptr1 < delim_ptr2)
                end_ptr = delim_ptr1, *end_ptr = '\0';
            else
                end_ptr = delim_ptr2, *end_ptr = '\0';
        }
        else if(delim_ptr1 != NULL)
            end_ptr = delim_ptr1, *end_ptr = '\0';
        else if(delim_ptr2 != NULL)
            end_ptr = delim_ptr2, *end_ptr = '\0';
        else
            end_ptr = NULL;

        if(!((*prev_ptr == '&' && EXIT_STATUS == 1) || (*prev_ptr == '|' && EXIT_STATUS == 0)))
        {
            // split into commands delimited by &
            token = cmd_ptr;
            trim(token);
                
            tok_ptr = token, tok_copy = strdup(token);
            while((subtoken = strtok_r(tok_ptr, "&", &tok_ptr)) != NULL)
            {
                background = (tok_copy[subtoken - token + strlen(subtoken)] == '&');
                trim(subtoken);
                EXIT_STATUS = pipeHandler(subtoken, background);
            }
        }

        if(end_ptr == NULL)
            break;

        cmd_ptr = end_ptr + 2;
        prev_ptr = end_ptr + 1;
    }

    free(tok_copy);
    return EXIT_STATUS;
}

int getCommand()
{
    // char command[1000];
    // fgets(command, sizeof(command), stdin);

    int EXIT_STATUS = 0;

    char* command = NULL;
    size_t read_size = 0;
    if(getline(&command, &read_size, stdin) == -1)
    {
        if(ferror(stdin))
        {
            perror("Error while reading command");
            return 1;
        }
        if(feof(stdin))
        {
            freeHistory();
            if(killAllChildProcesses() == -1)
            {
                exit(1);
            }

            printf("\nThanks for using my shell! :)\n");
            exit(0);
        }       
    }

    // update history
    if(strcmp(command, "\n") != 0)
        updateHistory(command);

    char *token;
    char *cmd_ptr = command;

    if(command != NULL && checkSyntaxErrors(command))
    {
    	fprintf(stderr, "shell: syntax error\n");
    	return 1;
    }

    // split into commands delimited by ;
    while((token = strtok_r(cmd_ptr, ";", &cmd_ptr)) != NULL)
    {               
        // remove extra spaces and \n
        removeSpaces(token);

        // empty command
        if(strlen(token) == 0)
        {
            continue;
        }

        EXIT_STATUS = logicalChain(token);
    }

    free(command);
    return EXIT_STATUS;
}
