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
    uint64_t    i = 0;
    char        *text = (char *)obj->ptr + obj->sc_offset;

    while (i < obj->sc_size)
    {
        text[i] ^= 42;
        i++;
    }
    return (0); 
}

int         nocypher_encrypt(t_wdy *obj)
{
    (void)obj;
    return (0); 
}

int         insert(t_wdy*obj, int offset)
{
    void        *addr;
    int32_t     jump_offset;
    int32_t     sec_offset;

    addr = obj->ptr + offset + obj->payloadLen;
    jump_offset = (void*)(obj->old_entry) - (void*)(obj->sg_addr + obj->sg_size + obj->payloadLen);
    sec_offset = (void*)(obj->sc_addr) - (void*)(obj->sg_addr + obj->sg_size + 0x3e + 4);
    ft_memcpy(obj->ptr + offset, g_payloads[obj->payloadIndex].data, obj->payloadLen);
    ft_memcpy((void *)addr - 4, &jump_offset, 4);
    ft_memcpy(obj->ptr + offset + 0x33, &obj->sc_size, 8);
    ft_memcpy(obj->ptr + offset + 0x3e, &sec_offset, 4);
    return (0);
}

int         rot13_encrypt(t_wdy *obj)
{
    uint64_t        i = 0;
    unsigned char   *text = (unsigned char*)obj->ptr + obj->sc_offset;

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
    funcrc4(input, obj->sc_size, obj->key);
    printf("Key: %s\n", obj->key);
	return (0);
}

int         rc4_insert(t_wdy*obj, int offset)
{
    insert(obj, offset);
    ft_memcpy(obj->ptr + offset + 0x54, obj->key, ft_strlen(obj->key));
    return (0);
}