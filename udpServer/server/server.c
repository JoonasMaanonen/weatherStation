#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <syslog.h>
#include <time.h>
#include "errorHandler.h"
#include "currTime.h"

/*
 * TODO: Make this into daemon so it can run on the background
 */

#define MAXBUFLEN 100

int main(int argc, char *argv[]){

    if(argc != 3){
        fprintf(stderr, "usage: %s hostname servicePort\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // During debugging
    //setbuf(stdout, NULL);

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // Ipv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // My own IP

    struct addrinfo *servinfo;

    int s = getaddrinfo(0, argv[2], &hints, &servinfo);
    if(s != 0){
        errExit("getaddrinfo()");
    }

    int sockfd;
    struct addrinfo *p;

    // Loop through the address list until we find an adress that we can bind into.
    for(p = servinfo; p != 0; p = p->ai_next){
        if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0){
            errMsg("socket()");
            continue;
        }
        if(bind(sockfd, p->ai_addr, p->ai_addrlen) < 0){
            close(sockfd);
            errMsg("bind()");
            continue;
        }
        break;
    }

    if(p == 0){
        errExit("Failed to bind socket");
    }

    freeaddrinfo(servinfo);

    printf("Waiting for packets...\n");
    struct sockaddr_storage clientAddr;
    char buf[MAXBUFLEN];
    for(;;){
        socklen_t len = sizeof(struct sockaddr_storage);
        ssize_t numRead = recvfrom(sockfd, buf, MAXBUFLEN, 0,
                                   (struct sockaddr *) &clientAddr, &len);
        if(numRead == -1){
            errExit("recvfrom");
        }
        else{
            printf("Number of bytes read: %d\n", numRead);
            printf("%s\n", buf);
            FILE *fptr;
            fptr = fopen("measurements.csv", "a");
            if(fptr == NULL){
                errExit("fopen");
            }
            fprintf(fptr, "%s\n" , buf);
            fclose(fptr);

        }
    }
}

