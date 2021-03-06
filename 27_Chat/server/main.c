#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define MAX_STR 1024

int main()
{
    int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(listenSocket < 0)
    {
        write(2, "Error: socket.\n", 15);
        return 1;
    }

    int error;
    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(7500);
    local.sin_addr.s_addr = htonl(INADDR_ANY);

    error = bind(listenSocket, (struct sockaddr *)&local, sizeof(local));
    if(error)
    {
        write(2, "Error: bind.\n", 13);
        return 1;
    }

    error = listen(listenSocket, 5);
    if(error)
    {
        write(2, "Error: listen.\n", 15);
        return 1;
    }

    int aSocket = accept(listenSocket, NULL, NULL); //2 и 3 параметры - определенная структура и длина
    if(aSocket < 0)
    {
        write(2, "Error: accept.\n", 15);
        return 1;
    }

    char buf[MAX_STR] = {0};
    printf("From exit enter 'exit'\n");

    do
    {
        memset(buf, 0, strlen(buf));
        error = read(aSocket, buf, MAX_STR);
        if(error <= 0)
        {
            write(2, "Error: read.\n", 13);
            return 1;
        }
        if(!strcmp(buf, "exit\n"))
        {
            printf("Your partner exited!\n");
            break;
        }
        write(1, ">> ", 3);
        write(1, buf, strlen(buf));

        memset(buf, 0, strlen(buf));

        write(1, "<< ", 3);
        fgets(buf, MAX_STR, stdin);
        error = write(aSocket, buf, MAX_STR);
        if(error <= 0)
        {
            write(2, "Error: write.\n", 14);
            return 1;
        }
    }
    while(strcmp(buf, "exit\n"));
    return 0;
}
