#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"

char * salloc(char* string){
	char* a = (char *)malloc((sizeof(char)*(strlen(string) + 1)));
	if(a == NULL){
	printf("Fatal error in malloc...\n");
	exit(0);
	}
	strcpy(a, string);
	return a;
}/*alloco dinamicamente una stringa identica a quella puntata da strng e ne restituisco il puntatore*/

char * strcat2(char* string1, char* string2){
	int i = strlen(string1);
	int j = strlen(string1);
	char *a = (char *)malloc(sizeof(char)*(i + j + 2));
	
	for( int ii = 0 ; ii<i ; ii++ )
		a[ii]=string1[ii];
	a[i]='/';
	for( int ii = 0 ; ii<j ; ii++ )
		a[ii+1+i]=string2[ii];
	a[i+j+1]='\0';
	return a;
}/*alloco dinamicamente una stringa composta da "string1"+'/'+"string2"+'\0'e ne restituisco il puntatore*/
