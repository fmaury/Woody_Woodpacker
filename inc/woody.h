/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   woody.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbarbier <cbarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/09 13:50:18 by cbarbier          #+#    #+#             */
/*   Updated: 2019/08/12 14:27:17 by cbarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOODY_H
# define WOODY_H

# include <libft.h>
# include <unistd.h>
# include <sys/mman.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <stdbool.h>
# include <stdio.h>
# include <sys/syscall.h>
#include <elf.h>

# define STDERR     2
# define ELF64_SHELLCODE "\xeb\x72\x48\x31\xff\x48\x31\xf6\x48\x31\xd2\x48\x31\xc0\x40\xb7\x01\x5e\xb2\x0e\xb0\x01\x0f\x05\x48\x8d\x3d\xe1\xff\xff\xff\xbe\x71\x01\x00\x00\xba\x07\x00\x00\x00\xb8\x0a\x00\x00\x00\x0f\x05\x48\x83\xf8\xff\x74\x67\x48\x8d\x1d\xc3\xff\xff\xff\xbf\x01\x00\x00\x00\x48\x89\xde\xba\x0e\x00\x00\x00\xb8\x01\x00\x00\x00\x0f\x05\xb9\x71\x01\x00\x00\x80\x03\x00\x48\xff\xc3\x48\xff\xc9\xeb\x48\x5e\xba\x0e\x00\x00\x00\xb8\x01\x00\x00\x00\xbf\x01\x00\x00\x00\x0f\x05\xc3\xe8\x89\xff\xff\xff\x2e\x2e\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x2e\x2e\x0a\x00\xe8\xd4\xff\xff\xff\x2e\x2e\x2e\x2e\x57\x4f\x4f\x44\x55\x2e\x2e\x2e\x2e\x0a\x00\xc3\xbf\x01\x00\x00\x00\xb8\x3c\x00\x00\x00\x0f\x05\xe9"
# define SHELLCODE_LEN 170
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
	DIRECTORY,
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
    char        *filename;
    void        *ptr;
    size_t      key;
    size_t      size;
	void		*entry_addr;
	uint64_t	entry;
	uint64_t	text_addr;
	int			text_offset;
	int			text_size;
	int			sec_offset;
}               t_wdy;

int					er(enum e_errtype type, char *filename);
int					load_file(char *filename, t_wdy *obj);
int					release_file(t_wdy *obj);
int 				insert_pack(t_wdy *obj);
int					chk_ptr(t_wdy *file, void *begin, size_t size);

#endif // !WOODY_H