# Linux Shell in C

## Introduction

This is an implementation of a Linux shell written in C.  


## Running the shell

1. Clone this directory and `cd` into it.
2. Run the command `make`.
3. Run `./shell` to get a prompt of the form `username@system_name:path$`.
4. You are now in the shell! 
5. Exit the shell by running `exit` or `quit` or pressing `CTRL-D`.

## Shell features

### Built-in commands

These are shell builtins.

1. `pwd`
    
    - Displays the present working directory.  
    - Implemented in [pwd.c](pwd.c)  

2. `cd <directory>`
    
    - Switches to the specified directory.  
    - Supports flags `cd, cd . , cd .. , cd ~, cd -`.  
    - `cd -` switches to the previous working directory and prints its path.
    - Implemented in [cd.c](cd.c)  

3. `ls [-l -a] [directory list]`
    
    - Lists all the files and directories in the specified directory/directories.  
    - Supports flags `ls, ls . , ls .. , ls ~`.  
    - Highlights directories in blue and executable files in green.  
    - Implemented in [ls.c](ls.c)  

4. `echo [message]`
    
    - Displays message verbatim.  
    - Does not handle escape characters or double quotes.  
    - Implemented in [echo.c](echo.c)  

5. `exit` and `quit` 

    - Exits the shell with exit code 0 (success). Kills child processes of the shell, if any.

6. `history [value]`

    - Displays shell history (last `value` commands). If `value` is unspecified, displays the last 10 commands.  
    - Displays a maximum of 20 commands.  
    - History stored in `.shell-history` which is stored in the `/home/username` directory.  
    - Implemented in [history.c](history.c)  

7. `setenv var [value]`, `unsetenv var` and `getenv var` 

    - `setenv var [value]` creates an enviornment variable `var` if it doesn't exist and assigns it the provided value, else overwrites its existing value.
    - `unsetenv var` destroys the environment variable `var`.  
    - `getenv var` displays the value of the ebnironment variable `var`.  
    - Implemented in [setenv.c](setenv.c)  
    
8.  `jobs`

    - Prints a list of all currerntly running background processes in order of their creation times, along with their job number, process ID and state.  
    - The state of the job may be on of the following: Running, Stopped or Killed.  
    - Implemented in [jobhandler.c](jobhandler.c)  

9. `kjob <job number> <signal number>` and `stopjob <job number>
    
    - `kjob <job number> <signal number>` takes the `job number` of a running job and sends the signal corresponding to `signal number` to that process.
    - `stopjob <job number>` stops the running background job corresponding to `job number`.     
    - Implemented in [jobhandler.c](jobhandler.c)  

10. `fg <job number>`
    
    - Brings a running or a stopped background job corresponding to `job number` to the foreground, and changes its state to `Running`.  
    - Implemented in [jobhandler.c](jobhandler.c)

11. `bg <job number>`

    - Changes the state of a stopped background job to running (in the background).  
    - Implemented in [jobhandler.c](jobhandler.c)  

12. `overkill`

    - Kills all background process at once.  
    - Implemented in [childhandler.c](childhandler.c)  

### Additional Commands

1. `pinfo [pid]`

    - Prints details about the process specified by `pid` including its pid, status, memory, and executable path.  
    - If `pid` is unspecified, details of the shell are printed.  
    - Implemented in [pinfo.c](pinfo.c)  

2. `nightswatch -n [seconds] [interrupt/newborn]`

    - The `interrupt` option prints the number of times the CPU has been interrupted by the keyboard.  
    - the `newborn` option prints the process ID of the most recently created process.  
    - Prints every `n` seconds. If not specified, prints every second.  
    - Exits when `q` is pressed (without enter).  
    - Implemented in [nightswatch.c](nightswatch.c)

### Foreground and Background Processes

- Commands other than shell builtins are treated as external commands.
- Only non-builtin commands can be sent to the background.
- To run a process in the background, follow the command with a `&` symbol. Eg. `vim &`.
- Multiple commands can be written in the same line separated by a `;` for foreground execution or `&` for background execution. Eg. `vim & ps; echo hello`   

### Command Piping and Input/Output Redirection

- Input/Output redirection can be done for both foreground and background processes using the symbols `<`, `>`, and `>>`.  
- Multiple commands can be piped using the pipe operator `|`.  
- Input/Output redirection can be done within command pipelines. Eg. `cat < in.txt | wc -l > out.txt`.  
- Piping in the background is not supported.  

### Signal Handling and EOF

1. `CTRL-Z`

    - Changes the state of a foreground process from Running to Stopped and pushes it into the background.

2. `CTRL-C` and `CTRL-\`

    - Send SIGINT and SIGQUIT signals respectively to the current foreground job of the shell, thereby terminating it.  
    - If there is no foreground job, the signal has no effect.  
    
3. `CTRL-D`

    - Sends an EOF marker thereby terminating the shell.  

### Logical Chaining of Commands

- Multiple commands can be chained using the logical operators `&&` (AND) and `||` (OR).  
- These operators short-circuit, and execute their second operands only if needed to evaluate their truth value.    
- `&&` executes the next command only if the previous command exited normally.  
- `||` executes the next command only if the previous command exited abnormally.  

### Exit codes
- The shell receives exit codes of all its child processes.  
- `:)` before the prompt indicates that the previous command exited normally.  
- `:(` before the prompt indicates that the previous command exited abnormally.  

## Coding style

- Function declarations are present in `.h` files and definitions in the corresponding `.c` files.  
- All included header files and macro definitions are present in `headers.h` and all global variables in `globalvars.h`.  
- SHELL_EXECUTABLE contains a working executable of the shell, created using the same steps as described above.  
