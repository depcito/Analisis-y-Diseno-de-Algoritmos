#include <stdio.h>
#include <stdlib.h>
#include "busqueda.h"

/* Busca un ID recorriendo la lista nodo por nodo.
   Complejidad: O(n). */
Nodo* busqueda_lineal(Nodo *cabeza, int id_buscado, long *comparaciones) {
    Nodo *actual = cabeza;
    long conteo = 0;

    while (actual != NULL) {
        conteo++;

        if (actual->dato.id == id_buscado) {
            if (comparaciones != NULL)
                *comparaciones = conteo;

            return actual;
        }

        actual = actual->siguiente;
    }

    if (comparaciones != NULL)
        *comparaciones = conteo;

    return NULL;
}


/* Construye un índice con los ID y punteros de cada nodo.
   Complejidad: O(n). */
EntradaIndice* construir_indice(Nodo *cabeza, int *tamano_out) {

    int n = contar_nodos(cabeza);

    EntradaIndice *indice =
        (EntradaIndice*) malloc(sizeof(EntradaIndice) * (n > 0 ? n : 1));

    if (indice == NULL) {
        fprintf(stderr,
                "Error: no se pudo asignar memoria para el indice.\n");
        exit(EXIT_FAILURE);
    }

    Nodo *actual = cabeza;
    int i = 0;

    while (actual != NULL) {
        indice[i].id = actual->dato.id;
        indice[i].puntero = actual;

        i++;
        actual = actual->siguiente;
    }

    *tamano_out = n;

    return indice;
}


/* Realiza búsqueda binaria sobre el índice.
   Requiere que los ID estén ordenados.
   Complejidad: O(log n). */
Nodo* busqueda_indexada(EntradaIndice *indice, int tamano,
                        int id_buscado, long *comparaciones) {

    int inicio = 0;
    int fin = tamano - 1;
    long conteo = 0;

    while (inicio <= fin) {

        conteo++;

        int medio = inicio + (fin - inicio) / 2;

        if (indice[medio].id == id_buscado) {

            if (comparaciones != NULL)
                *comparaciones = conteo;

            return indice[medio].puntero;

        } else if (indice[medio].id < id_buscado) {

            inicio = medio + 1;

        } else {

            fin = medio - 1;
        }
    }

    if (comparaciones != NULL)
        *comparaciones = conteo;

    return NULL;
}


/* Libera la memoria reservada para el índice auxiliar. */
void liberar_indice(EntradaIndice *indice) {
    free(indice);
}