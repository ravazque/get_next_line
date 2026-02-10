
#include "../include/get_next_line.h"                                                     // archivo de cabecera con funciones auxiliares: ft_strlen, ft_strjoin y ft_strchr

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
		line = (char *)malloc(i + 1);                                                           // malloc i+1: la línea final más el carácter nulo

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
		line[i++] = '\n';                                                                       // añadimos el salto de línea a la línea copiada y avanzamos i
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
		return (*flag = 0, NULL);                                                               // marcamos el flag como 0 (sin error) y devolvemos NULL
	}

	new_buffer = (char *)malloc(ft_strlen(buffer) - i);                                         // reservamos espacio para el buffer restante sin la línea extraída
	if (!new_buffer)                                                                            // si falla el malloc, liberamos el buffer original y devolvemos NULL
	{
		return (free(buffer), *flag = 1, NULL);                                                 // liberamos buffer, marcamos flag como 1 (error) y devolvemos NULL
	}

	i++;                                                                                        // avanzamos i para empezar desde el siguiente carácter después del salto
	j = 0;                                                                                      // inicializamos j en 0
	while (buffer[i])                                                                           // mientras queden caracteres en el buffer original
	{
		new_buffer[j++] = buffer[i++];                                                          // copiamos cada carácter restante al nuevo buffer
	}

	new_buffer[j] = '\0';                                                                       // añadimos el carácter nulo al final del nuevo buffer
	free(buffer);                                                                               // liberamos el buffer original
	return (*flag = 0, new_buffer);                                                             // marcamos flag como 0 (sin error) y devolvemos el nuevo buffer
}

static char	*read_until_newline(int fd, char **buffer)
{
	char	*tmp_buffer;                                                                         // buffer temporal para almacenar los datos leídos del fd
	ssize_t	bytes_read;                                                                          // cantidad de bytes leídos del fd

	tmp_buffer = (char *)malloc(BUFFER_SIZE + 1);                                               // reservamos espacio para el buffer temporal
	if (!tmp_buffer)                                                                            // si falla el malloc, liberamos el buffer principal y devolvemos NULL
	{
		return (free(*buffer), *buffer = NULL, NULL);                                           // liberamos *buffer, lo asignamos a NULL y devolvemos NULL
	}

	bytes_read = 1;                                                                             // inicializamos bytes_read a 1 para entrar al bucle
	while (!ft_strchr(*buffer, '\n') && bytes_read > 0)                                         // mientras no se encuentre un salto de línea en buffer y bytes leídos > 0
	{
		bytes_read = read(fd, tmp_buffer, BUFFER_SIZE);                                         // leemos datos del fd al buffer temporal
		if (bytes_read < 0)                                                                     // si hay un error en la lectura, liberamos los buffers y devolvemos NULL
		{
			return (free(tmp_buffer), free(*buffer), *buffer = NULL, NULL);                     // liberamos ambos buffers, asignamos NULL a *buffer y devolvemos NULL
		}

		tmp_buffer[bytes_read] = '\0';                                                          // añadimos el carácter nulo al final de los datos leídos en tmp_buffer
		*buffer = ft_strjoin(*buffer, tmp_buffer);                                              // concatenamos el buffer temporal al buffer principal

		if (!*buffer)                                                                           // si falla la concatenación, liberamos el buffer temporal y devolvemos NULL
		{
			return (free (tmp_buffer), NULL);                                                   // liberamos tmp_buffer y devolvemos NULL
		}
	}

	return (free(tmp_buffer), *buffer);                                                         // liberamos tmp_buffer (ya que los datos están en el buffer principal) y devolvemos *buffer
}

char	*get_next_line(int fd)
{
	static char	*buffer;                                                                         // buffer estático para mantener el contenido entre llamadas a la función
	char		*line;                                                                           // línea a devolver
	ssize_t		flag;                                                                            // indicador de error para actualizar el buffer

	if (fd < 0 || BUFFER_SIZE <= 0)                                                             // comprobamos si el fd es válido y si BUFFER_SIZE es mayor a 0
	{
		return (free(buffer), buffer = NULL, NULL);                                             // liberamos buffer, lo asignamos a NULL y devolvemos NULL
	}

	if (!read_until_newline(fd, &buffer) || (buffer && *buffer == '\0'))                        // llamamos a read_until_newline para guardar en buffer todo el texto del fd
	{
		return (free(buffer), buffer = NULL, NULL);                                             // liberamos buffer si devuelve NULL o el buffer está vacío, lo asignamos a NULL y devolvemos NULL
	}

	line = extract_line(buffer);                                                                // extraemos la primera línea del buffer
	if (!line)                                                                                  // si no hay línea para extraer, liberamos el buffer y devolvemos NULL
	{
		return (free(buffer), buffer = NULL, NULL);                                             // liberamos buffer, lo asignamos a NULL y devolvemos NULL
	}

	buffer = update_buffer(buffer, &flag);                                                      // actualizamos el buffer para eliminar la línea extraída
	if (!buffer && flag == 1)                                                                   // si ocurre un error al actualizar el buffer (flag = 1)
	{
		return (free(line), NULL);                                                              // liberamos la línea y devolvemos NULL
	}

	return (line);                                                                              // devolvemos la línea extraída
}

// --------------- main ------------------

// #include <stdio.h>
// #include <fcntl.h>

// int main(int argc, char **argv)
// {
// 	int		fd;
// 	char	*line;
// 	if (argc != 2)
// 	{
// 		printf("Uso: %s <archivo>\n", argv[0]);
// 		return (1);
// 	}
// 	fd = open(argv[1], O_RDONLY);
// 	if (fd == -1)
// 	{
// 		printf("Error: No se pudo abrir el archivo %s\n", argv[1]);
// 		return (1);
// 	}
// 	while ((line = get_next_line(fd)) != NULL)
// 	{
// 		printf("%s", line);
// 		free(line);                                                                            
// 	}
// 	close(fd);                                                                                  
// 	return (0);
// }
