/*
 * paquetes.h
 * Estructuras y funciones básicas de la lista enlazada simple de paquetes
 * del centro de distribución.
 */
#ifndef PAQUETES_H
#define PAQUETES_H

/* Un paquete: ID único, peso y prioridad (1 a 5) + enlace al siguiente nodo */
typedef struct Nodo {
    int id;
    float peso;
    int prioridad;
    struct Nodo *siguiente;
} Nodo;

/* Lista enlazada simple con puntero a la cola para insertar al final en O(1) */
typedef struct Lista {
    Nodo *cabeza;
    Nodo *cola;
    int tamano;
} Lista;

/* Devuelve una lista vacía. */
Lista crearLista(void);

/*
 * Inserta un paquete al final de la lista.
 * Recorre la lista para validar que el ID no esté repetido.
 * Devuelve 1 si insertó, 0 si falló (ID repetido o sin memoria).
 */
int insertar(Lista *lista, int id, float peso, int prioridad);

/*
 * Imprime los primeros n y los últimos n nodos de la lista.
 * Si la lista tiene 2*n nodos o menos, la imprime completa.
 */
void imprimirLista(const Lista *lista, int n);

/* Libera todos los nodos y deja la lista vacía. */
void liberarLista(Lista *lista);

#endif /* PAQUETES_H */