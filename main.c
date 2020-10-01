# include "headers.h"
# include "globalvars.h"
# include "prompt.h"
# include "pwd.h"

char cwd[PATH_MAX], home[PATH_MAX], prev_wd[PATH_MAX];

struct child_proc_info *child_proc_arr[100];
int child_proc_num = 0;

int main()
{
	printf("%s%s", CLEAR, HOME);
	setPwd(home), setPwd(cwd);

	prompt();

	return 0;
}
