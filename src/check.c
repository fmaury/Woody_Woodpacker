/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbarbier <cbarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/10 16:23:06 by cbarbier          #+#    #+#             */
/*   Updated: 2019/08/13 11:47:55 by cbarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <woody.h>

int					jmp(t_wdy *obj, void *ptr, unsigned int off, void *ret)
{
	void					**var;

	var = (void **)ret;
	if (!ptr || ptr + off > obj->end || ptr + off <= obj->ptr)
		return (er(INVALID, obj->filename));
	*var = ptr + off;
	return (0);
}

int					ck(t_wdy *obj, void *ptr, unsigned int off)
{
	if (!ptr || ptr + off > obj->end || ptr + off <= obj->ptr)
		return (er(INVALID, obj->filename));
	return (0);
}