/*
 * busqueda.c
 * Búsqueda lineal sobre la lista y búsqueda binaria sobre un índice
 * (arreglo de punteros a nodos) ordenado por ID.
 */
#include <stdlib.h>
#include "busqueda.h"

/* ------------------------------------------------------------------ */
/* Merge Sort del arreglo de punteros (ordena por ID ascendente)       */
/* ------------------------------------------------------------------ */

/* Mezcla las mitades ordenadas a[izq..medio] y a[medio+1..der] usando tmp */
static void mezclarIndice(Nodo **a, Nodo **tmp, int izq, int medio, int der) {
    int i = izq;
    int j = medio + 1;
    int k = izq;

    while (i <= medio && j <= der) {
        if (a[i]->id <= a[j]->id) {
            tmp[k++] = a[i++];
        } else {
            tmp[k++] = a[j++];
        }
    }
    while (i <= medio) {
        tmp[k++] = a[i++];
    }
    while (j <= der) {
        tmp[k++] = a[j++];
    }
    for (k = izq; k <= der; k++) {
        a[k] = tmp[k];
    }
}

/* Merge Sort recursivo sobre el rango [izq, der] (la profundidad es log2 n) */
static void mergeSortIndice(Nodo **a, Nodo **tmp, int izq, int der) {
    if (izq >= der) {
        return;
    }
    int medio = izq + (der - izq) / 2;
    mergeSortIndice(a, tmp, izq, medio);
    mergeSortIndice(a, tmp, medio + 1, der);
    mezclarIndice(a, tmp, izq, medio, der);
}

/* ------------------------------------------------------------------ */
/* Índice                                                              */
/* ------------------------------------------------------------------ */

Indice construirIndice(const Lista *lista) {
    Indice indice;
    indice.nodos = NULL;
    indice.tamano = 0;

    if (lista->tamano <= 0) {
        return indice;
    }

    Nodo **nodos = (Nodo **)malloc((size_t)lista->tamano * sizeof(Nodo *));
    Nodo **tmp = (Nodo **)malloc((size_t)lista->tamano * sizeof(Nodo *));
    if (nodos == NULL || tmp == NULL) {
        free(nodos);
        free(tmp);
        return indice;   /* índice vacío si falla la memoria */
    }

    /* Un puntero por cada nodo de la lista */
    int n = 0;
    for (Nodo *actual = lista->cabeza; actual != NULL; actual = actual->siguiente) {
        nodos[n++] = actual;
    }

    mergeSortIndice(nodos, tmp, 0, n - 1);
    free(tmp);

    indice.nodos = nodos;
    indice.tamano = n;
    return indice;
}

void liberarIndice(Indice *indice) {
    free(indice->nodos);
    indice->nodos = NULL;
    indice->tamano = 0;
}

/* ------------------------------------------------------------------ */
/* Búsquedas                                                           */
/* ------------------------------------------------------------------ */

Nodo *busquedaLineal(const Lista *lista, int id) {
    for (Nodo *actual = lista->cabeza; actual != NULL; actual = actual->siguiente) {
        if (actual->id == id) {
            return actual;
        }
    }
    return NULL;
}

/* Búsqueda binaria recursiva sobre nodos[bajo..alto] */
static Nodo *binariaRec(Nodo **nodos, int bajo, int alto, int id) {
    if (bajo > alto) {
        return NULL;
    }
    int medio = bajo + (alto - bajo) / 2;

    if (nodos[medio]->id == id) {
        return nodos[medio];
    }
    if (id < nodos[medio]->id) {
        return binariaRec(nodos, bajo, medio - 1, id);
    }
    return binariaRec(nodos, medio + 1, alto, id);
}

Nodo *busquedaBinaria(const Indice *indice, int id) {
    if (indice->nodos == NULL || indice->tamano <= 0) {
        return NULL;
    }
    return binariaRec(indice->nodos, 0, indice->tamano - 1, id);
}