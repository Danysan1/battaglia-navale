#include "interfaccia.h"

void debug(const char *testo){
#ifdef debug
        printf("DEBUG: %s\n", testo);
#endif
}

/* 
 * Stampa a schermo il campo del giocatore (in chiaro) e del nemico (se nemico!=NULL)(solo le caselle scoperte)
 * 
 */
void stampa(int dim, int **giocatore, int **nemico){
    /* Esempio 3x3
     * 0 = vuoto
     * 1 = nave
     * X = nave colpita
     * ? = sconosciuto
     * 
     *   ABC     ABC
     * 0|010   0|??0
     * 1|000   1|???
     * 2|0X1   2|0X?
     */
    
    // Prima riga (lettere)
    printf("  ");
    for(int i=0; i<dim; i++)
        putchar(65 + i); // A, B, C, ...
    
    if(nemico){
        printf("     ");
        for(int i=0; i<dim; i++)
            putchar(65 + i); // A, B, C, ...
    }
    
    putchar('\n');
        
    // Le altre righe
    for(int i=0; i<dim; i++){
        printf("%d|",i);
        // Le colonne della tabella giocatore
        for(int k=0; k <dim; k++)
            printf("%d",giocatore[i][k]);
        
        if(nemico){
            printf("   %d|",i);
            //Le colonne della tabella nemico
            for(int k=0; k<dim; k++)
                printf("%d", nemico[i][k]);
        }
        
        putchar('\n');
    }
}
