#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#define MAX 1000

void chat(int sockfd) 
{ 
    char buff[MAX];
    char delimiter[2] = "\n"; 
    int n = 0; 
    int len = 0;

    while(1) 
    { 
        bzero(buff, MAX); 
        
        //receive messages
        printf("Client : ");
        while(strcmp(&buff[n], delimiter) != 0)
        {
            recv(sockfd, &buff[n], sizeof(buff[n]), 0);
            printf("%s", buff);
        }

        if(strncmp("exit", buff, 4) == 0) 
        { 
            printf("Server Exit...\n"); 
            break; 
        }

        bzero(buff, MAX); 

        //send messages
        printf("Server : ");
        fgets(buff, sizeof(buff), stdin);
        send(sockfd, &buff[n], sizeof(buff), 0);

        if(strncmp("exit", buff, 4) == 0) 
        { 
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
    server_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_adr.sin_port = htons(port);

    //binding
    if (bind(sockfd, (struct sockaddr *) &server_adr, sizeof(server_adr)) < 0)
    {
        printf("Error binding. \n");
        exit(0);
    }
    else
    {
        printf("Successfully binding/ \n");
    }
    
    //listening to the client
    if(listen(sockfd, 5) < 0)
    {
        printf("Error listening. \n");
        exit(0);
    }
    else
    {
        printf("Listening. \n");
    }

    //accept connection
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

    //begin chatting
    chat(clientfd);   
    
    close(clientfd); 

    return 0;
}