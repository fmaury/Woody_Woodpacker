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
    {XOR42, "1", "XOR42", XOR42_DATA, XOR42_DATA_LEN, xor42_encrypt, xor42_insert},
    {ROT13, "2", "ROT13", ROT13_DATA, ROT13_DATA_LEN, rot13_encrypt, rot13_insert},
    {RC4, "3", "RC4", RC4_DATA, RC4_DATA_LEN, rc4_encrypt, rc4_insert},
    {END_CYPHER, 0, 0, 0, 0, 0, 0}
};


int		parse_arg(t_wdy *obj, int ac, char **av)
{
    int         i = 0;

	if (ac < 2 || ac > 4)
        return (-1);
    if (ac == 2)
    {
        obj->payloadLen = g_payloads[0].len;
		obj->payloadIndex = 0;
        return (0);
    }
    if (ft_strcmp(av[2], "-c"))
        return (-1);
    while (g_payloads[i].type != END_CYPHER && ft_strcmp(g_payloads[i].name1, av[3]) && ft_strcmp(g_payloads[i].name2, av[3]))
        i++;
    if (g_payloads[i].type == END_CYPHER)
        return (-1);
    obj->payloadLen = g_payloads[i].len;
	obj->payloadIndex = i;
	return (0);
}