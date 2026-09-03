#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "nodo.h"
#include "ordenamiento.h"
#include "busqueda.h"
#include "simulacion.h"

/* Lista principal de paquetes utilizada durante la ejecución del programa. */
static Nodo *lista_paquetes = NULL;


/* Limpia los caracteres restantes del buffer de entrada. */
static void limpiar_buffer_entrada(void) {
    int c;

    while ((c = getchar()) != '\n' && c != EOF) {
        /* Se descartan los caracteres restantes. */
    }
}


/* Muestra el menú principal del sistema. */
void mostrar_menu(void) {

    printf("\n=================================\n");
    printf("   SISTEMA DE PAQUETES - MENU\n");
    printf("=================================\n");
    printf("1. Generar lista aleatoria de paquetes (50,000)\n");
    printf("2. Mostrar los primeros paquetes de la lista\n");
    printf("3. Ordenar (Bubble Sort vs Merge Sort) y comparar tiempos\n");
    printf("4. Buscar paquete por ID (Lineal vs Indexada) y comparar tiempos\n");
    printf("5. Ver reporte empirico final\n");
    printf("0. Salir\n");
    printf("---------------------------------\n");
    printf("Seleccione una opcion: ");
}


/* Genera una nueva lista de 50,000 paquetes aleatorios. */
static void opcion_generar(void) {

    /* Si ya existe una lista, se libera antes de crear otra. */
    if (lista_paquetes != NULL) {
        liberar_lista(&lista_paquetes);
    }

    printf("\nGenerando 50000 paquetes aleatorios...\n");

    generar_lista_aleatoria(&lista_paquetes, 50000);

    printf("Listo. Total de nodos: %d\n",
           contar_nodos(lista_paquetes));
}


/* Muestra los primeros 20 paquetes de la lista. */
static void opcion_mostrar(void) {

    if (lista_paquetes == NULL) {
        printf("\nPrimero genere la lista (opcion 1).\n");
        return;
    }

    printf("\nMostrando los primeros 20 paquetes:\n");

    imprimir_lista(lista_paquetes, 20);
}


/* Ejecuta y compara los algoritmos de ordenamiento. */
static void opcion_ordenar(void) {

    if (lista_paquetes == NULL) {
        printf("\nPrimero genere la lista (opcion 1).\n");
        return;
    }

    medir_ordenamientos(&lista_paquetes);
}


/* Solicita un ID y ejecuta la comparación de búsquedas. */
static void opcion_buscar(void) {

    if (lista_paquetes == NULL) {
        printf("\nPrimero genere la lista (opcion 1).\n");
        return;
    }

    int id;

    printf("\nIngrese el ID de paquete a buscar: ");

    /* Verifica que el usuario haya ingresado un número válido. */
    if (scanf("%d", &id) != 1) {
        limpiar_buffer_entrada();
        printf("ID invalido.\n");
        return;
    }

    medir_busquedas(lista_paquetes, id);
}


/* Ejecuta la acción correspondiente a la opción seleccionada. */
void ejecutar_opcion(int opcion, int *continuar) {

    switch (opcion) {

        case 1:
            opcion_generar();
            break;

        case 2:
            opcion_mostrar();
            break;

        case 3:
            opcion_ordenar();
            break;

        case 4:
            opcion_buscar();
            break;

        case 5:
            mostrar_reporte_final();
            break;

        case 0:
            *continuar = 0;

            printf("\nSaliendo del programa...\n");

            /* Libera la memoria antes de finalizar. */
            liberar_lista(&lista_paquetes);
            break;

        default:
            printf("\nOpcion no valida.\n");
    }
}