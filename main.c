#include "ls.h"

const char *available_options = "lRart";

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

// Function to insert a given node at its correct sorted position into a given
// list sorted in increasing order
void sortedInsert(t_list **head, t_list *newNode)
{
    t_list dummy;
    t_list *current = &dummy;
    dummy.next = *head;

    if (!(options & r))
    {
        while (current->next != NULL && ft_strcmp((char *)current->next->content, (char *)newNode->content) < 0)
            current = current->next;
    }
    else
    {
        while (current->next != NULL && ft_strcmp((char *)current->next->content, (char *)newNode->content) > 0)
            current = current->next;
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
void print_list_str(t_list *lst)
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
    print_list_str(buff);
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

// function to insert a Node at required position
void insertPos(t_list **current, int pos, t_list *data)
{
    // This condition to check whether the
    // position given is valid or not.
    // if (pos < 1 || pos > size + 1)
    ;
    //    cout << "Invalid position!" << endl;
    // else
    //{

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
    // size++;
    //}
}

void display()
{
    if (ls.content)
    {
        insertSort(&ls.content);
        for (t_list *tmp = ls.content; tmp != NULL; tmp = tmp->next)
        {
            if (!(options & l))
            {
                printf("%s", (char *)tmp->content);
                tmp->next ? printf(" ") : printf("\n");
            }
            else
            {
                char *full_path = ft_strjoin("./", (char *)tmp->content);
                struct stat mystat;
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

                printf("%s", details1);
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
                sprintf(details, " %ld %s %s %4ld %s %s\n", mystat.st_nlink, test->pw_name, grp->gr_name, mystat.st_size, u, (char *)tmp->content);
                printf("%s", details);
                free(full_path);
                free(u);
            }
        }
        printf("\n");
    }

    if (ls.buffers)
    {
        for (t_list *tmp = ls.buffers; tmp != NULL; tmp = tmp->next)
        {
            // printf("--start buff--\n");
            for (t_list *tmp2 = tmp->content; tmp2 != NULL; tmp2 = tmp2->next)
            {
                // for (t_list *tmp3 = tmp2->content; tmp3 != NULL; tmp3 = tmp3->next)
                // {
                //     //printf("|");
                //     //printf("%s", (char *)tmp3->content);
                // }
                // printf("|");
                printf("%s", (char *)tmp2->content);
            }
            // printf("|");
            // printf("%s", (char *)tmp->content);
            // printf("--end buff--\n");
        }
    }
}

void output_to_buff()
{
    t_dirs *dir = ls.dirs;
    t_list *content = ls.content;

    /*
        t_dirs *tr = ls.dirs;
        int o = 0;
        while (tr)
        {
            o++;
            printf("dir:%s\n", tr->dir_name);
            tr = tr->next;
        }
        printf("number of dirs: %d\n", o);

        while (content)
        {
            printf("content: %s\n", (char *)content->content);
            content = content->next;
        }
    */
    while (dir)
    {
        // printf("%s///\n", dir->dir_name);
        insertSort(&dir->content);

        t_list *dirbuff = 0;
        t_list *buff1 = 0;
        if (ft_lstsize(ls.args) >= 1 || (options & R))
        {
            // printf("lstsize +1 (name)\n");
            ft_lstadd_back(&buff1, ft_lstnew(ft_strjoin(&dir->path[2], ":\n")));
            ft_lstadd_back(&dirbuff, ft_lstnew(buff1));
        }
        int i = 0;

        t_list *tmp = dir->content;
        while (tmp)
        {

            char *arg = (char *)tmp->content;
            t_list *buff = 0;
            if (arg[0] == '.' && !(options & a))
                ;
            else
            {
                if (!(options & l))
                {
                    ft_lstadd_back(&buff, ft_lstnew(arg));
                    if (tmp->next)
                        ft_lstadd_back(&buff, ft_lstnew(" "));
                    else
                        ft_lstadd_back(&buff, ft_lstnew("\n"));
                }
                else
                {

                    char *init_path = ft_strjoin(dir->path, "/");
                    char *full_path = ft_strjoin(init_path, arg);

                    struct stat mystat;

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
                    ft_lstadd_back(&buff, new1);

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
                    sprintf(details, " %ld %s %s %4ld %s %s\n", mystat.st_nlink, test->pw_name, grp->gr_name, mystat.st_size, u, arg);
                    t_list *new = ft_lstnew(ft_strdup(details));
                    ft_lstadd_back(&buff, new);

                    dir->total_block += mystat.st_blocks / 2;
                    free(full_path);
                    free(u);

                    // ft_lstadd_back(&buff, ft_lstnew(ft_strjoin(arg, "\n")));
                }
            }
            int len;
            for (t_list *tmp = buff; tmp != NULL; tmp = tmp->next)
                len += strlen(tmp->content);
            char *str = ft_strnew(len);
            for (t_list *tmp = buff; tmp != NULL; tmp = tmp->next)
            {
                str = ft_strjoin(str, (char *)tmp->content);
                i++;
            }
            // printf("strlen:%d\n", len);
            // printf("str:%s\n", str);

            // t_list *front_buff = buff->content;

            // print_list_str(buff);

            // ft_lstadd_back(&ls.buffers, ft_lstnew(buff));
            // printf("lstsize +1 (buff)\n");
            ft_lstadd_back(&dirbuff, ft_lstnew(buff));

            tmp = tmp->next;
        }
        // printf("i %d\n", i);
        if (!tmp)
        {
            if (dir->next && (options & R))
            {
                // printf("lstsize +1 (\\n)\n");
                t_list *space = ft_lstnew("\n");
                ft_lstadd_back(&dirbuff, ft_lstnew(space));
            }
            // lse if (!dir->next)
            //{
            //    ft_lstadd_back(&dirbuff, ft_lstnew(""));
            //}

            if ((options & l))
            {
                char *itoa = ft_itoa(dir->total_block);
                char *dir = ft_strjoin(itoa, "\n");
                char *join = ft_strjoin("total ", dir);
                // ft_lstadd_front(&dirbuff, ft_lstnew(ft_strdup(join)));
                // ft_lstadd_front(&dirbuff, ft_lstnew(ft_strdup(join)));
                // printf(" add: %s on pos:%d\n", join, ft_lstsize(dirbuff) - i / 2);
                t_list *total = ft_lstnew(ft_strdup(join));
                // if (ft_lstsize(dirbuff) - i / 2 == 0)
                // {
                //     printf("lstsize = 0\n");
                //     insertPos(&dirbuff, 1, total);
                // }
                // else

                /*
                                printf("lstsize: %d\n", ft_lstsize(dirbuff));
                                t_list *tmpdir = dirbuff;
                                while (tmpdir)
                                {
                                    t_list *c = tmpdir->content;
                                    if (ft_strcmp("\n", (char *)c->content) == 0)
                                        printf("content of dirbuff:space\n");
                                    else
                                        printf("content of dirbuff:%s\n", (char *)c->content);
                                    tmpdir = tmpdir->next;
                                }
                                t_list *last = ft_lstlast(dirbuff);
                                */

                // if (ft_strcmp("\n", last->content) != 0)
                //     insertPos(&dirbuff, ft_lstsize(dirbuff) - i / 2 + 1, total);
                // else
                insertPos(&dirbuff, ft_lstsize(dirbuff) - i / 2, total);

                // ft_lstadd_front(&dirbuff, total);
                free(dir);
                free(itoa);
                free(join);
            }
        }
        ft_lstadd_back(&ls.buffers, dirbuff);
        if (!(options & R))
            break;
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
    return get_dir_w_path(test);
}

void listdir(char *name, char *from)
{
    DIR *dir;
    struct dirent *entry;

    // printf("name %s\n", name);
    if ((dir = opendir(name)) == NULL)
    {
        switch (errno)
        {
        case EACCES:
            break;
            // printf("Permission denied\n");
        case ENOENT:
            printf("ls: cannot access '%s': %s\n", name, strerror(errno));
            break;
        case ENOTDIR:
            ft_lstadd_back(&ls.content, ft_lstnew(name));
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
            listdir(path, "NULL");
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

    /*
        printf("--- ARGS ----\n");
        print_list_str(ls.args);
        printf("\n--- OPTS ---\n");
        print_list_str(ls.opts);
    */
    if (ft_lstsize(ls.args) == 0)
        ft_lstadd_back(&ls.args, ft_lstnew(ft_strdup("./")));
}

int main(int ac, char *av[])
{
    store_params(ac, av);
    // printf("\n");
    for (t_list *tmp = ls.opts; tmp != NULL; tmp = tmp->next)
        add_option(tmp->content);
    for (t_list *tmp = ls.args; tmp != NULL; tmp = tmp->next)
    {
        // printf("go to listdir with:%s\n\n", (char *)tmp->content);
        listdir(tmp->content, "main");
        // printf("\n");
    }
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
    return 0;
}