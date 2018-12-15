#include <string.h>
#include <stdlib.h>

char * salloc(char* string){
	int i;
	for(  i = 0 ; string[i] != '\0' ; i++ ){}
	char* a = (char *)malloc(sizeof(char)*(i+1));
	if(a == NULL){
		perror("Fatal error in malloc...\n");
		exit(0);
	}
	for(;i>=0;i--){
		a[i] = string[i];
	}
	return a;
}

char * strcat2(char* strng1, char* strng2){
	char* t1 = salloc(strng1);
	char* t2 = salloc(strng2);
	strcat(t1,"/");
	strcat(t1,t2);
	return t1;
}
