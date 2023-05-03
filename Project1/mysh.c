#include "execute.h"
#include "limits.h"
#include "parser.h"
#include <stdio.h>
#include <sys/wait.h>

int main() {
    char buffer[BUFFER_SIZE];
    char* commands[MAX_COMMANDS][MAX_ARGS];
    char* file_remap[2];
    while (1) {
        int num_commands = 0;
        file_remap[0] = NULL;
        file_remap[1] = NULL;
        // read user input
        printf("MyShPrompt> ");
        fgets(buffer, BUFFER_SIZE, stdin);
        if (feof(stdin)) {
            exit(EXIT_SUCCESS);
        }
        // parse commands
        parse_command(buffer, commands, file_remap, &num_commands);
        // execute commands
        int pipes[num_commands - 1][2];
        execute_piped_command(commands, pipes, file_remap, num_commands);
        // wait for child processes to finish
        for (int i = 0; i < num_commands; i++) {
            wait(NULL);
        }
    }
    return 0;
}