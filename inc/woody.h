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
# define SIZE		4096

#define XOR42_DATA_LEN 91
#define ROT13_DATA_LEN 97
#define RC4_DATA_LEN 680
#define NOCYPHER_DATA_LEN 76

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
	NOCYPHER,
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
    char*      	key;
    size_t      size;
	size_t		payloadLen;
	size_t		payloadIndex;
	uint64_t	old_entry;
	uint64_t	text_addr;
	uint64_t	text_offset;
	uint64_t	text_size;
	uint64_t	sc_addr;
	uint64_t	sc_offset;
	uint64_t	sc_size;
	uint64_t	new_offset;
	uint64_t	new_addr;
	uint64_t	new_size;
	void		*txt_seghdr;
	
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

void				_xor42(void);
void				_rot13(void);
void				_rc4(void);
void				funcrc4(unsigned char*input, size_t len, char *key);
void				_nocypher(void);

int			        keygen(t_wdy *obj);

int					parse_arg(t_wdy *obj, int ac, char **av);
int					load_file(char *filename, t_wdy *obj);
int					release_file(t_wdy *obj);
int 				insert_pack(t_wdy *obj);
int					chk_ptr(t_wdy *file, void *begin, size_t size);

int					nocypher_encrypt(t_wdy *obj);
int					insert(t_wdy*obj, int offset);
int					xor42_encrypt(t_wdy *obj);
int					rot13_encrypt(t_wdy *obj);
int					rc4_encrypt(t_wdy *obj);
int					rc4_insert(t_wdy*obj, int offset);

int					handle_elf64(t_wdy *obj);

int					dispatcher(t_wdy *obj);

int					er(enum e_errtype type, char *filename);
int					jmp(t_wdy *obj, void *ptr, unsigned int off, void *ret);
int					ck(t_wdy *obj, void *ptr, unsigned int off);

extern t_wdy_payload	g_payloads[5];

#endif // !WOODY_H