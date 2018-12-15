#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "include/list.h"
#include <sys/types.h>
#include <dirent.h>
#include <pthread.h>
#include <string.h>
#define NCORE 2

void *thread_function(void *arg);

char* rootDir;

list f;
list d;

unsigned int terminated_thread = 0;

pthread_mutex_t terminated_thread_mutex;
pthread_mutex_t rootDir_mutex;
pthread_mutex_t f_mutex;
pthread_mutex_t d_mutex;

pthread_t th[NCORE];

int main(int argc, char *argv[]){
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
	DIR* A = opendir(rootDir);
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
				break;
			default:
				break;
		}
	}
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
	}
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
			exit(0);
			}
		pthread_mutex_unlock(&terminated_thread_mutex);
		if(t != NULL)
			printf("%s\n",t);
	}
}

void *thread_function(void *arg){
	pthread_mutex_lock(&d_mutex);
	char *a = pop(&d);
	pthread_mutex_unlock(&d_mutex);
	if(a != NULL){
		for( DIR*A = opendir(a) ; a != NULL ;){
			if(A == NULL){
				printf("path name non valido...\n");
				exit(2);
			}
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
						break;
					default:
						break;
				}	
			}
			if(closedir(A) == -1){
				printf("errore nella chiusura di una directory\n");
				exit(3);
			}
			pthread_mutex_lock(&d_mutex);
			a = pop(&d);
			pthread_mutex_unlock(&d_mutex);
			if(a != NULL)
				 A = opendir(a);
		}
	}
	pthread_mutex_lock(&terminated_thread_mutex);
	terminated_thread++;
	pthread_mutex_unlock(&terminated_thread_mutex);
}





















