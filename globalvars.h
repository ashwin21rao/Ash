# ifndef _GLOBALVARS_H_
# define _GLOBALVARS_H_

extern char cwd[];
extern char home[];
extern char prev_wd[];

extern int histsize;
extern char* history_arr[];
extern char history_file_path[];

struct termios orig_termios;

struct child_proc_info
{
	int job_no;
	int pid;
	char state[20];
	char command[PATH_MAX];
	char name[20];
};
extern struct child_proc_info *child_proc_arr[];
extern int child_proc_num;

# endif