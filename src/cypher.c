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
    char *encr = (char*)obj->ptr;

    while (i < obj->text_size)
    {
        encr[obj->entry + i] ^= 42;
        i++;
    }
    return (1); 
}

int         xor42_insert(t_wdy*obj, int offset)
{
    uint32_t jump_offset;
    const char *data;

    data = g_payloads[obj->payloadIndex].data;
    // Copie le shellcode dans la section qui va bien
    ft_memcpy(obj->ptr + offset, data, obj->payloadLen);
    // Calcule l'offset du jump. Debut de la section texte, moins l'offset de la fin de notre shellcode (-1 pour l'octet du jump)
    jump_offset = (obj->entry) - (offset + obj->payloadLen - 1);
    // On ajoute a l'octet du jump la valeur de jump_offset pour jumper au debut de la section text
    ft_memcpy(obj->ptr + offset + obj->payloadLen, (void *)&jump_offset, 4);
    // Ensuite on change la valeur du mov dans notre shellcode par l'offset pour aller au debut de la sec text
    // Debut de la section text + la valeur deja presente dans le mov ($$) moins l'offset du debut de notre shellcode
    jump_offset = obj->entry - offset + *((uint32_t*)(obj->ptr + offset + 27));
    ft_memcpy(obj->ptr + offset + 27, (void *)&jump_offset, 4);
    // La taille de notre section text
    ft_memcpy(obj->ptr + offset + 32, (void *)&obj->text_size, 4);
    *(uint64_t *)obj->entry_addr = offset;
    return (0);
}

int         rot13_encrypt(t_wdy *obj)
{
    (void)obj;
    return (0);
}

int         rot13_insert(t_wdy*obj, int offset)
{
    (void)obj;
    (void)offset;
    return (0);
}

int         rc4_encrypt(t_wdy *obj)
{
    (void)obj;
    return (0);
}

int         rc4_insert(t_wdy*obj, int offset)
{
    (void)obj;
    (void)offset;
    return (0);
}