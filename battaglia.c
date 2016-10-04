/** Daniele Santini, Giuseppe Tempesta **/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>

// Messaggi di debug abilitati/disabilitati
int debug_enable = 0;

void debug(const char *testo);
int ** allocaCampo(int dimensione);
void stampa(int dimensione, int **giocatore, int **nemico);

int main (int argc, char **argv){
    //Dimensione del campo di battaglia
    int dim = 0;
    
    // Puntatori ai campi di battaglia (computer, giocatore, e parte della mappa del computer scoperta dal giocatore
    int **computer, **giocatore;

    // Navi abilitate nella partita
    //      1x1     2x1     3x1     4x1
    int piccola, media, grande, enorme;
    
    int fine = 0;
    
    debug("Controllo i parametri");
    if(argc > 1)
        dim=atoi(argv[1]);
    
    if(argc > 2)
        debug_enable = atoi(argv[2]);

    while(dim < 2 || dim > 10){
        debug("Chiedo la dimensione");
        printf("Inserire la dimensione (fra 2 e 10): ");
        scanf("%d", &dim);
    }
    debug("Dimensione ottenuta");
    
    printf("Creazione del campo di battaglia %d x %d\n", dim, dim);
    
    debug("Alloco i campi di battaglia");
    computer = allocaCampo(dim);
    giocatore = allocaCampo(dim);
    
    debug("Calcolo delle navi necessarie");
    // dim=2 => Solo nave 1x1
    // dim=3 => Navi 1x1 e 2x1
    // dim=4 => Navi 1x1, 2x1 e 3x1
    // dim > 4 => Navi 1x1, 2x1, 3x1 e 4x1
    piccola = dim > 1;
    media = dim > 2;
    grande = dim > 3;
    enorme = dim > 4;
    
    debug("Posizionamento navi giocatore");
    // ...
    
    debug("Posizionamento navi computer");
    // ...
    
    do{
        debug("Stampo i campi di battaglia");
        stampa(dim, giocatore, computer);
        
        debug("Richiesta mossa giocatore");
        // ...
        
        debug("Controllo vittoria");
        // ...
        
        debug("Calcolo mossa computer");
        // ...
        
        debug("Controllo sconfitta");
        // ...
        
        
        fine = 1; //Temporaneo
    } while ( ! fine);
    
    debug("Dealloco i campi di battaglia");
    free(computer);
    free(giocatore);
    
    return 0;
}

void debug(const char *testo){
    if(debug_enable)
        printf("DEBUG: %s\n", testo);
}

void update() {
	printf("//box//");
}

int ** allocaCampo(int dim){
    int ** matrice = (int**) malloc(dim * sizeof(int*));
    
    for(int i=0; i<dim; i++){
        matrice[i] = (int *) malloc(dim*sizeof(int));
        
        for(int k=0; k<dim; k++)
            matrice[i][k] = 0;
    }
    
    return matrice;
}

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
    puts("\n  ");
    for(int i=0; i<dim; i++)
        putchar(65 + i); // A, B, C, ...
    puts("     ");
    for(int i=0; i<dim; i++)
        putchar(65 + i); // A, B, C, ...
        
    // Le altre righe
    for(int i=0; i<dim; i++){
        printf("%d|",i);
        // Le colonne della tabella giocatore
        for(int k=0; k <dim; k++)
            printf("%d",giocatore[i][k]);
        
        printf("   %d|",i);
        //Le colonne della tabella nemico
        for(int k=0; k<dim; k++)
            printf("%d", nemico[i][k]);
        putchar('\n');
            
            
        
    }
    
    
    
    // ...
    
}
