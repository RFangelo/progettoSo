#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "include/list.h"
#include <sys/types.h>
#include <dirent.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#define NCORE 2

void *thread_function(void *arg);

char* rootDir;/*directory sulla quale lavorera il processo*/

list f;/*lista file*/
list d;/*lista directory*/

unsigned int terminated_thread = 0;/*numero di thread che hanno terminato*/
unsigned int error = 0;/*numero di errori in opendir*/
unsigned int success = 0;/*numero di file aggiunti con successo alla lista*/

pthread_mutex_t terminated_thread_mutex;
pthread_mutex_t rootDir_mutex;
pthread_mutex_t f_mutex;
pthread_mutex_t d_mutex;
pthread_mutex_t error_mutex;
pthread_mutex_t success_mutex;/*vari mutex per la sincronizzazione delle variabili globali*/

pthread_t th[NCORE];

int main(int argc, char *argv[]){
	//si inizializzano liste e mutex e si passa argv a rootDir
	rootDir = argv[1];
	list_init(&d);
	list_init(&f);
	int res = pthread_mutex_init(&f_mutex, NULL);
	if (res != 0){
		printf("errore nell'inizializzazione del mutex");
		exit(1);
		}
	res = pthread_mutex_init(&d_mutex, NULL);
	if (res != 0){
		perror("errore nell'inizializzazione del mutex");
		exit(1);
		}
	res = pthread_mutex_init(&terminated_thread_mutex, NULL);
	if (res != 0){
		printf("errore nell'inizializzazione del mutex");
		exit(1);
		}
	res = pthread_mutex_init(&rootDir_mutex, NULL);
	if (res != 0){
		perror("errore nell'inizializzazione del mutex");
		exit(1);
		}
	res = pthread_mutex_init(&success_mutex, NULL);
	if (res != 0){
		printf("errore nell'inizializzazione del mutex");
		exit(1);
		}
	res = pthread_mutex_init(&error_mutex, NULL);
	if (res != 0){
		perror("errore nell'inizializzazione del mutex");
		exit(1);
		}
	DIR* A = opendir(rootDir);/*Apro rootDir*/
	if(A == NULL){
		printf("path name non valido...\n");
		exit(2);
	}
	for( struct dirent *i = readdir(A) ; i != NULL ; i = readdir(A) ){
		switch(i->d_type){
			case DT_DIR:
				if( strcmp(i->d_name,"..")!=0 && strcmp(i->d_name,".")!=0 )
					push(&d,strcat2(argv[1],i->d_name));
				break;
			case DT_REG:
				push(&f,strcat2(argv[1],i->d_name));
				success++;/*incremeto il numero di operazoni eseguite con successo*/
				break;
			default:
				break;
		}
	}/*leggo e smisto i file in f e le directory in d finche readdir non incontra EOF restituendo NULL*/
	if(closedir(A) == -1){
		printf("errore nella chiusura di una directory\n");
		exit(3);	
	}
	
	for(int i = 0 ; i < NCORE ; i++){
		res=pthread_create(&th[i],NULL,thread_function,NULL);
		if (res != 0){
			printf("thread creazione fallita");
			exit(4);
		}
	}/*creo NCORE thread*/
	for(;;){
		pthread_mutex_lock(&f_mutex);
		char* t = pop(&f);
		pthread_mutex_unlock(&f_mutex);
		pthread_mutex_lock(&terminated_thread_mutex);
		if(t == NULL && terminated_thread == NCORE){
			pthread_mutex_unlock(&terminated_thread_mutex);
			pthread_mutex_destroy(&d_mutex);
			pthread_mutex_destroy(&f_mutex);
			pthread_mutex_destroy(&rootDir_mutex);
			pthread_mutex_destroy(&terminated_thread_mutex);
			pthread_mutex_destroy(&error_mutex);
			pthread_mutex_destroy(&success_mutex);
			printf("\t\t-\t\t-\t\terror:%d\t\tsuccess:%d\t\t-\t\t-\t\t\n",error,success);
			exit(0);
			}/*se pop di f ritorna NULL significa che la lista è vuota, inoltre se terminated_thread == NCORE significa che abbiamo finito e possiamo procedere alla distruzione dei mutex ed alla stampa a video di un resoconto*/
		pthread_mutex_unlock(&terminated_thread_mutex);
		if(t != NULL)
			printf("%s\n",t);
		free(t);
	}
}/*stampo elementi di f e rimango in attesa se è vuota, termino se <VEDI SOPRA>*/

void *thread_function(void *arg){
	pthread_mutex_lock(&d_mutex);
	char *a = pop(&d);/*leggo una directory in attesa di essere esplorata*/
	pthread_mutex_unlock(&d_mutex);
	if(a != NULL){
		for( DIR*A = opendir(a) ; a != NULL ;){
			if(A != NULL){
				for( struct dirent *i = readdir(A) ; i != NULL ; i = readdir(A) ){
					switch(i->d_type){
						case DT_DIR:
							pthread_mutex_lock(&d_mutex);
							if( strcmp(i->d_name,"..")!=0 && strcmp(i->d_name,".")!=0 ){
								push(&d,strcat2(a,i->d_name));
								}
							pthread_mutex_unlock(&d_mutex);
							break;
						case DT_REG:
							pthread_mutex_lock(&f_mutex);
							push(&f,strcat2(a,i->d_name));
							pthread_mutex_unlock(&f_mutex);
							pthread_mutex_lock(&success_mutex);
							success++;/*incremeto il numero di operazoni eseguite con successo*/
							pthread_mutex_unlock(&success_mutex);
							break;
						default:
							break;
					}	
				}/*leggo e smisto i file in f e le directory in d finche readdir non incontra EOF restituendo NULL*/
				if(closedir(A) == -1){
					printf("errore nella chiusura di una directory\n");
					exit(3);
				}
			}
			else{
				pthread_mutex_lock(&error_mutex);
				error++;/*incremeto il numero di directory aperte senza successo*/
				pthread_mutex_unlock(&error_mutex);				
			}
			pthread_mutex_lock(&d_mutex);
			a = pop(&d);/*una volta usciti dal ciclo for più interno leggo un altra directory in attesa di essere esplorata */
			pthread_mutex_unlock(&d_mutex);
			if(a != NULL)
				 A = opendir(a);
		}/*continuo a scansionare directory finche pop non riceve null, questo perchè la lista è vuota se nell'ultimo ciclo i vari thread non hanno aggiunto directory, ciò implica che abbiamo raggiunto le estremità dell'albero...*/
	}
	pthread_mutex_lock(&terminated_thread_mutex);
	terminated_thread++;
	pthread_mutex_unlock(&terminated_thread_mutex);
}





















