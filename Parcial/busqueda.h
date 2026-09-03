#ifndef BUSQUEDA_H
#define BUSQUEDA_H

#include "nodo.h"

/* Búsqueda lineal de un nodo por ID.
   Guarda la cantidad de comparaciones realizadas. */
Nodo* busqueda_lineal(Nodo *cabeza, int id_buscado, long *comparaciones);

/* Estructura utilizada para crear un índice auxiliar.
   Guarda el ID y un puntero al nodo correspondiente. */
typedef struct {
    int id;
    Nodo *puntero;
} EntradaIndice;

/* Construye un arreglo auxiliar a partir de la lista.
   La lista debe estar ordenada por ID para usar búsqueda binaria. */
EntradaIndice* construir_indice(Nodo *cabeza, int *tamano_out);

/* Realiza búsqueda binaria sobre el índice auxiliar. */
Nodo* busqueda_indexada(EntradaIndice *indice, int tamano,
                        int id_buscado, long *comparaciones);

/* Libera la memoria utilizada por el índice. */
void liberar_indice(EntradaIndice *indice);

#endif