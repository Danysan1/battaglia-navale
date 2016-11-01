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



int inviaInt(const int socket, int val){
	val = htons(val);
	return (sizeof(val) == write(socket, &val, sizeof(val)));
}



int riceviInt(const int socket){
	int val, ricevuti = read(socket, &val, sizeof(val));
	
	if(ricevuti == sizeof(val))
		return ntohs(val);
	else
		return -1;
}



int inviaMappa(const int socket, const int dimensione, int **mappa){
	int count=0;
	
	for(int i=0; i<dimensione; i++){
		for(int k=0; k<dimensione; k++){
			int val = htons(mappa[i][k]);
			count += write(socket, &val, sizeof(val));
		}
	}
	
	return count == ((int)sizeof(int)*dimensione*dimensione);
}



int riceviMappa(const int socket, const int dimensione, int **buffer){
	int count=0;
	
	for(int i=0; i<dimensione; i++){
		for(int k=0; k<dimensione; k++){
			int val;
			count += read(socket, &val, sizeof(val));
			buffer[i][k] = ntohs(val);
		}
	}
	
	return count == ((int)sizeof(int)*dimensione*dimensione);
}



int chiudi(const int socket){
	shutdown(socket, 2);
	return close(socket);
}
