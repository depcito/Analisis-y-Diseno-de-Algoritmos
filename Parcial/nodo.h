#ifndef NODO_H
#define NODO_H

/* Representa la información de cada paquete. */
typedef struct Paquete {
    int id;
    float peso;
    int prioridad; /* Valores entre 1 y 5 */
} Paquete;

/* Nodo de una lista enlazada simple. */
typedef struct Nodo {
    Paquete dato;
    struct Nodo *siguiente;
} Nodo;

/* Crea un nuevo nodo con los datos recibidos. */
Nodo* crear_nodo(int id, float peso, int prioridad);

/* Inserta un nodo al final de la lista.
   Complejidad: O(n). */
void insertar_final(Nodo **cabeza, int id, float peso, int prioridad);

/* Inserta un nodo al inicio de la lista.
   Complejidad: O(1). */
void insertar_inicio(Nodo **cabeza, int id, float peso, int prioridad);

/* Imprime los nodos de la lista hasta el límite indicado.
   Si limite <= 0, imprime toda la lista. */
void imprimir_lista(Nodo *cabeza, int limite);

/* Libera todos los nodos de la lista. */
void liberar_lista(Nodo **cabeza);

/* Cuenta la cantidad de nodos de la lista.
   Complejidad: O(n). */
int contar_nodos(Nodo *cabeza);

/* Crea una copia independiente de toda la lista.
   Complejidad: O(n). */
Nodo* copiar_lista(Nodo *cabeza);

#endif