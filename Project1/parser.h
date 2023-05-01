#ifndef VICTORHUANG_SHELL_PARSER_H
#define VICTORHUANG_SHELL_PARSER_H

#include <string.h>
#include "limits.h"
extern void parse_command(char* command, char* commands[MAX_COMMANDS][MAX_ARGS], int* num_commands);

#endif