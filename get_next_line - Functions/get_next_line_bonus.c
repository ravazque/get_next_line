/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:34:42 by ravazque          #+#    #+#             */
/*   Updated: 2024/10/22 18:40:07 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*ext_line(char *buffer)
{
	char	*line;
	size_t	i;

	i = 0;
	if (!buffer[i])
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

static char	*upd_buffer(char *buffer)
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
		return (NULL);
	}
	new_buffer = (char *)malloc(ft_strlen(buffer) - i);
	if (!new_buffer)
		return (NULL);
	i++;
	j = 0;
	while (buffer[i])
		new_buffer[j++] = buffer[i++];
	new_buffer[j] = '\0';
	free(buffer);
	return (new_buffer);
}

char	*get_next_line(int fd)
{
	static char	*buff[MAX_FD];
	char		*line;
	char		*tmp_buffer;
	ssize_t		bytes_read;

	if (fd < 0 || fd >= MAX_FD || BUFFER_SIZE <= 0)
		return (NULL);
	tmp_buffer = (char *)malloc(BUFFER_SIZE + 1);
	if (!tmp_buffer)
		return (NULL);
	bytes_read = 1;
	while (!ft_strchr(buff[fd], '\n') && bytes_read > 0)
	{
		bytes_read = read(fd, tmp_buffer, BUFFER_SIZE);
		if (bytes_read < 0)
			return (free(tmp_buffer), free(buff[fd]), buff[fd] = NULL, NULL);
		tmp_buffer[bytes_read] = '\0';
		buff[fd] = ft_strjoin(buff[fd], tmp_buffer);
		if (!buff[fd])
			return (free(tmp_buffer), NULL);
	}
	free(tmp_buffer);
	if (!buff[fd] || *buff[fd] == '\0')
		return (free(buff[fd]), buff[fd] = NULL, NULL);
	return (line = ext_line(buff[fd]), buff[fd] = upd_buffer(buff[fd]), line);
}
