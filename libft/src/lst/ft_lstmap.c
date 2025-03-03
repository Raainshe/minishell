/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 16:18:48 by ksinn             #+#    #+#             */
/*   Updated: 2024/10/14 13:47:00 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
Iterates the list ’lst’ and applies the function ’f’ on the content of each node.
Creates a new list resulting of the successive applications of the function ’f’.
The ’del’ function is used to delete the content of a node if needed
Return
The new list
NULL if the allocation fails
*/
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*start;
	t_list	*cur;
	void	*tmp;

	if (!lst || !(*f) || !(*del))
		return (NULL);
	start = NULL;
	while (lst)
	{
		tmp = (*f)(lst->content);
		cur = ft_lstnew(tmp);
		if (!cur)
		{
			(*del)(tmp);
			ft_lstclear(&start, (*del));
			return (NULL);
		}
		ft_lstadd_back(&start, cur);
		lst = lst->next;
	}
	return (start);
}
