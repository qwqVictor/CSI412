#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
    int size, tmp = 0, oddsum = 0, evensum = 0;
    char buf[200];
    int f = open("numbers.txt", O_RDONLY);
    if (f == -1) {
        perror("Error opening");
    }
    printf("File numbers.txt fd (consumer) is: %d\n", f);
    while (~scanf("%d", &size)) {
        if (size == 0) break;
        read(f, buf, size);
        sscanf(buf, "%d", &tmp);
        if (tmp & 1)
            oddsum += tmp;
        else
            evensum += tmp;
    }
    printf("Oddsum: %d\n", oddsum);
    printf("Evensum: %d\n", evensum);

    return 0;
}
