#ifndef SHELL_H
#define SHELL_H

#define MAX_ARGS 64 // Define maximum number of arguments for a command
#define MAX_COMMANDS 10 // Define maximum number of commands in a command line

// Structure to represent a single command with its associated details
typedef struct {
    char *argv[MAX_ARGS]; // Array to hold the command and its arguments
    char *input_file;     // Pointer to a string representing the input redirection file, if specified
    char *output_file;    // Pointer to a string representing the output redirection file, if specified
    int append;           // Flag to indicate append mode (1) or overwrite mode (0) for output redirection
} Command;

// Function prototype for parsing a command line into an array of Command structures
void parse_command(char *cmd, Command commands[], int *num_commands);

// Function prototype for executing an array of parsed Command structures
void execute_commands(Command commands[], int num_commands);

#endif // SHELL_H
