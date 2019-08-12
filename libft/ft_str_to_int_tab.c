/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_to_int_tab.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbarbier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/19 11:05:35 by cbarbier          #+#    #+#             */
/*   Updated: 2017/09/19 11:09:46 by cbarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	unsigned int	add_int(char *ptr, int *res)
{
	unsigned int	index;
	char			*tmp;

	index = 0;
	while (ptr[index] != ' ' && ptr[index])
		index++;
	tmp = ft_strsub(ptr, 0, index);
	*res = ft_atoi(tmp);
	ft_strdel(&tmp);
	while (ptr[index] && ptr[index] == ' ')
		index++;
	return (index);
}

int						*ft_str_to_int_tab(char *s, int w)
{
	int				*res;
	char			*ptr;
	int				i_res;

	if (!s)
		return (0);
	ptr = (char *)s;
	if (!(res = (int *)ft_memalloc(w * sizeof(int))))
		return (0);
	while (*ptr && *ptr == ' ')
		ptr++;
	i_res = 0;
	while (i_res < w)
	{
		ptr += add_int(ptr, res + i_res);
		i_res++;
	}
	return (res);
}
