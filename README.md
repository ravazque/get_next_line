# Get Next Line

<div align="center">
  <img src="https://github.com/ayogun/42-project-badges/raw/main/badges/get_next_linem.png" alt="Get Next Line Badge" width="150" height="150">
</div>

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Implementation Details](#implementation-details)
- [Buffer Management](#buffer-management)
- [Testing](#testing)
- [Bonus Features](#bonus-features)

## 🎯 Overview

**Get Next Line** is a 42 School project that implements a function to read lines from file descriptors one at a time. This function is essential for handling large files efficiently without loading the entire content into memory.

The project teaches fundamental concepts:
- **Static variables** for maintaining state between function calls
- **Dynamic memory allocation** and buffer management
- **File descriptor operations** and system calls
- **String manipulation** and memory optimization
- **Edge case handling** for various input scenarios

## ✨ Features

- **Line-by-line reading**: Reads one line at a time from any file descriptor
- **Memory efficient**: Uses minimal memory regardless of file size
- **Multiple file descriptors**: Supports reading from multiple files simultaneously (bonus)
- **Flexible buffer size**: Configurable buffer size via compilation flag
- **Robust handling**: Manages various edge cases and error conditions
- **No memory leaks**: Proper memory management and cleanup

## 🛠️ Installation

```bash
# Clone the repository
git clone https://github.com/ravazque/get_next_line.git

# Navigate to project directory
cd get_next_line

# Compile with your project (example)
gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c

# Or use with custom buffer size
gcc -Wall -Wextra -Werror -D BUFFER_SIZE=1024 get_next_line.c get_next_line_utils.c main.c
```

## 🚀 Usage

### Function Prototype
```c
char *get_next_line(int fd);
```

### Basic Usage Example

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int fd;
    char *line;

    // Open file for reading
    fd = open("example.txt", O_RDONLY);
    if (fd == -1)
        return (1);

    // Read file line by line
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);  // Don't forget to free!
    }

    close(fd);
    return (0);
}
```

### Reading from Standard Input
```c
#include "get_next_line.h"
#include <stdio.h>

int main(void)
{
    char *line;

    printf("Enter lines (Ctrl+D to exit):\n");
    while ((line = get_next_line(0)) != NULL)  // 0 = stdin
    {
        printf("You entered: %s", line);
        free(line);
    }
    return (0);
}
```

## 🔧 Implementation Details

<details>
<summary><strong>Core Algorithm</strong></summary>

### Reading Strategy
1. **Buffer management**: Maintains a static buffer for each file descriptor
2. **Line detection**: Searches for newline characters (`\n`) in buffer
3. **Memory allocation**: Dynamically allocates memory for each line
4. **State preservation**: Uses static variables to maintain reading position
5. **EOF handling**: Properly handles end-of-file conditions

### Key Functions
- `get_next_line()`: Main function that returns next line
- `read_buffer()`: Reads data from file descriptor into buffer
- `extract_line()`: Extracts line from buffer up to newline
- `update_buffer()`: Updates buffer after line extraction
- `ft_strchr()`: Finds newline character in string
- `ft_strjoin()`: Joins strings for line construction

</details>

<details>
<summary><strong>Static Variable Usage</strong></summary>

### State Management
```c
static char *buffer[MAX_FD];  // Buffer for each file descriptor
```

### Benefits
- **Persistent state**: Maintains buffer content between function calls
- **Multiple FD support**: Each file descriptor has its own buffer
- **Memory efficiency**: Reuses buffer space for subsequent reads
- **Context preservation**: Remembers partial lines across calls

### Considerations
- **Thread safety**: Static variables are not thread-safe
- **Memory cleanup**: Requires careful cleanup on errors
- **Scope limitation**: Variables persist for program lifetime

</details>

<details>
<summary><strong>Memory Management</strong></summary>

### Allocation Strategy
- **Dynamic allocation**: Each line is dynamically allocated
- **Buffer reuse**: Internal buffer is reused for efficiency
- **Cleanup protocol**: Caller must free returned lines
- **Error handling**: Proper cleanup on allocation failures

### Memory Safety
- **Leak prevention**: All allocated memory is tracked
- **Buffer bounds**: Careful bounds checking in all operations
- **Null termination**: All strings are properly null-terminated
- **Edge case handling**: Safe handling of empty lines and EOF

</details>

## 🗂️ Buffer Management

### Buffer Size Configuration
```bash
# Compile with different buffer sizes
gcc -D BUFFER_SIZE=1    # Minimum buffer (stress test)
gcc -D BUFFER_SIZE=42   # Default buffer size
gcc -D BUFFER_SIZE=1024 # Large buffer (performance)
gcc -D BUFFER_SIZE=8192 # Very large buffer
```

### Buffer Size Impact

| Buffer Size | Memory Usage | Performance | Use Case |
|-------------|--------------|-------------|----------|
| 1 | Minimal | Slow | Testing edge cases |
| 42 | Low | Moderate | General purpose |
| 1024 | Medium | Good | Normal files |
| 8192+ | High | Best | Large files |

## 🧪 Testing

### Test Cases

**Basic functionality:**
```c
// Test normal file reading
fd = open("normal_file.txt", O_RDONLY);
while ((line = get_next_line(fd)) != NULL)
{
    printf("%s", line);
    free(line);
}
```

**Edge cases:**
```c
// Test empty file
fd = open("empty_file.txt", O_RDONLY);
line = get_next_line(fd);  // Should return NULL

// Test file with no newline at end
fd = open("no_final_newline.txt", O_RDONLY);
// Should still return the last line

// Test very long lines
fd = open("long_lines.txt", O_RDONLY);
// Should handle lines longer than buffer size
```

**Error conditions:**
```c
// Test invalid file descriptor
line = get_next_line(-1);    // Should return NULL
line = get_next_line(1000);  // Should return NULL

// Test closed file descriptor
fd = open("file.txt", O_RDONLY);
close(fd);
line = get_next_line(fd);    // Should return NULL
```

### Recommended Test Files

Create these test files for comprehensive testing:
```bash
# Empty file
touch empty.txt

# Single line without newline
echo -n "Single line without newline" > single_no_nl.txt

# Single line with newline
echo "Single line with newline" > single_with_nl.txt

# Multiple lines
printf "Line 1\nLine 2\nLine 3\n" > multiple_lines.txt

# Very long line
python -c "print('A' * 10000)" > long_line.txt

# Mixed line lengths
printf "Short\nThis is a medium length line\n%s\nShort again\n" "$(python -c "print('B' * 1000)")" > mixed_lines.txt
```

## 🎁 Bonus Features

<details>
<summary><strong>Multiple File Descriptors</strong></summary>

### Functionality
- **Simultaneous reading**: Read from multiple files at the same time
- **Independent state**: Each file descriptor maintains its own buffer
- **Context switching**: Switch between files without losing position

### Example Usage
```c
int fd1 = open("file1.txt", O_RDONLY);
int fd2 = open("file2.txt", O_RDONLY);
int fd3 = open("file3.txt", O_RDONLY);

char *line1 = get_next_line(fd1);  // Read from file1
char *line2 = get_next_line(fd2);  // Read from file2
char *line1_again = get_next_line(fd1);  // Continue file1
char *line3 = get_next_line(fd3);  // Read from file3

// Each file maintains independent reading position
```

### Implementation
- **Buffer array**: Static buffer array indexed by file descriptor
- **State isolation**: Each FD has completely independent state
- **Resource management**: Proper cleanup for each file descriptor

</details>

## 📚 Learning Outcomes

This project teaches essential programming concepts:

- **Static variables** and their behavior in C
- **File I/O operations** and system calls
- **Dynamic memory management** and allocation strategies
- **String manipulation** and buffer handling
- **Edge case consideration** and robust programming
- **Memory leak prevention** and resource management
- **Function design** for reusability and efficiency

## 🏗️ Project Structure

```
get_next_line/
├── get_next_line.c        # Main function implementation
├── get_next_line_utils.c  # Utility functions
├── get_next_line.h        # Header file
├── get_next_line_bonus.c  # Bonus implementation
├── get_next_line_bonus.h  # Bonus header
└── README.md             # This file
```

## ⚠️ Important Notes

- **Memory responsibility**: Caller must free each returned line
- **NULL return**: Function returns NULL when EOF is reached or on error
- **Buffer size**: Must be defined during compilation
- **File descriptor validity**: Always check FD validity before use
- **Thread safety**: Function is not thread-safe due to static variables

---

*This project is part of the 42 School curriculum, focusing on file operations and memory management in C.*
