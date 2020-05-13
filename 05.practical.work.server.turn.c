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
        bzero(buff, MAX); 
  
        recv(sockfd, buff, sizeof(buff), 0); 
        printf("Client : %sServer : ", buff); 

        bzero(buff, MAX); 
        n = 0; 
        
        while ((buff[n++] = getchar()) != '\n') ; 
        send(sockfd, buff, sizeof(buff), 0); 
  
        if (strncmp("exit", buff, 4) == 0) { 
            printf("Server Exit...\n"); 
            break; 
        } 
    } 
}

int main()
{
    int client_len, clientfd, sockfd;
    struct sockaddr_in server_adr, client_adr;
    unsigned short port = 8784;
    char *buffer = malloc(sizeof(buffer));
    
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
    server_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_adr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *) &server_adr, sizeof(server_adr)) < 0)
    {
        printf("Error binding. \n");
        exit(0);
    }
    else
    {
        printf("Successfully binding/ \n");
    }
    
    if(listen(sockfd, 5) < 0)
    {
        printf("Error listening. \n");
        exit(0);
    }
    else
    {
        printf("Listening. \n");
    }

    client_len = sizeof(client_adr);
    clientfd = accept(sockfd, (struct sockaddr *) &client_adr, (socklen_t *) &client_len);
    if(clientfd < 0)
    {
        printf("Can't accept connection. \n");
        exit(0);
    } 
    else
    {
        printf("Accept connection. \n");
    }

    // Function for chatting between client and server 
    func(clientfd); 
  
    // After chatting close the socket 
    close(clientfd); 

    return 0;
}
