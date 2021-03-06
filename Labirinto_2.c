#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct nodo
	{
		char *parola;
		struct nodo *next;
		struct nodo *prev;
	};
	
//calcola la distanza di levenshtein

int distanza(char *x, char *y)
	{
    int m=strlen(x);
    int n=strlen(y);
    register int i,j;
    int distance;
    int **d= (int**)malloc((m+1)*sizeof(int));
    for(i=0;i<=m;i++)
        d[i]=(int*)malloc((n+1)*sizeof(int)); 
    for(i=0;i<=m;i++)
        d[i][0]=i;
    for(j=1;j<=n;j++)
        d[0][j]=j;
    for(i=1;i<=m;i++){
        for(j=1;j<=n;j++){
            if(x[i-1]!=y[j-1]){
                int k=minimum(d[i][j-1],d[i-1][j],d[i-1][j-1]);
                d[i][j]=k+1;
            }else{
                d[i][j]=d[i-1][j-1];
            }
        }
    }
    distance=d[m][n];
    for(i=0;i<=m;i++)
        free(d[i]);
    free(d);
    return distance;
}

 
int minimum(int a,int b,int c) /* funzione che calcola il minimo di 3 valori */
{
	int min=a;
 	if(b<min)	min=b;
 	if(c<min)	min=c;
	return min;
}

/////////FUNZIONI CHE OPERANO SULLE LISTE/////////////

void append_am(struct nodo *lista,char *amico)
{
	struct nodo *i;
	for (i=lista;i->next!=NULL;i=i->next);
	i->next=malloc(sizeof(struct nodo));
	(i->next)->parola=amico;
	(i->next)->next=NULL;
	(i->next)->prev=lista;
	return;
}

void print(struct nodo *lista)
{
	struct nodo *i;
	for (i=lista;i->prev!=NULL;i=i->prev);
	for (i=lista;i!=NULL;i=i->next)
	{
        printf ("%s ,",i->parola);
	}
    printf ("\n");
}

////////////////////FUNZIONI DI CONTROLLO DELLA COMPATIBILITA' TRA PAROLE////////////////////////

int anagr(char *a, char *b)/*prende in ingresso due stringhe e dice se una è anagramma dell'altra*/
{
	int c=strlen(a);
	int d=strlen(b);
	int i,j,counter;
	if(c!=d) return 0;
	for(i=0;i<c;i++)
	    {
	    counter=0;
	    for(j=0;j<d;j++)
	        {
	        if(a[i]==b[j])
			{
				b[j]=27;
				j=d;
				counter++;
			} 
			
	        }
	    if(counter==0) return 0;
	    }
	return 1;
}

int carCamb(char *a, char *b) /*prende in ingresso due stringhe e dice se differiscono per un singolo carattere*/
{
    int c=strlen(a);
    int d=strlen(b);
    int i;
    int j=0;
    if (c!=d)
        return 0;
    for (i=0; i<c; i++)
    {
        if (a[i]!=b[i])
            j++;
    }
    if (j!=1)
        return 0;
    return 1;
}

int aggCar(char *a, char *b)/*dice se una stringa differisce da un'altra perchè ha un carattere in più*/
{
	int c=strlen(a);
	int d=strlen(b);
	int i;
	if(c!=d-1) return 0;
	for(i=0;i<c;i++)
	{
	    if(a[i]!=b[i])
	    {
	        for(;i<c;i++)
	        {
	        	if(a[i]!=b[i+1]) return 0;
	        }    
	        return 1;
	    }
	}
	return 1;
}

int verifica(char *a, char *b) /*verifica che almeno una delle regole necessarie siano rispettate*/
{
	if(anagr(a,strdup(b))==1 || carCamb(a,b)==1 || aggCar(a,b)==1 || aggCar(b,a)==1 ) return 1;
	return 0;
}

int conta()//carica le parole dal file dizionario.txt in un array
{
	char *buffer;
	int i=0;
	FILE *f;
	buffer=malloc(sizeof(char)*50);
	f=fopen("dizionario.txt","r");
	while(!feof(f))
	{
		fscanf(f,"%s",buffer);
		i++;
	}
	fclose(f);
	return i;
}


void carica(char **dizionario, int *flag)//carica le parole dal file dizionario.txt in un array
{
	char *buffer;
	int i=0;
	FILE *f;
	buffer=malloc(sizeof(char)*50);
	f=fopen("dizionario.txt","r");
	while(!feof(f))
	{
		fscanf(f,"%s",buffer);
		dizionario[i]=strdup(buffer);
		flag[i]=0;
		i++;
	}
	fclose(f);
}
//amici ritorna l'indice della parola compatibile con l'ultima parola aggiunta, che ha minore
//distanza dalla parola finale. L'array flag viene utilizzato per far si che quando il
//programma non trova soluzioni torna indietro di una parola, evitando di ripercorrere lo stesso
//percorso.
int amici(char **dizionario, char *parola, int *flag, int diz, char* fine)
{
	int disfine=0;
	int dis=100;
	int i=0,ris=-1;
	for(i=diz-1;i>=0;i--)
	{
		if((strcmp(parola,dizionario[i])!=0)&&(verifica(parola,dizionario[i]))&&!flag[i])
		{
			disfine=distanza(dizionario[i],fine);
			if(disfine<dis)
			{
				ris=i;
				dis=disfine;
			}
		}
	}
	return ris;
}

int soluzione(char *fine, char **dizionario, int diz, int *flag, struct nodo *lista)
{
	int i;
	struct nodo *testa;
	testa=lista;
	while(!verifica(lista->parola,fine))
	{
		i=amici(dizionario,lista->parola,flag,diz,fine);
		while(i<0)
		{	
			if(lista->prev==NULL)
			{
				return 0;
			}
			lista=lista->prev;
			i=amici(dizionario,lista->parola,flag,diz,fine);
		}
		flag[i]=1;
		lista->next=NULL;
		append_am(lista,dizionario[i]);
		lista=lista->next;		
	}
	return 1;
}

void cls()
{
	int i=0;
	for(i=0;i<5;i++)	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

int main()
{
	char *inizio=malloc(sizeof(char)*50);
	char *fine=malloc(sizeof(char)*50);
	struct nodo *lista;
	int diz=conta();
	char **dizionario=(malloc(sizeof(char*)*diz));
	int *flag=malloc(sizeof(int)*diz);
	int esci=0;
	carica(dizionario,flag);
	lista->parola=inizio;
	lista->prev=NULL;
	lista->next=NULL;
	puts("BENVENUTO!");
	while(esci!=1)
	{
		cls();
		puts("INSERISCI LA PAROLA DI INIZIO! (MASSIMO 50 CARATTERI)\n");
		scanf("%s",inizio);
		puts("\n");
		puts("INSERISCI ORA LA PAROLA DI FINE\n");
		scanf("%s",fine);
		puts("\n");
		puts("sto elaborando...\n");
		if(soluzione(fine,dizionario,diz,flag,lista)==0)
		{
			puts("non c'e' soluzione\n");		
			puts("Digita 1 e premi invio per uscire, 0 per continuare\n");
			scanf("%i",&esci);
		}
		else
		{
			append_am(lista,fine);	
			print(lista);
			puts("\nDigita 1 e premi invio per uscire, 0 per continuare\n");
			scanf("%i",&esci);
		}
	}
}
