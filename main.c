#include "ls.h"

const char *available_options = "lRart";
//// else if(params.flags & n) //bonus -
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

void add_subdir(t_dirs *curr, char *name, int fromsub);

t_list *swap(t_list *ptr1, t_list *ptr2)
{
    t_list *tmp = ptr2->next;
    ptr2->next = ptr1;
    ptr1->next = tmp;
    return ptr2;
}

void sort_content_desc(t_list **head, int count)
{
    t_list **h;
    int i, j, swapped;

    for (i = 0; i <= count; i++)
    {
        h = head;
        swapped = 0;
        for (j = 0; j < count - i - 1; j++)
        {

            t_list *p1 = *h;
            t_list *p2 = p1->next;

            if (ft_strcmp((char *)p1->content, (char *)p2->content) < 0)
            {
                *h = swap(p1, p2);
                swapped = 1;
            }
            h = &(*h)->next;
        }
        if (swapped == 0)
            break;
    }
}

void sort_content_asc(t_list **head, int count)
{
    t_list **h;
    int i, j, swapped;

    for (i = 0; i <= count; i++)
    {
        h = head;
        swapped = 0;
        for (j = 0; j < count - i - 1; j++)
        {
            t_list *p1 = *h;
            t_list *p2 = p1->next;

            if (ft_strcmp((char *)p1->content, (char *)p2->content) > 0)
            {
                *h = swap(p1, p2);
                swapped = 1;
            }
            h = &(*h)->next;
        }
        if (swapped == 0)
            break;
    }
}

void sort_content(t_list *content)
{
    if (!(options & r))
        sort_content_asc(&content, ft_lstsize(content));
    else
        sort_content_desc(&content, ft_lstsize(content));
}

/*
void fill_subdir(t_dirs *curr)
{
    // printf("\tTEST: %s\n", curr->dir_name);
    // printf("\tTEST: %s\n", curr->path);
    DIR *dir;
    if ((dir = opendir(curr->path)) == NULL)
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
        struct dirent *dirp2;
        while ((dirp2 = readdir(dir)) != NULL)
        {
            t_list *new2 = ft_lstnew(dirp2->d_name);
            ft_lstadd_back(&curr->content, new2);

            char *path2 = ft_strjoin(curr->path, "/");
            char *test = ft_strjoin(path2, dirp2->d_name);
            free(path2);
            printf("\tTry opendir: %s: ", test);
            DIR *sub_dir = opendir(test);
            if (sub_dir != NULL)
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
                    fill_subdir(new);
                    free(t);
                    // closedir(sub_dir);
                }
            }
            else
                printf("fail\n");
            free(test);
        }
        if (!(options & r))
            sort_content_asc(&curr->content, ft_lstsize(curr->content));
        else
            sort_content_desc(&curr->content, ft_lstsize(curr->content));
    }
    // closedir(dir);
    printf("\n");
}
*/

/*
void add_subdir(t_dirs *curr, char *name, int from_sub)
{
    t_dirs *newsub = dirnew();
    newsub->dir_name = name;
    newsub->path = ft_strjoin(curr->path, name);
    newsub->lvl = curr->lvl + 1;
    diradd_back(&curr->sub_dir, newsub);
    fill_subdir(newsub);
}
*/

void parse(int ac, char **av)
{
    // if (ac == 1)
    //     dir_to_open = "./";
    {
        for (int i = 1; i < ac; i++)
        {
            if (av[i][0] == '-')
                add_option(av[i]);
            else
            {
                /*
                DIR *dp = opendir(av[i]);
                if (dp == NULL)
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
                        DIR *sub_dir;
                        char *path = ft_strjoin("./", av[i]);
                        char *path2 = ft_strjoin(path, "/");
                        new->path = ft_strdup(path2);
                        char *test = ft_strjoin(path2, dirp->d_name);
                        free(path);
                        free(path2);
                        printf("Try opendir: %s: ", test);
                        if ((sub_dir = opendir(test)) != NULL)
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
                        free(test);
                    }
                    sort_content(new->content);
                }
                // closedir(dp);
                */
            }
        }
    }
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

void add_details_l(char *str, char *parent_path, t_dirs *dir, t_list **lst)
{
    printf("add details l\n");
    printf("str: %s\n", str);
    printf("parent_path: %s\n", parent_path);
    printf("\n");
    struct stat mystat;

    // if (parent_path[strlen(parent_path) - 1] != '/')
    //     parent_path = ft_strjoin(parent_path, "/");
    char *full_path = (ft_strjoin(parent_path, str));
    stat(full_path, &mystat);

    char details1[10000];
    sprintf(details1, "%s%s%s%s%s%s%s%s%s%s",
            (S_ISDIR(mystat.st_mode)) ? "d" : "-",
            (mystat.st_mode & S_IRUSR) ? "r" : "-",
            (mystat.st_mode & S_IWUSR) ? "w" : "-",
            (mystat.st_mode & S_IXUSR) ? "x" : "-",
            (mystat.st_mode & S_IRGRP) ? "r" : "-",
            (mystat.st_mode & S_IWGRP) ? "w" : "-",
            (mystat.st_mode & S_IXGRP) ? "x" : "-",
            (mystat.st_mode & S_IROTH) ? "r" : "-",
            (mystat.st_mode & S_IWOTH) ? "w" : "-",
            (mystat.st_mode & S_IXOTH) ? "x" : "-");
    t_list *new1 = ft_lstnew(details1);
    ft_lstadd_back(lst, new1);

    char *date = ctime(&mystat.st_mtime);
    char **trimmed_date = ft_split(date, ' ');
    char *str1 = ft_add_char(ft_strdup(trimmed_date[1]), ' ');
    char *str2 = ft_add_char(ft_strdup(trimmed_date[2]), ' ');
    char *trim = ft_strjoin(str1, str2);
    free(str1);
    free(str2);
    char *trim2 = ft_strjoin(trim, trimmed_date[3]);
    free(trim);
    free_split(&trimmed_date);
    char *u = ft_substr(trim2, 0, ft_strlen(trim2) - 3);
    free(trim2);
    struct passwd *test = getpwuid(mystat.st_uid);
    struct group *grp = getgrgid(mystat.st_gid);

    char details[10000];
    sprintf(details, " %ld %s %s %4ld %s %s\n", mystat.st_nlink, test->pw_name, grp->gr_name, mystat.st_size, u, str);
    t_list *new = ft_lstnew(ft_strdup(details));
    ft_lstadd_back(lst, new);

    dir->total_block += mystat.st_blocks / 2;
    free(full_path);
    free(u);
}

/**
 * @brief print the content of a linked-list as char *
 *
 * @param lst
 */
void print_buffer(t_list *lst)
{
    if (lst)
        for (t_list *tmp = lst; tmp != NULL; tmp = tmp->next)
            printf("%s", (char *)tmp->content);
}

void print_dir_content_real(t_dirs *curr_directory, int i)
{
    t_list *tmp_curr_dir_content = curr_directory->content;
    t_list *buff = 0;

    while (tmp_curr_dir_content)
    {
        char *arg = (char *)tmp_curr_dir_content->content;
        if (arg[0] == '.' && !(options & a))
            ;
        else
        {
            if (!(options & l))
            {
                ft_lstadd_back(&buff, ft_lstnew(arg));
                if (tmp_curr_dir_content->next)
                    ft_lstadd_back(&buff, ft_lstnew(" "));
                else
                    ft_lstadd_back(&buff, ft_lstnew("\n"));
            }
            else
                add_details_l(arg, curr_directory->path, curr_directory, &buff);
        }
        if (!tmp_curr_dir_content->next)
            ft_lstadd_back(&buff, ft_lstnew("\n"));
        tmp_curr_dir_content = tmp_curr_dir_content->next;
    }

    if ((options & l))
    {
        char *itoa = ft_itoa(curr_directory->total_block);
        char *tmp = ft_strjoin(itoa, "\n");
        char *join = ft_strjoin("total ", tmp);

        ft_lstadd_front(&buff, ft_lstnew(ft_strdup(join)));
        free(tmp);
        free(itoa);
        free(join);
    }
    print_buffer(buff);
    t_dirs *tmp_sub = curr_directory->sub_dir;
    tmp_curr_dir_content = curr_directory->content;
    if (tmp_sub && (options & R))
    {
        while (tmp_sub)
        {
            t_list *tmp_c2 = tmp_curr_dir_content;
            while (tmp_c2)
            {
                if (tmp_sub->dir_name == tmp_c2->content)
                {
                    printf("%s:\n", &tmp_sub->path[2]);
                    print_dir_content_real(tmp_sub, tmp_sub->lvl);
                }
                tmp_c2 = tmp_c2->next;
            }
            tmp_sub = tmp_sub->next;
        }
    }
}

/**
 * @brief Print the directory path and all the content located in it
 *
 * @param curr_head_dir the head of our dir list to display
 */
void display_dir_args(t_dirs *curr_head_dir)
{

    /*
        for (t_dirs *tmp = curr_head_dir; tmp != NULL; tmp = tmp->next)
        {
            printf("%s:\n", curr_head_dir->path);
            print_dir_content_real(curr_head_dir, curr_head_dir->lvl);
        }
    */
}

void display_other_args(t_list *curr)
{
    // printf("display-others-args\n");
    t_list *buf = 0;

    if (curr)
    {
        t_list *tmp = ls.content;
        while (tmp)
        {
            if (!(options & l))
                printf("%s", (char *)tmp->content);
            else
                add_details_l((char *)tmp->content, "./", ls.dirs, &buf);
            if (tmp->next)
            {
                printf(" "); // TODO put to buffer
                /*
                t_list *space = ft_lstnew(" ");
                ft_lstadd_back(&buffer_lst, space);
                */
            }
            else
            {
                printf("\n"); // TODO put to buffer
                /*
                t_list *space = ft_lstnew("\n");
                ft_lstadd_back(&buffer_lst, space);
                */
            }
            tmp = tmp->next;
        }
    }
    if (buf)
        print_buffer(buf);
}

void simple_list_display()
{

    t_list *tmpc = ls.content;
    while (tmpc)
    {
        printf("content: %s\n", (char *)tmpc->content);
        tmpc = tmpc->next;
    }
}

void simple_dir_display()
{

    t_dirs *tmpc = ls.dirs;
    while (tmpc)
    {
        printf("dir: %s\n", tmpc->dir_name);

        t_list *tmp = tmpc->content;
        while (tmp)
        {
            t_list *tmp = tmp->content;
            printf("o\n");
            // printf("content: %s\n", (char *)tmp->content);
            tmp = tmp->next;
        }
        tmpc = tmpc->next;
    }
}

void print_dir()
{
    t_dirs *tmp = ls.dirs;
    t_list *tmpc = ls.content;
    // printf("%s:\n", str);
    while (tmpc)
    {
        printf("content: %s\n", (char *)tmpc->content);
        tmpc = tmpc->next;
    }
    printf("\n");
    while (tmp)
    {
        printf("name: %s\n", tmp->dir_name);
        printf("path: %s\n", tmp->path);
        // if(tmp->next)
        //     printf("next->name: %s\n", tmp->next->dir_name);

        t_list *tmp_content = tmp->content;
        while (tmp_content)
        {
            printf("content: %s\n", (char *)tmp_content->content);
            tmp_content = tmp_content->next;
        }
        tmp = tmp->next;
        printf("\n");
    }
    printf("\n");
    printf("-----------------\n");
    t_dirs *aaa = ls.dirs;
    t_list *bbb = ls.content;
    display_other_args(bbb);
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
    // printf("On %s\n", curr->dir_name);
    //  printf("On %s\n", curr->path);
    char *test = ft_substr(curr->path, 0, strlen(curr->path) - strlen(curr->dir_name) - 1);
    // printf("On %s\n", test);
    return get_dir_w_path(test);
}

void listdir(char *name, int indent, char *from)
{
    DIR *dir;
    struct dirent *entry;

    // printf("name %s\n", name);
    if (!(dir = opendir(name)))
        return;
    else
    {
        char path[1024];
        snprintf(path, sizeof(path), "./%s", name);
        if (!ls.dirs)
        {
            diradd_back(&ls.dirs, dirnew(name, path));
        }
    }
    while ((entry = readdir(dir)) != NULL)
    {
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
            diradd_back(&ls.dirs, dirnew(entry->d_name, path));

            t_dirs *curr = get_dir_w_path(path);
            if (ft_strcmp("main", from) == 0)
            {
                // printf("add %s to %s in content\n", entry->d_name, name);
                //  ft_lstadd_back(&ls.content, ft_lstnew(ft_strdup(entry->d_name)));
                ft_lstadd_back(&ls.dirs->content, ft_lstnew(ft_strdup(entry->d_name)));
            }
            else if (curr)
            {
                t_dirs *prev = get_prev_dir(curr);
                if (prev)
                {
                    // printf("we are in %s adding %s as content to %s\n", curr->dir_name, entry->d_name, prev->dir_name);
                    ft_lstadd_back(&prev->content, ft_lstnew(ft_strdup(entry->d_name)));
                }
            }
            // printf("%*s[%s]\n", indent, "", entry->d_name);
            listdir(path, indent + 2, "NULL");
        }
        else
        {
            char path[1024];
            snprintf(path, sizeof(path), "%s", name);

            // printf("%s: - %s\n", path, entry->d_name);
            t_dirs *curr = get_dir_w_path(path);

            if (ft_strcmp("main", from) == 0)
            {
                ft_lstadd_back(&ls.dirs->content, ft_lstnew(ft_strdup(entry->d_name)));
                // ft_lstadd_back(&ls.content, ft_lstnew(ft_strdup(entry->d_name)));
            }
            else if (curr)
                ft_lstadd_back(&curr->content, ft_lstnew(ft_strdup(entry->d_name)));
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
    if (!tmp)
    {
        // ft_lstadd_back(&ls.content, ft_lstnew(ft_strdup(".")));
        // ft_lstadd_back(&ls.content, ft_lstnew(ft_strdup("..")));
    }
}

int main(int ac, char *av[])
{
    if (ac == 1)
    {
        listdir("./", 0, "main");
        add_dot_folders();
        print_dir(av[0]);
        return 0;
    }
    else
    {
        for (int i = 1; i < ac; i++)
        {
            if (av[i][0] == '-')
                add_option(av[i]);
            else
            {
                listdir(av[i], 0, "main");
                add_dot_folders();
            }
        }
    }
    print_dir();
    return 0;
}