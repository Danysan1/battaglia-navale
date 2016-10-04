#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>


/* ** Daniele Santini, Giuseppe Tempesta **
*
* COMPILAZIONE: ok
* STATO: incompleto
* NOTE: -> ogni funzione è dettagliatamente commentata
*	-> si è modificato qualcosa
*	-> possibilità di separare il progetto in piu' .c e .h, soluzione troppo compatta in un solo file (da valutare in seguito)
*/

// Messaggi di debug abilitati/disabilitati
int debug_enable = 0;
void debug(const char *testo);

//funzione di allocazione dinamica + deallocazione
int ** allocaCampo(int dimensione);
void deallocazione(int **computer, int **giocatore, int dim);

//funzioni di posizionamento e controllo
int ** random_computer(int dimensione, int ** matrix);
int ** posizionamento_giocatore(int dimensione, int ** matrix);
int check(int x, int y);

//funzioni grafiche
void stampa(int dimensione, int **giocatore, int **nemico);

//funzioni di interazione utente
int mossa_giocatore(int **computer, int **giocatore);
int mossa_computer(int **computer, int **giocatore);


int main (int argc, char **argv){
    //Dimensione del campo di battaglia
    int dim = 0, esito = 0;
    
    // Puntatori ai campi di battaglia (computer, giocatore, e parte della mappa del computer scoperta dal giocatore
    int **computer, **giocatore;

    // Navi abilitate nella partita NON PIU NECESSARIO..NAVI DEFINITE NELLE FUNZIONI
    //      1x1     2x1     3x1     4x1
   // int piccola, media, grande, enorme;
    
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
    
    /*debug("Calcolo delle navi necessarie"); NON PIU NECESSARIO, IL CALCOLO E' EFFETTUATO A LIVELLO DI CREAZIONE RANDOM/INSERIMENTO 
    // dim=2 => Solo nave 1x1
    // dim=3 => Navi 1x1 e 2x1
    // dim=4 => Navi 1x1, 2x1 e 3x1
    // dim > 4 => Navi 1x1, 2x1, 3x1 e 4x1
    piccola = dim > 1;
    media = dim > 2;
    grande = dim > 3;
    enorme = dim > 4; */
    
    debug("Posizionamento navi giocatore");
    giocatore = posizionamento_giocatore(dim,giocatore);
    
    debug("Posizionamento navi computer");
    computer = random_computer(dim,computer);
    
    do{
        debug("Stampo i campi di battaglia");
        stampa(dim, giocatore, computer);
        
        debug("Richiesta mossa giocatore");
        esito = mossa_giocatore(computer, giocatore);
        
        //debug("Controllo vittoria"); //non piu necessario, integrato in mossa giocatore 
        
        debug("Calcolo mossa computer");
        esito = mossa_computer(computer, giocatore);
        
      	//debug("Controllo sconfitta"); //non piu necessario, integrato in mossa computer
        
        
    } while (esito == 0); //--> successivamente un flag di esito [da definire] decreterà vittoria usr o sys
    
   deallocazione(computer,giocatore,dim);
    
    return 0;
}

/*
* funzioni da definire, si richiede che venga integrato prima ambiente grafico
*/
int mossa_giocatore(int **computer, int **giocatore){
	return 0;
}
int mossa_computer(int **computer, int **giocatore){
	return 0;
}

void debug(const char *testo){
    if(debug_enable)
        printf("DEBUG: %s\n", testo);
}

/*
* dealloca ogni variabile allocata dinamicamente
* STATO: completa
*/
void deallocazione(int **computer, int **giocatore, int dim) {
	int i;
	debug("Dealloco i campi di battaglia");

	for(i=0; i<dim; i++)
		free(computer[i]);
	for(i=0; i<dim; i++)
		free(giocatore[i]);
	
	free(computer);
	free(giocatore);
}

int ** allocaCampo(int dim){
    int ** matrice = (int**) malloc(dim * sizeof(int*));
    
    for(int i=0; i<dim; i++){
        matrice[i] = (int *) malloc(dim * sizeof(int));
        
        for(int k=0; k<dim; k++)
            matrice[i][k] = 0;
    }
    
    return matrice;
}
/*
* chiede all'utente di inserire le navi nella griglia di battaglia, proporzionalmente al numero di caselle scelte
* ps si spera in un utente con un Q.I.>0, sebbene vengano supportati casi di Q.I.<0 [livello "grande fratello"]
*
*    ATTENZIONE: --> SI RICHIEDI L'INSERIMENTO DI SUPPORTO GRAFICO NEI PUNTI SPECIFICATI
*		 --> STAMPA PRIMA DELL'INSERIMENTO, in modo che utente possa digitare .. a1..b4.. ecc ecc .. per poi essere trasdotti
*    STATO: incompleta
*/
int ** posizionamento_giocatore(int dimensione, int ** matrix) {

	int i,k,piccole=0, medie=0, grandi=0, enormi=0, verso, x, y;
	
	//--------> PLOT 

	piccole=dimensione/2;
	medie=dimensione/2;
	grandi=dimensione/4;
	enormi=dimensione/5;

	printf("Inserire: %d piccole, %d medie, %d grandi, %d enormi\n", piccole, medie, grandi,enormi);
	
	//inserimento piccole
	for(i=0; i<piccole; i++) {
		printf("inserire riga(x): ");
		scanf("%d", &x);

		printf("inserire colonna(y): ");
		scanf("%d", &y);

		if(matrix[x][y]==0 && check(x,y)==1 )
			matrix[x][y]=1;
		else {
			debug("posizione già occupata o inserimento errato");
			i--; //faccio ripetere l'operazione
		}
	}

	//--------> PLOT 

	//inserimento medie
	for(i=0; i<medie; i++) {
		printf("inserire riga(x): ");
		scanf("%d", &x);

		printf("inserire colonna(y): ");
		scanf("%d", &y);
	
		printf("inserire verso(1-orizzontale , 0-verticale): ");
		scanf("%d", &verso);
		
		
		if(matrix[x][y]==0 && check(x,y)==1 && verso==1 && matrix[x][y]==0 && matrix[x][y+1]==0){ //controllo posizioni adiacenti libere
			matrix[x][y]=2;
			matrix[x][y+1]=2;
		}
		else if(matrix[x][y]==0 && check(x,y)==1 && verso==0 && matrix[x][y]==0 && matrix[x+1][y]==0){ //controllo posizioni adiacenti libere
			matrix[x][y]=2;
			matrix[x+1][y]=2;
		}
		else {
			debug("posizione già occupata o inserimento errato");
			i--; //faccio ripetere l'operazione
		}
	}

	//--------> PLOT 
	//inserimento grandi
	for(i=0; i<grandi; i++) {
		printf("inserire riga(x): ");
		scanf("%d", &x);

		printf("inserire colonna(y): ");
		scanf("%d", &y);
	
		printf("inserire verso(1-orizzontale , 0-verticale): ");
		scanf("%d", &verso);
		
		
		if(matrix[x][y]==0 && check(x,y)==1 && verso==1 && matrix[x][y]==0 && matrix[x][y+1]==0 
			&& matrix[x][y+2]==0){ //controllo posizioni adiacenti libere
			matrix[x][y]=3;
			matrix[x][y+1]=3;
			matrix[x][y+2]=3;
		}
		else if(matrix[x][y]==0 && check(x,y)==1 && verso==0 && matrix[x][y]==0 && matrix[x+1][y]==0 
			&& matrix[x+2][y]==0){ //controllo posizioni adiacenti libere
			matrix[x][y]=3;
			matrix[x+1][y]=3;
			matrix[x+2][y]=3;
		}
		else {
			debug("posizione già occupata o inserimento errato");
			i--; //faccio ripetere l'operazione
		}
	}
	
	//--------> PLOT 

	//inserimento enormi
	for(i=0; i<enormi; i++) {
		printf("inserire riga(x): ");
		scanf("%d", &x);

		printf("inserire colonna(y): ");
		scanf("%d", &y);
	
		printf("inserire verso(1-orizzontale , 0-verticale): ");
		scanf("%d", &verso);
		
		
		if(matrix[x][y]==0 && check(x,y)==1 && verso==1 && matrix[x][y]==0 && matrix[x][y+1]==0 
			&& matrix[x][y+2]==0 && matrix[x][y+3]==0){ //controllo posizioni adiacenti libere
			matrix[x][y]=4;
			matrix[x][y+1]=4;
			matrix[x][y+2]=4;
			matrix[x][y+3]=4;
		}
		else if(matrix[x][y]==0 && check(x,y)==1 && verso==0 && matrix[x][y]==0 && matrix[x+1][y]==0 
			&& matrix[x+2][y]==0 && matrix[x+3][y]==0){ //controllo posizioni adiacenti libere
			matrix[x][y]=4;
			matrix[x+1][y]=4;
			matrix[x+2][y]=4;
			matrix[x+3][y]=4;
		}
		else {
			debug("posizione già occupata o inserimento errato");
			i--; //faccio ripetere l'operazione
		}
	}

	//--------> PLOT 
}

/*
* controlla che l'inserimento sia un numero compreso in un certo range
*  STATO: completa
*/
int check(int x,int y) {
	if(x!=3 && x!=4 && x!=5 && x!=6 && x!=7 && x!=8 && x!=9 && x!=10 
	&& y!=3 && y!=4 && y!=5 && y!=6 && y!=7 && y!=8 && y!=9 && y!=10)
		return 1;
	else
		return 0;
}


/*riempie la matrice del sistema con navi posizionate casualmente ed in numero proporzionale alla dimensione
* --> num1 (numero di piccola) = dimensione / 2
* --> num2 (numero di media) = dimensione / 2
* --> num3 (numero di grande) = dimensione / 4
* --> num4 (numero di enorme) = dimensione / 5
*	questo dovrebbe garantire una distribuzione abbastanza proporzionale
* !!warning!! : dal punto di vista statistico questa funzione potrebbe richiedere molto tempo o addirittura instaurare un loop infinito se 
* non si generano mai numeri che propongono posizioni ACCETTATE
*
*  STATO: completa
*/

int ** random_computer(int dimensione, int ** matrix) { 
	int i,piccole=0, medie=0, grandi=0, enormi=0, posI, posK, verso;
	srand(time(NULL)*time(NULL));

	piccole=dimensione/2;
	medie=dimensione/2;
	grandi=dimensione/4;
	enormi=dimensione/5;
	
	//distribuzione delle piccole
	for(i=0; i<piccole; i++){
		posI=rand()%dimensione+0;
		posK=rand()%dimensione+0;
		if(matrix[posI][posK]==0)
			matrix[posI][posK]=1;
		else
			i--; //ripeti l'operazione, perchè già presente
	}

	//distribuzione delle medie
	for(i=0; i<medie; i++){
		posI=rand()%dimensione+0;
		posK=rand()%dimensione+0;
		verso=rand()%1+0; //1 --> orizzontale , 0 --> verticale
		if(verso==1 && matrix[posI][posK]==0 && matrix[posI][posK+1]==0 ){ //mi assicuro che ci sia posto ANCHE NEI DINTORNI per la nave
			matrix[posI][posK]=2; 
			matrix[posI][posK+1]=2;
		} else if(verso==0 && matrix[posI][posK]==0 && matrix[posI+1][posK]==0 ){ //mi assicuro che ci sia posto ANCHE NEI DINTORNI per la nave
			matrix[posI][posK]=2;
			matrix[posI+1][posK]=2;
		}
		else
			i--; //ripeti l'operazione, perchè già presente
	}

	//distribuzione delle grandi
	for(i=0; i<grandi; i++){
		posI=rand()%dimensione+0;
		posK=rand()%dimensione+0;
		verso=rand()%1+0; //1 --> orizzontale , 0 --> verticale
		if(verso==1 && matrix[posI][posK]==0 && matrix[posI][posK+1]==0 
		&& matrix[posI][posK+2]==0 ){ //mi assicuro che ci sia posto ANCHE NEI DINTORNI per la nave
			matrix[posI][posK]=3; 
			matrix[posI][posK+1]=3;
			matrix[posI][posK+2]=3;
		} else if(verso==0 && matrix[posI][posK]==0 
			&& matrix[posI+1][posK]==0 && matrix[posI+2][posK]==0){ //mi assicuro che ci sia posto ANCHE NEI DINTORNI per la nave
			matrix[posI][posK]=3;
			matrix[posI+1][posK]=3;
			matrix[posI+2][posK]=3;
		}
		else
			i--; //ripeti l'operazione, perchè già presente
	}
	
	//distribuzione delle grandi
	for(i=0; i<enormi; i++){
		posI=rand()%dimensione+0;
		posK=rand()%dimensione+0;
		verso=rand()%1+0; //1 --> orizzontale , 0 --> verticale
		if(verso==1 && matrix[posI][posK]==0 && matrix[posI][posK+1]==0 
		&& matrix[posI][posK+2]==0 && matrix[posI][posK+3]==0 ){ //mi assicuro che ci sia posto ANCHE NEI DINTORNI per la nave
			matrix[posI][posK]=4; 
			matrix[posI][posK+1]=4;
			matrix[posI][posK+2]=4;
			matrix[posI][posK+3]=4;
		} else if(verso==0 && matrix[posI][posK]==0 && matrix[posI+1][posK]==0 
			&& matrix[posI+2][posK]==0&& matrix[posI+3][posK]==0){ //mi assicuro che ci sia posto ANCHE NEI DINTORNI per la nave
			matrix[posI][posK]=4;
			matrix[posI+1][posK]=4;
			matrix[posI+2][posK]=4;
			matrix[posI+3][posK]=4;
		}
		else
			i--; //ripeti l'operazione, perchè già presente
	}

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
