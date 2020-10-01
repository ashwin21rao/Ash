# include "headers.h"
# include "globalvars.h"
# include "ls.h"
# include "helperfunctions.h"
# include "pwd.h"

long long int getTotalBlocks(DIR* dp, int a_flag)
{
    long long int total = 0;
    struct stat file_info;
    struct dirent* entry = readdir(dp);

    while(entry != NULL)
    {
        if(!a_flag && entry->d_name[0] == '.')
        {
            entry = readdir(dp);
            continue;
        }
        if(stat(entry->d_name, &file_info) == -1)
        {
            perror("ls");
            closedir(dp);
            exit(1);
        }
        total += file_info.st_blocks;
        entry = readdir(dp);
    } 
    return total / 2;
}

void printInfo(DIR* dp, char* file)
{
    struct stat file_info;
    if(stat(file, &file_info) == -1)
    {
        perror("ls");
        closedir(dp);
        exit(1);
    }

    // print permissions
    printf("%s", (S_ISDIR(file_info.st_mode))  ? "d" : "-");
    printf("%s", (file_info.st_mode & S_IRUSR) ? "r" : "-");
    printf("%s", (file_info.st_mode & S_IWUSR) ? "w" : "-");
    printf("%s", (file_info.st_mode & S_IXUSR) ? "x" : "-");
    printf("%s", (file_info.st_mode & S_IRGRP) ? "r" : "-");
    printf("%s", (file_info.st_mode & S_IWGRP) ? "w" : "-");
    printf("%s", (file_info.st_mode & S_IXGRP) ? "x" : "-");
    printf("%s", (file_info.st_mode & S_IROTH) ? "r" : "-");
    printf("%s", (file_info.st_mode & S_IWOTH) ? "w" : "-");
    printf("%s", (file_info.st_mode & S_IXOTH) ? "x" : "-");

    // print number of links
    printf(" %4.ld", file_info.st_nlink);

    // print owner name and group name
    struct passwd *p = getpwuid(file_info.st_uid);
    if(p == NULL)
    {
        perror("ls");
        closedir(dp);
        exit(1);
    }
    struct group *g = getgrgid(file_info.st_gid);   
    if(g == NULL)
    {
        perror("ls");
        closedir(dp);
        exit(1);
    } 
    printf(" %12s %12s", p->pw_name, g->gr_name);

    // print file size
    printf(" %12.ld", file_info.st_size);

    // print date and time of last modification
    time_t t = time(NULL);
    struct tm *time = localtime(&t);
    int current_year = time->tm_year;

    time = localtime(&(file_info.st_mtime));
    char timeInfo[20];
    if(time->tm_year == current_year)
        strftime(timeInfo, 20, "%b %d %H:%M", time);
    else
        strftime(timeInfo, 20, "%b %d  %Y", time);
    printf(" %s ", timeInfo);
}

int checkIfFile(char* file)
{
    struct stat file_info;
    if(stat(file, &file_info) == -1)
    {
        perror("ls");
        exit(1);
    }

    return !S_ISDIR(file_info.st_mode);
}

int checkIfExecutable(char* file)
{
    struct stat file_info;
    if(stat(file, &file_info) == -1)
    {
        perror("ls");
        exit(1);
    }

    return (file_info.st_mode & S_IXUSR) && 
           (file_info.st_mode & S_IXGRP) &&
           (file_info.st_mode & S_IXOTH);
}

void list(char* directory, int a_flag, int l_flag, int multiple)
{
    DIR* dp = opendir(directory);
    if(dp == NULL)
    {
        fprintf(stderr, "ls: %s: %s\n", directory, strerror(errno));
        exit(1);
    }

    if(multiple)
        printf("%s:\n", directory);

    if(l_flag)
        printf("total %lld\n", getTotalBlocks(dp, a_flag));

    rewinddir(dp);
    struct dirent* entry = readdir(dp);
    while(entry != NULL)
    {
        if(!a_flag && entry->d_name[0] == '.')
        {
            entry = readdir(dp);
            continue;
        }

        if(l_flag)
            printInfo(dp, entry->d_name); 

        if(checkIfFile(entry->d_name))
        {
            if(checkIfExecutable(entry->d_name))
                printf(GREEN "%s\n" RESET, entry->d_name);
            else
                printf("%s\n", entry->d_name);
        }
        else
            printf(BLUE "%s\n" RESET, entry->d_name);
        
        entry = readdir(dp);
    } 

    closedir(dp);
}

int ls(char* args)
{
    // fork ls process
    int id = fork();
    
    if(id < 0)
    {
        perror("ls");
        return 1;
    }
    if(id > 0)
    {
        int status;
        waitpid(id, &status, 0);
        return WEXITSTATUS(status);
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
    int a_flag = 0, l_flag = 0, option;
    optind = 0;
    while((option = getopt(argc, argv, ":la")) != -1)
    {
        if(option == 'a')
            a_flag = 1;
        else if(option == 'l')
            l_flag = 1;
        else if(option == '?')
        {
            fprintf(stderr, "ls: invalid option -- '%c'\n", optopt);
            exit(1);
        }
    }

    // check non-option arguments
    if(argc - optind == 0)
        list(cwd, a_flag, l_flag, 0);

    // single or multiple directories
    char path[PATH_MAX];
    for(int i=optind; i<argc; i++)
    {
        strcpy(path, argv[i]);
        if(path[0] == '~')
        {
            char new_path[PATH_MAX];
            strcpy(new_path, home);
            strcat(new_path, path + 1);
            strcpy(path, new_path);
        }
        if(chdir(path) == -1)
        {
            perror("ls");
            exit(1);
        }
        setPwd(path);
        list(path, a_flag, l_flag, (argc - optind > 1));
        chdir(cwd);

        if(argc - optind > 1 && i != argc-1)
            printf("\n");
    }

    // return from fork
    exit(0);
}
