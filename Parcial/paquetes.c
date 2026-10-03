/*
 * paquetes.c
 * Implementación de la lista enlazada simple de paquetes.
 */
#include <stdio.h>
#include <stdlib.h>
#include "paquetes.h"

Lista crearLista(void) {
    Lista lista;
    lista.cabeza = NULL;
    lista.cola = NULL;
    lista.tamano = 0;
    return lista;
}

int insertar(Lista *lista, int id, float peso, int prioridad) {
    /* Validación: el ID debe ser único (recorre la lista completa) */
    for (Nodo *actual = lista->cabeza; actual != NULL; actual = actual->siguiente) {
        if (actual->id == id) {
            return 0;
        }
    }

    Nodo *nuevo = (Nodo *)malloc(sizeof(Nodo));
    if (nuevo == NULL) {
        return 0;
    }
    nuevo->id = id;
    nuevo->peso = peso;
    nuevo->prioridad = prioridad;
    nuevo->siguiente = NULL;

    /* Inserción al final usando el puntero a la cola (O(1)) */
    if (lista->cola == NULL) {
        lista->cabeza = nuevo;
    } else {
        lista->cola->siguiente = nuevo;
    }
    lista->cola = nuevo;
    lista->tamano++;
    return 1;
}

/* Imprime un nodo con formato de una línea */
static void imprimirNodo(const Nodo *nodo) {
    printf("  ID: %-8d Peso: %8.2f kg  Prioridad: %d\n",
           nodo->id, nodo->peso, nodo->prioridad);
}

void imprimirLista(const Lista *lista, int n) {
    if (lista->cabeza == NULL) {
        printf("  (lista vacía)\n");
        return;
    }

    /* Lista corta: se imprime completa */
    if (lista->tamano <= 2 * n) {
        for (const Nodo *actual = lista->cabeza; actual != NULL; actual = actual->siguiente) {
            imprimirNodo(actual);
        }
        return;
    }

    /* Primeros n nodos */
    const Nodo *actual = lista->cabeza;
    for (int i = 0; i < n; i++) {
        imprimirNodo(actual);
        actual = actual->siguiente;
    }

    printf("  ... (%d nodos omitidos) ...\n", lista->tamano - 2 * n);

    /* Avanza hasta la posición tamano - n para imprimir los últimos n nodos */
    for (int i = n; i < lista->tamano - n; i++) {
        actual = actual->siguiente;
    }
    for (int i = 0; i < n; i++) {
        imprimirNodo(actual);
        actual = actual->siguiente;
    }
}

void liberarLista(Lista *lista) {
    Nodo *actual = lista->cabeza;
    while (actual != NULL) {
        Nodo *siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
    lista->cabeza = NULL;
    lista->cola = NULL;
    lista->tamano = 0;
}