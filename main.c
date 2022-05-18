#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include "./libft/libft.h"

DIR *dp;
struct dirent *dirp;
char *dir_to_open;
const char *available_options = "lRart";
int options;
t_list *args;
t_list *dirs;

typedef enum e_options
{
	l = 1 << 0,
	R = 1 << 1,
	a = 1 << 2,
	r = 1 << 3,
	t = 1 << 4
}				t_options;

//else if(params.flags & n) //bonus -n

void add_option(char *opt_arg)
{
    printf("options: %s\n", opt_arg);

    for (int i = 1; i < ft_strlen(opt_arg); i++)
    {
        if (!ft_strchr(available_options, opt_arg[i]))
        {
            printf("ls: invalid option -- '%c'\n", opt_arg[i]);
            exit(2);
        }
        else
        {
            switch(opt_arg[i])
            {
                case 'l': options |= l; break;
                case 'R': options |= R; break;
                case 'a': options |= a; break;
                case 'r': options |= r; break;
                case 't': options |= t; break;
            }
        }
    }
    //printf("%d\n", options);
}

void parse(int ac, char **av)
{
    if(ac == 1)
        dir_to_open = "./";
    else
    {
        for(int i = 1; i < ac; i++)
        {
            if(av[i][0] == '-')
                add_option(av[i]);
            else
            {
                t_list new;
                new.next = NULL;
                new.content = av[i];
                printf("argument: %s\n", av[i]);
                ft_lstadd_back(&args, &new);
            }
        }
    }

    if(ft_lstsize(args) == 0)
    {
        printf("empty args\n");
        dir_to_open = "./";
    }
}

int main(int ac, char *av[])
{
    parse(ac, av);
    /*
    if (ac != 2)
    {
        fprintf(stderr, "Usage: ./program directory_name\n");
        exit(EXIT_FAILURE);
    }
    */

    errno = 0;
    printf("%s is dir to open\n", dir_to_open);
    if ((dp = opendir(dir_to_open)) == NULL)
    {
        switch (errno)
        {
        case EACCES:
            printf("Permission denied\n");
            break;
        case ENOENT:
            printf("Directory does not exist\n");
            break;
        case ENOTDIR:
            printf("'%s' is not a directory\n", av[1]);
            break;
        }
        exit(EXIT_FAILURE);
    }

    struct stat mystat;
    char buf[512];

    errno = 0;
    printf("total\n");

    while ((dirp = readdir(dp)) != NULL)
        {
            struct stat mystat;
            if(dirp->d_name[0] == '.' && !(options & a))
                ;
            else
            {
                /*
                if(dirp->d_type == DT_REG)
                    printf("its a file:");
                if (dirp->d_type ==DT_DIR)
                    printf("its a dir:");
                */
                //printf("type: %d\n", dirp->d_type);
                //printf("%s\n", dirp->d_name);

                stat(dirp->d_name, &mystat);


                printf((S_ISDIR(mystat.st_mode)) ? "d" : "-");
                printf((mystat.st_mode & S_IRUSR) ? "r" : "-");
                printf((mystat.st_mode & S_IWUSR) ? "w" : "-");
                printf((mystat.st_mode & S_IXUSR) ? "x" : "-");
                printf((mystat.st_mode & S_IRGRP) ? "r" : "-");
                printf((mystat.st_mode & S_IWGRP) ? "w" : "-");
                printf((mystat.st_mode & S_IXGRP) ? "x" : "-");
                printf((mystat.st_mode & S_IROTH) ? "r" : "-");
                printf((mystat.st_mode & S_IWOTH) ? "w" : "-");
                printf((mystat.st_mode & S_IXOTH) ? "x" : "-");
                char *date = ctime(&mystat.st_mtime);
               // printf("date: %s\n\n", date);
                char **trimmed_date = ft_split(date, ' ');
                char *trim = ft_strjoin(ft_add_char(trimmed_date[1], ' '), ft_add_char(trimmed_date[2], ' '));
                char *trim2 = ft_strjoin(trim, trimmed_date[3]);
                char *u = ft_substr(trim2, 0, ft_strlen(trim2)- 3);
                struct passwd *test = getpwuid(mystat.st_uid);
                struct group *grp = getgrgid(mystat.st_gid);
                printf(" %ld %s %s %5ld %s %s %ld\n", mystat.st_nlink, test->pw_name, grp->gr_name, mystat.st_size, u, dirp->d_name, mystat.st_blocks /2);

                //printf("The file %s a symbolic link\n", (S_ISLNK(mystat.st_mode)) ? "is" : "is not");
            }
        }

    if (errno != 0)
    {
        if (errno == EBADF)
            printf("Invalid directory stream descriptor\n");
        else
            perror("readdir");
    }
    else
    {
        printf("End-of-directory reached\n");
    }

    if (closedir(dp) == -1)
        perror("closedir");

    exit(EXIT_SUCCESS);
}