#ifndef VICTORHUANG_SHELL_EXECUTE_H
#define VICTORHUANG_SHELL_EXECUTE_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "limits.h"

extern void execute_command(char* command[], int input_fd, int output_fd);
extern void execute_piped_command(char* commands[MAX_COMMANDS][MAX_ARGS], int pipes[][2], int num_commands);
#endif