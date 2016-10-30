#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>

#include "interfaccia.h"

/* ** Daniele Santini, Giuseppe Tempesta **
 * 
 * NOTE: Per compilare:
 * gcc battaglia.c interfaccia.c rete.c
 * 
 * NOTE: Per compilare, con debug:
 * gcc battaglia.c interfaccia.c rete.c -D debug_enable
 * 
 * NOTE: Per scaricare le modifiche fatte su un'altra macchina:
 * git pull --all
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

//funzioni di posizionamento
void posizionamento_computer(const int dimensione, int **matrix, int *somma_matrice, int *navi); // Riempie la matrice con navi posizionate casualmente
void posizionamento_giocatore(const int dimensione, int **matrix, int *somma_matrice, int *navi); // Chiede all'utente di inserire le navi nella griglia di battaglia
void reset(int **matrix, const int dimensione);

//funzioni di controllo
int check(const int x,const int y,const int dimensione,const int lunghezza,const int orizzontale);//Controlla che la nave inserita stia nella matrice
int calcolo_esito(const int dimensione, int **matrice_avversario, const int somma_matrice_avversario);

//funzioni di interazione utente
void mossa_giocatore(int **computer, int **scoperti, int dimensione);
void mossa_computer(int **giocatore, int **scoperti, int dimensione);



int main (int argc, char **argv){
	int dim = 0, // Dimensione del campo di battaglia
	esito = 0; // 0->Partita in corso; 1->Ha vinto il giocatore; 2->Ha vinto il computer
	int **computer, **giocatore, // Campi di battaglia del computer e del giocatore
	**comp_scoperti, **gioc_scoperti; // Matrici che indicano se le caselle sono state già scoperte
	int somma_matrice_computer = 0, somma_matrice_giocatore = 0,
	navi[4]; // Numero di navi [piccole,medie,grandi,enormi] da posizionare
	
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
	
	navi[0] = dim/2; // Navi piccole: dimensione/2
	navi[1] = dim/3; // Navi medie: dimensioni/3
	navi[2] = dim/4; // Navi grandi: dimensioni/4
	navi[3] = dim/5; // Navi enormi: dimensioni/5
	
	debug("Posizionamento navi giocatore");
	posizionamento_giocatore(dim, giocatore, &somma_matrice_giocatore, navi);
	
	debug("Posizionamento navi computer");
	posizionamento_computer(dim, computer, &somma_matrice_computer, navi);
	
	legenda();
	
	do{
		debug("Stampo i campi di battaglia");
		stampa(dim, giocatore, computer, comp_scoperti);
		
		debug("Richiesta mossa giocatore");
		mossa_giocatore(computer, comp_scoperti, dim);
		esito = calcolo_esito(dim, computer, somma_matrice_computer);
		
		if(esito != 1){ 
			debug("Calcolo mossa computer");
			mossa_computer(giocatore, gioc_scoperti, dim);
			esito = calcolo_esito(dim, giocatore, somma_matrice_giocatore) ? 2 : 0;
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



// FUNZIONE PER LA GESTIONE DI UNA MOSSA FATTA DAL GIOCATORE + CONTROLLO E CAMBIAMENTO SULLA MATRICE COMPUTER
void mossa_giocatore(int **computer, int **comp_scoperti, int dimensione){
	int x,y;
	
	do {
		x=chiediNumero("Inserire riga(numero) del campo avversario: ");
	} while (x<0 || x>=dimensione);
	
	do {
		y=chiediLettera("Inserire colonna(lettera) del campo avversario: ");
	} while (y<0 || y>=dimensione);
	
	if(computer[x][y]>0){
		printf("GIOCATORE COLPISCE NAVE COMPUTER!\n");
		computer[x][y] = -computer[x][y];
	} else 
		printf("GIOCATORE COLPISCE NIENTE!\n");
	
	comp_scoperti[x][y] = 1;
}



int calcolo_esito(const int dimensione, int **matrix, const int somma_matrice){
	int i,k,res1=0;
	
	for(i=0; i<dimensione; i++){
		for(k=0; k<dimensione; k++)
			res1+=matrix[i][k];
	}
	
	return somma_matrice == -res1;
}



/*
 * FUNZIONE PER LA GESTIONE DI UNA MOSSA FATTA DAL COMPUTER + CONTROLLO E CAMBIAMENTO SULLA MATRICE GIOCATORE
 * La matrice gioc_scoperti è 0 laddove non ha ancora colpito, 1 laddove ha già colpito
 */
void mossa_computer(int **giocatore, int **gioc_scoperti, int dimensione){
	int x,y;
	srand(time(NULL)*time(NULL));
	
	do{ //continui a cambiare finchè becchi una posizione gia COLPITA
		x=rand()%dimensione+0;
		y=rand()%dimensione+0;
	}while( gioc_scoperti[x][y]==1 );
	
	if(giocatore[x][y]>0){
		printf("COMPUTER COLPISCE NAVE GIOCATORE!\n");
		giocatore[x][y] = -giocatore[x][y];
	} else
		printf("COMPUTER COLPISCE NIENTE!\n");
	
	gioc_scoperti[x][y]=1;
}



// dealloca ogni variabile allocata dinamicamente
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
	
	reset(matrice, dim);
	
	return matrice;
}



// chiede all'utente di inserire le navi nella griglia di battaglia
void posizionamento_giocatore(const int dimensione, int **matrix, int *somma_matrice, int *navi) {
	reset(matrix, dimensione);
	
	printf("Inserire: %d piccole, %d medie, %d grandi, %d enormi\n",navi[0],navi[1],navi[2],navi[3]);
	for(int dimNave=1; dimNave<5; dimNave++){
		const int numNavi = navi[dimNave-1]; // Numero navi di questa dimensione da posizionare
		printf("Posizionamento delle navi da %d caselle (%d)\n", dimNave, numNavi);
		
		for(int i=0; i<numNavi;){
			stampa(dimensione, matrix, NULL, NULL);
			const int x=chiediNumero("inserire riga(numero): "),
				y=chiediLettera("inserire colonna(lettera): "),
				verso = (dimNave==1) ? 1 : chiediNumero("inserire verso(1-orizzontale , 0-verticale): ");
			int caselleLibere = check(x,y,dimensione,dimNave,verso); // 0->invalido, 1->valido
			
			// Mi assicuro che ci sia posto ANCHE NEI DINTORNI per la nave
			for(int k=0; k<dimNave; k++){
				if(verso)
					caselleLibere = caselleLibere && matrix[x][y+k]==0;
				else
					caselleLibere = caselleLibere && matrix[x+k][y]==0;
			}
			
			if(caselleLibere){
				// Le caselle necessarie alla nave sono libere, prosegui a riempirla
				puts("Nave inserita");
				for(int k=0; k<dimNave; k++){
					if(verso)
						matrix[x][y+k] = dimNave;
					else
						matrix[x+k][y] = dimNave;
				}					
				i++; // Prosegui alla nave successiva
			} else
				puts("Inserimento non valido");
		}
	}
	
	//fondamentale per il meccanismo di determinazione dell'esito, controllato per ora con meccanismo elementare
	for(int i=0; i<dimensione; i++){
		for(int k=0; k<dimensione; k++)
			*somma_matrice+=matrix[i][k];
	}
}



// Controlla che la nave inserita stia nella matrice
int check(const int x,const int y,const int dim,const int lunghezza,const int orizzontale) {
	return x>=0 && x<dim && y>=0 && y<dim && // La casella iniziale è nella matrice e...
	(lunghezza == 1 || // ...la nave occupa una sola casella o...
	(orizzontale && (y+lunghezza-1<dim)) || // ...la nave è orizzontale e la casella finale è nella matrice o...
	( ! orizzontale && (x+lunghezza-1<dim))); // ...la nave è verticale e la casella finale è nella matrice
}



/*riempie la matrice del sistema con navi posizionate casualmente ed in numero proporzionale alla dimensione
 * !!warning!! : dal punto di vista statistico questa funzione potrebbe richiedere molto tempo o addirittura instaurare un loop infinito se non si generano mai numeri che propongono posizioni ACCETTATE
 */
void posizionamento_computer(const int dimensione, int **matrix, int *somma_matrice, int *navi) {
	const int caselle=dimensione-1;
	int maxT=dimensione*dimensione, currentT; // Controllo per evitare loop infiniti
	srand(time(NULL));
	
	do {
		currentT = 0;
		reset(matrix, dimensione);
		
		for(int dimNave=1; dimNave<5 && currentT<maxT; dimNave++){ // dimNave = Caselle occupate dalla nave
			const int numNavi = navi[dimNave-1]; // Numero navi di questa dimensione da posizionare
#ifdef debug_enable
			char debugString[50];
			snprintf(debugString,49,"Posizionamento delle navi da %d caselle (%d)", dimNave, numNavi);
			debug(debugString);
#endif
			
			for(int i=0; i<numNavi && currentT<maxT; currentT++){
				const int x=rand()%caselle, // riga
				y=rand()%caselle, // colonna
				verso=rand()%1; // 0->verticale, 1->orizzontale 
				int caselleLibere = check(x,y,dimensione,dimNave,verso); // 0->invalido, 1->valido
				
				// Mi assicuro che ci sia posto ANCHE NEI DINTORNI per la nave
				for(int k=0; k<dimNave; k++){
					if(verso)
						caselleLibere = caselleLibere && matrix[x][y+k]==0;
					else
						caselleLibere = caselleLibere && matrix[x+k][y]==0;
				}
				
				if(caselleLibere){
					// Le caselle necessarie alla nave sono libere, prosegui a riempirla
					for(int k=0; k<dimNave; k++){
						if(verso)
							matrix[x][y+k] = dimNave;
						else
							matrix[x+k][y] = dimNave;
					}
					i++; // Prosegui alla nave successiva
				}
			} // Se currentT==maxT esce dal ciclo
		} // Se currentT==maxT esce dal ciclo
	} while (currentT==maxT); // Se currentT==maxT resetta il campo di gioco e ricomincia
	
	//fondamentale per il meccanismo di determinazione dell'esito, controllato per ora con meccanismo elementare
	for(int i=0; i<dimensione; i++){
		for(int k=0; k<dimensione; k++)
			*somma_matrice+=matrix[i][k];
	}
}



void reset(int **matrix, const int dimensione) {
	for(int i=0; i<dimensione; i++)
		for(int k=0; k<dimensione; k++)
			matrix[i][k]=0;
}

