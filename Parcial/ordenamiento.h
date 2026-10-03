/*
 * ordenamiento.h
 * Algoritmos de ordenamiento aplicados directamente sobre la lista enlazada.
 * Criterio: prioridad ascendente y, si empatan, ID ascendente.
 */
#ifndef ORDENAMIENTO_H
#define ORDENAMIENTO_H

#include "paquetes.h"

/*
 * Compara dos nodos según el criterio de ordenamiento.
 * Devuelve un valor negativo si a va antes que b, positivo si va después
 * y 0 si son equivalentes.
 */
int comparar(const Nodo *a, const Nodo *b);

/*
 * Fuerza bruta: Selection Sort.
 * Extrae en cada pasada el nodo mínimo de la parte sin ordenar y lo agrega
 * al final de la lista ordenada, reenlazando nodos (sin copiar datos).
 * Al terminar deja cabeza, cola y tamano correctos.
 */
void selectionSort(Lista *lista);

/*
 * Dividir y conquistar: Merge Sort adaptado a listas enlazadas.
 * Divide con punteros lento/rápido, ordena cada mitad recursivamente y
 * mezcla de forma iterativa con un nodo centinela (sin arreglos temporales).
 * Al terminar deja cabeza, cola y tamano correctos.
 */
void mergeSort(Lista *lista);

#endif /* ORDENAMIENTO_H */