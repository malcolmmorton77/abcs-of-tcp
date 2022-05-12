/*Name:    Malcolm M
 *Section: 3530.002
 *Date:    2/1/2022
 *Descr:   creates a count server for changing a sentence, counting
	   characters in a sentence
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdbool.h>

#define MAXSIZE 1024

bool spaceLoc(char j){
    return 32 == j;
}

bool alphaLoc(char j){
    return (64<j && j<91) || (96<j && j <123);
}

char uppercase(char j){
    if(64<j && j<91){ return j; }
    else return j-32;
}

char lowercase(char j){
    if(j >= 'A' && j <= 'Z'){ return j+32; }
    else return j;
}

int main(int argc, char *argv[]) {
    
    int listenfd = 0, connfd = 0, clientSize, portno;
    char message[200];

    //used to convert integers to strings for concatenation
    struct sockaddr_in servaddr, cliaddr;

    // error checking for usage
    if(argv[1] == NULL || argc > 2){
	printf("Usage: ./[name] <port number>\n");
	exit(0);
    }

    // creating socket file descriptor with tcp connection
    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
	perror("socket creation failed");
	exit(EXIT_FAILURE);
    }

    //allocate space for servaddr and the two buffers
    bzero(&servaddr, sizeof(servaddr));

    //assign IP
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);

    //grab port given in the command line args and assign to the server
    portno = atoi(argv[1]);
    servaddr.sin_port = htons(portno);

    //reuse the port number
    int on = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    //assign servaddr to listenfd socket
    if( bind( listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
        printf("bind error\n");
        exit(EXIT_FAILURE);
    }

    //listen with backlog of 10
    if( listen( listenfd, 10 ) == -1){
        printf("listen error\n");
        exit(EXIT_FAILURE);
    }

    //accept the connection, generating an error if it doesn't work
	if ((connfd = accept( listenfd, (struct sockaddr*)NULL, NULL )) < 0){
		printf("accept error\n");
		exit(EXIT_FAILURE);
	}
    //connfd = accept( listenfd, (struct sockaddr*) NULL, NULL);

    while(1) {
	
        // used to iterate through message and count words, alphabets, and punctuation
        int i = 0, j = 1, countWords = 1, countAlpha = 0, countPunc = 0;
        
        bzero(message, sizeof(message));
        //receiving from client and placing message in buffer
        read(connfd, message, sizeof(message));

        //check if the user wants to quit
        if(strcmp(message, "quit\n") == 0){ exit(EXIT_SUCCESS); }

        //toupper the first letter of the sentence
        message[0] = uppercase(message[0]);

        while(message[j] != '\0'){
            message[j] = lowercase(message[j]);
            j++;
        }

        //count spaces, alphas, and punctuation marks
        while(message[i] != '\0'){
            //count spaces
            if(spaceLoc(message[i]))
                countWords++;
            //count alphabets
            if(alphaLoc(message[i]))
                countAlpha++;
            //count punctuation marks with a long if else branch
            if(message[i] == '.'){
                if(message[i-1] == '.' && message[i+1] == '.'){ //if branch for ellipsis
                    i = i+2;
                    continue;
                }
                else
                    countPunc++;
            }
            else if(message[i] == ',')
                countPunc++;
            else if(message[i] == ';')
                countPunc++;
            else if(message[i] == ':')
                countPunc++;
            else if(message[i] == '[')
                countPunc++;
            else if(message[i] == ']')
                countPunc++;
            else if(message[i] == '{')
                countPunc++;
            else if(message[i] == '}')
                countPunc++;
            else if(message[i] == '(')
                countPunc++;
            else if(message[i] == ')')
                countPunc++;
            else if(message[i] == '!')
                countPunc++;
            else if(message[i] == '?')
                countPunc++;
            else if(message[i] == '\"')
                countPunc++;
            else if(message[i] == '-')
                countPunc++;
            else if(message[i] == '\'')
                countPunc++;
            i++; //increment i to iterate through message
        }

        //three variables to hold plain text
        char temp[300];
        char tWord[25];
        char tAlph[25];
        char tPunc[50];
        const char *strw = "\nWords: ";
        const char *stra = "\nAlphas: ";
        const char *strp = "\nPunctuation Marks: ";
        char *w, *a, *p;
        
        //convert integers to char * and concatenate to buffer
        if(asprintf(&w, "%d", countWords) == -1)
            perror("asprintf");
        if(asprintf(&a, "%d", countAlpha) == -1)
            perror("asprintf");
        if(asprintf(&p, "%d\n", countPunc) == -1)
            perror("asprintf");

        //concatenate into three separate character pointers
        strcat(strcpy(tWord, strw), w); free(w);
        strcat(strcpy(tAlph, stra), a); free(a);
        strcat(strcpy(tPunc, strp), p); free(p);

        //concatenate tWord, tAlph, and tPunc
        strcat(tWord, tAlph);
        strcat(tWord, tPunc);

        //change buffers to concatenate correctly, again...
        strcat(strcpy(temp, message), tWord);
        write(connfd, temp, strlen(temp));
        bzero(message, sizeof(message));
    }

    return 0;
}