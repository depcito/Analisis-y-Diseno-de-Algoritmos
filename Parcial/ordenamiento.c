#include <stdio.h>
#include <stdlib.h>
#include "ordenamiento.h"
#include "nodo.h"

/* ---------- Fuerza bruta: Bubble Sort ---------- */
static void intercambiar_datos(Nodo *a, Nodo *b) {
    Paquete temp = a->dato;
    a->dato = b->dato;
    b->dato = temp;
}

void bubble_sort_lista(Nodo *cabeza) {
    if (cabeza == NULL) return;

    int huboIntercambio;
    Nodo *actual;
    Nodo *ultimo = NULL; /* frontera del segmento ya ordenado */

    do {
        huboIntercambio = 0;
        actual = cabeza;
        while (actual->siguiente != ultimo) {
            if (actual->dato.id > actual->siguiente->dato.id) {
                intercambiar_datos(actual, actual->siguiente);
                huboIntercambio = 1;
            }
            actual = actual->siguiente;
        }
        ultimo = actual;
    } while (huboIntercambio);
}

/* ---------- Divide y venceras: Merge Sort ---------- */
static Nodo* dividir_lista(Nodo *cabeza) {
    /* Tecnica de punteros lento/rapido para hallar el punto medio */
    if (cabeza == NULL || cabeza->siguiente == NULL) return NULL;

    Nodo *lento = cabeza;
    Nodo *rapido = cabeza->siguiente;

    while (rapido != NULL && rapido->siguiente != NULL) {
        lento = lento->siguiente;
        rapido = rapido->siguiente->siguiente;
    }

    Nodo *mitad = lento->siguiente;
    lento->siguiente = NULL; /* corta la primera mitad */
    return mitad;
}

static Nodo* mezclar(Nodo *izq, Nodo *der) {
    if (izq == NULL) return der;
    if (der == NULL) return izq;

    Nodo *resultado;
    if (izq->dato.id <= der->dato.id) {
        resultado = izq;
        resultado->siguiente = mezclar(izq->siguiente, der);
    } else {
        resultado = der;
        resultado->siguiente = mezclar(izq, der->siguiente);
    }
    return resultado;
}

Nodo* merge_sort_lista(Nodo *cabeza) {
    if (cabeza == NULL || cabeza->siguiente == NULL) {
        return cabeza;
    }

    Nodo *mitad = dividir_lista(cabeza);

    Nodo *izq = merge_sort_lista(cabeza);
    Nodo *der = merge_sort_lista(mitad);

    return mezclar(izq, der);
}
