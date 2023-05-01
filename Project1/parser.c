#include "parser.h"

void parse_command(char* command, char* commands[MAX_COMMANDS][MAX_ARGS], int* num_commands) {
    int command_length = 0;
    // split token with spaces
    char* token = strtok(command, " \n");
    while (token != NULL) {
        // check if the token is the pipe symbol
        if (strcmp(token, "|") == 0) {
            // terminate the last command
            commands[(*num_commands)][command_length] = NULL;
            // increase the command counter
            (*num_commands)++;
            // reset command length
            command_length = 0;
        } else {
            // save the command token
            commands[(*num_commands)][command_length] = token;
            // increase the command length
            command_length++;
        }
        // fetch the next token
        token = strtok(NULL, " \n");
    }
    // terminate the last command
    commands[(*num_commands)][command_length] = NULL;
    (*num_commands)++;
}