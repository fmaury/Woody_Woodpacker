/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_macho32.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbarbier <cbarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 11:49:46 by cbarbier          #+#    #+#             */
/*   Updated: 2019/08/13 13:00:39 by cbarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <woody.h>

static int		get_txt_section(t_wdy *obj
, struct segment_command *sg, bool swap)
{
	unsigned int			i;
	struct section		*sc;

	if (jmp(obj, sg, sizeof(struct segment_command), &sc) < 0)
		return (-1);
	i = 0;
	if (ck(obj, sc, sizeof(struct section)) < 0)
		return (-1);
	read32(&sg->nsects, swap);
	while (i++ < sg->nsects)
	{
		if (ck(obj, sc, sizeof(*sc)) < 0)
			return (-1);
		if (!ft_strcmp(SECT_TEXT, sc->sectname))
		{
			if (jmp(obj, obj->ptr, read32(&sc->offset, swap), &obj->txt) < 0)
				return (-1);
			if (ck(obj, obj->txt, read32(&sc->size, swap)) < 0)
				return (-1);
			// obj->txtsize = sc->size;
			// obj->txtaddr = read32(&sc->addr, swap);
		}
		sc++;
	}
	return (0);
}

int				handle_macho32(t_wdy *obj, bool swap)
{
	size_t						i;
	struct mach_header		*header;
	struct load_command			*lc;

	obj->swap = swap;
	i = 0;
	header = (struct mach_header *)obj->ptr;
	if (jmp(obj, header, sizeof(*header), &lc) < 0)
		return (-1);
	obj->type = MACHO32;
	read32(&header->ncmds, swap);
	while (i++ < header->ncmds)
	{
		if (ck(obj, lc, sizeof(struct load_command)))
			return (-1);
		if (read32(&lc->cmd, swap) == LC_SEGMENT)
			get_txt_section(obj, (struct segment_command *)lc, swap);
		lc = (void*)lc + read32(&lc->cmdsize, swap);
	}
	if (i < header->ncmds)
		return (er(INVALID, obj->filename));
	return (0);
}