/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 12:53:11 by mjamil            #+#    #+#             */
/*   Updated: 2025/01/23 13:51:24 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	num_len(int nb)
{
	int	len;

	len = 0;
	if (nb == 0)
		return (1);
	if (nb < 0)
	{
		len++;
		nb *= -1;
	}
	while (nb)
	{
		len++;
		nb /= 10;
	}
	return (len);
}

static void	ft_strrev(char *str)
{
	char	tmp;
	size_t	i;
	size_t	j;

	i = 0;
	j = ft_strlen(str) - 1;
	while (i < j)
	{
		tmp = str[i];
		str[i] = str[j];
		str[j] = tmp;
		i++;
		j--;
	}
}

static int	ft_abs(int n)
{
	if (n < 0)
		n *= -1;
	return (n);
}

char	*ft_itoa(int n)
{
	char	*nbstr;
	int		nb;
	size_t	i;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n == 0)
		return (ft_strdup("0"));
	nbstr = malloc((num_len(n) + 1) * sizeof(char));
	if (!nbstr)
		return (NULL);
	nb = ft_abs(n);
	i = 0;
	while (nb)
	{
		nbstr[i++] = (nb % 10) + '0';
		nb /= 10;
	}
	if (n < 0)
		nbstr[i++] = '-';
	nbstr[i] = '\0';
	ft_strrev(nbstr);
	return (nbstr);
}
