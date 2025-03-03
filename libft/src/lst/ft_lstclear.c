/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:07:16 by ksinn             #+#    #+#             */
/*   Updated: 2024/10/14 13:45:20 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
Deletes and frees the given node and every successor of that node,
using the function ’del’ and free(3).
Finally, the pointer to the list must be set to NULL
*/
void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*curr;
	t_list	*nxt;

	if (!lst || !(*lst) || !del)
		return ;
	curr = (*lst);
	while (curr)
	{
		nxt = curr->next;
		ft_lstdelone(curr, *del);
		curr = nxt;
	}
	*lst = NULL;
}
