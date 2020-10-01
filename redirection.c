# include "headers.h"
# include "redirection.h"
# include "helperfunctions.h"

int redirectIO(char* delim, char* file)
{
    int fd;
    if(delim[0] == '>')
    {
        if(strlen(delim) == 1)
            fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        else
            fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);

        if(fd == -1)
        {
            fprintf(stderr, "shell: %s: %s\n", file, strerror(errno));
            return 1;
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
    else if(strcmp(delim, "<") == 0)
    {
        fd = open(file, O_RDONLY, 0);
        if(fd == -1)
        {
            fprintf(stderr, "shell: %s: %s\n", file, strerror(errno));
            return 1;
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
    return 0;
}

int redirectionHandler(char* input_command, char* first_token)
{
	char* command = strdup(input_command);
    char *cmd_copy = strdup(command), *cmd_ptr = command;
    char *token = strtok_r(cmd_ptr, "<>>", &cmd_ptr);
    char token_copy[strlen(cmd_copy)+1];
    
    strcpy(first_token, token);
    trim(first_token);

    char delim[2];
    int return_status = 0;

    while(1)
    {
        delim[0] = cmd_copy[token - command + strlen(token)];
        delim[1] = cmd_copy[token - command + strlen(token) + 1];

        token = strtok_r(cmd_ptr, "<>>", &cmd_ptr);
        if(token == NULL)
            break;

        // trim leading and trailing spaces (if any)
        strcpy(token_copy, token);
        trim(token_copy);

        if(delim[0] == '>')
        {
            if(delim[1] == '>')
                return_status = redirectIO(">>", token_copy);
            else
                return_status = redirectIO(">", token_copy);
        }
        else if(delim[0] == '<')
        {
            return_status = redirectIO("<", token_copy);
        }
        
        if(return_status == 1)
        {
            break;
        }
    }

    free(cmd_copy);
    free(command);
    return return_status;
}
