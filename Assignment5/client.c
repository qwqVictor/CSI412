/*
 * Simple client to work with server.c program.
 * Host name and port used by server are to be
 * passed as arguments.
 *
 * To test: Open a terminal window.
 * At prompt ($ is my prompt symbol) you may
 * type the following as a test:
 *
 * $./client 127.0.0.1 54554
 * Please enter the message: Programming with sockets is fun!
 * I got your message
 * $
 *
 */
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_BUF_SIZE 1024

enum { TO_UPPER,
       COUNT };

void error(const char* msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char* argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent* server;
    int type = -1;
    char buffer[MAX_BUF_SIZE];

    if (argc < 2) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }

    // create connection
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*)server->h_addr,
          (char*)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    while (!feof(stdin)) {
        printf("> ");
        // get command line
        bzero(buffer, MAX_BUF_SIZE);
        fgets(buffer, MAX_BUF_SIZE, stdin);
        if (buffer[0] == 't')
            type = TO_UPPER;
        else if (buffer[0] == 'c') {
            type = COUNT;
        }
        // send command
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0)
            error("ERROR writing to socket");

        // receive response
        bzero(buffer, MAX_BUF_SIZE);

        if (type == COUNT) {
            n = read(sockfd, buffer, MAX_BUF_SIZE - 1);
            if (n < 0)
                error("ERROR reading from socket");
            printf("Count: %s\n", buffer);
        } else if (type == TO_UPPER) {
            // receive to buffer
            n = read(sockfd, buffer, MAX_BUF_SIZE - 1);
            if (n < 0)
                error("ERROR reading from socket");
            else if (n != 0 && n < 8) {
                printf("ERROR parsing: n=%d,buffer=\"%s\"\n", n, buffer);
                return 0;
            }

            // first: parse the file length
            buffer[7] = 0;
            int totalReceived = 0;
            sscanf(buffer, "%d", &totalReceived);
            if (totalReceived == 0)
                return 0;
            if (n == totalReceived + 8) {
                totalReceived += totalReceived;
                buffer[n] = '\0';
                printf("%s", buffer + 8);
            } else {
                // handle sticky packet
                totalReceived += n - 8;
                printf("%s", buffer + 8);
                totalReceived -= n - 8;
                while (totalReceived > 0) {
                    n = read(sockfd, buffer, MAX_BUF_SIZE - 1);
                    buffer[n] = '\0';
                    totalReceived += n;
                    printf("%s", buffer);
                    totalReceived -= n;
                }
            }
            printf("\n");
        } else {
            error("ERROR unknown op");
            return 0;
        }
    }

    close(sockfd);

    return 0;
}