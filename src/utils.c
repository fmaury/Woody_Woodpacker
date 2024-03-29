/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbarbier <cbarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 17:09:11 by cbarbier          #+#    #+#             */
/*   Updated: 2019/08/14 14:44:43 by cbarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <woody.h>

t_wdy_payload	g_payloads[4] = {
    {XOR42, "1", "XOR42", (char *)_xor42, XOR42_DATA_LEN, xor42_encrypt, insert},
    {ROT13, "2", "ROT13", (char *)_rot13, ROT13_DATA_LEN, rot13_encrypt, insert},
    {RC4, "3", "RC4", (char *)_rc4, RC4_DATA_LEN, rc4_encrypt, rc4_insert},
    {END_CYPHER, 0, 0, 0, 0, 0, 0}
};

int     is_good_key(char *s)
{
    size_t      ln = ft_strlen(s);
    char        c;

    if (ln < 3 || ln > 16)
        return (0);
    while (ln--)
    {
        c = s[ln];
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')))
            return (0);
    }
    return (1);
}

int		parse_arg(t_wdy *obj, int ac, char **av)
{
    int         i = 0;

	if (ac < 2 || ac > 6)
        return (-1);
    if (ac == 2)
    {
        obj->payloadLen = g_payloads[0].len;
		obj->payloadIndex = 0;
        return (0);
    }
    if (ac != 4 && ac != 6)
        return (-1);
    if (!ft_strcmp(av[2], "-c"))
    {
        while (g_payloads[i].type != END_CYPHER && ft_strcmp(g_payloads[i].name1, av[3]) && ft_strcmp(g_payloads[i].name2, av[3]))
            i++;
        if (g_payloads[i].type == END_CYPHER)
            return (-1);
        obj->payloadLen = g_payloads[i].len;
        obj->payloadIndex = i;
        
    }
    else
        return (-1);
    if (ac == 4 || obj->payloadIndex != RC4)
        return (0);
    if (ft_strcmp(av[4], "-s") || !is_good_key(av[5]))
        return (-1);
    obj->key = ft_strdup(av[5]);
    return (0);
}

int             keygen(t_wdy *obj)
{
    int         fd;
    char        *key;
    size_t      count = 0;
    char        c;

    if (obj->key)
        return (0);
    if ((fd = open("/dev/random", O_RDONLY)) < 0)
        return (er(OPEN, "/dev/random"));
    if (!(key = ft_memalloc(17)))
        return (er(MALLOC, "/dev/random"));
    while (count < 16)
    {
        if (read(fd, &c, 1) < 0)
            return (er(DEFAULT_ERR, "/dev/random"));
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c<= 'z') || (c >= '0' && c <= '9'))
            key[count++] = c;
    }
    if (close(fd) == -1)
    	return (er(CLOSE, "/dev/random"));
    obj->key = key;
    return (0);
}