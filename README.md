COMPILING AND RUNNING THE SHELL

1) Use the command "make" to execute the makefile. This creates an executable called "shell" in the same directory.
2) Run ./shell to execute the shell.

FILES AND THEIR OVERVIEW

1) shell.c
	This contains the main function of the program.

2) headers.h
	Contains all required header files and macro definitions.

3) globalvars.h
	This contains all global variables.

4) prompt.h and prompt.c
	Conatins the infinite shell loop for inputting commands and displaying prompt.

5) getcommands.h and getcommands.c
	Used to input the command. Handles logical chaining of commands with AND (&&) and OR (||). Calls interpretcommands.c for interpretation.

6) syntax.h and syntax.c
	Checks for syntax errors in the input.

7) interpretcommands.h and interpretcommands.c
	Handles piping. Interprets the command and performs the required action on it.

8) redirection.h and redirection.c
	Handles I/O redirection.

9) pwd.h and pwd.c
	Resets the present working directory.

10) echo.h and echo.c
	Implementation of echo command (shell builtin).

11) cd.h and cd.c
	Implementation of cd command (shell builtin).

12) ls.h and ls.c
	Implementation of ls command with support for flags -l and -a.

13) pinfo.h and pinfo.c
	Implementation of pinfo command. Reads from /proc/[pid]/stat file.

14) nightswatch.h and nightswatch.c
	Implementaion of nightswatch command with 2 options: interrupt and newborn. Default time interval is 1 second.

15) history.h and history.c
	Implements displaying history, updating history, loading history at shell start, and writing history to the /home/[username]/.shell_history file.

16) setenv.h and setenv.c
	Implements adding and removing environment variables of the shell. Includes setenv, unsetenv and getenv.

17) externalapi.h and externalapi.c
	Implements running all external processes, including support for both background and foreground processes. Ex: cat, vim, grep etc. Uses the execvp command. Also checks the exit status of background processes.

18) childhandler.h and childhandler.c
	Implements the API for adding, updating and removing background child processes. Also handles sending shell to background during a foreground processes. 

19) jobhandler.h and jobhandler.c
	Implements functions related to job control: jobs, fg, bg, kjob and stopjob.

20) helperfunctions.h and helperfunctions.c
	Implements various helper fucntions used in other files, including converting a string to integer, counting the number of tokens in a string, and removing extra whitespaces from a string.

21) rawterminal.h and rawterminal.c
	Changes the mode of the terminal from canonical mode to raw mode, so that characters are input as soon as they are typed. This functionality is used to terminate nightswatch on pressing 'q' without having to press enter.

22) makefile
	This is the makefile to compile the entire shell (name of executable created: shell).

23) SHELL_EXECUTABLE
	A complete sample executable of the shell obtained by executing the makefile.


SHELL DESIGN CHOICES

1) The shell has been designed to replicate bash as far as possible.

2) Multiple statements on the same line are delimited using & (for background processes) or ; (for foreground processes). Logical chaining of commands is done using && (AND) and || (OR). Syntax errors are handled. 

3) The ls command supports functionality only for directories. It displays directories in blue and executables in green. 

4) Like bash, commands starting with a space or tab are not stored in history. A single command repeated multiple times in succession is stored in history only once. Requesting more history than available prints the maximum amount of history available. By default 10 lines of history are printed. A maximum of 20 lines of history are stored. History of commands from multiple instances of the shell is handled. This can be easily changed to support history only from the last shell which exits (like bash).

5) Piping and redirection is handled for both builtin and non-builtin processes.

6) Multiple I/O redirections in a single statement is handled like bash. If there are multiple output redirections, all files are created (if they do not exist) and truncated (if > is used) and the output is redirected into the last file. If there are multiple input redirections, the contents of the last file are redirected while other files are ignored. 

7) Piping of multiple commands is supported. Piping of processes in the background is not supported.

8) Foreground processes can be terminated using ctrl+C and stopped using ctrl+Z. Stopped processes can be resumed at a later stage.

9) Only non-builtin commands can be sent into the background. A background process can be brought back into the foreground, or stopped/resumed while remaining in the background.

10) The shell automatically kills all its child processes (if any) on exit (using the exit/quit command or ctrl+D).
