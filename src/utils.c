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

uint16_t			read16(uint16_t *ptr, bool swap)
{
	uint16_t				x;

	if (!swap)
		return (*ptr);
	x = *ptr;
	*ptr = ((x >> 8) | (x << 8));
	return (*ptr);
}

uint32_t			read32(uint32_t *ptr, bool swap)
{
	uint32_t				x;

	if (!swap)
		return (*ptr);
	x = *ptr;
	*ptr = (((x) >> 24) | (((x) & 0x00FF0000) >> 8)
	| (((x) & 0x0000FF00) << 8) | ((x) << 24));
	return (*ptr);
}

uint64_t			read64(uint64_t *ptr, bool swap)
{
	uint64_t				x;

	if (!swap)
		return (*ptr);
	x = *ptr;
	*ptr = (((x) >> 56)
		| (((x) & (0xFFLL << 48)) >> 40)
		| (((x) & (0xFFLL << 40)) >> 24)
		| (((x) & (0xFFLL << 32)) >> 8)
		| (((x) & (0xFFLL >> 32)) << 8)
		| (((x) & (0xFFLL >> 40)) << 24)
		| (((x) & (0xFFLL >> 48)) << 40)
		| ((x) << 56));
	return (*ptr);
}

uint64_t			*write64(uint64_t *ptr, uint64_t x, bool swap)
{
	if (!swap)
	{
		*ptr = x;
		return (ptr);
	}
	*ptr = (((x) >> 56)
		| (((x) & (0xFFLL << 48)) >> 40)
		| (((x) & (0xFFLL << 40)) >> 24)
		| (((x) & (0xFFLL << 32)) >> 8)
		| (((x) & (0xFFLL >> 32)) << 8)
		| (((x) & (0xFFLL >> 40)) << 24)
		| (((x) & (0xFFLL >> 48)) << 40)
		| ((x) << 56));
	return (ptr);
}

uint32_t			*write32(uint32_t *ptr, uint32_t x, bool swap)
{
	if (!swap)
	{
		*ptr = x;
		return (ptr);
	}
	*ptr = (((x) >> 24) | (((x) & 0x00FF0000) >> 8)
	| (((x) & 0x0000FF00) << 8) | ((x) << 24));
	return (ptr);
}
