/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   woody.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbarbier <cbarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/09 13:50:18 by cbarbier          #+#    #+#             */
/*   Updated: 2019/08/09 14:31:25 by cbarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOODY_H
# define WOODY_H

# include <unistd.h>
# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <ar.h>
# include <mach-o/ranlib.h>
# include <mach-o/arch.h>
# include <mach-o/swap.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <stdbool.h>

# define STDERR     2

enum                e_filetype
{
    MACHO32,
    MACHO64,
    ELF32,
    ELF64
};

enum				e_errtype
{
	OPEN,
	MMAP,
	MUNMAP,
	FSTAT,
	INVALID,
	DEFAULT_ERR,
};

typedef struct		s_wdy_err
{
	enum e_errtype	type;
	const char		*mess;
}					t_wdy_err;

typedef struct s_wdy
{
    void        *file;
    size_t      key;
    char        *filename;
}               t_wdy;

int					er(enum e_errtype type, t_wdy *wdy);

#endif // !WOODY_H