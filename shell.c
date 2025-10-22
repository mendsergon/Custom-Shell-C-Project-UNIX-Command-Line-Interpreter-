#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

#define DELIMS " \t\r\n" // Delimiters for tokenizing the command string

// Parses the command line into Command structures
void parse_command(char *cmd, Command commands[], int *num_commands) {
    char *token;
    int cmd_index = 0;
    int argc = 0;
    char *temp = strdup(cmd); // Copy of input to preserve original string
    if (!temp) { perror("strdup"); exit(EXIT_FAILURE); }

    token = strtok(temp, DELIMS);
    while (token != NULL) {
        if (strcmp(token, "|") == 0) { // Pipe separator
            commands[cmd_index].argv[argc] = NULL; // End current command
            cmd_index++;
            argc = 0;
        } else if (strcmp(token, "<") == 0) { // Input redirection
            token = strtok(NULL, DELIMS);
            if (!token) { fprintf(stderr, "Syntax error: no input file\n"); free(temp); exit(EXIT_FAILURE); }
            commands[cmd_index].input_file = strdup(token);
        } else if (strcmp(token, ">") == 0 || strcmp(token, ">>") == 0) { // Output redirection
            int append = (strcmp(token, ">>") == 0);
            token = strtok(NULL, DELIMS);
            if (!token) { fprintf(stderr, "Syntax error: no output file\n"); free(temp); exit(EXIT_FAILURE); }
            commands[cmd_index].output_file = strdup(token);
            commands[cmd_index].append = append;
        } else { // Regular command argument
            if (argc >= MAX_ARGS - 1) { fprintf(stderr, "Error: too many arguments\n"); free(temp); exit(EXIT_FAILURE); }
            commands[cmd_index].argv[argc++] = strdup(token);
        }
        token = strtok(NULL, DELIMS);
    }

    // Null-terminate last command and set command count
    if (argc > 0 || commands[cmd_index].input_file != NULL || commands[cmd_index].output_file != NULL) {
        commands[cmd_index].argv[argc] = NULL;
        *num_commands = cmd_index + 1;
    } else {
        *num_commands = cmd_index;
    }

    free(temp);

    // Debug print
    for (int i = 0; i < *num_commands; i++) {
        printf("Command %d: ", i);
        for (int j = 0; commands[i].argv[j] != NULL; j++) printf("'%s' ", commands[i].argv[j]);
        if (commands[i].input_file) printf("<%s ", commands[i].input_file);
        if (commands[i].output_file) printf("%s%s ", commands[i].append ? ">>" : ">", commands[i].output_file);
        printf("\n");
    }
}

// Executes an array of parsed Command structures
void execute_commands(Command commands[], int num_commands) {
    int pipefds[2 * (num_commands - 1)];
    pid_t pid;
    int fd_in = 0;

    for (int i = 0; i < num_commands; i++) {
        if (i < num_commands - 1) {
            if (pipe(pipefds + i * 2) < 0) { perror("pipe"); exit(EXIT_FAILURE); }
        }

        pid = fork();
        if (pid == 0) { // Child
            // Input from previous pipe
            if (i != 0) {
                dup2(fd_in, STDIN_FILENO);
                close(fd_in);
            }

            // Output to next pipe
            if (i < num_commands - 1) {
                dup2(pipefds[i * 2 + 1], STDOUT_FILENO);
            }

            // Close all pipe fds in child
            for (int j = 0; j < 2 * (num_commands - 1); j++) close(pipefds[j]);

            // Input redirection
            if (commands[i].input_file != NULL) {
                int fd = open(commands[i].input_file, O_RDONLY);
                if (fd == -1) { perror("open input"); exit(EXIT_FAILURE); }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }

            // Output redirection
            if (commands[i].output_file != NULL) {
                int flags = O_WRONLY | O_CREAT | (commands[i].append ? O_APPEND : O_TRUNC);
                int fd = open(commands[i].output_file, flags, 0644);
                if (fd == -1) { perror("open output"); exit(EXIT_FAILURE); }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }

            // Execute command
            if (commands[i].argv[0] != NULL) {
                execvp(commands[i].argv[0], commands[i].argv);
                perror("execvp");
            }
            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        // Parent updates fd_in for next command
        if (i != 0) close(fd_in);
        if (i < num_commands - 1) {
            close(pipefds[i * 2 + 1]);
            fd_in = pipefds[i * 2];
        }
    }

    // Close remaining pipes in parent
    for (int i = 0; i < 2 * (num_commands - 1); i++) close(pipefds[i]);

    // Wait for all children
    int status;
    while (wait(&status) > 0);
}
