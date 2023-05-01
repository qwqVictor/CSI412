#include "execute.h"

void execute_command(char* command[], int input_fd, int output_fd) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork() failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        if (input_fd != STDIN_FILENO) {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }
        if (output_fd != STDOUT_FILENO) {
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }
        execvp(command[0], command);
        perror("execvp() failed");
        exit(EXIT_FAILURE);
    }
}

void execute_piped_command(char* commands[MAX_COMMANDS][MAX_ARGS], int pipes[][2], int num_commands) {
    for (int i = 0; i < num_commands; i++) {
        if (i < num_commands - 1) {
            if (pipe(pipes[i]) == -1) {
                perror("pipe() failed");
                exit(EXIT_FAILURE);
            }
        }
        int input_fd = (i > 0) ? pipes[i - 1][0] : STDIN_FILENO;
        int output_fd = (i < num_commands - 1) ? pipes[i][1] : STDOUT_FILENO;
        execute_command(commands[i], input_fd, output_fd);
        if (i < num_commands - 1) {
            close(pipes[i][1]);
        }
    }
}