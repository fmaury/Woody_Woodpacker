/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   woody.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbarbier <cbarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/09 13:50:18 by cbarbier          #+#    #+#             */
/*   Updated: 2019/08/27 17:01:09 by cbarbier         ###   ########.fr       */
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
# include <elf.h>

# define STDERR     2
# define DEBUG		1
# define BIN_NAME	"woody"

# define LOG(x) if (DEBUG){dprintf(STDERR, "%s\n", x);}


# define ELF64_SHELLCODE "\xeb\x18\x48\x31\xff\x48\x31\xf6\x48\x31\xd2\x48\x31\xc0\x40\xb7\x01\x5e\xb2\x07\xb0\x01\x0f\x05\xeb\x0d\xe8\xe3\xff\xff\xff\x63\x6f\x75\x63\x6f\x75\x0a\x00\xe9"
# define SHELLCODE_LEN 40
enum                e_filetype
{
    MACHO32,
    MACHO64,
    ELF32,
    ELF64,
	NONE
};

enum				e_errtype
{
	OPEN,
	CLOSE,
	DIRECTORY,
	MMAP,
	MUNMAP,
	MALLOC,
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
<<<<<<< HEAD
	enum e_filetype type;
    char        	*filename;
    void        	*ptr;
	void			*end;
    size_t      	key;
    size_t      	size;
	uint8_t			*txt;
	bool			swap;
	uint64_t		*p_entry;
	uint64_t		entry;
	uint64_t		old_entry;
	uint64_t		txtsize;
}               	t_wdy;

typedef struct		s_woody_handler
{
	enum e_filetype	type;
	size_t			nbytes;
	uint32_t		mag[2];
	int				(*f)(t_wdy *, bool);
	bool			swap;
}					t_woody_handler;
=======
    char        *filename;
    void        *ptr;
    size_t      key;
    size_t      size;
	void		*entry_addr;
	uint64_t		entry;
}               t_wdy;
>>>>>>> origin/insert_pack

int					load_file(char *filename, t_wdy *obj);
int					release_file(t_wdy *obj);
int 				insert_pack(t_wdy *obj);
int					chk_ptr(t_wdy *file, void *begin, size_t size);

int					handle_macho32(t_wdy *obj, bool swap);
int					handle_macho64(t_wdy *obj, bool swap);

int					dispatcher(t_wdy *obj);
int					render(t_wdy *obj);




int					er(enum e_errtype type, char *filename);
int					jmp(t_wdy *obj, void *ptr, unsigned int off, void *ret);
int					ck(t_wdy *obj, void *ptr, unsigned int off);



uint16_t			read16(uint16_t *ptr, bool swap);
uint32_t			read32(uint32_t *ptr, bool swap);
uint64_t			read64(uint64_t *ptr, bool swap);

uint64_t			*write64(uint64_t *ptr, uint64_t x, bool swap);
uint32_t			*write32(uint32_t *ptr, uint32_t x, bool swap);








#endif // !WOODY_H