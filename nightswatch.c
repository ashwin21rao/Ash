# include "headers.h"
# include "nightswatch.h"
# include "helperfunctions.h"
# include "rawterminal.h"

// GLOBLAL VARIABLES ACCESSED: struct termios orig_termios

void findMostRecentProcess()
{
	FILE* fp = fopen("/proc/loadavg", "r");
	if(fp == NULL)
	{
		perror("nightswatch");
		exit(1);
	}

	int pid;
	fscanf(fp, "%*s %*s %*s %*s %d", &pid);
	printf("PID of most recently created process: %d\n", pid);
	fclose(fp);
}

void findCPUInterrupts(int printHeading)
{
	FILE* fp = fopen("/proc/interrupts", "r");
	if(fp == NULL)
	{
		perror("nightswatch");
		exit(1);
	}

	char line1[1000], line3[1000];
	fgets(line1, sizeof(line1), fp);	
	fgets(line3, sizeof(line3), fp);
	fgets(line3, sizeof(line3), fp);

	char* start_pos = strchr(line3, ':');
	char* end_pos = strrchr(line1, 'U');

    if(printHeading)
	   printf("%.*s\n", (int)((line3 - line1) + end_pos - start_pos + 2), line1 + (start_pos - line3 + 1));
	
    printf("%.*s\n", (int)((line3 - line1) + end_pos - start_pos + 2), line3 + (start_pos - line3 + 1));

	fclose(fp);
}

int nightswatch(char* args)
{
	// fork nightswatch process
	int id = fork();

	if(id < 0)
    {
        perror("nightswatch");
        return 1;
    }
    if(id > 0)
    {
        int c, status;
        enableRawMode();
        while(1)
        {
            while(!kbhit())
            {
                if(waitpid(id, &status, WNOHANG) > 0)
                {
                    disableRawMode();
                    if(WIFEXITED(status))
                        return WEXITSTATUS(status);
                    else
                        return 0;
                }
            }

            c = getInputChar();
            if(c == -1)
            {
                perror("nightswatch");
                kill(id, SIGKILL);
                waitpid(id, NULL, 0);
                break;
            }
            if(c == 'q')
            {
                kill(id, SIGKILL);
            }
        }

        disableRawMode();
        return 1;
    }

	int argc = countTokens(args, " ");
    char* argv[argc+1];

    int i = 0;
    char* token = strtok(args, " ");
    while(token != NULL)
    {
        argv[i++] = token;
        token = strtok(NULL, " ");
    }
    argv[i] = NULL;

    // check option flags
    int n_flag = 0, option;
    optind = 0;
    char* n_value = NULL;

    while((option = getopt(argc, argv, ":n:")) != -1)
    {
        if(option == 'n')
        {
            n_flag = 1;
            n_value = optarg;

            // handle non-integers and integers <= 0
            if(stringToInt(n_value) <= 0)
            {
            	fprintf(stderr, "nightswatch: invalid argument for -- '%c'\n", option);
            	exit(1);
            }
        }
        else if(option == ':')
        {
        	fprintf(stderr, "nightswatch: missing argument for -- '%c'\n", optopt);
        	exit(1);
        }
        else if(option == '?')
        {
            fprintf(stderr, "nightswatch: invalid option -- '%c'\n", optopt);
            exit(1);
        }
    }

    // check non-option arguments or too many arguments
    if(argc - optind == 0)
    {
    	fprintf(stderr, "nightswatch: missing argument\n");
    	exit(1);
    }
    if(argc - optind > 1)
    {
    	fprintf(stderr, "nightswatch: too many arguments\n");
    	exit(1);
    }

    char command;
    if(strcmp(argv[optind], "interrupt") == 0)
		command = 'i';
	else if(strcmp(argv[optind], "newborn") == 0)
		command = 'n';
	else
	{
		fprintf(stderr, "nightswatch: invalid argument\n");
    	exit(1);
	}

    int seconds = (n_flag ? stringToInt(n_value) : 1);
    int printHeading = 1;
    while(1)
    {
    	if(command == 'i')
        {
			findCPUInterrupts(printHeading);
            printHeading = 0;
        }
		else if(command == 'n')
			findMostRecentProcess();
    	sleep(seconds);
    }

    // exit from fork
    exit(0);
}
