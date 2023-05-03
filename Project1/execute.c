#include "execute.h"

void execute_command(char* command[], int input_fd, int output_fd) {
    pid_t pid = fork();
    // check if fork is failed
    if (pid == -1) {
        perror("fork() failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // redirect stdin and stdout to fd
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

void execute_piped_command(char* commands[MAX_COMMANDS][MAX_ARGS], int pipes[][2], char* file_remap[2], int num_commands) {
    for (int i = 0; i < num_commands; i++) {
        if (i < num_commands - 1) {
            // make pipes
            if (pipe(pipes[i]) == -1) {
                perror("pipe() failed");
                exit(EXIT_FAILURE);
            }
        }
        int file_infd = -1, file_outfd = -1;
        if (i == 0 && file_remap[0] != NULL) {
            if ((file_infd = open(file_remap[0], O_RDONLY)) == -1) {
                perror("open < failed");
                exit(EXIT_FAILURE);
            }
        }
        if (i == num_commands - 1 && file_remap[1] != NULL) {
            if ((file_outfd = open(file_remap[1], O_WRONLY | O_CREAT, 0644)) == -1) {
                perror("open > failed");
                exit(EXIT_FAILURE);
            }
        }
        // specify input and output fd
        int input_fd = (i > 0) ? pipes[i - 1][0] : (file_infd == -1 ? STDIN_FILENO : file_infd);
        int output_fd = (i < num_commands - 1) ? pipes[i][1] : (file_outfd == -1 ? STDOUT_FILENO : file_outfd);
        // execute command with specified fd
        execute_command(commands[i], input_fd, output_fd);
        // close the previous fd
        if (file_infd != -1)
            close(file_infd);
        if (file_outfd != -1)
            close(file_outfd);
        if (i < num_commands - 1) {
            close(pipes[i][1]);
        }
    }
}