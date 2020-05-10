#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>


int main(int argc, char *argv[])
{
    char hostname[100];
    int sockfd;
    unsigned port = 8784;
    struct sockaddr_in server_adr;

    if(argc < 2)
    {
        printf("Enter host name: ");
        scanf("%s", hostname);
    }
    else
    {
        strcpy(hostname,argv[1]);
    }
    
    //get host
    struct hostent *host = gethostbyname(hostname);
    if(host == NULL)
    {
        printf("Unknown host. \n");
    }

    //create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0)
    {
        printf("Error creating socket. \n");
        exit(0);
    }
    else
    {
        printf("Socket succesfully created. \n");
    }


    memset(&server_adr, 0, sizeof(server_adr));
    server_adr.sin_family = AF_INET;
    memcpy((char *) &server_adr.sin_addr.s_addr, host->h_addr_list[0], host->h_length);
    server_adr.sin_port = htons(port);
    
    if(connect(sockfd, (struct sockaddr *) &server_adr, sizeof(server_adr)) < 0)
    {
        printf("Can't connect.\n");
    }
    else
    {
        printf("Connect succesfully. \n");
    }

    return 0;
}
