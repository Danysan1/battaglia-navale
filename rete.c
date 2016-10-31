#include "rete.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>



int creaServer(const unsigned short porta){
    const int sockFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int out = sockFD;
    
    if(sockFD < 0)
        perror("Errore nella creazione della socket di ascolto");
    else {
        struct sockaddr_in local;
        local.sin_family = AF_INET;
        local.sin_port = htons(porta);
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



int chiudiServer(const int socketAscolto){
    // rifiuta connessioni
    
    return close(socketAscolto);
}
