/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 22:38:40 by ravazque          #+#    #+#             */
/*   Updated: 2025/06/24 18:40:33 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/get_next_line.h"

static char	*extract_line(char *buffer)
{
	char	*line;
	size_t	i;

	i = 0;
	if (!buffer || !buffer[i])
		return (NULL);
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\n')
		line = (char *)malloc(i + 2);
	else
		line = (char *)malloc(i + 1);
	if (!line)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		line[i] = buffer[i];
		i++;
	}
	if (buffer[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

static char	*update_buffer(char *buffer, ssize_t *flag)
{
	char	*new_buffer;
	size_t	i;
	size_t	j;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (!buffer[i])
	{
		free(buffer);
		return (*flag = 0, NULL);
	}
	new_buffer = (char *)malloc(ft_strlen(buffer) - i);
	if (!new_buffer)
		return (free(buffer), *flag = 1, NULL);
	i++;
	j = 0;
	while (buffer[i])
		new_buffer[j++] = buffer[i++];
	new_buffer[j] = '\0';
	free(buffer);
	return (*flag = 0, new_buffer);
}

static char	*read_until_newline(int fd, char **buffer)
{
	char	*tmp_buffer;
	ssize_t	bytes_read;

	tmp_buffer = (char *)malloc(BUFFER_SIZE + 1);
	if (!tmp_buffer)
		return (free(*buffer), *buffer = NULL, NULL);
	bytes_read = 1;
	while (!ft_strchr(*buffer, '\n') && bytes_read > 0)
	{
		bytes_read = read(fd, tmp_buffer, BUFFER_SIZE);
		if (bytes_read < 0)
			return (free(tmp_buffer), free(*buffer), *buffer = NULL, NULL);
		tmp_buffer[bytes_read] = '\0';
		*buffer = ft_strjoin(*buffer, tmp_buffer);
		if (!*buffer)
			return (free (tmp_buffer), NULL);
	}
	return (free(tmp_buffer), *buffer);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*line;
	ssize_t		flag;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (free(buffer), buffer = NULL, NULL);
	if (!read_until_newline(fd, &buffer) || (buffer && *buffer == '\0'))
		return (free(buffer), buffer = NULL, NULL);
	line = extract_line(buffer);
	if (!line)
		return (free(buffer), buffer = NULL, NULL);
	buffer = update_buffer(buffer, &flag);
	if (!buffer && flag == 1)
		return (free(line), NULL);
	return (line);
}
