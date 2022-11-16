/* Name:   Malcolm M.
 *Date:    2/1/2022
 *Class:   CSCE 3530.002
 *Descr:   client side of tcp connection
 *Replace "0.0.0.0" with the ip address of the server
*/

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define MAXSIZE 40960

int main(int argc, char *argv[]) {

    int sockfd = 0, n = 0, portno;
    char message[MAXSIZE];
    struct sockaddr_in servaddr;

    //allocate space for message
    memset(message, '0', sizeof(message));

    //create socket for use and error check
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
	    printf("socket error");
	    exit(EXIT_FAILURE);
    }

    //assign IP, PORT
    servaddr.sin_family = AF_INET;
    portno = atoi(argv[1]);
    servaddr.sin_port = htons(portno);
    inet_pton(AF_INET,"0.0.0.0",&(servaddr.sin_addr));

    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))<0){
        printf("connect error\n");
        exit(EXIT_FAILURE);
    }

    while(1){
        
        bzero(message, sizeof(message));
        printf("\nGiven: ");
        fgets(message, 40959, stdin); //get the message
        write(sockfd, message, strlen(message)); //write the message to server

        //check if the user wants to quit
        if(strcmp(message, "quit\n") == 0 ) { exit(EXIT_SUCCESS); }
        bzero(message, sizeof(message));

        //read the return message from server
        printf("Return:");
        read(sockfd, message, sizeof(message));
        
        //print the message to the terminal
        if(fputs(message, stdout) == EOF){
            printf("fputs error\n");
        }
    }

    if(n < 0){
	printf("read error\n");
    }
    return 0;
}
