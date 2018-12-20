#include "list.c"

void list_init(list *i);/*Inizializza una lista a null*/

void push(list *i, char *name);/*dato un puntatore a list e un puntatore a stringa, aggiunge come primo elemento a list un node che ha come membro name la stringa passata come parametro*/

char * pop(list *i);/*dato un puntatore a list, rimuove il primo elemento, ne libera la memoria e restituisce un puntatore ad una stringa contenente il campo name del nodo prima liberato*/








