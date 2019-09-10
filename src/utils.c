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
    {XOR42, "1", "XOR42", XOR42_DATA, XOR42_DATA_LEN, xor42_encrypt, insert},
    {ROT13, "2", "ROT13", ROT13_DATA, ROT13_DATA_LEN, rot13_encrypt, rot13_insert},
    {RC4, "3", "RC4", RC4_DATA, RC4_DATA_LEN, rc4_encrypt, rc4_insert},
    {END_CYPHER, 0, 0, 0, 0, 0, 0}
};

int     is_good_key(char *s)
{
    size_t i = 0;

    if (ft_strlen(s) != 3)
        return (0);
    while (i < ft_strlen(s))
    {
        if (s[i] < 33 || s[i] > 125)
            return (0);
        i++;
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
    if (ac < 5)
    {
        if (!av[3])
            return (-1);
        if (!ft_strcmp(av[2], "-c"))
        {
            while (g_payloads[i].type != END_CYPHER && ft_strcmp(g_payloads[i].name1, av[3]) && ft_strcmp(g_payloads[i].name2, av[3]))
                i++;
            if (g_payloads[i].type == END_CYPHER)
                return (-1);
            obj->payloadLen = g_payloads[i].len;
            obj->payloadIndex = i;
            return (0);
        }
        else if (!ft_strcmp(av[2], "-s"))
        {
            if (!is_good_key(av[3]))
                return (-1);
            obj->key = ft_strdup(av[3]);
            return (0);
        }
        else
            return (-1);

    }
    int j = 1;
    while (j < 3)
    {
        i = 0;
        if (!av[j * 2 + 1])
            return (-1);
        if (!ft_strcmp(av[j * 2], "-c"))
        {
            while (g_payloads[i].type != END_CYPHER && ft_strcmp(g_payloads[i].name1, av[j * 2 + 1]) && ft_strcmp(g_payloads[i].name2, av[j * 2 + 1]))
                i++;
            if (g_payloads[i].type == END_CYPHER)
                return (-1);
            obj->payloadLen = g_payloads[i].len;
            obj->payloadIndex = i;
            break ;
        }
        j++;
    }
    if (j == 3)
        return (-1);
    if (j == 1)
        j++;
    else 
        j--;
    if (ft_strcmp("-s", av[j * 2]) || !av[j * 2 + 1])
        return (-1);
    if (!is_good_key(av[j * 2+ 1]))
        return (-1);
    obj->key = ft_strdup(av[j * 2+ 1]);
    return (0);
}