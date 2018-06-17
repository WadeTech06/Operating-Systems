/* server.c */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
 
#define LEN 150

char* GetTime()
{
    char buf[LEN];
    time_t curtime;
    struct tm *loc_time;

    //Getting current time of system
    curtime = time(NULL);

    // Converting current time to local time
    loc_time = localtime(&curtime);

    // Displaying date and time in standard format
    return asctime(loc_time);
}

int main(int argc, char *argv[])
{
    socklen_t server_id, client_len;
    char buffer_size[1024];
    struct sockaddr_in server_addr, client_addr;
    int p, s, i = 0;

    if (argc < 2)
    {
        fprintf(stderr, "Enter the port number \n");
        exit(1);
    }
    server_id = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_id < 0)
    {
        perror("socket_create");
    }
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_id, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("socket_bind");
    }
    while (i < 10)
    {
        client_len = sizeof(client_addr);

        bzero(buffer_size, 1024);
        p = recvfrom(server_id, buffer_size, 1023, 0, (struct sockaddr *)&client_addr, &client_len);

        if (p < 0)
        {
            perror("receive");
        }
        printf("client message is %s", buffer_size);

        s = sendto(server_id, GetTime(), 1023, 0, (struct sockaddr *)&client_addr, sizeof(client_addr));

        if (p < 0)
        {
            perror("Error in sending the message");
        }

        i++;
    }
    close(server_id);
    return 0;
}