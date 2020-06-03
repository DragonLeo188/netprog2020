#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#define MAX 1000

int chat(int sockfd) 
{ 
    char buff[MAX]; //buffer
    char msg[MAX] = ""; //store received messages
    char delimiter[2] = "\n"; 
    int n = 0, check = 0; 

    while(1) 
    { 
        bzero(buff, MAX); 
        
        //receive message
        printf("Client : ");
        while(strcmp(&buff[n], delimiter) != 0)
        {
            recv(sockfd, &buff[n], sizeof(buff[n]), 0); 
            printf("%c", buff[n]);

            strcat(msg, &buff[n]);
        }

        //client request to disconnect and exit
        if(strncmp("/quit", msg, 4) == 0) 
        { 
            printf("Client Exit...\n");
            printf("//if received ""/quit"" and still not disconnecting from the current client\n//type ""/dc"" to disconnect from current client\n");
            close(sockfd);
            
            break; 
        }
        //if received "/quit" and still not disconnecting from the current client
        //type "/dc" to disconnect from current client 


        bzero(buff, MAX); 

        //send messages
        printf("Server : ");
        fgets(buff, sizeof(buff), stdin);
        send(sockfd, &buff[n], sizeof(buff), 0);

        //disconnect current client
        if(strncmp("/dc", buff, 3) == 0) 
        { 
            printf("Disconnecting Client...\n"); 
            break;
        }

        //server exit and quit
        if(strncmp("/exit", buff, 5) == 0) 
        { 
            check = 1;

            printf("Server Disconnecting...\n");
            shutdown(sockfd, SHUT_RDWR);
            close(sockfd); 

            break; 
        }
    }
    
    close(sockfd);
    return check;
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
    //reusing address
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));

    //enable nonblocking for server
    int fl = fcntl(sockfd, F_GETFL, 0);
    fl |= O_NONBLOCK;
    fcntl(sockfd, F_SETFL, fl);


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
        printf("Successfully binding. \n");
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
    
    while(1)
    {
        //accept connection
        client_len = sizeof(client_adr);
        clientfd = accept(sockfd, (struct sockaddr *) &client_adr, (socklen_t *) &client_len);
        while(clientfd < 0)
        {
            clientfd = accept(sockfd, (struct sockaddr *) &client_adr, (socklen_t *) &client_len); 

            if(clientfd > 0)
            {
                printf("Accept connection. \n");
                int fl = fcntl(clientfd, F_GETFL,0); //enable nonblocking for client
                fl |=  O_NONBLOCK;
                fcntl(clientfd, F_SETFL, fl);
            }
        }
        
        //begin chatting
        if(chat(clientfd) == 1)
        {
            break;
        }
        else
        {
            chat(clientfd);
        }    
    }
       
    return 0;
}
