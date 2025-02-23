/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 12:53:11 by mjamil            #+#    #+#             */
/*   Updated: 2025/01/23 13:53:03 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	read_line(int fd, char **buffer, char **temp);
void	free_strs(char **str1, char **str2, char **str3);
char	*parse_line(char **buffer, char **temp);
char	*ft_strjoin_gnl(char *temp, char *read_buf);

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*temp;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	temp = NULL;
	line = NULL;
	read_line(fd, &buffer, &temp);
	if (buffer && *buffer != '\0')
		line = parse_line(&buffer, &temp);
	if (!line || *line == '\0')
	{
		free_strs(&line, &buffer, &temp);
		return (NULL);
	}
	return (line);
}

void	read_line(int fd, char **buffer, char **temp)
{
	char	*read_buf;
	int		bytes_read;

	read_buf = (char *)malloc(BUFFER_SIZE + 1);
	if (!read_buf)
		return ;
	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, read_buf, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free_strs(&read_buf, buffer, temp);
			return ;
		}
		read_buf[bytes_read] = '\0';
		*temp = ft_strdup_gnl(*buffer);
		free_strs(buffer, NULL, NULL);
		*buffer = ft_strjoin_gnl(*temp, read_buf);
		free_strs(temp, NULL, NULL);
		if (newline_reached(*buffer))
			break ;
	}
	free_strs(&read_buf, NULL, NULL);
}

void	free_strs(char **str1, char **str2, char **str3)
{
	if (str1 && *str1)
	{
		free(*str1);
		*str1 = NULL;
	}
	if (str2 && *str2)
	{
		free(*str2);
		*str2 = NULL;
	}
	if (str3 && *str3)
	{
		free(*str3);
		*str3 = NULL;
	}
}

char	*parse_line(char **buffer, char **temp)
{
	char	*line;

	*temp = ft_strdup_gnl(*buffer);
	free_strs(buffer, NULL, NULL);
	*buffer = get_after_newline(*temp);
	line = get_before_newline(*temp);
	free_strs(temp, NULL, NULL);
	return (line);
}

char	*ft_strjoin_gnl(char *temp, char *read_buf)
{
	char	*result;
	int		len;
	int		i;

	if (!temp && !read_buf)
		return (NULL);
	len = ft_strlen_gnl(temp, '\0');
	i = ft_strlen_gnl(read_buf, '\0');
	result = (char *)malloc(len + i + 1);
	if (!result)
		return (NULL);
	len = -1;
	while (temp && temp[++len])
		result[len] = temp[len];
	i = -1;
	while (read_buf && read_buf[++i])
		result[len + i] = read_buf[i];
	result[len + i] = '\0';
	return (result);
}
