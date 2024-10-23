# get_next_line

El proyecto **get_next_line** consiste en la creación de una función que lee y devuelve una línea desde un file descriptor. El objetivo principal es manejar operaciones de lectura de archivos de manera eficiente y gestionar buffers de tamaño variable en C.

‎ 
 

## Funcionalidad

La función **get_next_line** lee del file descriptor (fd) pasado como parámetro y devuelve la siguiente línea encontrada. Si no hay más líneas por leer o ocurre un error, devuelve **NULL**.

## Formato de la línea

- La línea devuelta incluye el carácter de nueva línea (`\n`), excepto en el último fragmento si el archivo no termina con una nueva línea.
- La función debe manejar tanto la lectura de archivos como de la entrada estándar (**stdin**).

## Variables estáticas

**get_next_line** utiliza una variable estática para conservar el contenido sobrante entre llamadas sucesivas. Por ejemplo, si la lectura de un buffer no alcanza a completar una línea, lo que sobra se guarda en esta variable estática para procesarse en la siguiente llamada.

```c
static char *buffer;
```

Esto asegura que la lectura continúa desde donde se dejó en la llamada anterior, sin pérdida de información.


‎ 

 

## Parte bonus
La parte bonus del proyecto añade los siguientes requisitos:

- **Manejo de múltiples file descriptors**: La función debe ser capaz de gestionar varios file descriptors simultáneamente, recordando el estado de lectura de cada uno.
- **Uso de una sola variable estática**: Toda la lógica debe implementarse usando una única variable estática.


‎ 

 

----

> [!WARNING]
> Ten cuidado con los tamaños de **BUFFER_SIZE** muy grandes o muy pequeños, ya que pueden afectar el rendimiento de la función o provocar un comportamiento inesperado. Prueba diferentes valores para optimizar su uso según las necesidades del archivo.
