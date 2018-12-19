#include <string.h>
#include <stdlib.h>


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
