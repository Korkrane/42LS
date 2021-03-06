#ifndef LS_H
#define LS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <stdbool.h>
#include "./libft/libft.h"

typedef enum e_options
{
    l = 1 << 0,
    R = 1 << 1,
    a = 1 << 2,
    r = 1 << 3,
    t = 1 << 4
} t_options;

typedef struct s_lol
{
    char *date;
    time_t date2;
    void *content;
    struct s_lol *next;
} t_lol;

typedef struct s_display
{
    char *location;
    char *total_line;
    t_lol *content;
} t_display;

typedef struct s_list2
{
    void *content;
    unsigned char type;
    struct s_list2 *next;
} t_list2;

typedef struct s_dirs
{
    char *dir_name;
    char *path;
    t_list *content;
    t_display *display;
    int lvl;
    int total_block; // total for ls -l
    int size;        // for padding
    struct s_dirs *next;
    struct s_dirs *sub_dir;
} t_dirs;

typedef struct s_ls
{
    t_list *buffers;
    t_list *opts;
    t_list *args;
    t_dirs *dirs;
    t_list2 *content;
} t_ls;

void debug_print();

void ft_lstclear2(t_list2 **lst, void (*del)(void *));
void ft_lstadd_back2(t_list2 **alst, t_list2 *new);
t_list2 *ft_lstlast2(t_list2 *lst);
t_list2 *ft_lstnew2(void *content, unsigned char type);
#endif