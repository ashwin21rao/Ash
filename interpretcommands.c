# include "headers.h"
# include "globalvars.h"
# include "interpretcommands.h"
# include "helperfunctions.h"
# include "cd.h"
# include "pwd.h"
# include "echo.h"
# include "ls.h"
# include "pinfo.h"
# include "nightswatch.h"
# include "history.h"
# include "setenv.h"
# include "externalapi.h"
# include "childhandler.h"
# include "jobhandler.h"
# include "redirection.h"

void closePipes(int fd[][2], int pipe_num)
{
    for(int i=0; i<pipe_num; i++)
    {
        close(fd[i][0]);
        close(fd[i][1]);
    }
}

int pipeHandler(char* command, int background)
{
    char *cmd_ptr = command;
    char *token;
    int pipe_num = countTokens(command, "|") - 1;
    int pids[pipe_num+1];

    if(pipe_num > 0 && background)
    {
        fprintf(stderr, "shell: background piping not supported\n");
        return 1;
    }

    // no piping
    if(pipe_num == 0)
    {
        return interpretCommand(command, background);
    }

    // pipe_num number of pipes run concurrently
    int fd[pipe_num][2];
    for(int i=0; i<pipe_num; i++)
    {
        if(pipe(fd[i]) == -1)
        {
            closePipes(fd, pipe_num);
            return 1;
        }
    }

    for(int i=0; i<=pipe_num; i++)
    {
        token = strtok_r(cmd_ptr, "|", &cmd_ptr);
        
        // trim leading and trailing spaces (if any)
        trim(token);

        int pid = fork();
        pids[i] = pid;
        if(pid == -1)
        {
            closePipes(fd, pipe_num);
            return 1;
        }
        if(pid == 0)
        {
            if(i > 0)
                dup2(fd[i-1][0], STDIN_FILENO);
            if(i != pipe_num)
                dup2(fd[i][1], STDOUT_FILENO);

            closePipes(fd, pipe_num);
            int exit_status = interpretCommand(token, background);
            exit(exit_status);
        }
    }

    closePipes(fd, pipe_num);

    int status;
    for(int i=0; i<pipe_num+1; i++)
    {
        waitpid(pids[i], &status, 0);
    }
    return WEXITSTATUS(status);
}

int interpretCommand(char* command, int background)
{   
    int EXIT_STATUS = 0;

    char command_copy[strlen(command)+1];
    char* cmd = strtok(strcpy(command_copy, command), " <>>");
    char first_token[strlen(command)+1], *second_token;

    // handle redirection if any
    int stdout_fd = dup(STDOUT_FILENO);
    int stdin_fd = dup(STDIN_FILENO);
    if(redirectionHandler(command, first_token) == 1)
    {
        return 1;
    }
    second_token = strchr(first_token, ' ');
    second_token = (second_token == NULL) ? second_token : second_token+1;

    // interpret command
    if(strcmp(cmd, "exit") == 0 || strcmp(cmd, "quit") == 0)
    {
        freeHistory();
        if(killAllChildProcesses() == 1)
            exit(1);
        
        printf("Thanks for using my shell! :)\n");
        exit(0);
    }
    if(strcmp(cmd, "pwd") == 0) {
        printf("%s\n", cwd);
    }
    else if(strcmp(cmd, "echo") == 0) {
        EXIT_STATUS = echo(second_token);
    }
    else if(strcmp(cmd, "cd") == 0) {
        EXIT_STATUS = cd(second_token);
    }
    else if(strcmp(cmd, "ls") == 0) {
        EXIT_STATUS = ls(first_token);
    }
    else if(strcmp(cmd, "pinfo") == 0) {
        EXIT_STATUS = pinfo(second_token);
    }
    else if(strcmp(cmd, "nightswatch") == 0) {
        EXIT_STATUS = nightswatch(first_token);
    }
    else if(strcmp(cmd, "history") == 0) {
        EXIT_STATUS = history(second_token);
    }
    else if(strcmp(cmd, "setenv") == 0) {
        EXIT_STATUS = setEnvironVar(first_token);
    }
    else if(strcmp(cmd, "unsetenv") == 0) {
        EXIT_STATUS = unsetEnvironVar(first_token);
    }
    else if(strcmp(cmd, "getenv") == 0) {
        EXIT_STATUS = getEnvironVar(first_token);
    }
    else if(strcmp(cmd, "overkill") == 0) {
        EXIT_STATUS = killAllChildProcesses();
    }
    else if(strcmp(cmd, "jobs") == 0) {
        EXIT_STATUS = jobs();
    }
    else if(strcmp(cmd, "bg") == 0) {
        EXIT_STATUS = bg(first_token);
    }
    else if(strcmp(cmd, "fg") == 0) {
        EXIT_STATUS = fg(first_token);
    }
    else if(strcmp(cmd, "kjob") == 0) {
        EXIT_STATUS = kjob(first_token);
    }
    else if(strcmp(cmd, "stopjob") == 0) {
        EXIT_STATUS = stopjob(first_token);
    }
    else {
        EXIT_STATUS = runExternalProcess(command, background);
    }

    // undo file redirection if any
    dup2(stdout_fd, STDOUT_FILENO);
    dup2(stdin_fd, STDIN_FILENO);
    close(stdout_fd), close(stdin_fd);

    return EXIT_STATUS;
}
