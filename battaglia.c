/** Daniele Santini, Giuseppe Tempesta **/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h> 

typedef char bool;

// Messaggi di debug abilitati/disabilitati
bool debug_enable = 0;

void debug(const char *testo);
bool ** allocaCampo(int dimensione);
void stampa(int dimensione, bool **giocatore, bool **scoperta);

int main (int argc, char **argv){
    //Dimensione del campo di battaglia
    int dim = 0;
    
    // Puntatori ai campi di battaglia (computer, giocatore, e parte della mappa del computer scoperta dal giocatore
    bool **computer, **giocatore, **scoperta;

    // Navi abilitate nella partita
    //      1x1     2x1     3x1     4x1
    bool piccola, media, grande, enorme;
    
    bool fine = 0;
    
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
    scoperta = allocaCampo(dim);
    
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
        stampa(dim, giocatore, scoperta);
        
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
    free(scoperta);
    
    return 0;
}

void debug(const char *testo){
    if(debug_enable)
        printf("DEBUG: %s\n", testo);
}

void update() {
	printf("//box//");
}

bool ** allocaCampo(int dim){
    bool ** matrice = (bool**) malloc(dim * dim * sizeof(bool));
    
    debug("Svuoto il campo");
    for(int i=0; i<dim; i++)
        for(int k=0; k<dim; k++)
            matrice[i][k] = 0;
        
    return matrice;
}

void stampa(int dim, bool **gioc, bool **nem){
    // 186 = ║
    // 187 = ╗ 
    // 188 = ╝
    // 200 = ╚
    // 201 = ╔
    // 202 = ╩
    // 203 = ╦
    // 205 = ═
    
    putchar(201);
    for(int i=0; i<dim; i++)
        putchar(205);
    putchar(203);
    for(int i=0; i<dim; i++)
        putchar(205);
    putchar(187);
    
    
    
    
    putchar(200);
    for(int i=0; i<dim; i++)
        putchar(205);
    putchar(202);
    for(int i=0; i<dim; i++)
        putchar(205);
    putchar(188);
    
}
