//
// Created by genius on 18-7-27.
//

#include "opensocket.h"
#include "csapp.h"
int open_clientfd(char* hostname, char* port) {
    int clientfd;
    struct addrinfo hints, *listp, *p;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;
    hints.ai_flags |= AI_ADDRCONFIG;
    getaddrinfo(hostname, port, &hints, &listp);

    for (p = listp; p; p = listp->ai_next) {
        if ((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol) < 0))
            continue;

        if (connect(clientfd, p->ai_addr, p->ai_addrlen) != -1) {
            break;
        }
        close(clientfd);
    }

    freeaddrinfo(listp);
    if (!p) {
        return -1;
    } else
        return clientfd;
}
int open_listenfd(char* hostname, char* port) {
    struct addrinfo hints, *listp, *p;
    int listenfd,optval = 1;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE| AI_ADDRCONFIG;
    hints.ai_flags|= AI_NUMERICSERV;
    getaddrinfo(NULL, port, &hints,&listp);

    for( p =listp;p;p = p->ai_next){
        if((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))<0)
            continue;
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,(const void*)&optval, sizeof(int));
        if(bind(listenfd,p->ai_addr,p->ai_addrlen)==0)
            break;
        close(listenfd);
    }

    freeaddrinfo(listp);
    if(!p)
        return -1;
    if(listen(listenfd,LISTENQ)<0)
    {
        close(listenfd);
        return -1;
    }
    return listenfd;

}


