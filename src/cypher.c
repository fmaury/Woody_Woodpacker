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
    int i = 0;
    unsigned char *encr = (unsigned char*)obj->ptr;

    while (i < obj->text_size)
    {
        encr[obj->entry + i] = (encr[obj->entry + i] + 13) % 256;
        i++;
    }
    return (1); 
}

int         rot13_insert(t_wdy*obj, int offset)
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

int         rc4_encrypt(t_wdy *obj)
{
    if (!obj->key)
        obj->key = ft_strdup("Key");
    unsigned char *input = (unsigned char*)(obj->ptr + obj->entry);
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
    ft_memcpy(obj->ptr + offset + 74, (void *)obj->key, 3);
    *(uint64_t *)obj->entry_addr = offset;
    free(obj->key);
    return (0);
}