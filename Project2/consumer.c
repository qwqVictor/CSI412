#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#define MAX_PATH_LINE (0xFF)

int main() {
    char filename[MAX_PATH_LINE];
    char location[MAX_PATH_LINE];
    char character[2];

    printf("Enter file name: ");
    scanf("%s", filename);
    printf("Enter file location: ");
    scanf("%s", location);
    printf("Enter character: ");
    scanf(" %c", &character[0]);
    character[1] = 0;

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        fprintf(stderr, "Error creating pipe.\n");
        return EXIT_FAILURE;
    }

    // Fork and execute command
    pid_t pid = fork();
    if (pid == 0) {
        // Child process will write to pipe and exit
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        execlp("./producer", "producer", filename, location, character, NULL);
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Fork error
        fprintf(stderr, "Error creating child process.\n");
        return EXIT_FAILURE;
    }
    char result_filename[MAX_PATH_LINE];
    // Parent process will read from pipe
    close(pipefd[1]);
    FILE* child_stdout = fdopen(pipefd[0], "r");
    wait(NULL);
    fgets(result_filename, MAX_PATH_LINE, child_stdout);
    printf("Acknowledged result file: %s\n", result_filename);

    // Remove newline character from result
    result_filename[strcspn(result_filename, "\n")] = 0;

    // Read contents of result file and print to standard output
    FILE* result_file = fopen(result_filename, "r");
    if (!result_file) {
        fprintf(stderr, "Error: Could not open result file\n");
        return 1;
    }

    char line[BUFSIZ];
    while (fgets(line, sizeof(line), result_file)) {
        fputs(line, stdout);
    }

    fclose(child_stdout);
    fclose(result_file);
    remove(result_filename);

    return 0;
}