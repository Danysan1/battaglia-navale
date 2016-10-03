/** Daniele Santini, Giuseppe Tempesta **/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h> 

#define DEBUG_ENABLE 1
typedef char bool;

void debug(const char *testo);

int main (int argc, char **argv){
    //Dimensione del campo di battaglia
    int dim = 0;
    
    // Puntatori ai campi di battaglia
    bool ***computer, ***giocatore;

    debug("Controllo i parametri");
    if(argc > 1)
        dim=atoi(argv[1]);

    debug("Se necessario chiedo la dimensione");
    while(dim < 2 || dim > 10){
        printf("Inserire la dimensione (fra 2 e 10): ");
        scanf("%d", &dim);
    }
    
    printf("Creazione del campo di battaglia %d x %d\n", dim, dim);
    
    debug("Alloco i campi di battaglia");
    computer = (bool***)malloc(dim * dim * sizeof(bool));
    giocatore = (bool***)malloc(dim * dim * sizeof(bool));
    
    
    
    
    
    
    
   // Non completo 
    
    
    
    
    
    
    
    
    
    debug("Dealloco i campi di battaglia");
    free(computer);
    free(giocatore);
    
    return 0;
}

void debug(const char *testo){
    if(DEBUG_ENABLE)
        printf("LOG: %s\n", testo);
}
