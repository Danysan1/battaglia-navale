#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>

#include "interfaccia.h"

/* ** Daniele Santini, Giuseppe Tempesta **
*
* COMPILAZIONE: ok
* STATO: incompleto
* NOTE: -> ogni funzione è dettagliatamente commentata
*	-> si è modificato qualcosa
*	-> possibilità di separare il progetto in piu' .c e .h, soluzione troppo compatta in un solo file (da valutare in seguito)
* 
* NOTE: Per compilare:
* gcc battaglia.c interfaccia.c
* 
* NOTE: Per compilare, con debug:
* gcc battaglia.c interfaccia.c -D debug_enable
* 
* NOTE: Per scaricare le modifiche fatte da qualcun'altro:
* git fetch --all
* git merge [persona]/master
* 
* NOTE: Per caricare le modifiche:
* git add [i file modificati]
* git commit
* git push
* 
* NOTE: Per scaricare le modifiche fatte da qualcun'altro (con merge conflict irrisolvibili):
* git fetch --all
* git reset --hard [persona]/master
* (Al momento di caricare le modifiche invece di "git push" bisognerà usare "git push --force")
*/


// funzioni per l'allocazione dinamica della memoria
int ** allocaCampo(int dimensione); // Alloca e restituisce una matrice quadrata della dimensione indicata
void deallocazione(int ** matrice, int dim); // Dealloca una matrice quadrata della dimensione indicata

//funzioni di posizionamento e controllo
int ** random_computer(int dimensione, int ** matrix); // Riempie la matrice con navi posizionate casualmente
int ** posizionamento_giocatore(int dimensione, int ** matrix); // Chiede all'utente di inserire le navi nella griglia di battaglia
int check(int x, int y); // Controlla che l'inserimento sia un numero compreso nel range valido
int calcolo_esito_giocatore(int dimensione, int **matrix);
int calcolo_esito_computer(int dimensione, int **matrix);
void cambio(int x, int y, int **matrix); 

//funzioni di interazione utente
int mossa_giocatore(int **computer, int **scoperti, int dimensione);
int mossa_computer(int **giocatore, int **scoperti, int dimensione);

int somma_matrice_computer=0;
int somma_matrice_giocatore=0;


int main (int argc, char **argv){
    int dim = 0, // Dimensione del campo di battaglia
		esito = 0; // 0->Partita in corso; 1->Ha vinto il giocatore; 2->Ha vinto il computer
    int **computer, **giocatore, // Campi di battaglia del computer e del giocatore
		**comp_scoperti, **gioc_scoperti; // Matrici che indicano se le caselle sono state già scoperte (colpite, se sono navi)
    
    debug("Controllo i parametri");
    if(argc > 1)
        dim=atoi(argv[1]);

    while(dim < 3 || dim > 10)
        dim=chiediNumero("Inserire la dimensione (fra 2 e 10): ");
    
    printf("Creazione del campo di battaglia %d x %d\n", dim, dim);
    
    debug("Alloco i campi di battaglia");
    computer = allocaCampo(dim);
    comp_scoperti = allocaCampo(dim);
    giocatore = allocaCampo(dim);
	gioc_scoperti = allocaCampo(dim);
    
    debug("Posizionamento navi giocatore");
    giocatore = posizionamento_giocatore(dim,giocatore);
    
    debug("Posizionamento navi computer");
    computer = random_computer(dim,computer);
    
    do{
        debug("Stampo i campi di battaglia");
        stampa(dim, giocatore, computer, comp_scoperti);

	debug("Richiesta mossa giocatore");
        esito = mossa_giocatore(computer, comp_scoperti, dim);

	if(esito != 1){ 
        	debug("Calcolo mossa computer");
       		esito = mossa_computer(giocatore, gioc_scoperti, dim);
        }
	printf("=======================================\n");
        
    } while (esito == 0); 
    
	debug("Annuncio l'esito");
	stampa(dim, giocatore, computer, NULL);
	if(esito == 1)
		annunciaVittoria();
	else if(esito == 2)
		annunciaSconfitta();
	
	debug("Dealloco i campi di battaglia");
	deallocazione(computer,dim);
	deallocazione(comp_scoperti, dim);
	deallocazione(giocatore,dim);
	deallocazione(gioc_scoperti, dim);
    
    return 0;
}

/*
* FUNZIONE PER LA GESTIONE DI UNA MOSSA FATTA DAL GIOCATORE + CONTROLLO E CAMBIAMENTO SULLA MATRICE COMPUTER
*/
int mossa_giocatore(int **computer, int **comp_scoperti, int dimensione){
	int x,y;
	x=chiediNumero("Inserire riga(numero) del campo avversario: ");

	y=chiediLettera("Inserire colonna(lettera) del campo avversario: ");

	if(computer[x][y]>0){
		printf("GIOCATORE COLPISCE NAVE COMPUTER!\n");
		cambio(x,y,computer);
	} else 
		printf("GIOCATORE COLPISCE NIENTE!\n");
	
	comp_scoperti[x][y] = 1;

	return calcolo_esito_giocatore(dimensione, computer);
}

void cambio(int x, int y, int **matrix) {
	int num=matrix[x][y];

	switch(num) {
		case 2: matrix[x][y]=-2;
		case 3: matrix[x][y]=-3;
		case 4: matrix[x][y]=-4;
		default: matrix[x][y]=0;
	}
}

int calcolo_esito_giocatore(int dimensione, int **matrix) {
	int i,k,res=0;

	for(i=0; i<dimensione; i++)
		for(k=0; k<dimensione; k++)
			res+=matrix[i][k];
	if(somma_matrice_computer == -res)
		return 1;
	else
		return 0;
}

/*
* FUNZIONE PER LA GESTIONE DI UNA MOSSA FATTA DAL COMPUTER + CONTROLLO E CAMBIAMENTO SULLA MATRICE GIOCATORE
* La matrice gioc_scoperti è 0 laddove non ha ancora colpito, 1 laddove ha già colpito
*/

int mossa_computer(int **giocatore, int **gioc_scoperti, int dimensione){
	int x,y;
	srand(time(NULL)*time(NULL));
	
	do{ //continui a cambiare finchè becchi una posizione gia COLPITA
		x=rand()%dimensione+0;
		y=rand()%dimensione+0;
	}while( gioc_scoperti[x][y]==1 );

	if(giocatore[x][y]>0){
		printf("COMPUTER COLPISCE NAVE GIOCATORE!\n");
		cambio(x,y,giocatore);
	} else
		printf("COMPUTER COLPISCE NIENTE!\n");
	
	gioc_scoperti[x][y]=1;

	return calcolo_esito_giocatore(dimensione, giocatore);
}

int calcolo_esito_computer(int dimensione, int **matrix) {
	int i,k,res=0;

	for(i=0; i<dimensione; i++)
		for(k=0; k<dimensione; k++)
			res+=matrix[i][k];
	if(somma_matrice_giocatore == -res)
		return 2;
	else
		return 0;
}


/*
* dealloca ogni variabile allocata dinamicamente
* STATO: completa
*/
void deallocazione(int **matrice, int dim) {
	debug("Dealloco un campo di battaglia");

	for(int i=0; i<dim; i++)
		free(matrice[i]);
	
	free(matrice);
}

int ** allocaCampo(int dim){
    int ** matrice = (int**) malloc(dim * sizeof(int*));
    
    for(int i=0; i<dim; i++)
        matrice[i] = (int *) malloc(dim * sizeof(int));
        
    for(int i=0; i<dim; i++)
        for(int k=0; k<dim; k++)
            matrice[i][k] = 0;
    
    
    return matrice;
}

/*
* chiede all'utente di inserire le navi nella griglia di battaglia, proporzionalmente al numero di caselle scelte
* ps si spera in un utente con un Q.I.>0, sebbene vengano supportati casi di Q.I.<0 [livello "grande fratello"]
*/

int ** posizionamento_giocatore(int dimensione, int ** matrix) {

	int i,k,piccole=0, medie=0, grandi=0, enormi=0, verso, x, y;
	
	stampa(dimensione, matrix, NULL, NULL);

	piccole=dimensione/2;
	medie=dimensione/2;
	grandi=dimensione/4;
	enormi=dimensione/5;

	printf("Inserire: %d piccole, %d medie, %d grandi, %d enormi\n", piccole, medie, grandi,enormi);
	
	//inserimento piccole
	
	for(i=0; i<piccole; i++) {
		printf("Inserimento piccole (1) \n");
        x=chiediNumero("inserire riga(numero): ");

        y=chiediLettera("inserire colonna(lettera): ");
		if(matrix[x][y]==0 && check(x,y)==1 ){
			matrix[x][y]=1;
			stampa(dimensione, matrix, NULL, NULL);
		}
		else {
			printf("posizione (%d,%d) già occupata o inserimento errato",x,y);
			i--; //faccio ripetere l'operazione
		}
		
	}

	printf("=================================\n");

	//inserimento medie
	for(i=0; i<medie; i++) {
		printf("Inserimento medie (2) \n");
        x=chiediNumero("inserire riga(numero): ");

        y=chiediLettera("inserire colonna(lettera): ");
	
        verso=chiediNumero("inserire verso(1-orizzontale , 0-verticale): ");
		
		if(matrix[x][y]==0 && check(x,y)==1 && verso==1 && matrix[x][y]==0 && matrix[x][y+1]==0){ //controllo posizioni adiacenti libere
			matrix[x][y]=2;
			matrix[x][y+1]=2;
			stampa(dimensione, matrix, NULL, NULL);
		}
		else if(matrix[x][y]==0 && check(x,y)==1 && verso==0 && matrix[x][y]==0 && matrix[x+1][y]==0){ //controllo posizioni adiacenti libere
			matrix[x][y]=2;
			matrix[x+1][y]=2;
			stampa(dimensione, matrix, NULL, NULL);
		}
		else {
			printf("posizione (%d,%d,%d) già occupata o inserimento errato",x,y,verso);
			i--; //faccio ripetere l'operazione
		}
		
	}
	
	printf("=================================\n");
	
	//inserimento grandi
	for(i=0; i<grandi; i++) {
		printf("Inserimento grandi (3) \n");
        x=chiediNumero("inserire riga(numero): ");

        y=chiediLettera("inserire colonna(lettera): ");

        verso=chiediNumero("inserire verso(1-orizzontale , 0-verticale): ");
		
		if(matrix[x][y]==0 && check(x,y)==1 && verso==1 && matrix[x][y]==0 && matrix[x][y+1]==0 
			&& matrix[x][y+2]==0){ //controllo posizioni adiacenti libere
			matrix[x][y]=3;
			matrix[x][y+1]=3;
			matrix[x][y+2]=3;
			stampa(dimensione, matrix, NULL, NULL);
		}
		else if(matrix[x][y]==0 && check(x,y)==1 && verso==0 && matrix[x][y]==0 && matrix[x+1][y]==0 
			&& matrix[x+2][y]==0){ //controllo posizioni adiacenti libere
			matrix[x][y]=3;
			matrix[x+1][y]=3;
			matrix[x+2][y]=3;
			stampa(dimensione, matrix, NULL, NULL);
		}
		else {
			printf("posizione (%d,%d,%d) già occupata o inserimento errato",x,y,verso);
			i--; //faccio ripetere l'operazione
		}
		
	}
	
	printf("=================================\n");

	//inserimento enormi
	for(i=0; i<enormi; i++) {
		printf("Inserimento enormi (4) \n");
        x=chiediNumero("inserire riga(numero): ");

        y=chiediLettera("inserire colonna(lettera): ");
	
        verso=chiediNumero("inserire verso(1-orizzontale , 0-verticale): ");
		
		if(matrix[x][y]==0 && check(x,y)==1 && verso==1 && matrix[x][y]==0 && matrix[x][y+1]==0 
			&& matrix[x][y+2]==0 && matrix[x][y+3]==0){ //controllo posizioni adiacenti libere
			matrix[x][y]=4;
			matrix[x][y+1]=4;
			matrix[x][y+2]=4;
			matrix[x][y+3]=4;
			stampa(dimensione, matrix, NULL, NULL);
		}
		else if(matrix[x][y]==0 && check(x,y)==1 && verso==0 && matrix[x][y]==0 && matrix[x+1][y]==0 
			&& matrix[x+2][y]==0 && matrix[x+3][y]==0){ //controllo posizioni adiacenti libere
			matrix[x][y]=4;
			matrix[x+1][y]=4;
			matrix[x+2][y]=4;
			matrix[x+3][y]=4;
			stampa(dimensione, matrix, NULL, NULL);
		}
		else {
			printf("posizione (%d,%d,%d) già occupata o inserimento errato",x,y,verso);
			i--; //faccio ripetere l'operazione
		}
		
	}

	printf("=================================\n");
	
	//fondamentale per il meccanismo di determinazione dell'esito, controllato per ora con meccanismo elementare
	for(i=0; i<dimensione; i++)
		for(k=0; k<dimensione; k++)
			somma_matrice_giocatore+=matrix[i][k];


	return matrix;
}

/*
* controlla che l'inserimento sia un numero compreso in un certo range
*  STATO: completa
*/
int check(int x,int y) {
    return x>=0 && x<=10 && y>=0 && y<=10;
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
	int i,k,piccole=0, medie=0, grandi=0, enormi=0, posI, posK, verso;
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

	//fondamentale per il meccanismo di determinazione dell'esito, controllato per ora con meccanismo elementare
	for(i=0; i<dimensione; i++)
		for(k=0; k<dimensione; k++)
			somma_matrice_computer+=matrix[i][k];

	return matrix;
}


