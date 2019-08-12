/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbarbier <cbarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/09 13:49:42 by cbarbier          #+#    #+#             */
/*   Updated: 2019/08/12 14:28:12 by cbarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <woody.h>

int main(int ac, char **av)
{
	t_wdy			obj;

	if (ac != 2)
	{
		dprintf(STDERR, "Usage: %s BINARY", av[0]);
		return (1);
	}
	ft_bzero(&obj, sizeof(t_wdy));
	if (load_file(av[1], &obj) < 0)
		return (1);
	release_file(&obj);
	return (0);
}
