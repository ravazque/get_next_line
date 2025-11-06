
# get next line

## 📖 About

Get Next Line is a project at 42 Madrid that involves creating a function capable of reading a text file line by line, regardless of the buffer size. This project introduces the concept of static variables in C and teaches efficient file descriptor handling.

The function `get_next_line` reads from a file descriptor and returns one line at a time, making it possible to read very large files without loading them entirely into memory.

## 🎯 Objectives

- Understanding and implementing static variables in C
- File descriptor manipulation and system calls
- Memory management with dynamic allocation
- Buffer management and optimization
- Error handling and edge case management

## 📋 Function Overview

<details>
<summary><strong>get_next_line</strong></summary>

### Concepts

- **Description:** Reads a line from a file descriptor.  
- **Parameters:** File descriptor to read from.  
- **Return:** The line read (including `\n` if present), or NULL if end of file or error.  
- **Files:** `get_next_line.c`, `get_next_line_utils.c`, `get_next_line.h`  
- **Buffer size:** Is uses a defined buffer size (`BUFFER_SIZE`) to read chunks of data from the file descriptor.  
- **Satatic variable:** Static variables maintain their value between function calls.

<br>

```c
char *get_next_line(int fd);
```

```c
static char *buffer;
```

</details>

<details>
<summary><strong>get_next_line_bonus</strong></summary>

### Bonus concepts

The bonus part extends the function to handle multiple file descriptors simultaneously, maintaining a separate reading state for each file descriptor.

**Key Features:**
- Support for multiple file descriptors
- Independent reading state per file descriptor
- Memory efficiency with static variable arrays

</details>

## 🚀 Installation & Structure

<details>
<summary><strong>📥 Download & Compilation</strong></summary>
    
<br>

```bash
# Clone the repository
git clone https://github.com/ravazque/get_next_line.git
cd get_next_line

# Compile with your program
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c your_program.c

# Compile bonus version
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c your_program.c
```

<br>

</details>

<details>
<summary><strong>📁 Project Structure</strong></summary>

<br>

```
get_next_line/
├──┬ doc/
│  ├── gnl_explanation              # Explanation of the “get next line” project
│  └── gnl_bonus_explanation        # Explanation of the “get next line” project with bonus
├──┬ include/
│  ├── get_next_line.h              # Header file with prototypes and definitions
│  └── get_next_line_bonus.h        # Bonus header file
├──┬ src/
│  ├── get_next_line.c              # Main function implementation
│  ├── get_next_line_utils.c        # Utility functions
│  ├── get_next_line_bonus.c        # Bonus implementation (multiple FDs)
│  └── get_next_line_utils_bonus.c  # Bonus utility functions
└── README.md                       # Project documentation
```

<br>

</details>

## 💡 Key Learning Outcomes

The Get Next Line project teaches fundamental concepts essential for systems programming:

- **Static Variables**: Understanding how static variables maintain state between function calls
- **File I/O Operations**: Learning to work with file descriptors and system calls like `read()`
- **Buffer Management**: Implementing efficient reading strategies with configurable buffer sizes
- **Memory Management**: Proper allocation and deallocation to prevent memory leaks
- **Edge Case Handling**: Managing end-of-file conditions, empty lines, and error states
- **Code Modularity**: Separating main logic from utility functions for better organization

## ⚙️ Technical Specifications

- **Language**: C (C90 standard)
- **Compiler**: cc with flags `-Wall -Wextra -Werror`
- **Buffer Size**: Configurable via `BUFFER_SIZE` macro during compilation
- **Memory Management**: Dynamic allocation with proper cleanup
- **Dependencies**: Only standard C library functions (`read`, `malloc`, `free`)

---

> [!NOTE]
> This project serves as the foundation for all subsequent 42 projects and demonstrates proficiency in low-level C programming concepts.
