#ifndef VICTORHUANG_SHELL_EXECUTE_H
#define VICTORHUANG_SHELL_EXECUTE_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include "limits.h"

/**
 * Fork and execute commands
 * @param command the command vector to be executed
 * @param input_fd the file descriptor to replace stdin
 * @param output_fd the file descriptor to replace stdout
 * @return if error it should be -1, otherwise 0.
 */
int execute_command(char* command[], int input_fd, int output_fd);

/**
 * Make the pipeline and execute
 * @param command the commands to be pipelined
 * @param pipes the file descriptor array storage
 * @param file_remap stdin and stdout redirection map
 * @param num_commands the command num count
 * @return if error it should be -1, otherwise 0.
 */
int execute_piped_command(char* commands[MAX_COMMANDS][MAX_ARGS], int pipes[][2], char* file_remap[2], int num_commands);

#endif