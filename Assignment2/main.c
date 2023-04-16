#include <unistd.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
    pid_t childpid;
    switch (childpid = fork()) {
    case 0:
        pid_t parentpid = getppid();
        pid_t thispid = getpid();
        printf("Child: My pid = %d. My parent has pid = %d\nChild: The current user is:\n", 
                thispid, parentpid);
        return execv("/bin/whoami", (char* const[]){"whoami", NULL});
        break;
    case -1:
        perror("fork failed");
        return 1;
        break;
    default:
        pid_t mypid = getpid();
        printf("Parent: My pid = %d. My child has pid = %d\n", mypid, childpid);
        break;
    }
    return 0;
}
