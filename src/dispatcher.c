/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatcher.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbarbier <cbarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 15:20:33 by cbarbier          #+#    #+#             */
/*   Updated: 2019/08/13 13:23:39 by cbarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <woody.h>

static t_woody_handler	g_nm_hdlrs[] = {
	{MACHO64, 1, {MH_MAGIC_64, 0}, handle_macho64, false},
	{MACHO64, 1, {MH_CIGAM_64, 0}, handle_macho64, true},
	{MACHO32, 1, {MH_MAGIC, 0}, handle_macho32, false},
	{MACHO32, 1, {MH_CIGAM, 0}, handle_macho32, true},
	{NONE, 0, {0, 0}, 0, false}
};

static int				magic_matched(t_wdy *obj, size_t ih, uint32_t *ptr)
{
	size_t			i;

	i = 0;
	if (ck(obj, ptr, g_nm_hdlrs[ih].nbytes * 4) < 0)
		return (-1);
	while (i < g_nm_hdlrs[ih].nbytes)
	{
		if (ptr[i] != g_nm_hdlrs[ih].mag[i])
			return (0);
		i++;
	}
	return (1);
}

int						dispatcher(t_wdy *obj)
{
	size_t			i;
	uint32_t		*ptr32;
	int				ret;

	i = 0;
	ret = 0;
	ptr32 = (u_int32_t *)(obj->ptr);
	while (g_nm_hdlrs[i].type != NONE && !magic_matched(obj, i, ptr32))
		i++;
	if (g_nm_hdlrs[i].type == NONE)
		return (er(INVALID, obj->filename));
	ret = g_nm_hdlrs[i].f(obj, g_nm_hdlrs[i].swap);
	return (!ret ? render(obj) : ret);
}
