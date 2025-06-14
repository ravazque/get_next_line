
<div align="center">
  <img src="https://github.com/ayogun/42-project-badges/raw/main/badges/get_next_linem.png" alt="Get Next Line Badge" width="150" height="150">
</div>

*Reading a line from a file descriptor is way too tedious*

## 📖 About

**get_next_line** is a 42 Madrid project that implements a function to read lines from a file descriptor one at a time. This project introduces the concept of static variables in C and teaches efficient file reading techniques. The function is designed to handle multiple file descriptors simultaneously and manage memory efficiently.

This project is particularly valuable for understanding file I/O operations, buffer management, and the importance of static variables in maintaining state between function calls.

## 🎯 Objectives

- Understanding file descriptors and file I/O operations
- Learning to use static variables effectively
- Implementing efficient buffer management
- Handling multiple file descriptors concurrently
- Memory management and leak prevention
- Understanding the read() system call

## 🛠️ Function Prototype

```c
char *get_next_line(int fd);
```

### Parameters
- `fd`: File descriptor to read from

### Return Value
- **Success**: Returns the line read from the file descriptor (including `\n` if present)
- **End of File**: Returns `NULL` when EOF is reached
- **Error**: Returns `NULL` if an error occurs

## 🚀 Installation & Usage

### Compilation

```bash
# Clone the repository
git clone https://github.com/ravazque/get_next_line.git
cd get_next_line

# Compile with your project (example)
gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 main.c get_next_line.c get_next_line_utils.c -o gnl_test
```

### Usage Example

<details>
<summary><strong>Basic Usage</strong></summary>

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int fd;
    char *line;
    int line_count = 0;

    // Open file
    fd = open("test.txt", O_RDONLY);
    if (fd == -1)
    {
        printf("Error opening file\n");
        return (1);
    }

    // Read lines one by one
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("Line %d: %s", ++line_count, line);
        free(line);
    }

    close(fd);
    printf("Total lines read: %d\n", line_count);
    return (0);
}
```

</details>

<details>
<summary><strong>Multiple File Descriptors</strong></summary>

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int fd1, fd2;
    char *line1, *line2;

    fd1 = open("file1.txt", O_RDONLY);
    fd2 = open("file2.txt", O_RDONLY);

    // Alternate reading from two files
    while (1)
    {
        line1 = get_next_line(fd1);
        line2 = get_next_line(fd2);
        
        if (!line1 && !line2)
            break;
            
        if (line1)
        {
            printf("File1: %s", line1);
            free(line1);
        }
        if (line2)
        {
            printf("File2: %s", line2);
            free(line2);
        }
    }

    close(fd1);
    close(fd2);
    return (0);
}
```

</details>

<details>
<summary><strong>Reading from stdin</strong></summary>

```c
#include "get_next_line.h"
#include <stdio.h>

int main(void)
{
    char *line;

    printf("Enter lines (Ctrl+D to end):\n");
    while ((line = get_next_line(0)) != NULL)  // 0 is stdin
    {
        printf("You entered: %s", line);
        free(line);
    }
    
    return (0);
}
```

</details>

## 📁 Project Structure

```
get_next_line/
├── get_next_line.h         # Header file with function prototype
├── get_next_line.c         # Main function implementation
├── get_next_line_utils.c   # Utility functions
├── main.c                  # Test file (optional)
└── README.md              # Project documentation
```

## ⚙️ Configuration

The function behavior can be customized using the `BUFFER_SIZE` macro:

```bash
# Compile with custom buffer size
gcc -D BUFFER_SIZE=1024 get_next_line.c get_next_line_utils.c main.c
```

- **Small buffer (1-10)**: More read() calls, good for testing edge cases
- **Medium buffer (42-1024)**: Balanced performance
- **Large buffer (4096+)**: Fewer system calls, more memory usage

## 🔍 Implementation Details

<details>
<summary><strong>Core Algorithm</strong></summary>

1. **Static Variable**: Maintains leftover data between function calls
2. **Buffer Reading**: Reads data in chunks using `read()` system call
3. **Line Extraction**: Searches for newline character (`\n`)
4. **Memory Management**: Properly allocates and frees memory
5. **State Preservation**: Saves remaining data for next function call

</details>

<details>
<summary><strong>Key Features</strong></summary>

- **Multi-FD Support**: Handles multiple file descriptors simultaneously
- **Memory Efficient**: Only allocates necessary memory for each line
- **Robust Error Handling**: Handles read errors and invalid file descriptors
- **Flexible Buffer Size**: Configurable at compile time
- **No Memory Leaks**: Proper cleanup of all allocated memory

</details>

<details>
<summary><strong>Utility Functions</strong></summary>

```c
// Common utility functions implemented
char *ft_strchr(const char *s, int c);
size_t ft_strlen(const char *s);
char *ft_strjoin(char const *s1, char const *s2);
char *ft_substr(char const *s, unsigned int start, size_t len);
```

</details>

## 🧪 Testing Scenarios

The function is tested with various scenarios:

- **Normal files**: Text files with regular line endings
- **Empty files**: Files with no content
- **Single line files**: Files without newline at end
- **Large files**: Files exceeding buffer size
- **Binary files**: Files with null bytes
- **Multiple FDs**: Reading from several files simultaneously
- **Edge cases**: Very small and very large buffer sizes

## 💡 Key Learning Points

- **Static Variables**: Understanding how static variables maintain state
- **File I/O**: Working with file descriptors and the read() system call
- **Buffer Management**: Efficiently handling data buffers
- **Memory Management**: Preventing memory leaks in complex scenarios
- **String Manipulation**: Working with dynamic strings and concatenation
- **Error Handling**: Robust error detection and recovery

## ⚡ Performance Considerations

- **Buffer Size Impact**: Larger buffers reduce system calls but use more memory
- **Memory Usage**: Each file descriptor maintains its own static buffer
- **Read Efficiency**: Minimizes the number of read() system calls
- **String Operations**: Optimized string joining and manipulation

## 🔧 Technical Specifications

- **Language**: C (C99 standard)
- **System Calls**: `read()`, `malloc()`, `free()`
- **Compiler Flags**: `-Wall -Wextra -Werror`
- **Memory Management**: Dynamic allocation with proper cleanup
- **File Descriptors**: Supports any valid file descriptor (files, stdin, pipes)
- **Buffer Size**: Configurable via `BUFFER_SIZE` macro (default: 42)

This project demonstrates advanced file handling techniques and serves as a foundation for understanding I/O operations in systems programming.
- **Thread safety**: Function is not thread-safe due to static variables

---

*This project is part of the 42 School curriculum, focusing on file operations and memory management in C.*
