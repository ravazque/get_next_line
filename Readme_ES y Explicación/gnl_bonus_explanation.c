/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl_bonus_explanatios.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 13:19:48 by ravazque          #+#    #+#             */
/*   Updated: 2024/10/23 22:42:56 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"                                    // librería con funciones como strlen, strjoin, etc...

static char *extract_line(char *buffer)
{
    char    *line;                                             // línea nueva para guardar la que hemos de devolver del buffer al GNL
    size_t  i;                                                 // contador del buffer

    i = 0;
    if (!buffer[i])                                            // si no hay buffer devuelvo NULL
    {
        return (NULL);
    }
    while (buffer[i] && buffer[i] != '\n')                     // avanzamos para calcular el tamaño de la línea a devolver
    {
        i++;
    }
    if (buffer[i] == '\n')                                     // comprobación para reservar en función del contenido
    {
        line = (char *)malloc(i + 2);                          // si había salto de línea reservo para el mismo y el carácter del final
    }
    else
    {
        line = (char *)malloc(i + 1);                          // si no había salto de línea reservo solo para el carácter final
    }
    if (!line)                                                 // control de error en la reserva de memoria
    {
        return (NULL);
    }
    i = 0;
    while (buffer[i] && buffer[i] != '\n')                     // mientras no sea el final del texto o la línea copiamos la línea del buffer
    {
        line[i] = buffer[i];
        i++;
    }
    if (buffer[i] == '\n')                                     // si no había acabado el texto y hay un salto de línea lo ponemos en la línea y avanzamos
    {
        line[i] = '\n';
        i++;
    }
    line[i] = '\0';                                            // ponemos el carácter final a la línea guardada
    return (line);                                             // devolvemos la línea que tendrá que ser devuelta en la función GNL
}

static char *upd_buffer(char *buffer)
{
    char    *new_buffer;                                       // nuevo buffer para devolver y almacenar en la función GNL todo el texto sin la línea que será devuelta
    size_t  i;                                                 // contador del buffer
    size_t  j;                                                 // contador del new_buffer

    i = 0;
    while (buffer[i] && buffer[i] != '\n')                     // calculamos los caracteres hasta el primer salto de línea ('\n')
    {
        i++;
    }
    if (!buffer[i])                                            // si hay errores con el buffer liberamos el mismo y devolvemos null
    {
        free (buffer);
        return (NULL);
    }
    new_buffer = (char *)malloc(ft_strlen(buffer) - i);        // reservamos con malloc el tamaño del buffer menos la línea que va a ser devuelta y retirada
    if (!new_buffer)                                           // si falla la reserva de memoria devolvemos NULL
    {
        return (NULL);
    }
    i++;                                                       // avanzamos una posición con i [buffer]
    
    j = 0;                                                     // inicializamos j
    
    while (buffer[i])                                          // mientras tengamos texto en el buffer copiamos este en el nuevo buffer
    {
        new_buffer[j] = buffer[i++];
        j++;
    }
    new_buffer[j] = '\0';                                      // ponemos el carácter final a la cadena de caracteres nueva
    free (buffer);                                             // liberamos el buffer original
    return (new_buffer);                                       // devolvemos el nuevo buffer
}

char    *get_next_line(int fd)
{
    static char *buffer[FD_MAX];                               // buffer estático de file descriptors para tener el contenido sin cambios
    char        *tem_buffer;                                   // buffer temporal para guardar el contenido que leamos
    char        *line;                                         // línea que va a ser devuelta con el contenido

    ssize_t     bytes_read;                                    // cantidad de "texto" (bytes) que se leen, si es = 0 significa que ha acabado el programa
    
    if (fd < 0 || BUFFER_SIZE <= 0 || fd >= FD_MAX)            // control de errores si hay demasiados fd, si el fd no es válido o no hay lectura
    {
        return (NULL);
    }
    tem_buffer = (char *)malloc(BUFFER_SIZE + 1);              // reserva de memoria dinámica del tamaño del BUFFER de lectura
    if (!tem_buffer)                                           // control de errores en el malloc
    {
        return (NULL);
    }
    bytes_read = 1;                                            // inicializamos el valor de los bytes leídos
    
    while(!ft_strchr(buffer[fd], '\n') && bytes_read > 0)      // si no hay saltos de líneas y la cantidad de contenido leído es mayor que 0 entonces ->
    {
        bytes_read = read(fd, tem_buffer, BUFFER_SIZE);        // guardamos en bytes_read la cantidad de bytes leídos del fd, estos son tantos como marca
                                                               // el BUFFER_SIZE y se guarda en el buffer temporal (tem_buffer)
                                                                
        if (bytes_read < 0)                                    // si leemos menos de 0 bytes (error), liberamos toda la memoria reservada y devolvemos NULL
        {
            free (tem_buffer);
            buffer[fd] = NULL;
            return (NULL);
        }
        tem_buffer[bytes_read] = '\0';                         // el texto que se ha guardado en el "tem_buffer" durante read se marca con \0 para indicar el final
        
        buffer[fd] = ft_strjoin(buffer[fd], tem_buffer);       // concatenamos en mi buffer estático, con su correspondiente fd, el contenido del buffer temporal
        if (!buffer[fd])
        {
            free (tem_buffer);
            return (NULL);
        }
    }

    free (tem_buffer);                                         // liberamos el buffer temporal ya que ya tengo el contenido del fd guardado en "buffer[fd]"
    
    if (!buffer[fd] || *buffer[fd] == '\0')                    // si no ha leído contenido o hay un error libera el buffer del texto almacenado, igualalo a NULL y devuelve NULL
    {
        free (buffer[fd]);
        buffer[fd] = NULL;
        return (NULL);
    }
    
    line = extract_line(buffer[fd]);                           // mandamos el buffer con el contenido para que me extraiga los primeros caracteres hasta encontrar un '\n'
    
    buffer[fd] = upd_buffer(buffer[fd]);                       // mandamos el buffer a la función para quitar toda la línea que ya se ha guardado en line
    
    return(line);                                              // devolvemos la línea correspondiente
}


// -------------- main -----------------

// #include <fcntl.h>
// #include <stdio.h>
// #include <unistd.h>

// int	main(int	argc, char	*argv[])
// {
// 	int		fd;
// 	char	*line;
			
// 	if(argc != 2)
// 		return(write(1, "Error al pasar el FD\n", 22), 0);

// 	fd = open(argv[1], O_RDONLY);
	
// 	if (fd < 0)
// 		return(write(1, "FD no válido\n", 15), 1);

// 	while ((line = get_next_line(fd)) != NULL)
	// {
	// 	printf("%s", line);
	// 	free (line);
	// }

// 	return (close (fd), 0);
// }
