/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbarbier <cbarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 13:34:55 by cbarbier          #+#    #+#             */
/*   Updated: 2019/08/12 13:58:59 by cbarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <woody.h>

static int					check_file(char *filename, int *fd
, struct stat *buf)
{
	if ((*fd = open(filename, O_RDONLY)) < 0)
		return (er(OPEN, filename));
	if (fstat(*fd, buf) < 0)
	{
		close(*fd);
		return (er(FSTAT, filename));
	}
	if ((buf->st_mode & S_IFMT) == S_IFDIR)
	{
		close(*fd);
		return (er(DIRECTORY, filename));
	}
	return (0);
}

int							load_file(char *filename, t_wdy *obj)
{
	int					fd;
	struct stat			buf;

	if (check_file(filename, &fd, &buf) < 0)
		return (-1);
	obj->filename = filename;
	obj->size = buf.st_size;
	if ((obj->ptr = mmap(0, obj->size, PROT_READ | PROT_WRITE
	, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		close(fd);
		return (er(MMAP, filename));
	}
	return (0);
}

int							release_file(t_wdy *obj)
{
	if (munmap(obj->ptr, obj->size) < 0)
		return (er(MUNMAP, obj->filename));
	return (0);
}