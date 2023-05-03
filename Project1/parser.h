#ifndef VICTORHUANG_SHELL_PARSER_H
#define VICTORHUANG_SHELL_PARSER_H

#include <string.h>
#include "limits.h"
int parse_command(char* command, char* commands[MAX_COMMANDS][MAX_ARGS], char* file_remap[2], int* num_commands);

#endif