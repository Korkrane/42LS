#include "ls.h"

const char *available_options = "lRart";

DIR *dp;
struct dirent *dirp;
char *dir_to_open;
int tot;
int options;
t_list *args;
t_list *dirs;
t_ls ls;

t_dirs *dirnew(char *name, char *path)
{
    t_dirs *new;
    new = (t_dirs *)malloc(sizeof(t_dirs));
    if (!new)
        return (NULL);
    new->next = NULL;
    new->sub_dir = NULL;
    new->path = ft_strdup(path);
    new->dir_name = ft_strdup(name);
    new->total_block = 0;
    new->content = NULL;
    new->display = NULL;
    return (new);
}

void add_option(char *opt_arg)
{
    // printf("options: %s\n", opt_arg);

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
}

t_dirs *dirlast(t_dirs *lst)
{
    if (!lst)
        return (NULL);
    while (lst->next)
        lst = lst->next;
    return (lst);
}

t_dirs *diradd_back(t_dirs **alst, t_dirs *new)
{
    t_dirs *dir;

    if (!alst || !*alst)
        *alst = new;
    else
    {
        dir = dirlast(*alst);
        dir->next = new;
    }
    return new;
}

char *calc_str(char *str)
{
    char *lower1;
    if (str[0] == '.' && ft_strlen(str) > 1 && str[1] && str[1] != '.')
        lower1 = ft_strlower(&str[1]);
    else
        lower1 = ft_strlower(str);
    return lower1;
}

// Function to insert a given node at its correct sorted position into a given
// list sorted in increasing order
void sortedInsert2(t_list2 **head, t_list2 *newNode)
{
    t_list2 dummy;
    t_list2 *current = &dummy;
    dummy.next = *head;

    if (!(options & r))
    {
        char *lower1 = NULL;
        char *lower2 = NULL;

        while (current->next != NULL)
        {
            lower1 = calc_str((char *)current->next->content);
            lower2 = calc_str((char *)newNode->content);
            if (ft_strcmp(lower1, lower2) < 0)
            {
                free(lower1);
                free(lower2);
                current = current->next;
            }
            else
            {
                free(lower1);
                free(lower2);
                break;
            }
        }
    }
    else
    {
        char *lower1 = NULL;
        char *lower2 = NULL;

        while (current->next != NULL)
        {
            lower1 = calc_str((char *)current->next->content);
            lower2 = calc_str((char *)newNode->content);
            if (ft_strcmp(lower1, lower2) > 0)
            {
                free(lower1);
                free(lower2);
                current = current->next;
            }
            else
            {
                free(lower1);
                free(lower2);
                break;
            }
        }
    }
    newNode->next = current->next;
    current->next = newNode;
    *head = dummy.next;
}

// Given a list, change it to be in sorted order (using `sortedInsert()`).
void insertSort2(t_list2 **head)
{
    t_list2 *result = NULL;   // build the answer here
    t_list2 *current = *head; // iterate over the original list
    t_list2 *next;

    while (current != NULL)
    {
        // tricky: note the next pointer before we change it
        next = current->next;

        sortedInsert2(&result, current);
        current = next;
    }

    *head = result;
}

// Function to insert a given node at its correct sorted position into a given
// list sorted in increasing order
void sortedInsertDate(t_lol **head, t_lol *newNode)
{
    t_lol dummy;
    t_lol *current = &dummy;
    dummy.next = *head;

    char *lower1 = NULL;
    char *lower2 = NULL;

    while (current->next != NULL)
    {
        if (current->next->date2 == 0 || current->date2 == 0)
        {
            current = current->next;
            continue;
        }
        if (current && current->next)
        {
            lower1 = (char *)current->next->content;
            lower2 = (char *)newNode->content;
            //  printf("compare (%s)%s && %s (%s)\n", (char *)current->next->content, lower1, lower2, (char *)current->content);
            //   if (ft_strcmp(lower1, lower2) < 0)
            // if (ft_strcmp(lower1, lower2) < 0)
            if (current->next->date2 > newNode->date2)
            {
                current = current->next;
            }
            else
            {
                // if (current->next->date2 == newNode->date2 && ft_strcmp(lower1, lower2) < 0)
                //     current = current->next;
                break;
            }
        }
        else
            break;
    }
    newNode->next = current->next;
    current->next = newNode;
    *head = dummy.next;
}

void printlol(t_lol **head)
{
    t_lol *current = *head; // iterate over the original list
    t_lol *next;
    while (current != NULL)
    {
        // tricky: note the next pointer before we change it
        char *arg = (char *)current->content;
        if (arg[0] == '\n')
            printf("\\n\n");
        else if (arg[0] == ' ')
            printf("space\n");
        else if (arg)
        {
            if (!(options & l))
                printf("date:%ld||| %s\n", current->date2, arg);
            else
                printf("date:%ld||| %s", current->date2, arg);
        }
        next = current->next;
        current = next;
    }
}
// Given a list, change it to be in sorted order (using `sortedInsert()`).
void dateSort(t_lol **head)
{
    // printf("sort by date of acces\n");
    t_lol *result = NULL;           // build the answer here
    t_lol *current = (*head)->next; // iterate over the original list
    t_lol *next;

    printf("before:\n");
    printlol(&current);
    while (current != NULL)
    {
        // tricky: note the next pointer before we change it
        next = current->next;

        sortedInsertDate(&result, current);
        current = next;
    }

    *head = result;
    printf("after:\n");
    printlol(head);
}

// Function to insert a given node at its correct sorted position into a given
// list sorted in increasing order
void sortedInsert(t_list **head, t_list *newNode)
{
    t_list dummy;
    t_list *current = &dummy;
    dummy.next = *head;

    if (!(options & r))
    {
        char *lower1 = NULL;
        char *lower2 = NULL;

        while (current->next != NULL)
        {
            lower1 = calc_str((char *)current->next->content);
            lower2 = calc_str((char *)newNode->content);
            if (ft_strcmp(lower1, lower2) < 0)
            {
                free(lower1);
                free(lower2);
                current = current->next;
            }
            else
            {
                free(lower1);
                free(lower2);
                break;
            }
        }
    }
    else
    {
        char *lower1 = NULL;
        char *lower2 = NULL;

        while (current->next != NULL)
        {
            lower1 = calc_str((char *)current->next->content);
            lower2 = calc_str((char *)newNode->content);
            if (ft_strcmp(lower1, lower2) > 0)
            {
                free(lower1);
                free(lower2);
                current = current->next;
            }
            else
            {
                free(lower1);
                free(lower2);
                break;
            }
        }
    }
    newNode->next = current->next;
    current->next = newNode;
    *head = dummy.next;
}

// Given a list, change it to be in sorted order (using `sortedInsert()`).
void insertSort(t_list **head)
{
    t_list *result = NULL;   // build the answer here
    t_list *current = *head; // iterate over the original list
    t_list *next;

    while (current != NULL)
    {
        // tricky: note the next pointer before we change it
        next = current->next;

        sortedInsert(&result, current);
        current = next;
    }

    *head = result;
}

// Function to insert a given node at its correct sorted position into a given
// list sorted in increasing order
void sortedInsertDir(t_dirs **head, t_dirs *newNode)
{
    t_dirs dummy;
    t_dirs *current = &dummy;
    dummy.next = *head;

    while (current->next != NULL && ft_strcmp((char *)current->next->path, (char *)newNode->path) < 0)
        current = current->next;
    newNode->next = current->next;
    current->next = newNode;
    *head = dummy.next;
}

// Given a list, change it to be in sorted order (using `sortedInsert()`).
void insertSortDir(t_dirs **head)
{
    t_dirs *result = NULL;   // build the answer here
    t_dirs *current = *head; // iterate over the original list
    t_dirs *next;

    while (current != NULL)
    {
        // tricky: note the next pointer before we change it
        next = current->next;

        sortedInsertDir(&result, current);
        current = next;
    }
    *head = result;
}

void free_split(char ***split)
{
    int i;

    i = -1;
    while ((*split)[++i])
    {
        free((*split)[i]);
        (*split)[i] = NULL;
    }
    free(*split);
    *split = NULL;
}

// function to insert a Node at required position
void insertPos(t_list **current, int pos, t_list *data)
{
    // Keep looping until the pos is zero
    while (pos--)
    {
        if (pos == 0)
        {
            // adding t_list at required position
            t_list *temp = ft_lstnew(data);

            // Making the new t_list to point to
            // the old t_list at the same position
            temp->next = *current;

            // Changing the pointer of the t_list previous
            // to the old t_list to point to the new t_list
            *current = temp;
        }
        else
            // Assign double pointer variable to point to the
            // pointer pointing to the address of next t_list
            current = &(*current)->next;
    }
}

bool find_in_args(t_dirs *lst)
{
    char *name = 0;
    if (lst)
        name = lst->dir_name;
    t_list *tmp = ls.args;
    while (tmp)
    {
        if (ft_strcmp((char *)tmp->content, name) == 0)
            return true;
        tmp = tmp->next;
    }
    return false;
}

void display()
{
    if (ls.content)
    {
        insertSort2(&ls.content);
        for (t_list2 *tmp = ls.content; tmp != NULL; tmp = tmp->next)
        {

            if (!(options & l))
            {
                printf("%s", (char *)tmp->content);
                tmp->next ? printf(" ") : printf("\n");
            }
            else
            {
                char *full_path = ft_strjoin("./", (char *)tmp->content);
                // printf("full path: %s\n", full_path);
                // printf("----------------\n");
                struct stat mystat;
                stat(full_path, &mystat);
                char details1[10000];
                char *sd = "-";

                char *buf = NULL;
                ssize_t nbytes, bufsiz;

                if (S_ISDIR(mystat.st_mode))
                    sd = "d";
                else if (S_ISLNK(mystat.st_mode))
                {
                    bufsiz = mystat.st_size;
                    if (mystat.st_size == 0)
                        bufsiz = PATH_MAX;

                    buf = malloc(bufsiz);
                    sd = "l";
                    nbytes = readlink(full_path, buf, bufsiz);
                }
                sprintf(details1, "%s%s%s%s%s%s%s%s%s%s",
                        sd,
                        (mystat.st_mode & S_IRUSR) ? "r" : "-",
                        (mystat.st_mode & S_IWUSR) ? "w" : "-",
                        (mystat.st_mode & S_IXUSR) ? "x" : "-",
                        (mystat.st_mode & S_IRGRP) ? "r" : "-",
                        (mystat.st_mode & S_IWGRP) ? "w" : "-",
                        (mystat.st_mode & S_IXGRP) ? "x" : "-",
                        (mystat.st_mode & S_IROTH) ? "r" : "-",
                        (mystat.st_mode & S_IWOTH) ? "w" : "-",
                        (mystat.st_mode & S_IXOTH) ? "x" : "-");

                printf("%s", details1);
                char *date = ctime(&mystat.st_mtime);
                char **trimmed_date = ft_split(date, ' ');
                char *str1 = ft_add_char(ft_strdup(trimmed_date[1]), ' ');
                char *str2 = ft_add_char(ft_strdup(trimmed_date[2]), ' ');
                char *trim = ft_strjoin(str1, str2);
                free(str1);
                free(str2);
                char *trim2 = ft_strjoin(trim, trimmed_date[3]);
                // char *arr[] = {str1, str2, trimmed_date[3]};
                // char *trim3 = ft_strjoins(arr);
                free(trim);
                free_split(&trimmed_date);
                char *u = ft_substr(trim2, 0, ft_strlen(trim2) - 3);
                free(trim2);
                struct passwd *test = getpwuid(mystat.st_uid);
                struct group *grp = getgrgid(mystat.st_gid);

                char *color = "";
                if (details1[0] == 'd')
                    color = COLOR_BOLD_BLUE;
                else if (details1[0] == 'l')
                    color = COLOR_BOLD_RED;
                else if (ft_strchr(details1, 'x'))
                    color = COLOR_BOLD_GREEN;
                char *restc = COLOR_RESET;
                char details[10000];
                if (buf)
                {
                    sprintf(details, " %ld %s %s %4ld %s %s%s%s -> %s%.*s%s\n", mystat.st_nlink, test->pw_name, grp->gr_name, mystat.st_size, u, color, (char *)tmp->content, restc, color, (int)nbytes, buf, restc);
                    free(buf);
                }
                else
                    sprintf(details, " %ld %s %s %4ld %s %s%s%s\n", mystat.st_nlink, test->pw_name, grp->gr_name, mystat.st_size, u, color, (char *)tmp->content, restc);
                printf("%s", details);
                free(full_path);
                free(u);
            }
        }
        if (ls.dirs)
            printf("\n");
    }

    if (ls.dirs)
    {
        t_dirs *tmp = ls.dirs;
        // printf("lol");
        int j = 0;
        int i = 0;
        while (tmp)
        {
            // printf("ls.dirs.name: %s\n", tmp->dir_name);
            // printf("ls.args size: %d\n", ft_lstsize(ls.args));
            if (find_in_args(tmp) || (options & R))
            {
                j++;
                // printf("i: %d\n", i);
                // printf("j: %d\n", j);
                if (tmp->display)
                {
                    t_lol *arg_to_display = tmp->display->content;

                    if (ft_lstsize(ls.args) > 1 || (options & R))
                        printf("%s", tmp->display->location);
                    if (options & l)
                        printf("%s", tmp->display->total_line);
                    while (arg_to_display)
                    {
                        if (ft_strcmp("init\n", (char *)arg_to_display->content) == 0)
                            ;
                        // else if (ft_strcmp("\n", (char *)arg_to_display->content) == 0 && !arg_to_display->next && tmp->next && !find_in_args(tmp->next))
                        //    ;
                        // else if (ft_strcmp("\n", (char *)arg_to_display->content) == 0 && i > j)
                        //    ;
                        else
                            printf("%s", (char *)arg_to_display->content);
                        if (!(options & l))
                        {
                            if (arg_to_display->next)
                                printf(" ");
                            else
                                printf("\n");
                        }
                        arg_to_display = arg_to_display->next;
                    }
                    // save last arg printed
                    if (tmp->next)
                        printf("\n");
                }
            }
            i++;
            tmp = tmp->next;
        }
        // si last arg printed not \n add one :) //TODO
    }
}

int max_padding(t_list *t, char *path)
{
    t_list *tmp = t;
    int max = 0;
    while (tmp)
    {
        char *arg = (char *)tmp->content;
        if (arg[0] == '.' && !(options & a))
        {
            tmp = tmp->next;
            continue;
        }
        char *init_path = ft_strjoin(path, "/");
        char *full_path = ft_strjoin(init_path, arg);
        free(init_path);
        struct stat mystat;

        stat(full_path, &mystat);
        if (max < ft_intlen(mystat.st_size))
            max = ft_intlen(mystat.st_size);
        free(full_path);
        tmp = tmp->next;
    }
    return max;
}

t_lol *ft_lolnew(void *content, char *date, time_t date2)
{
    t_lol *new;

    new = (t_lol *)malloc(sizeof(t_lol));
    if (!new)
        return (NULL);
    new->content = content;
    if (date)
        new->date = date;
    else
        new->date = NULL;
    if (date2)
        new->date2 = date2;
    else
        new->date2 = 0;
    new->next = NULL;
    return (new);
}

t_lol *ft_lollast(t_lol *lst)
{
    if (!lst)
        return (NULL);
    while (lst->next)
        lst = lst->next;
    return (lst);
}

void ft_loladd_back(t_lol **alst, t_lol *new)
{
    t_lol *list;

    if (!alst || !*alst)
    {
        *alst = new;
    }
    else
    {
        list = ft_lollast(*alst);
        list->next = new;
    }
}

void output_to_buff()
{
    t_dirs *dir = ls.dirs;

    while (dir)
    {
        insertSort(&dir->content);
        // printf("lol dir->name:%s\n", dir->dir_name);
        dir->display = malloc(sizeof(t_display) * 1);
        char *str = ft_strjoin(&dir->path[2], ":\n");

        dir->display->location = str;
        // dir->display->content = malloc(sizeof(t_lol) * 1);
        // dir->display->content = NULL;
        // dir->display->content = ft_lstnew(ft_strdup("init\n"));
        dir->display->content = ft_lolnew(ft_strdup("init\n"), 0, 0);
        // dir->display->total_line = NULL;

        t_list *tmp = dir->content;
        int padding = max_padding(tmp, dir->path);
        while (tmp)
        {
            // printf("lol dir->content:%s\n", (char *)tmp->content);
            char *arg = (char *)tmp->content;
            if (arg[0] == '.' && !(options & a))
                ;
            else
            {
                // if (!(options & l))
                // {
                //     ft_lstadd_back(&dir->display->content, ft_lstnew(ft_strdup(arg)));

                //     if (tmp->next)
                //         ft_lstadd_back(&dir->display->content, ft_lstnew(ft_strdup(" ")));
                //     else
                //         ft_lstadd_back(&dir->display->content, ft_lstnew(ft_strdup("\n")));
                // }
                // else
                //{

                char *init_path = ft_strjoin(dir->path, "/");
                char *full_path = ft_strjoin(init_path, arg);
                free(init_path);

                // printf("full path: %s\n", full_path);
                // printf("----------------\n");

                struct stat mystat;
                lstat(full_path, &mystat);
                char details1[10000];
                char *sd = "-";

                char *buf = NULL;
                ssize_t nbytes, bufsiz;

                if (S_ISDIR(mystat.st_mode))
                    sd = "d";
                else if (S_ISLNK(mystat.st_mode))
                {
                    bufsiz = mystat.st_size;
                    if (mystat.st_size == 0)
                        bufsiz = PATH_MAX;

                    buf = malloc(bufsiz);
                    sd = "l";
                    nbytes = readlink(full_path, buf, bufsiz);
                }
                sprintf(details1, "%s%s%s%s%s%s%s%s%s%s",
                        sd,
                        (mystat.st_mode & S_IRUSR) ? "r" : "-",
                        (mystat.st_mode & S_IWUSR) ? "w" : "-",
                        (mystat.st_mode & S_IXUSR) ? "x" : "-",
                        (mystat.st_mode & S_IRGRP) ? "r" : "-",
                        (mystat.st_mode & S_IWGRP) ? "w" : "-",
                        (mystat.st_mode & S_IXGRP) ? "x" : "-",
                        (mystat.st_mode & S_IROTH) ? "r" : "-",
                        (mystat.st_mode & S_IWOTH) ? "w" : "-",
                        (mystat.st_mode & S_IXOTH) ? "x" : "-");

                char *date = ctime(&mystat.st_mtime);
                time_t date2 = mystat.st_mtime;
                // ft_lstadd_back(&dir->display->content, ft_strdup(date));
                // ft_loladd_back(&dir->display->date, ft_strdup(date));
                char **trimmed_date = ft_split(date, ' ');
                char *str1 = ft_add_char(ft_strdup(trimmed_date[1]), ' ');
                char *str2 = ft_add_char(ft_strdup(trimmed_date[2]), ' ');
                char *trim = ft_strjoin(str1, str2);
                free(str1);
                free(str2);
                char *trim2 = ft_strjoin(trim, trimmed_date[3]);
                // char *arr[] = {str1, str2, trimmed_date[3]};
                // char *trim3 = ft_strjoins(arr);
                free(trim);
                free_split(&trimmed_date);
                char *u = ft_substr(trim2, 0, ft_strlen(trim2) - 3);
                free(trim2);
                struct passwd *test = getpwuid(mystat.st_uid);
                struct group *grp = getgrgid(mystat.st_gid);

                char *color = "";
                if (details1[0] == 'd')
                    color = COLOR_BOLD_BLUE;
                else if (details1[0] == 'l')
                    color = COLOR_BOLD_RED;
                else if (ft_strchr(details1, 'x'))
                    color = COLOR_BOLD_GREEN;
                char *restc = COLOR_RESET;
                char details[10000];
                if (buf)
                {
                    sprintf(details, " %ld %s %s %*ld %s %s%s%s -> %s%.*s%s\n", mystat.st_nlink, test->pw_name, grp->gr_name, padding, mystat.st_size, u, color, arg, restc, color, (int)nbytes, buf, restc);
                    free(buf);
                }
                else
                    sprintf(details, " %ld %s %s %*ld %s %s%s%s\n", mystat.st_nlink, test->pw_name, grp->gr_name, padding, mystat.st_size, u, color, arg, restc);

                char *full_string = ft_strjoin(details1, details);

                if (!(options & l))
                {
                    char t[1000000];
                    sprintf(t, "%s%s%s", color, arg, restc);
                    // ft_lstadd_back(&dir->display->content, ft_lstnew(ft_strdup(t)));
                    if (dir->display->content != NULL)
                        ft_loladd_back(&dir->display->content, ft_lolnew(ft_strdup(t), 0, date2));
                    else
                        dir->display->content = ft_lolnew(ft_strdup(t), 0, date2);

                    // 55//if (tmp->next)
                    // 55//    ft_loladd_back(&dir->display->content, ft_lolnew(ft_strdup(" "), 0, 0));
                    //   ft_lstadd_back(&dir->display->content, ft_lstnew(ft_strdup(" ")));
                    // 55//else
                    // 55//ft_loladd_back(&dir->display->content, ft_lolnew(ft_strdup("\n"), 0, 0));
                    // ft_lstadd_back(&dir->display->content, ft_lstnew(ft_strdup("\n")));
                }
                else
                {
                    if (dir->display->content != NULL)
                    {
                        // printf("do not build the t_lol\n");
                        ft_loladd_back(&dir->display->content, ft_lolnew(ft_strdup(full_string), date, date2));
                    }
                    else
                    {
                        printf("build the t_lol\n");
                        dir->display->content = ft_lolnew(ft_strdup(full_string), date, date2);
                    }
                }
                // ft_lstadd_back(&dir->display->content, ft_lstnew(ft_strdup(full_string)));
                free(full_string);
                // printf("full string: %s\n", full_string);
                dir->total_block += mystat.st_blocks / 2;
                free(full_path);
                free(u);
                //}
            }
            tmp = tmp->next;
            // if (!(options & R))
            //     break;
        }
        if (!tmp)
        {
            // if (dir->next)
            //     ft_loladd_back(&dir->display->content, ft_lolnew(ft_strdup("\n"), 0, 0));
            //   ft_lstadd_back(&dir->display->content, ft_lstnew(ft_strdup("\n")));

            if ((options & l))
            {
                char *itoa = ft_itoa(dir->total_block);
                char *qwe = ft_strjoin(itoa, "\n");
                char *join = ft_strjoin("total ", qwe);
                dir->display->total_line = strdup(join);

                free(qwe);
                free(itoa);
                free(join);
            }
        }
        if (options & t)
        {
            printf("sort by date %s\n", dir->dir_name);
            dateSort(&dir->display->content);
        }
        dir = dir->next;
    }
    display();
}

t_dirs *get_dir_w_path(char *path)
{
    t_dirs *tmp = ls.dirs;

    while (tmp)
    {
        if (ft_strcmp(path, tmp->path) == 0)
            return tmp;
        tmp = tmp->next;
    }
    return NULL;
}

t_dirs *get_prev_dir(t_dirs *curr)
{
    char *test = ft_substr(curr->path, 0, strlen(curr->path) - strlen(curr->dir_name) - 1);
    t_dirs *ret = get_dir_w_path(test);
    free(test);
    return ret;
}

void listdir(char *name, char *from)
{
    DIR *dir;
    struct dirent *entry;

    // printf("name %s\n", name);
    if ((dir = opendir(name)) == NULL)
    {
        struct stat mystat;
        switch (errno)
        {
        case EACCES:
            break;
        case ENOENT:
            printf("ls: cannot access '%s': %s\n", name, strerror(errno));
            break;
        case ENOTDIR:
            ft_lstadd_back2(&ls.content, ft_lstnew2(name, DT_DIR));
            break;
        }
        return;
    }
    else
    {
        char path[1024];
        if (ft_strcmp(name, "./") == 0)
            snprintf(path, sizeof(path), "%s", name);
        else
            snprintf(path, sizeof(path), "./%s", name);
        if (!ls.dirs || ft_strcmp("main", from) == 0)
        {
            diradd_back(&ls.dirs, dirnew(name, path));
        }
    }
    while ((entry = readdir(dir)) != NULL)
    {
        // printf("readdir %s\n", entry->d_name);
        if (entry->d_type == DT_DIR)
        {
            char path[1024];

            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            if (name[0] == '.' && name[1] == '/')
                snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
            else
                snprintf(path, sizeof(path), "./%s/%s", name, entry->d_name);
            // printf("path %s\n", path);
            t_dirs *new = dirnew(entry->d_name, path);
            // printf("new dir added %s\n", new->dir_name);
            diradd_back(&ls.dirs, new);
            t_dirs *curr = get_dir_w_path(path);
            if (ft_strcmp("main", from) == 0)
            {
                // printf("add %s to %s in content\n", entry->d_name, name);
                //   ft_lstadd_back(&ls.content, ft_lstnew(ft_strdup(entry->d_name)));
                t_dirs *prev = get_prev_dir(curr);
                if (prev)
                {
                    // printf("we are in %s adding %s as content to %s\n", curr->dir_name, entry->d_name, prev->dir_name);
                    ft_lstadd_back(&prev->content, ft_lstnew(ft_strdup(entry->d_name)));
                }
                // 1/ft_lstadd_back(&ls.dirs->content, ft_lstnew(ft_strdup(entry->d_name)));
            }
            else if (curr)
            {
                // printf("we are in %s\n", curr->dir_name);
                t_dirs *prev = get_prev_dir(curr);
                if (prev)
                {
                    // printf("we are in %s adding %s as content to %s\n", curr->dir_name, entry->d_name, prev->dir_name);
                    ft_lstadd_back(&prev->content, ft_lstnew(ft_strdup(entry->d_name)));
                }
            }
            // printf("%*s[%s]\n", indent, "", entry->d_name);
            // if (options & R)
            listdir(path, "NULL");
        }
        else
        {
            char path[1024];
            snprintf(path, sizeof(path), "%s", name);
            char *new;
            t_dirs *curr;
            if (path[0] != '.')
            {
                new = ft_strjoin("./", path);
                curr = get_dir_w_path(new);
                // if (curr)
                //     printf("we are in %s adding %s as content\n", curr->dir_name, entry->d_name);
                free(new);
            }
            else
            {
                curr = get_dir_w_path(path);

                // if (curr)
                //     printf("we are in %s adding %s as content\n", curr->dir_name, entry->d_name);
            }

            // printf("%s: - %s\n", path, entry->d_name);

            if (curr)
                ft_lstadd_back(&curr->content, ft_lstnew(ft_strdup(entry->d_name)));
            else if (ft_strcmp("main", from) == 0)
            {
                ft_lstadd_back(&ls.dirs->content, ft_lstnew(ft_strdup(entry->d_name)));
                // ft_lstadd_back(&ls.content, ft_lstnew(ft_strdup(entry->d_name)));
            }
        }
    }
    closedir(dir);
}

void add_dot_folders()
{
    t_dirs *tmp = ls.dirs;

    while (tmp)
    {
        ft_lstadd_back(&tmp->content, ft_lstnew(ft_strdup(".")));
        ft_lstadd_back(&tmp->content, ft_lstnew(ft_strdup("..")));
        tmp = tmp->next;
    }
}

void store_params(int ac, char **av)
{
    int i = 1;
    while (av[i])
    {
        if (av[i][0] == '-')
            ft_lstadd_back(&ls.opts, ft_lstnew(ft_strdup(av[i])));
        else
            ft_lstadd_back(&ls.args, ft_lstnew(ft_strdup(av[i])));
        i++;
    }

    if (ft_lstsize(ls.args) == 0)
        ft_lstadd_back(&ls.args, ft_lstnew(ft_strdup("./")));
}

void free_list(void *content)
{
    char *e;

    e = (char *)content;
    ft_strdel(&e);
}

void ft_dirdel(t_dirs **as)
{
    if (as)
    {
        free(*as);
        *as = NULL;
    }
}

void free_dirs(void *content)
{
    t_dirs *e;

    e = (t_dirs *)content;
    ft_dirdel(&e);
}

void ft_dirclear(t_dirs **lst, void (*del)(void *))
{
    t_dirs *ptr_list;
    t_dirs *ptr_next;

    ptr_list = *lst;
    while (ptr_list)
    {
        ptr_next = ptr_list->next;
        (*del)(ptr_list->content);
        free(ptr_list);
        ptr_list = ptr_next;
    }
    *lst = NULL;
}

int main(int ac, char *av[])
{
    store_params(ac, av);
    for (t_list *tmp = ls.opts; tmp != NULL; tmp = tmp->next)
        add_option(tmp->content);
    for (t_list *tmp = ls.args; tmp != NULL; tmp = tmp->next)
        listdir(tmp->content, "main");
    add_dot_folders();
    /*
    t_dirs *tr = ls.dirs;
    while (tr)
    {
        printf("ls.dir:%s\n", tr->dir_name);
        printf("ls.dir.path:%s\n", tr->path);
        t_list *tmp = tr->content;
        while (tmp)
        {
            printf("  content:%s\n", (char *)tmp->content);
            tmp = tmp->next;
        }
        tr = tr->next;
    }
    */
    /*
     printf("----------\n");
     t_list *ta = ls.content;
     while (ta)
     {
         printf("ls.content:%s\n", (char *)ta->content);
         ta = ta->next;
     }
 */
    // printf("----------\n");
    insertSortDir(&ls.dirs);

    output_to_buff();
    ft_lstclear(&ls.args, &free_list);
    ft_lstclear(&ls.opts, &free_list);

    t_dirs *tmp = ls.dirs;
    while (tmp)
    {
        if (tmp->display)
        {
            while (tmp->display->content)
            {
                char *t = (char *)tmp->display->content->content;
                free(t);
                t_lol *ptr_next = tmp->display->content->next;
                free(tmp->display->content);
                tmp->display->content = NULL;
                tmp->display->content = ptr_next;
            }
            if (tmp->display->location)
                free(tmp->display->location);
            if (tmp->display->total_line)
                free(tmp->display->total_line);
            if (tmp->display->content)
                free(tmp->display->content);
        }
        free(tmp->display);

        free(tmp->dir_name);
        free(tmp->path);
        ft_lstclear(&tmp->content, &free_list);

        t_dirs *ptr_next = tmp->next;
        free(tmp);
        tmp = ptr_next;
    }
    ls.dirs = NULL;

    return 0;
}