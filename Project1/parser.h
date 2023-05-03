#ifndef VICTORHUANG_SHELL_PARSER_H
#define VICTORHUANG_SHELL_PARSER_H

#include <string.h>
#include "limits.h"
/**
 * Parsing the command line arguments
 * @param command the command string to be parsed
 * @param commands the parsing result
 * @param file_remap stdin and stdout redirection map result
 * @param num_commands the command num counter result
 * @return if error it should be -1, otherwise 0.
*/
int parse_command(char* command, char* commands[MAX_COMMANDS][MAX_ARGS], char* file_remap[2], int* num_commands);

#endif