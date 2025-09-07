
#include "../include/get_next_line_bonus.h"                                                     // archivo de cabecera con funciones auxiliares: ft_strlen, ft_strjoin y ft_strchr

static char	*extract_line(char *buffer)
{
	char	*line;                                                                              // línea a devolver al llamar a get_next_line
	size_t	i;                                                                                  // índice para recorrer el buffer

	i = 0;
	if (!buffer || !buffer[i])                                                                  // si el buffer está vacío o no tiene contenido, devolvemos NULL
		return (NULL);
	
	while (buffer[i] && buffer[i] != '\n')                                                      // mientras el buffer tenga caracteres y no haya salto de línea
	{
		i++;                                                                                    // avanzamos i para contar el tamaño de la línea hasta el salto
	}
	
	if (buffer[i] == '\n')                                                                      // si hay un salto de línea, reservamos espacio extra para el salto
		line = (char *)malloc(i + 2);                                                           // malloc i+2: la línea y el salto de línea
	else                                                                                        // si no hay salto, reservamos espacio solo para la línea
		line = (char *)malloc(i + 1);                                                           // malloc i: la línea final
	
	if (!line)                                                                                  // si falla el malloc, devolvemos NULL
		return (NULL);
	
	i = 0;                                                                                      // reiniciamos i a 0
	while (buffer[i] && buffer[i] != '\n')                                                      // mientras el buffer tenga caracteres y no haya salto de línea
	{
		line[i] = buffer[i];                                                                    // copiamos cada carácter del buffer a la línea
		i++;                                                                                    // avanzamos i para continuar copiando caracteres
	}
	
	if (buffer[i] == '\n')                                                                      // si encontramos un salto de línea en el buffer
	{
		line[i] = '\n';                                                                         // añadimos el salto de línea a la línea copiada
		i++;                                                                                    // avanzamos i después de añadir el salto de línea
	}
	
	line[i] = '\0';                                                                             // añadimos el carácter nulo al final de la línea
	return (line);                                                                              // devolvemos la línea creada
}

static char	*update_buffer(char *buffer, ssize_t *flag)
{
	char	*new_buffer;                                                                         // nuevo buffer con el contenido restante después de extraer la línea
	size_t	i;                                                                                   // índice para recorrer el buffer original
	size_t	j;                                                                                   // índice para el nuevo buffer

	i = 0;
	while (buffer[i] && buffer[i] != '\n')                                                      // mientras el buffer tenga caracteres y no haya salto de línea
		i++;                                                                                    // avanzamos i hasta encontrar el salto o el final del buffer
	
	if (!buffer[i])                                                                             // si no hay salto de línea (fin del buffer), liberamos y devolvemos NULL
	{
		free(buffer);                                                                           // liberamos el buffer original
		*flag = 0;                                                                              // marcamos el flag como 0 (sin error)
		return (NULL);
	}
	
	new_buffer = (char *)malloc(ft_strlen(buffer) - i);                                         // reservamos espacio para el buffer restante sin la línea extraída
	if (!new_buffer)                                                                            // si falla el malloc, liberamos el buffer original y devolvemos NULL
	{
		free(buffer);                                                                           // liberamos el buffer original
		*flag = 1;                                                                              // marcamos el flag como 1 (error)
		return (NULL);
	}
	
	i++;                                                                                        // avanzamos i para empezar desde el siguiente carácter después del salto
	j = 0;                                                                                      // inicializamos j en 0
	
	while (buffer[i])                                                                           // mientras queden caracteres en el buffer original
		new_buffer[j++] = buffer[i++];                                                          // copiamos cada carácter restante al nuevo buffer
	
	new_buffer[j] = '\0';                                                                       // añadimos el carácter nulo al final del nuevo buffer
	free(buffer);                                                                               // liberamos el buffer original
	*flag = 0;                                                                                  // marcamos el flag como 0 (sin error)
	return (new_buffer);                                                                        // devolvemos el nuevo buffer actualizado
}

static char	*read_until_newline(int fd, char **buff)
{
	char	*tmp_buffer;                                                                         // buffer temporal para almacenar los datos leídos del fd
	ssize_t	bytes_read;                                                                          // cantidad de bytes leídos del fd

	tmp_buffer = (char *)malloc(BUFFER_SIZE + 1);                                               // reservamos espacio para el buffer temporal
	
	if (!tmp_buffer)                                                                            // si falla el malloc, liberamos el buffer principal y devolvemos NULL
	{
		free(*buff);                                                                            // liberamos el buffer principal
		*buff = NULL;                                                                           // asignamos NULL al buffer principal
		return (NULL);
	}
	
	bytes_read = 1;
	
	while (!ft_strchr(*buff, '\n') && bytes_read > 0)                                           // mientras no se encuentre un salto de línea en buff y bytes leídos > 0
	{
		bytes_read = read(fd, tmp_buffer, BUFFER_SIZE);                                         // leemos datos del fd al buffer temporal
		
		if (bytes_read < 0)                                                                     // si hay un error en la lectura, liberamos los buffers y devolvemos NULL
		{
			free(tmp_buffer);                                                                   // liberamos el buffer temporal
			free(*buff);                                                                        // liberamos el buffer principal
			*buff = NULL;                                                                       // asignamos NULL al buffer principal
			return (NULL);
		}
		
		tmp_buffer[bytes_read] = '\0';                                                          // añadimos el carácter nulo al final de los datos leídos en tmp_buffer
		
		*buff = ft_strjoin(*buff, tmp_buffer);                                                  // concatenamos el buffer temporal al buffer principal
		if (!*buff)                                                                             // si falla la concatenación, liberamos el buffer temporal y devolvemos NULL
		{
			free(tmp_buffer);                                                                   // liberamos el buffer temporal
			return (NULL);
		}
	}
	
	free(tmp_buffer);                                                                           // liberamos el buffer temporal ya que los datos están en el buffer principal
	return (*buff);                                                                             // devolvemos el buffer principal actualizado
}

char	*get_next_line(int fd)
{
	static char	*buff[MAX_FD];                                                                   // buffer estático para mantener el contenido entre llamadas a la función
	char		*line;                                                                           // línea a devolver
	ssize_t		flag;                                                                            // indicador de error para actualizar el buffer

	if (fd < 0 || fd >= MAX_FD || BUFFER_SIZE <= 0)                                              // comprobamos si el fd es válido y si BUFFER_SIZE es mayor a 0
	{
		free(buff[fd]);                                                                         // si no es válido, liberamos el buffer correspondiente a fd
		buff[fd] = NULL;                                                                        // asignamos NULL al buffer para evitar fugas de memoria
		return (NULL);
	}
	
	if (!read_until_newline(fd, &buff[fd]) || (buff[fd] && *buff[fd] == '\0'))                   // llamamos a si read_until_newline para guardar en buffer todo el texto del fd
	{
		free(buff[fd]);                                                                         // liberamos el buffer correspondiente a fd si devuelve NULL o el buffer está vacío
		buff[fd] = NULL;                                                                        // asignamos NULL al buffer si devuelve NULL o el buffer está vacío
		return (NULL);
	}
	
	line = extract_line(buff[fd]);                                                               // extraemos la primera línea del buffer
	
	if (!line)                                                                                   // si no hay línea para extraer, liberamos el buffer y devolvemos NULL
	{
		free(buff[fd]);                                                                         // liberamos el buffer correspondiente a fd
		buff[fd] = NULL;                                                                        // asignamos NULL al buffer para evitar fugas de memoria
		return (NULL);
	}
	
	buff[fd] = update_buffer(buff[fd], &flag);                                                   // actualizamos el buffer para eliminar la línea extraída
	
	if (!buff[fd] && flag == 1)                                                                  // si ocurre un error al actualizar el buffer (flag = 1)
	{
		free(line);                                                                             // liberamos la línea
		return (NULL);
	}
	
	return (line);                                                                               // devolvemos la línea extraída
}

// --------------- main ------------------

// #include <stdio.h>
// #include <fcntl.h>

// int main(int argc, char **argv)
// {
//     int i = 0;
//     int finished = 0;
//     int fds[argc - 1];

//     while (++i < argc)
//         fds[i - 1] = open(argv[i], O_RDONLY);
//     while (!finished)
//     {
//         finished = 1;
//         i = 0;
//         while (i < argc - 1)
//         {
//             if (fds[i] != -1)
//             {
//                 char *line = get_next_line(fds[i]);
//                 if (line)
//                 {
//                     printf("FD %d: %s", fds[i], line);
//                     free(line);
//                     finished = 0;
//                 }
//             }
//             i++;
//         }
//     }
//     i = 0;
//     while (i < argc - 1)
//     {
//         if (fds[i] != -1)
//             close(fds[i]);
//         i++;
//     }
//     return (0);
// }
