#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 1024      // Maximum length of input line
#define MAX_COMMANDS 10   // Maximum number of commands that can be parsed

int main() {
    char line[MAX_LEN];               // Buffer for user input
    Command commands[MAX_COMMANDS];   // Array to hold parsed commands
    int num_commands = 0;             // Number of parsed commands

    // Main shell loop
    while (1) {
        // Print shell prompt
        printf("my_shell$ ");
        fflush(stdout);  // Ensure prompt is displayed immediately

        // Read input line; break on EOF (Ctrl+D)
        if (!fgets(line, MAX_LEN, stdin)) break;

        // Remove trailing newline character
        line[strcspn(line, "\n")] = 0;

        // Skip empty lines
        if (strlen(line) == 0) continue;

        // Exit shell if user types 'exit'
        if (strcmp(line, "exit") == 0) break;

        // Clear commands array for new input
        memset(commands, 0, sizeof(commands));

        // Parse the input line into commands
        parse_command(line, commands, &num_commands);

        // Execute the parsed commands
        execute_commands(commands, num_commands);

        // Free dynamically allocated memory for command arguments and file names
        for (int i = 0; i < num_commands; ++i) {
            for (int j = 0; commands[i].argv[j] != NULL; ++j) {
                free(commands[i].argv[j]);
                commands[i].argv[j] = NULL;
            }

            if (commands[i].input_file != NULL) {
                free(commands[i].input_file);
                commands[i].input_file = NULL;
            }
            if (commands[i].output_file != NULL) {
                free(commands[i].output_file);
                commands[i].output_file = NULL;
            }
        }
    }

    return 0; // Exit successfully
}
