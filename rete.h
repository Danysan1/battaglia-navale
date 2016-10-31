#ifndef rete
#define rete

// Funzioni Server
int creaServer(); // Prepara una socket di ascolto e restituisce il file descriptor (o <0 in caso di errore)
int attendiConnessione(const int socketAscolto); // Attende che un client si connetta e restituisce la nuova socket (o <0 in caso di errore)
int inviaDimensione(const int socket, const int dim); // Invia la dimensione del campo di gioco (dim==-1 -> Partita rifiutata)

// Funzioni Client
int creaClient(char *indirizzo); // Si connette ad un server e restituisce la socket (o <0 in caso di errore)
int riceviDimensione(const int socket); // Riceve la dimensione del campo di gioco (dim==-1 -> errore di comunicazione o partita rifiutata)

// Funzioni comuni
int inviaMossa(const int socket, const int riga, const int colonna); // 
int inviaMappa(const int socket, int **mappa);
int riceviMossa(const int socket, int *riga, int *colonna); // Inserisce la mossa ricevuta nelle variabili puntate
int ** riceviMappa(const int socket);
int chiudi(const int socket); // Termina la comunicazione e chiude la socket

#endif
