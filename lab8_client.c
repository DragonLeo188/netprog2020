#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#define MAX 1000

void chat(int sockfd) 
{ 
    char buff[MAX]; //buffer
    char msg[MAX] = ""; //store received messages
    char delimiter[2] = "\n"; 
    int n = 0; 

    while(1)
    {   
        bzero(buff, sizeof(buff)); 

        //sending messages
        printf("Client : ");
        fgets(buff, sizeof(buff), stdin);
        send(sockfd, &buff[n], sizeof(buff), 0);
        
        //disconnect and exit
        if(strncmp("/quit", buff, 4) == 0) 
        { 
            printf("Client Exit...\n"); 
            break; 
        }
        
        bzero(buff, sizeof(buff)); 
        
        //receive message
        printf("Server : ");
        while(strcmp(&buff[n], delimiter) != 0)
        {
            recv(sockfd, &buff[n], sizeof(buff[n]), 0); 
            printf("%c", buff[n]);

            strcat(msg, &buff[n]);
        }

        //server disconnect
        if(strncmp("/dc", msg, 3) == 0) 
        { 
            printf("Server Disconnecting...\n"); 
            shutdown(sockfd, SHUT_RDWR);
            close(sockfd);

            break; 
        }

        if(strncmp("/exit", msg, 5) == 0) 
        { 
            printf("Server Disconnecting...\n"); 
            shutdown(sockfd, SHUT_RDWR);
            close(sockfd);

            break; 
        }
    }

    shutdown(sockfd, SHUT_RDWR);
    close(sockfd);
            
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
    
    //connect to the server
    if(connect(sockfd, (struct sockaddr *) &server_adr, sizeof(server_adr)) < 0)
    {
        printf("Can't connect.\n");
        exit(0);
    }
    else
    {
        printf("Connect succesfully. \n");
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
        fcntl(sockfd, F_SETFL, O_NONBLOCK);
    }

    //begin chatting 
    chat(sockfd);

    return 0;
}

// guests
// ICTLab4Guests
// linklist