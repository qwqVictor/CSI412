#include "parser.h"

int parse_command(char* command, char* commands[MAX_COMMANDS][MAX_ARGS], char* file_remap[2], int* num_commands) {
    int command_length = 0;
    // split token with spaces
    for (char* token = strtok(command, " \n"); token != NULL; token = strtok(NULL, " \n")) {

        // check if the token is the pipe symbol
        if (strcmp(token, "|") == 0) {
            // terminate the last command
            commands[(*num_commands)][command_length] = NULL;
            // increase the command counter
            (*num_commands)++;
            // reset command length
            command_length = 0;
        } else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " \n");
            if (token != NULL) {
                file_remap[1] = token;
            }
            else return 1;
        } else if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " \n");
            if (token != NULL) {
                if (file_remap[0] == NULL) {
                    file_remap[0] = token;
                }
            } else return 1;
        } else {
            int len = strlen(token);
            char* cmd = token;
            if (token[0] == '\'') {
                if (token[len - 1] != '\'') {
                    for (char next = 0; token != NULL; token = strtok(NULL, " \n")) {
                        if (next != 0)
                            *(token - 1) = ' ';
                        next = 1;
                        int len = strlen(token);
                        if (token[len - 1] == '\'') {
                            token[len - 1] = 0;
                            break;
                        }
                    }
                }
                else token[len - 1] = 0;
                cmd++;
                
            }
            // save the command token
            commands[(*num_commands)][command_length] = cmd;
            // increase the command length
            command_length++;
        }
    }
    // terminate the last command
    commands[(*num_commands)][command_length] = NULL;
    (*num_commands)++;
    return 0;
}