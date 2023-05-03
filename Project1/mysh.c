#include "execute.h"
#include "limits.h"
#include "parser.h"
#include <stdio.h>
#include <sys/wait.h>

#define instream (in_stream == NULL ? stdin : in_stream)
int main(int argc, char const *argv[]) {
    char buffer[BUFFER_SIZE];
    char* commands[MAX_COMMANDS][MAX_ARGS];
    char* file_remap[2];
    // read the stream
    FILE* in_stream = NULL;
    if (argc >= 1) {
        if ((in_stream = fopen(argv[1], "r")) == NULL) {
            fprintf(stderr, "Failed to open file %s.\n", argv[1]);
            return 1;
        }
    }
    while (1) {
        int num_commands = 0;
        file_remap[0] = NULL;
        file_remap[1] = NULL;
        // read user input
        if (in_stream == NULL)
            printf("MyShPrompt> ");
        fgets(buffer, BUFFER_SIZE, instream);
        if (feof(instream)) {
            exit(EXIT_SUCCESS);
        }
        // parse commands
        if (parse_command(buffer, commands, file_remap, &num_commands) == -1) {
            fprintf(stderr, "Invalid syntax!\n");
            continue;
        };
        // execute commands
        int pipes[num_commands - 1][2];
        if (execute_piped_command(commands, pipes, file_remap, num_commands) == -1) {
            fprintf(stderr, "Error executing.\n");
            continue;
        };
        // wait for child processes to finish
        for (int i = 0; i < num_commands; i++) {
            wait(NULL);
        }
    }
    fclose(instream);
    return 0;
}
#undef instream