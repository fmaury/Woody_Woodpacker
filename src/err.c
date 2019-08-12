/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbarbier <cbarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/09 14:12:01 by cbarbier          #+#    #+#             */
/*   Updated: 2019/08/12 13:58:00 by cbarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <woody.h>

static t_wdy_err		g_wdy_err[] = {
	{OPEN, "Open failed"},
	{DIRECTORY, "Is a directory"},
	{MMAP, "Mmap failed"},
	{MUNMAP, "Munmap failed"},
	{FSTAT, "Fstat failed"},
	{INVALID, "The file was not recognized as a valid binary"},
	{DEFAULT_ERR, "Something wrong happened"},
};

int					er(enum e_errtype type, char * filename)
{
	size_t					i;

	i = 0;
	while (g_wdy_err[i].type != DEFAULT_ERR && g_wdy_err[i].type != type)
		i++;
	dprintf(STDERR, "woody_woodpacker: %s: %s\n",
	filename ? filename : "", g_wdy_err[i].mess);
	return (-1);
}