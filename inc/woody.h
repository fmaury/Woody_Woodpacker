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

#define XOR42_DATA "\xeb\x33\x48\x31\xff\x48\x31\xf6\x48\x31\xd2\x48\x31\xc0\x40\xb7\x01\x5e\xb2\x0e\xb0\x01\x0f\x05\x48\x8d\x1d\xe1\xff\xff\xff\xb9\x41\x41\x41\x41\x80\x33\x2a\x48\xff\xc3\x48\xff\xc9\x48\x83\xf9\x00\x75\xf1\xeb\x14\xe8\xc8\xff\xff\xff\x2e\x2e\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x2e\x2e\x0a\x00\xe9"
#define XOR42_DATA_LEN 74

#define ROT13_DATA "\xeb\x39\x48\x31\xff\x48\x31\xf6\x48\x31\xd2\x48\x31\xc0\x40\xb7\x01\x5e\xb2\x0e\xb0\x01\x0f\x05\x48\x8d\x1d\xe1\xff\xff\xff\xb9\x41\x41\x41\x41\x48\x31\xc0\x8a\x03\x2c\x0d\x88\x03\x48\xff\xc3\x48\xff\xc9\x48\x83\xf9\x00\x75\xeb\xeb\x14\xe8\xc2\xff\xff\xff\x2e\x2e\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x2e\x2e\x0a\x00\xe9"
#define ROT13_DATA_LEN 80

#define RC4_DATA "\xeb\x2f\x48\x31\xff\x48\x31\xf6\x48\x31\xd2\x48\x31\xc0\x40\xb7\x01\x5e\xb2\x0e\xb0\x01\x0f\x05\x48\x8d\x3d\xe1\xff\xff\xff\xbe\x41\x41\x41\x41\xeb\x1f\x5a\xe8\x22\x00\x00\x00\xe9\x70\x02\x00\x00\xe8\xcc\xff\xff\xff\x2e\x2e\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x2e\x2e\x0a\x00\xe8\xdc\xff\xff\xff\x41\x41\x41\x00\x55\x48\x89\xe5\x48\x81\xec\xc0\x00\x00\x00\x48\x89\xbd\xd8\xfe\xff\xff\x48\x89\xb5\xd0\xfe\xff\xff\x48\x89\x95\xc8\xfe\xff\xff\xc7\x45\xf8\x00\x00\x00\x00\xc7\x45\xf4\x00\x00\x00\x00\xc7\x45\xe4\x00\x00\x00\x00\xc7\x45\xf0\x00\x00\x00\x00\xeb\x15\x8b\x45\xf0\x89\xc2\x8b\x45\xf0\x48\x98\x88\x94\x05\xe0\xfe\xff\xff\x83\x45\xf0\x01\x81\x7d\xf0\xff\x00\x00\x00\x7e\xe2\xc7\x45\xf4\x00\x00\x00\x00\xc7\x45\xfc\x00\x00\x00\x00\xeb\x04\x83\x45\xfc\x01\x8b\x45\xfc\x48\x63\xd0\x48\x8b\x85\xc8\xfe\xff\xff\x48\x01\xd0\x0f\xb6\x00\x84\xc0\x75\xe5\xc7\x45\xec\x00\x00\x00\x00\xe9\x86\x00\x00\x00\x8b\x45\xec\x48\x98\x0f\xb6\x84\x05\xe0\xfe\xff\xff\x0f\xb6\xd0\x8b\x45\xf4\x8d\x0c\x02\x8b\x45\xec\x99\xf7\x7d\xfc\x89\xd0\x48\x63\xd0\x48\x8b\x85\xc8\xfe\xff\xff\x48\x01\xd0\x0f\xb6\x00\x0f\xbe\xc0\x8d\x14\x01\x89\xd0\xc1\xf8\x1f\xc1\xe8\x18\x01\xc2\x0f\xb6\xd2\x29\xc2\x89\xd0\x89\x45\xf4\x8b\x45\xec\x48\x98\x0f\xb6\x84\x05\xe0\xfe\xff\xff\x88\x45\xe3\x8b\x45\xf4\x48\x98\x0f\xb6\x94\x05\xe0\xfe\xff\xff\x8b\x45\xec\x48\x98\x88\x94\x05\xe0\xfe\xff\xff\x8b\x45\xf4\x48\x98\x0f\xb6\x55\xe3\x88\x94\x05\xe0\xfe\xff\xff\x83\x45\xec\x01\x81\x7d\xec\xff\x00\x00\x00\x0f\x8e\x6d\xff\xff\xff\xc7\x45\xf8\x00\x00\x00\x00\xc7\x45\xf4\x00\x00\x00\x00\xc7\x45\xfc\x00\x00\x00\x00\xeb\x04\x83\x45\xfc\x01\x8b\x45\xfc\x48\x63\xd0\x48\x8b\x85\xd8\xfe\xff\xff\x48\x01\xd0\x0f\xb6\x00\x84\xc0\x75\xe5\xc7\x45\xe8\x00\x00\x00\x00\xe9\xda\x00\x00\x00\x8b\x45\xf8\x8d\x50\x01\x89\xd0\xc1\xf8\x1f\xc1\xe8\x18\x01\xc2\x0f\xb6\xd2\x29\xc2\x89\xd0\x89\x45\xf8\x8b\x45\xf8\x48\x98\x0f\xb6\x84\x05\xe0\xfe\xff\xff\x0f\xb6\xd0\x8b\x45\xf4\x01\xc2\x89\xd0\xc1\xf8\x1f\xc1\xe8\x18\x01\xc2\x0f\xb6\xd2\x29\xc2\x89\xd0\x89\x45\xf4\x8b\x45\xf8\x48\x98\x0f\xb6\x84\x05\xe0\xfe\xff\xff\x88\x45\xe3\x8b\x45\xf4\x48\x98\x0f\xb6\x94\x05\xe0\xfe\xff\xff\x8b\x45\xf8\x48\x98\x88\x94\x05\xe0\xfe\xff\xff\x8b\x45\xf4\x48\x98\x0f\xb6\x55\xe3\x88\x94\x05\xe0\xfe\xff\xff\x8b\x45\xf8\x48\x98\x0f\xb6\x94\x05\xe0\xfe\xff\xff\x8b\x45\xf4\x48\x98\x0f\xb6\x84\x05\xe0\xfe\xff\xff\x01\xd0\x0f\xb6\xc0\x89\x45\xe4\x8b\x45\xe8\x48\x63\xd0\x48\x8b\x85\xd8\xfe\xff\xff\x48\x01\xd0\x0f\xb6\x08\x8b\x45\xe4\x48\x98\x0f\xb6\x84\x05\xe0\xfe\xff\xff\x89\xc6\x8b\x45\xe8\x48\x63\xd0\x48\x8b\x85\xd8\xfe\xff\xff\x48\x01\xd0\x31\xf1\x89\xca\x88\x10\x83\x45\xe8\x01\x8b\x45\xe8\x3b\x45\xfc\x0f\x8c\x1a\xff\xff\xff\x48\x8b\x85\xd8\xfe\xff\xff\xc9\xc3\xe9"
#define RC4_DATA_LEN 674

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
    char*      key;
    size_t      size;
	size_t		payloadLen;
	size_t		payloadIndex;
	void		*entry_addr;
	uint64_t	entry;
	int			text_size;
	int			diff;
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

int					xor42_encrypt(t_wdy *obj);
int					xor42_insert(t_wdy*obj, int offset);
int					rot13_encrypt(t_wdy *obj);
int					rot13_insert(t_wdy*obj, int offset);
int					rc4_encrypt(t_wdy *obj);
int					rc4_insert(t_wdy*obj, int offset);

int					handle_elf64(t_wdy *obj);

int					dispatcher(t_wdy *obj);

int					er(enum e_errtype type, char *filename);
int					jmp(t_wdy *obj, void *ptr, unsigned int off, void *ret);
int					ck(t_wdy *obj, void *ptr, unsigned int off);

extern t_wdy_payload	g_payloads[4];


#endif // !WOODY_H