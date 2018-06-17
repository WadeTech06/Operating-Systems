/*client.c program*/
/*Header file declaration*/
#include <netdb.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

/*Main function*/
int main(int argc, char *argv[])
{
    socklen_t  server_len;
    int server_id, p, i = 0;
    char buffer_size[1024];
    struct sockaddr_in server_address;
    struct hostent *hostname;
    if (argc < 3) //Verifying argument count
    {
        fprintf(stderr, "Please enter Host name and port \n");
        exit(1);
    }
    /* create a socket. The three arguments are:*/
    /*1)Internet Domain 2. Stream Socket 3. Default Protocol*/
    server_id = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_id < 0)
    {
        perror("Create socket");
    }
    hostname = gethostbyname(argv[1]); // Setting server address in Local host for establishing connection
    if (hostname == NULL)
    {
        fprintf(stdout, "Host is not available");
        exit(1);
    }
    /*Configure settings of the server address struct*/
    server_address.sin_family = AF_INET;            // Address type: INTERNET
    server_address.sin_port = htons(atoi(argv[2])); //Port number assign
    bcopy((char *)hostname->h_addr, (char *)&server_address.sin_addr.s_addr, hostname->h_length);

    if (bind(server_id, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("Socket bind error ");
    }
    printf("Enter the message u want to send : \n");

    while (i < 10)
    {
        fgets(buffer_size, 1023, stdin); // Storing a client message to server in buffer
        p = sendto(server_id, buffer_size, 1023, 0, (struct sockaddr *)&server_address, sizeof(server_address));

        if (p < 0)
        {
            perror("Error in sending the message");
        }

        server_len = sizeof(server_address);

        bzero(buffer_size, 1024);
        p = recvfrom(server_id, buffer_size, 1023, 0, (struct sockaddr *)&server_address, &server_len);

        if (p < 0)
        {
            perror("receive");
        }
        printf("server time is %s", buffer_size);

        i++;
    }
    close(server_id);
    return (0);
}