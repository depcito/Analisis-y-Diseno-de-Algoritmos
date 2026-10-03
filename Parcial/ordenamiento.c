/*
 * ordenamiento.c
 * Implementación de Selection Sort y Merge Sort sobre la lista enlazada.
 * Ambos algoritmos reordenan los enlaces de los nodos; nunca copian datos
 * ni usan arreglos temporales.
 */
#include <stddef.h>
#include "ordenamiento.h"

int comparar(const Nodo *a, const Nodo *b) {
    if (a->prioridad != b->prioridad) {
        return a->prioridad - b->prioridad;
    }
    /* Desempate por ID (se evita restar para no arriesgar desbordamiento) */
    return (a->id > b->id) - (a->id < b->id);
}

/* ------------------------------------------------------------------ */
/* Fuerza bruta: Selection Sort                                        */
/* ------------------------------------------------------------------ */

void selectionSort(Lista *lista) {
    Nodo *sinOrdenar = lista->cabeza;   /* parte pendiente de ordenar */
    Nodo *ordenadaCabeza = NULL;        /* parte ya ordenada */
    Nodo *ordenadaCola = NULL;

    while (sinOrdenar != NULL) {
        /* Busca el mínimo de la parte sin ordenar y el nodo anterior a él */
        Nodo *minimo = sinOrdenar;
        Nodo *anteriorMinimo = NULL;
        Nodo *anterior = sinOrdenar;
        for (Nodo *actual = sinOrdenar->siguiente; actual != NULL;
             anterior = actual, actual = actual->siguiente) {
            if (comparar(actual, minimo) < 0) {
                minimo = actual;
                anteriorMinimo = anterior;
            }
        }

        /* Desenlaza el mínimo de la parte sin ordenar */
        if (anteriorMinimo == NULL) {
            sinOrdenar = minimo->siguiente;
        } else {
            anteriorMinimo->siguiente = minimo->siguiente;
        }
        minimo->siguiente = NULL;

        /* Lo agrega al final de la parte ordenada */
        if (ordenadaCola == NULL) {
            ordenadaCabeza = minimo;
        } else {
            ordenadaCola->siguiente = minimo;
        }
        ordenadaCola = minimo;
    }

    lista->cabeza = ordenadaCabeza;
    lista->cola = ordenadaCola;
}

/* ------------------------------------------------------------------ */
/* Dividir y conquistar: Merge Sort                                    */
/* ------------------------------------------------------------------ */

/*
 * Mezcla iterativamente dos listas ya ordenadas usando un nodo centinela.
 * Con <= se toma primero el nodo de a en caso de empate (ordenamiento estable).
 */
static Nodo *mezclar(Nodo *a, Nodo *b) {
    Nodo centinela;
    Nodo *cola = &centinela;
    centinela.siguiente = NULL;

    while (a != NULL && b != NULL) {
        if (comparar(a, b) <= 0) {
            cola->siguiente = a;
            a = a->siguiente;
        } else {
            cola->siguiente = b;
            b = b->siguiente;
        }
        cola = cola->siguiente;
    }
    cola->siguiente = (a != NULL) ? a : b;

    return centinela.siguiente;
}

/*
 * Parte la lista en dos mitades con punteros lento y rápido.
 * Corta la primera mitad y devuelve la cabeza de la segunda.
 * Requiere que la lista tenga al menos 2 nodos.
 */
static Nodo *dividir(Nodo *cabeza) {
    Nodo *lento = cabeza;
    Nodo *rapido = cabeza->siguiente;

    while (rapido != NULL && rapido->siguiente != NULL) {
        lento = lento->siguiente;
        rapido = rapido->siguiente->siguiente;
    }

    Nodo *segunda = lento->siguiente;
    lento->siguiente = NULL;
    return segunda;
}

/* Merge Sort recursivo sobre una cadena de nodos; devuelve la nueva cabeza */
static Nodo *mergeSortNodos(Nodo *cabeza) {
    if (cabeza == NULL || cabeza->siguiente == NULL) {
        return cabeza;
    }

    Nodo *segunda = dividir(cabeza);
    Nodo *izquierda = mergeSortNodos(cabeza);
    Nodo *derecha = mergeSortNodos(segunda);

    return mezclar(izquierda, derecha);
}

void mergeSort(Lista *lista) {
    lista->cabeza = mergeSortNodos(lista->cabeza);

    /* Recalcula la cola recorriendo la lista una vez */
    Nodo *ultimo = lista->cabeza;
    if (ultimo != NULL) {
        while (ultimo->siguiente != NULL) {
            ultimo = ultimo->siguiente;
        }
    }
    lista->cola = ultimo;
}