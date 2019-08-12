/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbarbier <cbarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/06 15:48:18 by cbarbier          #+#    #+#             */
/*   Updated: 2019/04/26 14:43:08 by cbarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

void		ft_lstsort(t_list *lst, int64_t (*fcmp)(t_list *a, t_list *b))
{
	t_list		*start;
	size_t		len;
	size_t		last;

	if (!lst)
		return ;
	start = lst;
	len = ft_lstlen(lst);
	while (len)
	{
		lst = start;
		last = len--;
		while (lst->next && last--)
		{
			if (fcmp(lst, lst->next) > 0)
				ft_memswap(&(lst->content), &(lst->next->content));
			lst = lst->next;
		}
	}
}
