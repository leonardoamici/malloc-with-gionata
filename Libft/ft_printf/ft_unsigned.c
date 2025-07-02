/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unsigned.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lamici <lamici@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 09:52:57 by lamici            #+#    #+#             */
/*   Updated: 2023/06/30 15:33:16 by lamici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"


int size_t_to_str(size_t value, char *buf) 
{
    char temp[32];
    int i = 0;

    if (value == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }

    while (value > 0) {
        temp[i++] = '0' + (value % 10);
        value /= 10;
    }

    int j = 0;
    while (i-- > 0) {
        buf[j++] = temp[i];
    }
    buf[j] = '\0';

	return (j);
}


static void	prova(long long unsigned n)
{
	if (n >= 10)
	{
		prova(n / 10);
		prova(n % 10);
	}
	else
	{
		n = n + 48;
		write(1, &n, 1);
	}
}

int	ft_unsigned(long long unsigned n)
{
	long	x;
	int		y;

	y = 1;
	x = n;
	prova(n);
	while (x >= 10)
	{
		x = x / 10;
		y++;
	}
	return (y);
}
