#ifndef interfaccia
#define interfaccia

#include<stdio.h>

void debug(const char *testo); // Se il debug è abilitato stampa a schermo il messaggio di debug
void stampa(const int dimensione, int **giocatore, int **nemico); // Stampa a schermo i campi del giocatore e del nemico
int chiediNumero(const char * testo); // Chiede un numero
int chiediLettera(const char * testo); // Chiede (e traduce in numero da 0 a 26) una lattera

#endif
