/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 12:53:11 by mjamil            #+#    #+#             */
/*   Updated: 2025/01/23 13:52:03 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_in_set(char c, const char *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

static int	count_words(const char *str, const char *set)
{
	int	i;
	int	count;
	int	in_word;

	i = 0;
	count = 0;
	in_word = 0;
	while (str[i])
	{
		if (is_in_set(str[i], set))
		{
			if (in_word)
			{
				count++;
				in_word = 0;
			}
		}
		else
			in_word = 1;
		i++;
	}
	if (in_word)
		count++;
	return (count);
}

static void	do_split(char **list, const char *str, const char *set)
{
	char	*word_start;
	char	*ptr;
	int		i;
	int		word_length;

	i = 0;
	ptr = (char *)str;
	while (*ptr)
	{
		if (!is_in_set(*ptr, set))
		{
			word_start = ptr;
			while (*ptr && !is_in_set(*ptr, set))
				ptr++;
			word_length = ptr - word_start + 1;
			list[i] = (char *)malloc(word_length);
			ft_strlcpy(list[i], word_start, word_length);
			i++;
		}
		else
			ptr++;
	}
	list[i] = NULL;
}

char	**ft_split(const char *str, const char *set)
{
	char	**list;

	list = (char **)malloc((count_words(str, set) + 1) * sizeof(char *));
	if (!list)
		return (NULL);
	do_split(list, str, set);
	return (list);
}
