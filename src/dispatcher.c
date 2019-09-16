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

static t_woody_handler	g_woody_hdlrs[] = {
	{ELF64, 5, {0x7f, 0x45, 0x4c, 0x46, 0x02}, &handle_elf64},
	{NONE, 0, {0, 0, 0, 0, 0}, 0}
};

static int				magic_matched(t_wdy *obj, size_t ih, unsigned char *ptr)
{
	size_t			i = 0;

	if (ck(obj, ptr, g_woody_hdlrs[ih].nbytes * 5) < 0)
		return (-1);
	while (i < g_woody_hdlrs[ih].nbytes)
	{
		if (ptr[i] != g_woody_hdlrs[ih].mag[i])
			return (0);
		i++;
	}
	return (1);
}

int						dispatcher(t_wdy *obj)
{
	size_t			i;
	unsigned char	*ptr;
	int				ret;

	i = 0;
	ret = 0;
	ptr = (unsigned char *)(obj->ptr);
	while (g_woody_hdlrs[i].type != NONE && !magic_matched(obj, i, ptr))
		i++;
	if (g_woody_hdlrs[i].type == NONE)
		return (er(INVALID, obj->filename));
	ret = g_woody_hdlrs[i].f(obj);
	return (ret);
}
