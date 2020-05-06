#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char hostname[100];

    if(argc < 2)
    {
        printf("Enter host name: ");
        scanf("%s", hostname);
    }
    else
    {
        strcpy(hostname,argv[1]);
    }

    struct hostent *host = gethostbyname(hostname);
    char *host_ip = inet_ntoa(*(struct in_addr*) host->h_addr_list[0]);


    printf("The ip of %s is %s. \n", hostname, host_ip);
    printf("The length of ip is %d. \n", host->h_length);
    printf("The ip type is %d. \n", host->h_addrtype);

    return 0;
}

/*
 argv argc
        c program argc argv

        struct hostent* ip = gethostbyname(argv[1]);
 */
