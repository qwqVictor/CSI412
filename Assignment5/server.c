/* Program: server.c
 * A simple TCP server using sockets.
 * Server is executed before Client.
 * Port number is to be passed as an argument.
 *
 * To test: Open a terminal window.
 * At the prompt ($ is my prompt symbol) you may
 * type the following as a test:
 *
 * $ ./server 54554
 * Run client by providing host and port
 *
 *
 */
#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_BUF_SIZE 1024

// 功能类型，固定为 toUpper 或 count
enum { TO_UPPER,
       COUNT };

// 错误处理函数
void error(const char* msg) {
    perror(msg);
    exit(1);
}

void makeTXTName(char* filename, char const *suffix, char* destination, int size) {
    bzero(destination, size);
    int len = strlen(filename), dot = len - 4;
    if (strcmp(filename + dot, ".txt") == 0) {
        strcpy(destination, filename);
        destination[dot] = 0;
    }
    else return;
    strcat(destination, suffix);
}

// 计算文件中某个字符的数量
int countCharInFile(char* fileName, char c) {
    int count = 0;
    FILE* fp = fopen(fileName, "r");

    if (fp == NULL)
        error("ERROR open file");

    char ch = fgetc(fp);
    while (ch != EOF) {
        if (ch == c)
            count++; // 如果是要计数的字符，计数器加一
        ch = fgetc(fp);
    }

    fclose(fp);
    return count;
}

int main(int argc, char* argv[]) {
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[MAX_BUF_SIZE];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    fprintf(stdout, "Run client by providing host and port\n");

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    bzero((char*)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    listen(sockfd, 5);

    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);

    if (newsockfd < 0)
        error("ERROR on accept");

    while (1) {
        // 接收并处理客户端发送的请求
        bzero(buffer, MAX_BUF_SIZE);
        n = read(newsockfd, buffer, MAX_BUF_SIZE - 1);

        if (n < 0)
            error("ERROR reading from socket");
        char* token = strtok(buffer, "<,> "); // split string with strtok
        int type = -1;                        // service type

        char* tokens[MAX_BUF_SIZE];
        char c = 0;
        char* fileName = NULL;
        bzero(tokens, sizeof(tokens));
        // 解析客户端请求中的参数
        int i = 0;
        do {
            tokens[i] = token;
        } while ((token = strtok(NULL, "<,> ")) != NULL && (++i < MAX_BUF_SIZE));
        if (strcmp(tokens[0], "count") == 0)
            type = COUNT;
        else if (strcmp(tokens[0], "toUpper") == 0)
            type = TO_UPPER;
        if (tokens[1])
            fileName = tokens[1];
        else
            error("ERROR no filename");
        char resultFileName[MAX_BUF_SIZE];
        // switch case for different types
        switch (type) {
        case TO_UPPER:
            FILE* inFP = fopen(fileName, "r");

            if (inFP == NULL)
                error("ERROR open file");

            makeTXTName(fileName, "Upper.txt", resultFileName, MAX_BUF_SIZE);
            fprintf(stderr, "resultFileName: %s\n", resultFileName);
            FILE* outFP = fopen(resultFileName, "w");
            if (outFP == NULL)
                outFP = stdout;
            char sendContent[MAX_BUF_SIZE];
            bzero(sendContent, MAX_BUF_SIZE);
            int i = 0;
            // read from the file
            // former 8 bytes is length description
            for (char ch = fgetc(inFP); ch != EOF; ch = fgetc(inFP)) {
                if (i >= MAX_BUF_SIZE - 8) {
                    snprintf(sendContent, 7, "%d", i);
                    fputs(sendContent, outFP);
                    n = write(newsockfd, sendContent, i + 8);
                    if (n < 0)
                        error("ERROR writing to socket");
                    bzero(sendContent, sizeof(sendContent));
                }
                sendContent[8 + i++] = toupper(ch);
            }
            int k = snprintf(sendContent, 7, "%d", i);
            for (int j = k; j < 8; j++)
                sendContent[j] = '\n';
            fputs(sendContent, outFP);
            fclose(outFP);
            n = write(newsockfd, sendContent, i + 8);
            if (n < 0)
                error("ERROR writing to socket");
            bzero(sendContent, sizeof(sendContent));
            fclose(inFP);
            break;

        case COUNT:
            if (tokens[2] != NULL)
                c = tokens[2][0];
            else
                error("ERROR no character");
            int count = countCharInFile(fileName, c);
            
            makeTXTName(fileName, "Char.txt", resultFileName, MAX_BUF_SIZE);
            FILE* fp = fopen(resultFileName, "w");
            if (fp == NULL)
                fp = stdout;
            char countStr[MAX_BUF_SIZE];
            // int to string
            snprintf(countStr, MAX_BUF_SIZE, "%d", count);
            fputs(countStr, fp);
            fclose(fp);
            n = write(newsockfd, countStr, strlen(countStr));
            if (n < 0)
                error("ERROR writing to socket");
            break;
        default:
            break;
        }
    }

    close(newsockfd);
    close(sockfd);

    return 0;
}