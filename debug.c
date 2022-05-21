#include "ls.h"

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