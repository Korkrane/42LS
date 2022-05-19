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
} t_options;

typedef struct s_dirs
{
    char *dir_name;
    char *path;
    t_list *content;
    int lvl;
    int total; // total for ls -l
    int size;  // for padding
    struct s_dirs *next;
    struct s_dirs *sub_dir;
} t_dirs;
// TODO
//  order asc

typedef struct s_ls
{
    t_dirs *dirs;
    t_list *content;
} t_ls;

t_ls ls;
// else if(params.flags & n) //bonus -n

t_dirs *dirnew()
{
    t_dirs *new;

    new = (t_dirs *)malloc(sizeof(t_dirs));
    if (!new)
        return (NULL);
    // new->content = content;
    new->next = NULL;
    return (new);
}

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
            switch (opt_arg[i])
            {
            case 'l':
                options |= l;
                break;
            case 'R':
                options |= R;
                break;
            case 'a':
                options |= a;
                break;
            case 'r':
                options |= r;
                break;
            case 't':
                options |= t;
                break;
            }
        }
    }
    // printf("%d\n", options);
}

t_dirs *dirlast(t_dirs *lst)
{
    if (!lst)
        return (NULL);
    while (lst->next)
        lst = lst->next;
    return (lst);
}

void diradd_back(t_dirs **alst, t_dirs *new)
{
    t_dirs *dir;

    if (!alst || !*alst)
    {
        *alst = new;
    }
    else
    {
        dir = dirlast(*alst);
        dir->next = new;
    }
}

void add_subdir(t_dirs *curr, char *name, int fromsub);

void sort_content_asc(t_list **head)
{

    t_list *tmp = *head;

    while (tmp)
    {
        printf("element: %s\n", (char *)tmp->content);
        tmp = tmp->next;
    }

    t_list *a = *head;
    t_list *b;
    while (a && a->next)
    {
        t_list *next = a->next;
        while (next)
        {
            if (a->content > next->content)
            {
                char *tmp = next->content;
                next->content = a->content;
                a->content = tmp;
            }
            next = next->next;
        }
        a = a->next;
    }

    /*
    t_list *i = *head;
    t_list *j = *head;

    while (i)
    {
        while (j->next != NULL)
        {
            if (j->content > j->next->content)
            {
                char *tmp = j->content;
                j->content = j->next->content;
                j->next->content = tmp;
            }
            j = j->next;
        }
        j = *head;
        i = i->next;
    }
    */
}

void fill_subdir(t_dirs *curr)
{
    printf("\tTEST: %s\n", curr->dir_name);
    printf("\tTEST: %s\n", curr->path);

    DIR *dp2;
    if ((dp2 = opendir(curr->path)) == NULL)
    {
        switch (errno)
        {
        case EACCES:
            printf("Permission denied\n");
            break;
        case ENOENT:
            printf(" %s Directory does not exist\n", curr->path);
            break;
        case ENOTDIR:
            printf("'%s' is not a directory\n", curr->path);
            break;
        }
    }
    else
    {
        // exit(EXIT_FAILURE);
        struct dirent *dirp2;
        while ((dirp2 = readdir(dp2)) != NULL)
        {
            t_list *new2 = ft_lstnew(dirp2->d_name);
            ft_lstadd_back(&curr->content, new2);
            DIR *subdp;

            char *path2 = ft_strjoin(curr->path, "/");
            printf("\tTry opendir: %s: ", ft_strjoin(path2, dirp2->d_name));
            if ((subdp = opendir(ft_strjoin(path2, dirp2->d_name))) != NULL)
            {
                if (strcmp(dirp2->d_name, ".") == 0 || strcmp(dirp2->d_name, "..") == 0)
                    printf("ignored\n");
                else
                {
                    printf("success\n");
                    printf("We have a subfolder: %s, located in %s folder\n", dirp2->d_name, curr->dir_name);

                    t_dirs *new = dirnew();
                    new->dir_name = dirp2->d_name;
                    char *t = ft_strjoin(curr->path, "/");
                    new->path = ft_strjoin(t, dirp2->d_name);
                    printf("We set path of folder: %s to %s\n", new->dir_name, new->path);
                    new->lvl = curr->lvl + 1;
                    diradd_back(&curr->sub_dir, new);
                    // add_subdir(new, dirp2->d_name, 1);
                    fill_subdir(new);
                }
            }
            else
                printf("fail\n");
        }
        printf("---- %s ----\n", curr->dir_name);
        sort_content_asc(&curr->content);
        /*
        printf("---- AFTER ----\n");
        while (curr->content)
        {
            printf("%s\n", curr->content->content);
            curr->content = curr->content->next;
        }
        */
    }
    printf("\n");
}

void add_subdir(t_dirs *curr, char *name, int from_sub)
{
    t_dirs *newsub = dirnew();
    newsub->dir_name = name;
    newsub->path = ft_strjoin(curr->path, name);
    newsub->lvl = curr->lvl + 1;
    diradd_back(&curr->sub_dir, newsub);
    fill_subdir(newsub);
}

void parse(int ac, char **av)
{
    if (ac == 1)
        dir_to_open = "./";
    else
    {
        for (int i = 1; i < ac; i++)
        {
            if (av[i][0] == '-')
                add_option(av[i]);
            else
            {
                if ((dp = opendir(av[i])) == NULL)
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
                        printf("'%s' is not a directory\n", av[i]);
                        t_list *new = ft_lstnew(av[i]);
                        ft_lstadd_back(&ls.content, new);
                        printf("'%s' added to ls content\n", av[i]);
                        break;
                    }
                    continue;
                }
                else
                {
                    printf("need to register a new dir: %s\n", av[i]);
                    t_dirs *new = dirnew();
                    new->dir_name = av[i];
                    new->lvl = 1;
                    diradd_back(&ls.dirs, new);

                    while ((dirp = readdir(dp)) != NULL)
                    {
                        t_list *new2 = ft_lstnew(dirp->d_name);
                        ft_lstadd_back(&new->content, new2);
                        DIR *subdp;
                        char *path = ft_strjoin("./", av[i]);
                        char *path2 = ft_strjoin(path, "/");
                        new->path = path2;
                        printf("Try opendir: %s: ", ft_strjoin(path2, dirp->d_name));
                        if ((subdp = opendir(ft_strjoin(path2, dirp->d_name))) != NULL)
                        {
                            if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0)
                                printf("ignored\n");
                            else
                            {
                                printf("success\n");
                                printf("We have a subfolder: %s, located in %s folder\n", dirp->d_name, new->dir_name);
                                add_subdir(new, dirp->d_name, 0);
                            }
                        }
                        else
                            printf("fail\n");
                    }
                }

                // while ((dirp = readdir(dp)) != NULL)
                //{
                // struct stat mystat;
                /*
                if (dirp->d_type == DT_REG)
                    printf("file:%s\n", dirp->d_name);
                if (dirp->d_type == DT_DIR)
                    printf("dir:%s\n", dirp->d_name);
                */
                // check if dir / symlin / file
                /*
                t_list new;
                new.next = NULL;
                new.content = av[i];
                printf("argument: %s\n", av[i]);
                ft_lstadd_back(&args, &new);
                */
                //  }
            }
        }
        /*
        if (ac == 1)
            dir_to_open = "./";
        else
        {
            for (int i = 1; i < ac; i++)
            {
                if (av[i][0] == '-')
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

        if (ft_lstsize(args) == 0)
        {
            printf("empty args\n");
            dir_to_open = "./";
        }
        */
    }
}

void print_dir_content(t_dirs *tmp, int i)
{
    int j = i;
    while (j--)
        printf("\t");
    printf("%s->path:%s\n", tmp->dir_name, tmp->path);
    j = i;
    while (j--)
        printf("\t");
    printf("%s->level:%d\n", tmp->dir_name, tmp->lvl);
    j = i;
    while (j--)
        printf("\t");
    printf("%s->content:\n", tmp->dir_name);
    t_list *tmp_c = tmp->content;
    while (tmp_c)
    {
        int k = i;
        while (k--)
            printf("\t");
        printf("%s\n", (char *)tmp_c->content);

        t_dirs *tmp_sub = tmp->sub_dir;
        if (tmp_sub)
        {
            while (tmp_sub)
            {
                if (tmp_sub->dir_name == tmp_c->content)
                    print_dir_content(tmp_sub, tmp_sub->lvl);
                tmp_sub = tmp_sub->next;
            }
        }
        tmp_c = tmp_c->next;
    }
}

void print_details(char *str, char *parent_path)
{
    struct stat mystat;

    char *full_path = (ft_strjoin(parent_path, str));
    stat(full_path, &mystat);

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
    char *u = ft_substr(trim2, 0, ft_strlen(trim2) - 3);
    struct passwd *test = getpwuid(mystat.st_uid);
    struct group *grp = getgrgid(mystat.st_gid);
    printf(" %ld %s %s %4ld %s %s (%ld)\n", mystat.st_nlink, test->pw_name, grp->gr_name, mystat.st_size, u, str, mystat.st_blocks / 2);
}

void print_dir_content_real(t_dirs *tmp, int i)
{
    t_list *tmp_c = tmp->content;
    if ((options & l))
        printf("total random\n");
    while (tmp_c)
    {
        // if a options
        char *arg = (char *)tmp_c->content;
        if (arg[0] == '.' && !(options & a))
            ;
        else
        {
            if (!(options & l))
                printf("%s", (char *)tmp_c->content);
            else
                print_details(arg, tmp->path);
        }

        if (arg[0] == '.' && !(options & a))
            ;
        else if (tmp_c->next)
            printf(" ");
        else
            printf("\n");
        // if -R
        /*
        t_dirs *tmp_sub = tmp->sub_dir;
        if (tmp_sub)
        {
            while (tmp_sub)
            {
                if (tmp_sub->dir_name == tmp_c->content)
                    print_dir_content(tmp_sub, tmp_sub->lvl);
                tmp_sub = tmp_sub->next;
            }
        }
        */
        tmp_c = tmp_c->next;
    }
}

void debug_print()
{
    t_dirs *tmp = ls.dirs;

    printf("\n");
    while (tmp)
    {
        printf("tmp->name = %s\n", tmp->dir_name);
        printf("tmp->path = %s\n", tmp->path);
        if (tmp->next != NULL)
            printf("tmp->next->name = %s\n", tmp->next->dir_name);
        else
            printf("tmp->next->name = %s\n", "NULL");
        print_dir_content(tmp, tmp->lvl);
        printf("\n");
        tmp = tmp->next;
    }
}

void real_print()
{

    t_dirs *tmp = ls.dirs;
    t_list *tmp_content = ls.content;
    printf("-----------------------------------------------------------------\n");
    if (tmp_content)
    {

        t_list *tmp = ls.content;
        while (tmp)
        {
            printf("%s\n", (char *)tmp->content);
            tmp = tmp->next;
        }
    }
    printf("\n");
    while (tmp)
    {
        // print_dir_content_real(tmp, tmp->lvl);
        //
        printf("%s:\n", tmp->path);
        print_dir_content_real(tmp, tmp->lvl);
        //
        tmp = tmp->next;
    }
}

int main(int ac, char *av[])
{
    parse(ac, av);
    debug_print();
    real_print();
    /*
    if (ac != 2)
    {
        fprintf(stderr, "Usage: ./program directory_name\n");
        exit(EXIT_FAILURE);
    }

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
        if (dirp->d_name[0] == '.' && !(options & a))
            ;
        else
        {

            if (dirp->d_type == DT_REG)
                printf("its a file:");
            if (dirp->d_type == DT_DIR)
                printf("its a dir:");

            // printf("type: %d\n", dirp->d_type);
            // printf("%s\n", dirp->d_name);

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
            char *u = ft_substr(trim2, 0, ft_strlen(trim2) - 3);
            struct passwd *test = getpwuid(mystat.st_uid);
            struct group *grp = getgrgid(mystat.st_gid);
            printf(" %ld %s %s %5ld %s %s %ld\n", mystat.st_nlink, test->pw_name, grp->gr_name, mystat.st_size, u, dirp->d_name, mystat.st_blocks / 2);

            // printf("The file %s a symbolic link\n", (S_ISLNK(mystat.st_mode)) ? "is" : "is not");

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
    }
    */
}