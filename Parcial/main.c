/*
 * main.c
 * Simulación del centro de distribución:
 *   1. Genera 50.000 paquetes aleatorios en una lista enlazada simple.
 *   2. Mide (en ms con clock()) Selection Sort y Merge Sort, 3 repeticiones cada uno.
 *   3. Construye el índice (arreglo de punteros ordenado por ID) y mide la
 *      búsqueda lineal frente a la búsqueda binaria en 5 rondas de 1.000 búsquedas.
 *   4. Imprime un reporte con una línea por medición y una conclusión final.
 *
 * Uso: ./simulacion [semilla]     (semilla por defecto: 2026)
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "paquetes.h"
#include "ordenamiento.h"
#include "busqueda.h"

/* ---------------- Parámetros de la simulación ---------------- */
#define NUM_PAQUETES          50000
#define RANGO_ID              1000000
#define SEMILLA_DEFECTO       2026u
#define REPETICIONES_ORDEN    3
#define RONDAS_BUSQUEDA       5
#define BUSQUEDAS_POR_RONDA   1000
#define REPETICIONES_BINARIA  1000
#define NODOS_A_IMPRIMIR      5

typedef void (*FuncionOrden)(Lista *);

typedef struct {
    double min;
    double max;
    double prom;
} Estadistica;

/* ---------------- Utilidades ---------------- */

/*
 * Número aleatorio de 30 bits. rand() solo garantiza 15 bits (RAND_MAX puede
 * ser 32767, como en Windows), por eso se combinan dos llamadas.
 */
static unsigned aleatorio30(void) {
    return ((unsigned)(rand() & 0x7FFF) << 15) | (unsigned)(rand() & 0x7FFF);
}

/* Convierte un intervalo de clock() a milisegundos */
static double milisegundos(clock_t inicio, clock_t fin) {
    return (double)(fin - inicio) * 1000.0 / (double)CLOCKS_PER_SEC;
}

static Estadistica calcularEstadistica(const double *valores, int n) {
    Estadistica e;
    double suma = 0.0;
    e.min = valores[0];
    e.max = valores[0];
    for (int i = 0; i < n; i++) {
        if (valores[i] < e.min) e.min = valores[i];
        if (valores[i] > e.max) e.max = valores[i];
        suma += valores[i];
    }
    e.prom = suma / n;
    return e;
}

/* ---------------- Generación de datos ---------------- */

/*
 * Genera NUM_PAQUETES paquetes con la semilla dada. Con la misma semilla
 * siempre se obtiene exactamente la misma lista desordenada.
 * ID único: aleatorio en [1, RANGO_ID] verificado con un arreglo de marcas.
 * Devuelve 1 si tuvo éxito y 0 si se quedó sin memoria.
 */
static int generarLista(Lista *lista, unsigned semilla) {
    unsigned char *usados = (unsigned char *)calloc(RANGO_ID + 1, 1);
    if (usados == NULL) {
        return 0;
    }

    srand(semilla);
    *lista = crearLista();

    while (lista->tamano < NUM_PAQUETES) {
        int id = (int)(aleatorio30() % RANGO_ID) + 1;
        if (usados[id]) {
            continue;   /* ID repetido: se sortea otro */
        }
        float peso = 1.0f + (float)((aleatorio30() / 1073741823.0) * 999.0);
        int prioridad = (int)(aleatorio30() % 5) + 1;

        if (!insertar(lista, id, peso, prioridad)) {
            liberarLista(lista);
            free(usados);
            return 0;
        }
        usados[id] = 1;
    }

    free(usados);
    return 1;
}

/* ---------------- Mediciones ---------------- */

/*
 * Mide un algoritmo de ordenamiento REPETICIONES_ORDEN veces. Cada repetición
 * regenera la lista con la misma semilla y cronometra solo el ordenamiento.
 * Si conservar no es NULL, la lista ordenada de la última repetición se
 * guarda ahí (quien la reciba debe liberarla).
 */
static int medirOrdenamiento(const char *nombre, FuncionOrden ordenar,
                             unsigned semilla, Estadistica *est, Lista *conservar) {
    double tiempos[REPETICIONES_ORDEN];

    printf("\n=== %s (%d paquetes) ===\n", nombre, NUM_PAQUETES);

    for (int r = 0; r < REPETICIONES_ORDEN; r++) {
        Lista lista;
        if (!generarLista(&lista, semilla)) {
            printf("Error: sin memoria al generar la lista.\n");
            return 0;
        }

        clock_t inicio = clock();
        ordenar(&lista);
        clock_t fin = clock();

        tiempos[r] = milisegundos(inicio, fin);
        printf("%s - repeticion %d: %.3f ms\n", nombre, r + 1, tiempos[r]);

        if (r == REPETICIONES_ORDEN - 1) {
            printf("Lista ordenada (primeros y ultimos %d nodos):\n", NODOS_A_IMPRIMIR);
            imprimirLista(&lista, NODOS_A_IMPRIMIR);
            if (conservar != NULL) {
                *conservar = lista;
            } else {
                liberarLista(&lista);
            }
        } else {
            liberarLista(&lista);
        }
    }

    *est = calcularEstadistica(tiempos, REPETICIONES_ORDEN);
    printf("%s - minimo: %.3f ms | maximo: %.3f ms | promedio: %.3f ms\n",
           nombre, est->min, est->max, est->prom);
    return 1;
}

/* Imprime quién fue más rápido (promedio) y más consistente (máx - mín) */
static void concluir(const char *titulo,
                     const char *nombreA, const Estadistica *a,
                     const char *nombreB, const Estadistica *b) {
    printf("\n--- Conclusion: %s ---\n", titulo);

    if (a->prom < b->prom) {
        printf("Mas rapido (promedio): %s (%.6f ms vs %.6f ms)\n", nombreA, a->prom, b->prom);
    } else if (b->prom < a->prom) {
        printf("Mas rapido (promedio): %s (%.6f ms vs %.6f ms)\n", nombreB, b->prom, a->prom);
    } else {
        printf("Mas rapido (promedio): empate (%.6f ms)\n", a->prom);
    }

    double rangoA = a->max - a->min;
    double rangoB = b->max - b->min;
    if (rangoA < rangoB) {
        printf("Mas consistente (menor max-min): %s (%.6f ms vs %.6f ms)\n", nombreA, rangoA, rangoB);
    } else if (rangoB < rangoA) {
        printf("Mas consistente (menor max-min): %s (%.6f ms vs %.6f ms)\n", nombreB, rangoB, rangoA);
    } else {
        printf("Mas consistente (menor max-min): empate (%.6f ms)\n", rangoA);
    }
}

/* ---------------- Programa principal ---------------- */

int main(int argc, char *argv[]) {
    unsigned semilla = SEMILLA_DEFECTO;

    if (argc > 1) {
        char *fin = NULL;
        unsigned long valor = strtoul(argv[1], &fin, 10);
        if (fin == argv[1] || *fin != '\0') {
            printf("Uso: %s [semilla]\n", argv[0]);
            return 1;
        }
        semilla = (unsigned)valor;
    }

    printf("Simulacion del centro de distribucion\n");
    printf("Paquetes: %d | Semilla: %u\n", NUM_PAQUETES, semilla);

    /* ---- Ordenamiento ---- */
    Estadistica estSelection, estMerge;
    Lista listaOrdenada;

    if (!medirOrdenamiento("Selection Sort", selectionSort, semilla, &estSelection, NULL)) {
        return 1;
    }
    if (!medirOrdenamiento("Merge Sort", mergeSort, semilla, &estMerge, &listaOrdenada)) {
        return 1;
    }
    concluir("ordenamiento", "Selection Sort", &estSelection, "Merge Sort", &estMerge);

    /* ---- Índice para la búsqueda binaria (se construye desde la lista ordenada) ---- */
    printf("\n=== Busqueda (%d rondas de %d busquedas de IDs existentes) ===\n",
           RONDAS_BUSQUEDA, BUSQUEDAS_POR_RONDA);

    clock_t inicioIndice = clock();
    Indice indice = construirIndice(&listaOrdenada);
    clock_t finIndice = clock();

    if (indice.tamano == 0) {
        printf("Error: no se pudo construir el indice.\n");
        liberarLista(&listaOrdenada);
        return 1;
    }
    printf("Construccion del indice (arreglo de punteros ordenado por ID): %.3f ms\n",
           milisegundos(inicioIndice, finIndice));

    /* ---- Rondas de búsqueda ---- */
    double tiemposLineal[RONDAS_BUSQUEDA];
    double tiemposBinaria[RONDAS_BUSQUEDA];
    int ids[BUSQUEDAS_POR_RONDA];

    srand(semilla + 1u);   /* semilla propia para elegir los IDs a buscar */

    for (int ronda = 0; ronda < RONDAS_BUSQUEDA; ronda++) {
        /* IDs existentes al azar; se preparan ANTES de cronometrar */
        for (int i = 0; i < BUSQUEDAS_POR_RONDA; i++) {
            ids[i] = indice.nodos[aleatorio30() % (unsigned)indice.tamano]->id;
        }

        /* Búsqueda lineal: un solo lote */
        volatile long encontradosLineal = 0;
        clock_t inicio = clock();
        for (int i = 0; i < BUSQUEDAS_POR_RONDA; i++) {
            if (busquedaLineal(&listaOrdenada, ids[i]) != NULL) {
                encontradosLineal++;
            }
        }
        clock_t fin = clock();
        tiemposLineal[ronda] = milisegundos(inicio, fin);

        /* Búsqueda binaria: lote repetido REPETICIONES_BINARIA veces, luego se divide */
        volatile long encontradosBinaria = 0;
        inicio = clock();
        for (int rep = 0; rep < REPETICIONES_BINARIA; rep++) {
            for (int i = 0; i < BUSQUEDAS_POR_RONDA; i++) {
                if (busquedaBinaria(&indice, ids[i]) != NULL) {
                    encontradosBinaria++;
                }
            }
        }
        fin = clock();
        tiemposBinaria[ronda] = milisegundos(inicio, fin) / REPETICIONES_BINARIA;

        printf("Busqueda lineal - ronda %d: %.3f ms por %d busquedas (%.6f ms por busqueda) | encontrados: %ld/%d\n",
               ronda + 1, tiemposLineal[ronda], BUSQUEDAS_POR_RONDA,
               tiemposLineal[ronda] / BUSQUEDAS_POR_RONDA,
               (long)encontradosLineal, BUSQUEDAS_POR_RONDA);
        printf("Busqueda binaria - ronda %d: %.6f ms por %d busquedas (%.9f ms por busqueda) | encontrados: %ld/%d\n",
               ronda + 1, tiemposBinaria[ronda], BUSQUEDAS_POR_RONDA,
               tiemposBinaria[ronda] / BUSQUEDAS_POR_RONDA,
               (long)encontradosBinaria / REPETICIONES_BINARIA, BUSQUEDAS_POR_RONDA);
    }

    Estadistica estLineal = calcularEstadistica(tiemposLineal, RONDAS_BUSQUEDA);
    Estadistica estBinaria = calcularEstadistica(tiemposBinaria, RONDAS_BUSQUEDA);

    printf("Busqueda lineal - minimo: %.3f ms | maximo: %.3f ms | promedio: %.3f ms (por %d busquedas)\n",
           estLineal.min, estLineal.max, estLineal.prom, BUSQUEDAS_POR_RONDA);
    printf("Busqueda binaria - minimo: %.6f ms | maximo: %.6f ms | promedio: %.6f ms (por %d busquedas)\n",
           estBinaria.min, estBinaria.max, estBinaria.prom, BUSQUEDAS_POR_RONDA);

    concluir("busqueda", "Busqueda lineal", &estLineal, "Busqueda binaria", &estBinaria);

    /* ---- Limpieza ---- */
    liberarIndice(&indice);
    liberarLista(&listaOrdenada);

    return 0;
}