#include <stdio.h>
#include <stdlib.h>
#include "nodo.h"

/* Crea un nodo y reserva memoria dinámicamente. */
Nodo* crear_nodo(int id, float peso, int prioridad) {

    Nodo *nuevo = (Nodo*) malloc(sizeof(Nodo));

    if (nuevo == NULL) {
        fprintf(stderr,
                "Error: no se pudo asignar memoria para el nodo.\n");
        exit(EXIT_FAILURE);
    }

    nuevo->dato.id = id;
    nuevo->dato.peso = peso;
    nuevo->dato.prioridad = prioridad;
    nuevo->siguiente = NULL;

    return nuevo;
}


/* Inserta un nuevo nodo al inicio de la lista. */
void insertar_inicio(Nodo **cabeza, int id, float peso, int prioridad) {

    Nodo *nuevo = crear_nodo(id, peso, prioridad);

    nuevo->siguiente = *cabeza;
    *cabeza = nuevo;
}


/* Inserta un nuevo nodo al final de la lista. */
void insertar_final(Nodo **cabeza, int id, float peso, int prioridad) {

    Nodo *nuevo = crear_nodo(id, peso, prioridad);

    /* Si la lista está vacía, el nuevo nodo será la cabeza. */
    if (*cabeza == NULL) {
        *cabeza = nuevo;
        return;
    }

    Nodo *actual = *cabeza;

    /* Se avanza hasta encontrar el último nodo. */
    while (actual->siguiente != NULL) {
        actual = actual->siguiente;
    }

    actual->siguiente = nuevo;
}


/* Imprime los datos de los nodos de la lista. */
void imprimir_lista(Nodo *cabeza, int limite) {

    Nodo *actual = cabeza;
    int contador = 0;

    printf("%-10s %-10s %-10s\n",
           "ID", "Peso", "Prioridad");

    printf("----------------------------------\n");

    while (actual != NULL &&
          (limite <= 0 || contador < limite)) {

        printf("%-10d %-10.2f %-10d\n",
               actual->dato.id,
               actual->dato.peso,
               actual->dato.prioridad);

        actual = actual->siguiente;
        contador++;
    }

    /* Indica si quedaron nodos sin mostrar. */
    if (limite > 0 && actual != NULL) {
        printf("... (%d nodos mas, lista truncada para impresion)\n",
               contar_nodos(cabeza) - limite);
    }
}


/* Libera toda la memoria utilizada por la lista. */
void liberar_lista(Nodo **cabeza) {

    Nodo *actual = *cabeza;
    Nodo *siguiente;

    while (actual != NULL) {

        /* Se guarda el siguiente nodo antes de liberar el actual. */
        siguiente = actual->siguiente;

        free(actual);

        actual = siguiente;
    }

    /* La lista queda vacía. */
    *cabeza = NULL;
}


/* Cuenta todos los nodos presentes en la lista. */
int contar_nodos(Nodo *cabeza) {

    int contador = 0;
    Nodo *actual = cabeza;

    while (actual != NULL) {
        contador++;
        actual = actual->siguiente;
    }

    return contador;
}


/* Crea una copia completa e independiente de la lista original. */
Nodo* copiar_lista(Nodo *cabeza) {

    Nodo *nueva_cabeza = NULL;
    Nodo *cola = NULL;
    Nodo *actual = cabeza;

    while (actual != NULL) {

        Nodo *nuevo = crear_nodo(
            actual->dato.id,
            actual->dato.peso,
            actual->dato.prioridad
        );

        /* Primer nodo de la nueva lista. */
        if (nueva_cabeza == NULL) {
            nueva_cabeza = nuevo;
            cola = nuevo;
        } else {

            /* Los siguientes nodos se agregan al final. */
            cola->siguiente = nuevo;
            cola = nuevo;
        }

        actual = actual->siguiente;
    }

    return nueva_cabeza;
}