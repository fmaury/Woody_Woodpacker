/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chk_ptr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaury <fmaury@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 14:16:29 by fmaury            #+#    #+#             */
/*   Updated: 2019/05/03 13:17:01 by fmaury           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <woody.h>

static int		bfr_file(void *bgn_file, void *bgn_ptr)
{
	if (bgn_file > bgn_ptr)
		return (0);
	return (1);
}

static int		aft_file(void *end_file, void *end_ptr)
{
	if (end_file < end_ptr)
		return (0);
	return (1);
}

int				chk_ptr(t_wdy *file, void *begin, size_t size)
{
	if (!bfr_file(file->ptr, begin))
		return (0);
	if (!aft_file(file->ptr + file->size, begin + size))
		return (0);
	return (1);
}