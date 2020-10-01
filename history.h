# ifndef _HISTORY_H_
# define _HISTORY_H_

int historySize();
void freeHistory();
void loadHistory();
void writeHistory();
void updateHistory(char* command);
int history(char* command);

# endif