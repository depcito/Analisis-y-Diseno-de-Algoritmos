#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "simulacion.h"
#include "ordenamiento.h"
#include "busqueda.h"

/* Guarda los resultados de la ultima simulacion para el reporte final */
static double tiempo_bubble_ms = -1.0;
static double tiempo_merge_ms = -1.0;
static double tiempo_busqueda_lineal_ms = -1.0;
static double tiempo_busqueda_indexada_ms = -1.0;
static long comparaciones_lineal = 0;
static long comparaciones_indexada = 0;
static int hubo_ordenamiento = 0;
static int hubo_busqueda = 0;

void generar_lista_aleatoria(Nodo **cabeza, int cantidad) {
    srand((unsigned int) time(NULL));
    for (int i = 0; i < cantidad; i++) {
        int id = i + 1; /* IDs unicos garantizados */
        float peso = (float) (rand() % 5000) / 100.0f;   /* 0.00 - 49.99 */
        int prioridad = (rand() % 5) + 1;                /* 1 - 5 */
        insertar_inicio(cabeza, id, peso, prioridad);
    }
    /* Se desordena el orden de llegada mezclando el atributo 'peso'/'prioridad'
       no es necesario: los ids ya llegan "desordenados" respecto al orden final
       requerido porque se insertan por inicio; para forzar mayor desorden en
       el propio id (simulando llegada real desordenada) se barajan los IDs. */
    int n = contar_nodos(*cabeza);
    int *ids = (int*) malloc(sizeof(int) * n);
    Nodo *actual = *cabeza;
    int i = 0;
    while (actual != NULL) {
        ids[i++] = actual->dato.id;
        actual = actual->siguiente;
    }
    /* Fisher-Yates shuffle sobre el arreglo de ids */
    for (i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = ids[i];
        ids[i] = ids[j];
        ids[j] = tmp;
    }
    actual = *cabeza;
    i = 0;
    while (actual != NULL) {
        actual->dato.id = ids[i++];
        actual = actual->siguiente;
    }
    free(ids);
}

void medir_ordenamientos(Nodo **cabeza) {
    /* Se trabaja sobre copias independientes para que ambos algoritmos
       partan exactamente de los mismos datos desordenados */
    Nodo *copia_bubble = copiar_lista(*cabeza);
    Nodo *copia_merge = copiar_lista(*cabeza);

    clock_t inicio, fin;

    printf("\nOrdenando con Bubble Sort (fuerza bruta)...\n");
    inicio = clock();
    bubble_sort_lista(copia_bubble);
    fin = clock();
    tiempo_bubble_ms = (double)(fin - inicio) * 1000.0 / CLOCKS_PER_SEC;
    printf("Bubble Sort: %.2f ms\n", tiempo_bubble_ms);

    printf("\nOrdenando con Merge Sort (divide y venceras)...\n");
    inicio = clock();
    copia_merge = merge_sort_lista(copia_merge);
    fin = clock();
    tiempo_merge_ms = (double)(fin - inicio) * 1000.0 / CLOCKS_PER_SEC;
    printf("Merge Sort: %.2f ms\n", tiempo_merge_ms);

    /* La lista principal queda ordenada por Merge Sort (la version rapida)
       para poder construir el indice usado en la busqueda indexada */
    liberar_lista(cabeza);
    *cabeza = copia_merge;
    liberar_lista(&copia_bubble);

    hubo_ordenamiento = 1;
    printf("\nResultado: %s fue mas rapido en esta ejecucion.\n",
           tiempo_merge_ms <= tiempo_bubble_ms ? "Merge Sort" : "Bubble Sort");
}

void medir_busquedas(Nodo *cabeza, int id_buscado) {
    clock_t inicio, fin;

    printf("\nBusqueda Lineal (fuerza bruta) del ID %d...\n", id_buscado);
    inicio = clock();
    Nodo *resultado_lineal = busqueda_lineal(cabeza, id_buscado, &comparaciones_lineal);
    fin = clock();
    tiempo_busqueda_lineal_ms = (double)(fin - inicio) * 1000.0 / CLOCKS_PER_SEC;
    printf("Resultado: %s | Comparaciones: %ld | Tiempo: %.4f ms\n",
           resultado_lineal ? "ENCONTRADO" : "NO ENCONTRADO",
           comparaciones_lineal, tiempo_busqueda_lineal_ms);

    printf("\nBusqueda Indexada/mixta (indice + binaria) del ID %d...\n", id_buscado);
    printf("Nota: requiere que la lista este ordenada (use la opcion 3 primero).\n");
    int tamano;
    inicio = clock();
    EntradaIndice *indice = construir_indice(cabeza, &tamano);
    Nodo *resultado_indexado = busqueda_indexada(indice, tamano, id_buscado, &comparaciones_indexada);
    fin = clock();
    tiempo_busqueda_indexada_ms = (double)(fin - inicio) * 1000.0 / CLOCKS_PER_SEC;
    printf("Resultado: %s | Comparaciones: %ld | Tiempo: %.4f ms\n",
           resultado_indexado ? "ENCONTRADO" : "NO ENCONTRADO",
           comparaciones_indexada, tiempo_busqueda_indexada_ms);
    liberar_indice(indice);

    hubo_busqueda = 1;
    printf("\nResultado: %s fue mas rapida y con menos comparaciones en esta ejecucion.\n",
           tiempo_busqueda_indexada_ms <= tiempo_busqueda_lineal_ms ? "la busqueda indexada" : "la busqueda lineal");
}

void mostrar_reporte_final(void) {
    printf("\n========= REPORTE EMPIRICO FINAL =========\n");
    if (!hubo_ordenamiento) {
        printf("Ordenamiento: aun no se ha ejecutado (opcion 3).\n");
    } else {
        printf("Bubble Sort : %.2f ms\n", tiempo_bubble_ms);
        printf("Merge Sort  : %.2f ms\n", tiempo_merge_ms);
        printf("-> Mas rapido: %s\n", tiempo_merge_ms <= tiempo_bubble_ms ? "Merge Sort" : "Bubble Sort");
    }
    printf("-------------------------------------------\n");
    if (!hubo_busqueda) {
        printf("Busqueda: aun no se ha ejecutado (opcion 4).\n");
    } else {
        printf("Busqueda Lineal   : %.4f ms | %ld comparaciones\n", tiempo_busqueda_lineal_ms, comparaciones_lineal);
        printf("Busqueda Indexada : %.4f ms | %ld comparaciones\n", tiempo_busqueda_indexada_ms, comparaciones_indexada);
        printf("-> Mas rapida: %s\n", tiempo_busqueda_indexada_ms <= tiempo_busqueda_lineal_ms ? "Indexada" : "Lineal");
    }
    printf("===========================================\n");
}
