#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/select.h>
#define MAX 1000
#define MAX_CLIENT 100

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

    char s[MAX] = "";
    //create array of clientfd
    int clientfds[100];
    memset(clientfds, 0, sizeof(clientfds));

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
        fd_set set;              //initialize a set
        FD_ZERO(&set);           //clear the set
        FD_SET(sockfd, &set);    //add listening socket to the set
        int maxfd = sockfd;     

        for (int i = 0; i < 100; i++) 
        {
            if (clientfds[i] > 0) //add connected client sockets to set
            {
                FD_SET(clientfds[i], &set); 
            }
            if (clientfds[i] > maxfd) 
            {
                maxfd = clientfds[i];
            }
        }
        // each time we accept, we add client socket to clientfd[] array but that's later

        // poll and wait, blocked indefinitely
        select(maxfd+1, &set, NULL, NULL, NULL);

        //accept connection
        client_len = sizeof(client_adr);
        clientfd = accept(sockfd, (struct sockaddr *) &client_adr, (socklen_t *) &client_len);
        
        if(FD_ISSET(sockfd, &set)) //if "listening soket"
        {
            while(clientfd < 0)
            {
                clientfd = accept(sockfd, (struct sockaddr *) &client_adr, (socklen_t *) &client_len); 

                if(clientfd > 0)
                {
                    printf("Accept connection. \n");

                    //enable nonblocking for client
                    setsockopt(clientfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
                    int fl = fcntl(clientfd, F_GETFL,0); 
                    fl |=  O_NONBLOCK;
                    fcntl(clientfd, F_SETFL, fl);
                }
                else
                {
                    printf("Failed. %d\n", clientfd);
                }
            }

            // add it to the clientfds array
            for (int i = 0; i < MAX_CLIENT; i++) 
            {
                if (clientfds[i] == 0) 
                {
                    clientfds[i] = clientfd;
                    break;
                }
            }   
        }

        // is that data from a previously-connect client?
        for(int i = 0; i < MAX_CLIENT; i++) 
        {
            if (clientfds[i] > 0 && FD_ISSET(clientfds[i], &set)) 
            {
                if (recv(clientfds[i], s, sizeof(s), 0) > 0) 
                {
                    printf("client %d says: %s\nserver>\n", clientfds[i], s);
                }
                else 
                {
                    // some error. remove it from the "active" fd array
                    printf("client %d has disconnected.\n", clientfds[i]);
                    clientfds[i] = 0;
                }
            }
        }

        //begin chatting
        // if(chat(clientfd) == 1)
        // {
        //     break;
        // }
        // else
        // {
        //     chat(clientfd);
        // }    
    }
       
    return 0;
}
