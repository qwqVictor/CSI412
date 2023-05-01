#include "parser.h"

void parse_command(char* command, char* commands[MAX_COMMANDS][MAX_ARGS], int* num_commands) {
    int command_length = 0;
    char* token = strtok(command, " \n");
    while (token != NULL) {
        if (strcmp(token, "|") == 0) {
            commands[(*num_commands)][command_length] = NULL;
            (*num_commands)++;
            command_length = 0;
        } else {
            commands[(*num_commands)][command_length] = token;
            command_length++;
        }
        token = strtok(NULL, " \n");
    }
    commands[(*num_commands)][command_length] = NULL;
    (*num_commands)++;
}