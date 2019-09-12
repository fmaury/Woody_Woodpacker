/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbarbier <cbarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 13:34:55 by cbarbier          #+#    #+#             */
/*   Updated: 2019/08/14 10:55:38 by cbarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <woody.h>

static int					check_file(char *filename, int *fd
, struct stat *buf)
{
	if ((*fd = open(filename, O_RDONLY)) < 0)
		return (er(OPEN, filename));
	if (syscall(5, *fd, buf) < 0)
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
	obj->end = obj->ptr + obj->size;
	return (0);
}

int							release_file(t_wdy *obj)
{
	if (!obj->ptr)
		return (0);
	free(obj->ptr);
	return (0);
}