#include "rete.h"
#include "interfaccia.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>

#define PORTA_SERVER 54321

int creaServer(){
    const int sockFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int out = sockFD;
    
    if(sockFD < 0)
        perror("Errore nella creazione della socket di ascolto");
    else {
        struct sockaddr_in local;
        local.sin_family = AF_INET;
        local.sin_port = htons(PORTA_SERVER);
        local.sin_addr.s_addr = INADDR_ANY;
        
        const int risBind = bind(sockFD, (struct sockaddr*)&local, sizeof(struct sockaddr_in));
        
        if(risBind < 0){
            perror("Errore nella bind()");
            out = risBind;
        } else {
            const int risListen = listen(sockFD, 2);
            
            if(risListen < 0){
                perror("Errore nella listen()");
                out = risListen;
            }
        }
        
        if(out < 0)
            close(sockFD);
    }        
    
    return out;
}



int attendiConnessione(const int socketAscolto){
    int out;
    
    do {
        out = accept(socketAscolto, 0, 0);
    } while (errno == EINTR);
    
    return out;
}



int inviaDimensione(const int socket, const int dim){
    const int tradotto = htons(dim);
    return (sizeof(tradotto) == write(socket, &tradotto, sizeof(tradotto)));
}



int creaClient(char *indirizzo){
    int sockFD, risConnect;
    struct sockaddr_in server;
    struct hostent *serverInfo;
    
    if(indirizzo == 0)
        return -1;
    
    serverInfo = gethostbyname(indirizzo);
    
    if(serverInfo == 0)
        return -1;

    server.sin_family = AF_INET;
    server.sin_port = htons(PORTA_SERVER);
    server.sin_addr.s_addr = ((struct in_addr*) (serverInfo->h_addr))->s_addr;
    
    sockFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if(sockFD < 0){
        perror("Errore nella creazione della socket");
        return -1;
    }
    
    do {
        risConnect = connect(sockFD, (struct sockaddr*)&server, sizeof(server));
    } while (errno == EINTR);
        
    if(risConnect < 0){
        perror("Errore nella connect()");
        return risConnect;
    }
    
    
    return sockFD;
}



int riceviDimensione(const int socket){
    int dim, ricevuti = read(socket, &dim, sizeof(dim));
    
    if(ricevuti != sizeof(dim))
        dim = -1;
    else
        dim = ntohs(dim);
        
    return dim;
}



int chiudi(const int socket){
    shutdown(socket, 2);
    return close(socket);
}
