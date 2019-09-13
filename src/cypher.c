/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cypher.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbarbier <cbarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 15:20:33 by cbarbier          #+#    #+#             */
/*   Updated: 2019/08/13 13:23:39 by cbarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <woody.h>

int         xor42_encrypt(t_wdy *obj)
{
    uint64_t i = 0;
    char *text = (char *)obj->ptr + obj->sc_offset;

    while (i < obj->sc_size)
    {
        text[i] ^= 42;
        i++;
    }
    return (0); 
}

int         insert(t_wdy*obj, int offset)
{
    void        *addr;
    int32_t     jump_offset;
    int32_t     sec_offset;

    addr = obj->ptr + offset + obj->payloadLen;
    jump_offset = (void*)(obj->old_entry) - (void*)(obj->text_addr + obj->text_size + obj->payloadLen);
    sec_offset = (void*)(obj->sc_addr) - (void*)(obj->text_addr + obj->text_size + 0x3e + 4);
    ft_memcpy(obj->ptr + offset, g_payloads[obj->payloadIndex].data, obj->payloadLen);
    ft_memcpy((void *)addr - 4, &jump_offset, 4);
    ft_memcpy(obj->ptr + offset + 0x33, &obj->sc_size, 8);
    ft_memcpy(obj->ptr + offset + 0x3e, &sec_offset, 4);
    return (0);
}

int         rot13_encrypt(t_wdy *obj)
{
    uint64_t i = 0;
    unsigned char *text = (unsigned char*)obj->ptr + obj->sc_offset;

    while (i < obj->sc_size)
    {
        text[i] = (text[i] + 13) % 256;
        i++;
    }
    return (0); 
}

int         rc4_encrypt(t_wdy *obj)
{
    if (keygen(obj) < 0)
        return (-1);
    unsigned char   *input = (unsigned char*)(obj->ptr + obj->sc_offset);
    unsigned char   S[256];
	unsigned char   t;
	int             n;
	int             i = 0;
	int             j = 0;
	int             temp = 0;

	for (int i=0; i < 256; i++) {
		S[i] = i;
	}
	j = 0;
	n = 0;
	while(obj->key[n])
		n++;
	for (int i=0; i < 256; i++) {
		j = (j + S[i] + obj->key[i % n]) % 256;
		t = S[i];
		S[i] = S[j];
		S[j] = t;
	}
	i = 0;
	j = 0;
	n = obj->sc_size;
	for (int k = 0; k < n; k++)
	{
		i = (i+1) % 256;
		j = (j + S[i]) % 256;
		t = S[i];
		S[i] = S[j];
		S[j] = t;
		temp = (S[i] + S[j]) % 256;
		input[k] ^= S[temp];
	}
    ft_putstr("key: ");
    i = 0;
    while (i < 256)
    {
        printf("%02hhx", S[i]);
        i++;
    }
    printf("\n");
	return (0);
}

int         rc4_insert(t_wdy*obj, int offset)
{
    insert(obj, offset);
    ft_memcpy(obj->ptr + offset + 0x54, obj->key, ft_strlen(obj->key));
    free(obj->key);
    obj->key = 0;
    return (0);
}