#include <stdio.h>
#include <stdlib.h>
#include "util.h"

typedef struct struct_node{
	char *name;
	struct struct_node* next;
}node;


typedef struct struct_node *list;

void list_init(list *i){
	*i = NULL;
}

void push(list *i, char *name){
	list t = (list)(malloc(sizeof(node)));
	if(t == NULL){
		perror("Fatal error in malloc...\n");
		exit(0);
	}
	t->name = name;
	t->next = *i;
	*i = t;
}

char * pop(list *i){
	if(*i != NULL){
		char * t = salloc((*i)->name);
		list h = *i;
		*i = (*i)->next;
		free(h);
		return t;
	}
	else
		return NULL;
}






