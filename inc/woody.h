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

#define XOR42_DATA "\xeb\x4b\x48\x31\xff\x48\x31\xf6\x48\x31\xd2\x48\x31\xc0\x40\xb7\x01\x5e\xb2\x0e\xb0\x01\x0f\x05\x48\x8d\x1d\xe1\xff\xff\xff\xb9\x41\x41\x41\x41\xbf\x01\x00\x00\x00\x48\x89\xde\xba\x0e\x00\x00\x00\xb8\x01\x00\x00\x00\x53\x51\x0f\x05\x59\x5b\x80\x33\x2a\x48\xff\xc3\x48\xff\xc9\x48\x83\xf9\x00\x75\xf1\xeb\x14\xe8\xb0\xff\xff\xff\x2e\x2e\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x2e\x2e\x0a\x00\xe9"
#define XOR42_DATA_LEN 98

#define ROT13_DATA "\xeb\x39\x48\x31\xff\x48\x31\xf6\x48\x31\xd2\x48\x31\xc0\x40\xb7\x01\x5e\xb2\x0e\xb0\x01\x0f\x05\x48\x8d\x1d\xe1\xff\xff\xff\xb9\x41\x41\x41\x41\x48\x31\xc0\x8a\x03\x2c\x0d\x88\x03\x48\xff\xc3\x48\xff\xc9\x48\x83\xf9\x00\x75\xeb\xeb\x14\xe8\xc2\xff\xff\xff\x2e\x2e\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x2e\x2e\x0a\x00\xe9"
#define ROT13_DATA_LEN 80

#define RC4_DATA "\xeb\x2f\x48\x31\xff\x48\x31\xf6\x48\x31\xd2\x48\x31\xc0\x40\xb7\x01\x5e\xb2\x0e\xb0\x01\x0f\x05\x48\x8d\x3d\xe1\xff\xff\xff\xbe\x41\x41\x41\x41\xeb\x1f\x5a\xe8\x22\x00\x00\x00\xe9\x2f\x01\x00\x00\xe8\xcc\xff\xff\xff\x2e\x2e\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x2e\x2e\x0a\x00\xe8\xdc\xff\xff\xff\x4b\x65\x79\x00\x53\x49\x89\xd0\x31\xc0\x48\x81\xec\x88\x00\x00\x00\x48\x8d\x4c\x24\x88\x66\x0f\x1f\x04\x00\x88\x04\x01\x48\x83\xc0\x01\x48\x3d\x00\x01\x00\x00\x75\xf1\x41\x0f\xbe\x10\x31\xf6\x84\xd2\x74\x0e\x0f\x1f\x00\x48\x83\xc6\x01\x41\x80\x3c\x30\x00\x75\xf5\x41\xb9\x01\x00\x00\x00\x4c\x8d\x91\xff\x00\x00\x00\x31\xdb\x49\x29\xc9\xeb\x17\x66\x0f\x1f\x04\x00\x49\x8d\x04\x09\x31\xd2\x48\x83\xc1\x01\x48\xf7\xf6\x41\x0f\xbe\x14\x10\x0f\xb6\x01\x41\x89\xc3\x01\xd8\x01\xd0\x99\xc1\xea\x18\x01\xd0\x0f\xb6\xc0\x29\xd0\x89\xc3\x48\x98\x0f\xb6\x54\x04\x88\x88\x11\x44\x88\x5c\x04\x88\x49\x39\xca\x75\xc4\x44\x0f\xb6\x0f\x45\x84\xc9\x74\x6a\x31\xc0\x66\x0f\x1f\x04\x00\x48\x83\xc0\x01\x80\x3c\x07\x00\x75\xf6\x48\x89\xfe\x4c\x8d\x04\x38\x31\xc9\x31\xd2\xeb\x09\x66\x0f\x1f\x04\x00\x44\x0f\xb6\x0e\x83\xc2\x01\x48\x83\xc6\x01\x44\x0f\xb6\xd2\x42\x0f\xb6\x44\x14\x88\x4c\x89\xd2\x01\xc1\x44\x0f\xb6\xd9\x42\x0f\xb6\x5c\x1c\x88\x4c\x89\xd9\x42\x88\x5c\x14\x88\x42\x88\x44\x1c\x88\x42\x02\x44\x14\x88\x0f\xb6\xc0\x44\x32\x4c\x04\x88\x44\x88\x4e\xff\x49\x39\xf0\x75\xb9\x48\x81\xc4\x88\x00\x00\x00\x48\x89\xf8\x5b\xc3\xe9"
#define RC4_DATA_LEN 353

#define SHELLCODE_LEN 98

enum                e_filetype
{
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
	TRUNCATED,
	NOSPACE,
	OPEN_NEW,
	ALR_PACKD,
	DEFAULT_ERR,
};

enum				e_cypher
{
	XOR42,
	ROT13,
	RC4,
	END_CYPHER
};

typedef struct		s_wdy_err
{
	enum e_errtype	type;
	const char		*mess;
}					t_wdy_err;

typedef struct		s_wdy_payload t_wdy_payload;

typedef struct s_wdy
{
    char        *filename;
    void        *ptr;
	enum e_cypher cypher;
	void		*end;
    size_t      key;
    size_t      size;
	size_t		payloadLen;
	size_t		payloadIndex;
	void		*entry_addr;
	uint64_t	entry;
	uint64_t	text_addr;
	int			text_offset;
	int			text_size;
}               t_wdy;

typedef struct		s_wdy_payload
{
	enum e_cypher	type;
	const char		*name1;
	const char		*name2;
	const char		*data;
	size_t			len;
	int				(*fencrypt)(t_wdy *);
	int				(*finsert)(t_wdy *, int);
}					t_wdy_payload;

typedef struct		s_woody_handler
{
	enum e_filetype	type;
	size_t			nbytes;
	uint8_t			mag[5];
	int				(*f)(t_wdy *);
}					t_woody_handler;


int					parse_arg(t_wdy *obj, int ac, char **av);
int					load_file(char *filename, t_wdy *obj);
int					release_file(t_wdy *obj);
int 				insert_pack(t_wdy *obj);
int					chk_ptr(t_wdy *file, void *begin, size_t size);

int					check_null_space(t_wdy *obj);

int					xor42_encrypt(t_wdy *obj);
int					xor42_insert(t_wdy*obj, int offset);
int					rot13_encrypt(t_wdy *obj);
int					rot13_insert(t_wdy*obj, int offset);
int					rc4_encrypt(t_wdy *obj);
int					rc4_insert(t_wdy*obj, int offset);

int					handle_elf32(t_wdy *obj);
int					handle_elf64(t_wdy *obj);

int					dispatcher(t_wdy *obj);
int					render(t_wdy *obj);

int					er(enum e_errtype type, char *filename);
int					jmp(t_wdy *obj, void *ptr, unsigned int off, void *ret);
int					ck(t_wdy *obj, void *ptr, unsigned int off);

extern t_wdy_payload	g_payloads[4];


#endif // !WOODY_H