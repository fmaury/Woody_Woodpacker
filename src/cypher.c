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
    int i = 0;
    char *encr = (char*)(obj->ptr + obj->text_off);

    while (i < obj->text_size)
    {
        encr[i] ^= 42;
        i++;
    }
    return (1); 
}

int         insert(t_wdy*obj)
{
    uint32_t jump_offset;
    const char *data;

    data = g_payloads[obj->payloadIndex].data;
    ft_memcpy(obj->ptr + obj->sec_off, data, obj->payloadLen);
    jump_offset = (obj->entry) - (obj->sec_vaddr + obj->payloadLen - 1);
    ft_memcpy(obj->ptr + obj->sec_off + obj->payloadLen, (void *)&jump_offset, 4);
    jump_offset = obj->text_vaddr - obj->sec_vaddr + *((uint32_t*)(obj->ptr + obj->sec_off + 27));
    ft_memcpy(obj->ptr + obj->sec_off + 27, (void *)&jump_offset, 4);
    ft_memcpy(obj->ptr + obj->sec_off + 32, (void *)&obj->text_size, 4);
    *(uint64_t *)obj->entry_addr = obj->sec_vaddr;
    return (0);
}

int         rot13_encrypt(t_wdy *obj)
{
    int i = 0;
    char *encr = (char*)obj->ptr;
    int tmp = obj->text_off;

    while (i < obj->text_size)
    {
        encr[tmp + i] = (encr[tmp + i] + 13) % 256;
        i++;
    }
    return (1); 
}

int         rc4_encrypt(t_wdy *obj)
{
    if (keygen(obj) < 0)
        return (-1);
    unsigned char *input = (unsigned char*)(obj->ptr + obj->text_off);
    unsigned char S[256];
	unsigned char t;
	int n;
	int i = 0;
	int j = 0;
	int temp = 0;

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
	n = obj->text_size;
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

int         rc4_insert(t_wdy*obj)
{
    insert(obj);
    ft_memcpy(obj->ptr + obj->sec_off + 74, (void *)obj->key, ft_strlen(obj->key));
    free(obj->key);
    return (0);
}