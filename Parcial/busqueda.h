/*
 * busqueda.h
 * Búsqueda de paquetes por ID:
 *   - Búsqueda lineal directamente sobre la lista enlazada.
 *   - Solución mixta: un índice (arreglo de punteros a los nodos) ordenado
 *     por ID sobre el que se aplica búsqueda binaria.
 */
#ifndef BUSQUEDA_H
#define BUSQUEDA_H

#include "paquetes.h"

/*
 * Arreglo auxiliar de punteros a los nodos de la lista, ordenado por ID.
 * Los nodos pertenecen a la lista; el índice solo guarda punteros a ellos.
 */
typedef struct Indice {
    Nodo **nodos;
    int tamano;
} Indice;

/*
 * Recorre la lista, guarda un puntero por nodo y ordena el arreglo por ID
 * con un Merge Sort propio. Si falla la reserva de memoria devuelve un
 * índice vacío (nodos = NULL, tamano = 0).
 */
Indice construirIndice(const Lista *lista);

/*
 * Fuerza bruta: recorre la lista nodo por nodo desde el inicio.
 * Devuelve el nodo con ese ID o NULL si no existe.
 */
Nodo *busquedaLineal(const Lista *lista, int id);

/*
 * Búsqueda binaria recursiva sobre el índice ordenado por ID.
 * Devuelve el nodo con ese ID o NULL si no existe.
 */
Nodo *busquedaBinaria(const Indice *indice, int id);

/* Libera el arreglo del índice (los nodos NO se liberan: son de la lista). */
void liberarIndice(Indice *indice);

#endif /* BUSQUEDA_H */