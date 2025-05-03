/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putsize_base_fd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 14:27:40 by ltheveni          #+#    #+#             */
/*   Updated: 2025/05/03 14:30:16 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putsize_base_fd(size_t n, char *base, int fd)
{
	size_t	base_len;
	char	result;

	base_len = 0;
	while (base[base_len])
		base_len++;
	if (n >= base_len)
		ft_putsize_base_fd(n / base_len, base, fd);
	result = base[n % base_len];
	write(fd, &result, 1);
}
