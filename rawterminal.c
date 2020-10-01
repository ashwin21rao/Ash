# include "headers.h"
# include "globalvars.h"
# include "rawterminal.h"

void disableRawMode() 
{
	tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

void enableRawMode() 
{
	tcgetattr(STDIN_FILENO, &orig_termios);
	atexit(disableRawMode);
	
	struct termios raw = orig_termios;
	raw.c_lflag &= ~(ECHO | ICANON | OPOST);
	
	tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

int kbhit()
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

int getInputChar()
{
    int r;
    unsigned char c;
    if((r = read(STDIN_FILENO, &c, sizeof(c))) < 0)
        return r;
    else 
        return c;
}
