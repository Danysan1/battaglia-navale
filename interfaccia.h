#ifndef interfaccia
#define interfaccia

void debug(const char * const testo); // Se il debug è abilitato stampa a schermo il messaggio di debug
void legenda();
void stampa(const int dimensione, int **giocatore, int **nemico, int **nemico_scoperti); // Stampa a schermo i campi del giocatore e del nemico
int chiediNumero(const char * const testo); // Chiede un numero
int chiediLettera(const char * const testo); // Chiede (e traduce in numero da 0 a 26) una lattera
void annunciaVittoria();
void annunciaSconfitta();

#endif
