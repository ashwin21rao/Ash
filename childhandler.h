# ifndef _CHILDHANDLER_H_
# define _CHILDHANDLER_H_

void addChild(int pid, char* command);
void getInfoFromFileSystem(struct child_proc_info* child_proc);
int getChildNumberFromPid(int pid);
int getChildNumberFromJobNo(int job_no);
void removeChild(int child_no);
void updateBackgroundChildStatus(int pid, int status);
int waitForForegroundChild(int child_no);
int killAllChildProcesses();

# endif