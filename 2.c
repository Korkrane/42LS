#include "ls.h"

t_list2 *ft_lstnew2(void *content, unsigned char type){
    t_list2 *new;

    new = (t_list2 *)malloc(sizeof(t_list2));
    if (!new)
        return (NULL);
    new->content = content;
    new->type = type;
    new->next = NULL;
    return (new);
}

t_list2 *ft_lstlast2(t_list2 *lst)
{
    if (!lst)
        return (NULL);
    while (lst->next)
        lst = lst->next;
    return (lst);
}


void		ft_lstadd_back2(t_list2 **alst, t_list2 *new)
{
	t_list2		*list;

	if (!alst || !*alst)
	{
		*alst = new;
	}
	else
	{
		list = ft_lstlast2(*alst);
		list->next = new;
	}
}

void ft_lstclear2(t_list2 **lst, void (*del)(void *))
{
    t_list2 *ptr_list;
    t_list2 *ptr_next;

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