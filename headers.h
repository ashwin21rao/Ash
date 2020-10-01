# ifndef _HEADERS_H_
# define _HEADERS_H_

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/dir.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/prctl.h>
# include <pwd.h>
# include <grp.h>
# include <limits.h>
# include <errno.h>
# include <time.h>
# include <signal.h>
# include <termios.h>

# define CLEAR "\033[2J"
# define HOME "\033[H"
# define BLUE "\033[1;34m"
# define GREEN "\033[1;32m"
# define RED "\033[1;31m"
# define CYAN "\033[1;36m"
# define RESET "\033[m"
# define MAX_HISTORY_SIZE 20

# endif
