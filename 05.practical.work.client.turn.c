#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#define MAX 1000

void func(int sockfd) 
{ 
    char buff[MAX]; 
    int n; 
    for (;;) { 
        bzero(buff, sizeof(buff)); 
        printf("Client : "); 
        n = 0; 
        
        while ((buff[n++] = getchar()) != '\n') ; 
        send(sockfd, buff, sizeof(buff), 0); 
        
        bzero(buff, sizeof(buff)); 
        recv(sockfd, buff, sizeof(buff), 0); 
        printf("Server : %s", buff); 
        
        if ((strncmp(buff, "exit", 4)) == 0) 
        { 
            printf("Client Exit...\n"); 
            break; 
        } 
    } 
} 

int main(int argc, char *argv[])
{
    char hostname[100];
    int sockfd;
    unsigned port = 8784;
    struct sockaddr_in server_adr;
    char *buffer = malloc(sizeof(buffer));

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
        exit(0);
    }
    else
    {
        printf("Connect succesfully. \n");
    }

    // function for chat 
    func(sockfd); 
  
    // close the socket 
    close(sockfd); 


    return 0;
}
