/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 12:53:11 by mjamil            #+#    #+#             */
/*   Updated: 2025/01/23 13:52:50 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*result;
	int		total_len;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	if (ft_strlen((char *)s + start) < len)
		total_len = ft_strlen((char *)s + start) + 1;
	else
		total_len = len + 1;
	result = (char *)malloc(total_len);
	if (!result)
		return (NULL);
	i = 0;
	while (s[start] && i < len)
		result[i++] = s[start++];
	result[i] = '\0';
	return (result);
}
