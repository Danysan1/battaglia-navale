#ifndef rete
#define rete

// Funzioni Server
int creaServer(); // Prepara una socket di ascolto e restituisce il file descriptor (o <0 in caso di errore)
int attendiConnessione(const int socketAscolto); // Attende che un client si connetta e restituisce la nuova socket (o <0 in caso di errore)

// Funzioni Client
int creaClient(char *indirizzo); // Si connette ad un server e restituisce la socket (o <0 in caso di errore)

// Funzioni comuni
int inviaInt(const int socket, int val);
int riceviInt(const int socket);
int inviaMappa(const int socket, const int dimensione, int **mappa);
int riceviMappa(const int socket, const int dimensione, int **buffer);
int chiudi(const int socket); // Termina la comunicazione e chiude la socket

#endif
