/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_macho64.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbarbier <cbarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 11:49:46 by cbarbier          #+#    #+#             */
/*   Updated: 2019/08/14 16:51:26 by cbarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <woody.h>

#define ASM "\x55\x48\x89\xe5\x48\x83\xec\x20\xb8\x01\x00\x00\x00\x48\x8d\x0d\x4e\x00\x00\x00\xba\x07\x00\x00\x00\xc7\x45\xfc\x00\x00\x00\x00\x89\x7d\xf8\x48\x89\x75\xf0\x89\xc7\x48\x89\xce\xe8\x0f\x00\x00\x00\x31\xff\x48\x89\x45\xe8\x89\xf8\x48\x83\xc4\x20\x5d\xc3"

static int		write_file(t_wdy *obj)
{
	int			fd;

	if ((fd = open(BIN_NAME, O_WRONLY | O_CREAT, 0777)) < 0)
		return (er(OPEN, obj->filename));
	ft_memcpy(obj->txt, ASM, 64);
	write(fd, obj->ptr, obj->size);
	if (close(fd) < 0)
		return (er(CLOSE, obj->filename));
	LOG("file created")
	return (0);
}

static int		get_txt_section(t_wdy *obj
, struct segment_command_64 *sg, bool swap)
{
	unsigned int			i;
	struct section_64		*sc;

	if (jmp(obj, sg, sizeof(struct segment_command_64), &sc) < 0)
		return (-1);
	if (ck(obj, sc, sizeof(struct section_64)) < 0)
		return (-1);
	if (strcmp(sg->segname, "__TEXT"))
		return (0);
	read32(&sg->nsects, swap);
	i = 0;
	while (i++ < sg->nsects)
	{
		if (ck(obj, sc, sizeof(*sc)) < 0)
			return (-1);
			
		if (!ft_strcmp(SECT_TEXT, sc->sectname))
		{
			if (jmp(obj, obj->ptr, read32(&sc->offset, swap), &obj->txt) < 0)
				return (-1);
			if (ck(obj, obj->txt, read64(&sc->size, swap)) < 0)
				return (-1);
			obj->txtsize = sc->size;
		}
		sc++;
	}
	return (0);
}

int				handle_macho64(t_wdy *obj, bool swap)
{
	size_t						i;
	struct mach_header_64		*header;
	struct load_command			*lc;

	LOG("handle macho 64")
	obj->swap = swap;
	i = 0;
	header = (struct mach_header_64 *)obj->ptr;
	if (jmp(obj, header, sizeof(*header), &lc) < 0)
		return (-1);
	obj->type = MACHO64;
	read32(&header->ncmds, swap);
	while (i++ < header->ncmds)
	{
		if (ck(obj, lc, sizeof(struct load_command)))
			return (-1);
		if (read32(&lc->cmd, swap) == LC_SEGMENT_64)
			get_txt_section(obj, (struct segment_command_64 *)lc, swap);
		if (read32(&lc->cmd, swap) == LC_MAIN)
		{
			obj->p_entry = (uint64_t *)(lc + 1);
			obj->old_entry = read64(obj->p_entry, obj->swap);
		}
		lc = (void*)lc + read32(&lc->cmdsize, swap);
	}
	if (i < header->ncmds)
		return (er(INVALID, obj->filename));
	write64(obj->p_entry, (void*)obj->txt - obj->ptr, obj->swap);
	write_file(obj);
	return (0);
}