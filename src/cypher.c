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
        text[i] ^= 0;
        i++;
    }
    return (1); 
}

int         xor42_insert(t_wdy*obj, int offset)
{
    void        *addr;
    int32_t     jump_offset;

    addr = obj->ptr + offset + obj->payloadLen;
    jump_offset = (void*)(obj->old_entry) - (void*)(obj->text_addr + obj->text_size + obj->payloadLen);
    printf("jump in file %ld jump offset %x   res %ld\n", offset + obj->payloadLen, jump_offset, offset + obj->payloadLen + jump_offset);
    ft_memcpy(obj->ptr + offset, (void *)xor42, obj->payloadLen);
    ft_memcpy((void *)addr - 4, &jump_offset, 4);
    // data = g_payloads[obj->payloadIndex].data;
    // ft_memcpy(obj->ptr + offset, data, obj->payloadLen);
    // jump_offset = obj->old_entry - offset;
    // ft_memcpy(obj->ptr + offset + obj->payloadLen, (void *)&jump_offset, 4);
    // jump_offset = obj->sc_addr - offset + *((uint32_t*)(obj->ptr + offset + 27));
    // ft_memcpy(obj->ptr + offset + 27, (void *)&jump_offset, 4);
    // ft_memcpy(obj->ptr + offset + 32, (void *)&obj->sc_size, 4);
    return (0);
}

int         rot13_encrypt(t_wdy *obj)
{
    uint64_t i = 0;
    unsigned char *encr = (unsigned char*)obj->ptr;

    while (i < obj->text_size)
    {
        encr[obj->old_entry + i] = (encr[obj->old_entry + i] + 13) % 256;
        i++;
    }
    return (1); 
}

int         rot13_insert(t_wdy*obj, int offset)
{
    uint32_t jump_offset;
    const char *data;

    data = g_payloads[obj->payloadIndex].data;
    ft_memcpy(obj->ptr + offset, data, obj->payloadLen);
    jump_offset = (obj->old_entry) - (offset + obj->payloadLen - 1);
    ft_memcpy(obj->ptr + offset + obj->payloadLen, (void *)&jump_offset, 4);
    jump_offset = obj->old_entry - offset + *((uint32_t*)(obj->ptr + offset + 27));
    ft_memcpy(obj->ptr + offset + 27, (void *)&jump_offset, 4);
    ft_memcpy(obj->ptr + offset + 32, (void *)&obj->text_size, 4);
    return (0);
}

int         rc4_encrypt(t_wdy *obj)
{
    if (!obj->key)
        obj->key = ft_strdup("Key");
    unsigned char *input = (unsigned char*)(obj->ptr + obj->old_entry);
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
	n = 0;
	while(input[n])
		n++;
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
    uint32_t jump_offset;
    const char *data;

    data = g_payloads[obj->payloadIndex].data;
    ft_memcpy(obj->ptr + offset, data, obj->payloadLen);
    jump_offset = (obj->old_entry) - (offset + obj->payloadLen - 1);
    ft_memcpy(obj->ptr + offset + obj->payloadLen, (void *)&jump_offset, 4);
    jump_offset = obj->old_entry - offset + *((uint32_t*)(obj->ptr + offset + 27));
    ft_memcpy(obj->ptr + offset + 27, (void *)&jump_offset, 4);
    ft_memcpy(obj->ptr + offset + 32, (void *)&obj->text_size, 4);
    ft_memcpy(obj->ptr + offset + 74, (void *)obj->key, 3);
    free(obj->key);
    return (0);
}