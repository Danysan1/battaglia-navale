#include "interfaccia.h"

void debug(const char *testo){
#ifdef debug_enable
        printf("DEBUG: %s\n", testo);
#endif
}

/* 
 * Stampa a schermo il campo del giocatore (in chiaro) e del nemico (se nemico!=NULL)(solo le caselle scoperte)
 * 
 */
void stampa(const int dim, int **giocatore, int **nemico){
    /* Esempio 3x3
     * 0 = vuoto
     * 1 = nave
     * X = nave colpita
     * ? = sconosciuto
     * 
     * Singola mappa:
     *        ABC
     *      0|010
     *      1|000
     *      2|0X1
     * 
     * Entrambe le mappe:
     *    ABC      ABC
     *  0|010    0|??0
     *  1|000    1|???
     *  2|0X1    2|0X?
     */
    
    putchar('\n');
        
    // Prima riga (lettere sopra la prima mappa)
    if(!nemico) //Se c'è solo una mappa viene spostata verso destra
        putchar('\t');
    printf("\t  ");
    for(int i=0; i<dim; i++)
        putchar(65 + i); // A, B, C, ...
    
    // Prima riga (lettere sopra la seconda mappa)
    if(nemico){
        printf("\t  ");
        for(int i=0; i<dim; i++)
            putchar(65 + i); // A, B, C, ...
    }
    
    putchar('\n');
        
    // Le altre righe
    for(int i=0; i<dim; i++){
        if(!nemico)
            putchar('\t');
        printf("\t%d|",i);
        // Le colonne della tabella giocatore
        for(int k=0; k <dim; k++)
            printf("%d",giocatore[i][k]);
        
        if(nemico){
            printf("\t%d|",i);
            //Le colonne della tabella nemico
            for(int k=0; k<dim; k++)
                printf("%d", nemico[i][k]);
        }
        
        putchar('\n');
    }
    
    putchar('\n');
}

int chiediNumero(const char * testo){
    int output = -1;

    if(testo)
        printf(testo);

    scanf("%d", &output);
    getchar(); // Raccoglie il fine riga

    return output;
}

int chiediLettera(const char * testo){
    char lettera;
    int output = -1;

    if(testo)
        printf(testo);

    scanf("%c", &lettera);
    getchar(); // Raccoglie il fine riga

    if(lettera>='a' && lettera<='z')
        output = lettera - 'a';
    else if(lettera>='A' && lettera<='Z')
        output = lettera - 'A';

    return output;
}

void annunciaVittoria(){
    printf("\n\tHAI VINTO!!\n"); 
}

void annunciaSconfitta(){
    printf("\n\tHA VINTO IL COMPUTER!\n");
}
