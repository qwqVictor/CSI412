#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
    int status;
    int pfd[2];
    pipe(pfd);
    pid_t childpid;
    switch (childpid = fork()) {
    case 0:
        pid_t parentpid = getppid();
        pid_t thispid = getpid();
        dup2(pfd[0], STDIN_FILENO);
        close(pfd[0]);
        printf("Child: My pid = %d. My parent has pid = %d\n", thispid, parentpid);
        return execv("./consumer", (char* const[]){"consumer", "numbers.txt", NULL});
        break;
    case -1:
        perror("fork failed");
        return 1;
        break;
    default:
        pid_t mypid = getpid();
        printf("Parent: My pid = %d. I created child pid = %d\n", mypid, childpid);
        srand(time(NULL));
        int f = open("numbers.txt", O_CREAT | O_WRONLY, 0644);
        if (f == -1) {
            perror("Error creating file");
            return 1;
        }
        printf("File numbers.txt fd (producer) is: %d\n", f);
        char buf[200];
        dup2(pfd[1], STDOUT_FILENO);
        close(pfd[1]);
        for (int i = 0; i < 20; i++) {
            int number = rand() % 100;
            size_t size = sprintf(buf, "%d\n", number);
            size_t ret = write(f, buf, size);
            size = sprintf(buf, "%ld\n", size);
            write(STDOUT_FILENO, buf, size);
        }
        write(STDOUT_FILENO, "0\n", sizeof("0\n"));
        close(STDOUT_FILENO);
        break;
    }
    wait(&status);
    return WEXITSTATUS(status);
}
