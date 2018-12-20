#include "util.c"

<<<<<<< HEAD
char * salloc(char* string);/*alloco dinamicamente una stringa identica a quella puntata da strng e ne restituisco il puntatore*/

char * strcat2(char* string1, char* string2);/*alloco dinamicamente una stringa composta da "string1"+'/'+"string2"+'\0'e ne restituisco il puntatore*/
=======

char * salloc(char* string){
	char* a = (char *)malloc((sizeof(char)*(strlen(string) + 1)));
	if(a == NULL){
		perror("Fatal error in malloc...\n");
		exit(0);
	}
	strcpy(a, string);
	return a;
}

char * strcat2(char* strng1, char* strng2){

	char tmp[(strlen(strng1) + strlen(strng2) + 2)];
	sprintf(tmp, "%s/%s", strng1, strng2);
	char * t = salloc(tmp);

	return t;
}
>>>>>>> eb3eec48be3f6f04d5667d7106210c7b34681900
