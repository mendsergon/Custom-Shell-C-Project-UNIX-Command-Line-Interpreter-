
**Custom Shell (C Project – UNIX Command Line Interpreter)**

**Project Summary**

This project is a console-based custom shell implemented in C, designed to interpret and execute user commands in a UNIX-like environment. It reads input from the user, parses commands, handles piping and redirection, and executes them using child processes. The shell supports multiple commands in a single line, input/output redirection (`<`, `>`), output appending (`>>`), and simple pipelines (`|`). Dynamic memory allocation is used to store command arguments and file names safely.

**Core Features**

* Interactive shell prompt (`my_shell$`) for user input.
* Parses command lines into multiple commands and arguments.
* Supports piping between commands (e.g., `ls | grep txt`).
* Handles input (`<`) and output (`>`, `>>`) redirection.
* Executes commands using child processes (`fork()`) and replaces them with the requested programs (`execvp()`).
* Waits for all child processes to finish before returning control to the user.
* Frees dynamically allocated memory to prevent leaks.
* Exit the shell using the `exit` command or `Ctrl+D`.

**Key Methods and Algorithms**

* **Command Parsing:** Uses `strtok()` to tokenize input based on spaces and special characters, building a `Command` structure for each command. Handles piping, input, and output redirection.
* **Process Management:** Implements child process creation using `fork()`, command execution with `execvp()`, and synchronization with `wait()`.
* **Pipes and Redirection:** Creates pipes dynamically between commands and redirects input/output using `dup2()` to enable pipelines and file I/O redirection.
* **Dynamic Memory Handling:** Allocates memory for command arguments and file names using `strdup()` and ensures proper cleanup with `free()`.
* **Error Handling:** Detects syntax errors (e.g., missing filenames for redirection) and system call failures (`fork`, `pipe`, `open`).

**Skills Demonstrated**

* Advanced C programming with dynamic memory management.
* Process creation and control in UNIX-like systems.
* Implementation of inter-process communication using pipes.
* Command-line parsing and tokenization techniques.
* Input/output redirection handling and file descriptor manipulation.
* Modular design using multiple source (`.c`) and header (`.h`) files.
* Debugging and error handling in systems programming.

**File Overview**

| File Name | Description                                                                    |
| --------- | ------------------------------------------------------------------------------ |
| `main.c`  | Implements the shell loop, reads user input, and calls parser and executor.    |
| `shell.c` | Contains command parsing and execution logic, including pipes and redirection. |
| `shell.h` | Defines the `Command` structure and declares function prototypes.              |

**How to Compile and Run**

Compile the program using GCC:

```bash
gcc main.c shell.c -o my_shell
```

Execute the shell:

```bash
./my_shell
```

Type commands at the prompt (`my_shell$`) like in a UNIX terminal. Use `exit` or `Ctrl+D` to terminate the shell.

---

