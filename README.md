# get_next_line

The **get_next_line** project involves creating a function that reads and returns a line from a file descriptor. The main goal is to efficiently handle file reading operations and manage variable buffer sizes in C.

‎ 
 

## Functionality

The **get_next_line** function reads from the given file descriptor (fd) and returns the next line found. If there are no more lines to read or an error occurs, it returns **NULL**.

## Line format

- The returned line includes the newline character (`\n`), except in the last fragment if the file doesn’t end with a newline.
- The function must handle reading from both files and **stdin**.

## Static variables

**get_next_line** uses a static variable to store the leftover content between successive calls. For example, if a buffer read does not complete a full line, the remaining data is saved in this static variable to be processed in the next call.

```c
static char *buffer;
```

This ensures that reading continues from where it left off in the previous call, without losing any data.


‎ 

 
## Bonus part
The bonus part of the project adds the following requirements:

- **Multiple file descriptor handling**: The function must handle multiple file descriptors simultaneously, remembering the reading state of each one.
- **Single static variable**: The logic must be implemented using only one static variable.



‎ 

 

----

> [!WARNING]
> Be cautious with very large or very small **BUFFER_SIZE** values, as they may impact the function's performance or cause unexpected behavior. Test different values to optimize its usage depending on the file’s requirements.
