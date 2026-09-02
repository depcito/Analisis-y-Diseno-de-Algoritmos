#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// =====================
// CONSTANTES DEL SISTEMA
// =====================

#define NUM_PAQUETES 50000
#define NUM_BUSQUEDAS 10000

// =====================
// ESTRUCTURA DEL NODO
// =====================

typedef struct Nodo {
    int id;
    float peso;
    int prioridad;
    struct Nodo *siguiente;
} Nodo;

// =====================
// DECLARACIÓN DE FUNCIONES
// =====================

// Funciones del menú
void mostrarMenu();
void ejecutarOpcion(int opcion);

// Funciones del sistema
void opcion1();
void opcion2();
void opcion3();
void opcion4();

// Funciones de lista enlazada
Nodo* crearNodo(int id, float peso, int prioridad);
void insertarFinal(Nodo **lista, int id, float peso, int prioridad);
void imprimirLista(Nodo *lista, int cantidad);
void liberarLista(Nodo **lista);
Nodo* copiarLista(Nodo *lista);

// Funciones de generación
Nodo* generarPaquetes(int cantidad);

// Funciones de búsqueda
Nodo* busquedaLineal(Nodo *lista, int id);

// Funciones de ordenamiento
void bubbleSort(Nodo *lista);
Nodo* mergeSort(Nodo *lista);
Nodo* dividirLista(Nodo *lista);
Nodo* mezclarListas(Nodo *lista1, Nodo *lista2);

// Funciones auxiliares
void limpiarBuffer();
long obtenerTiempoMs();
int obtenerCantidad(Nodo *lista);
void mostrarResultados(double tiempoBubble,
                      double tiempoMerge,
                      double tiempoBusqueda);
void seleccionarIdsBusqueda(int ids[], int cantidad);

// Variables globales para conservar las listas
Nodo *listaOriginal = NULL;
Nodo *listaBubble = NULL;
Nodo *listaMerge = NULL;

// =====================
// FUNCIÓN PRINCIPAL
// =====================

int main() {

    int opcion;
    int continuar = 1;

    srand((unsigned int)time(NULL));

    while (continuar) {

        mostrarMenu();

        if (scanf("%d", &opcion) != 1) {
            limpiarBuffer();

            printf("\nEntrada invalida. Intente nuevamente.\n");

            continue;
        }

        limpiarBuffer();

        if (opcion == 0) {

            continuar = 0;

            liberarLista(&listaOriginal);
            liberarLista(&listaBubble);
            liberarLista(&listaMerge);

            printf("\nSaliendo del programa...\n");

        } else {

            ejecutarOpcion(opcion);
        }
    }

    return 0;
}

// =====================
// IMPLEMENTACIÓN
// =====================

void mostrarMenu() {

    printf("\n=============================================\n");
    printf("       SISTEMA DE GESTION DE PAQUETES\n");
    printf("=============================================\n");
    printf("1. Ejecutar simulacion completa\n");
    printf("2. Mostrar paquetes\n");
    printf("3. Buscar paquete por ID\n");
    printf("4. Liberar listas\n");
    printf("0. Salir\n");
    printf("---------------------------------------------\n");
    printf("Seleccione una opcion: ");
}

// =====================
// EJECUTAR OPCIÓN
// =====================

void ejecutarOpcion(int opcion) {

    switch (opcion) {

        case 1:
            opcion1();
            break;

        case 2:
            opcion2();
            break;

        case 3:
            opcion3();
            break;

        case 4:
            opcion4();
            break;

        default:
            printf("\nOpcion no valida.\n");
    }
}

// =====================
// OPCIÓN 1
// SIMULACIÓN COMPLETA
// =====================

void opcion1() {

    long inicio;
    long fin;

    double tiempoBubble;
    double tiempoMerge;
    double tiempoBusqueda;

    int ids[NUM_BUSQUEDAS];

    printf("\n=============================================\n");
    printf("          SIMULACION COMPLETA\n");
    printf("=============================================\n");

    // -----------------------------------------
    // Liberar datos anteriores
    // -----------------------------------------

    liberarLista(&listaOriginal);
    liberarLista(&listaBubble);
    liberarLista(&listaMerge);

    // -----------------------------------------
    // Generar 50,000 paquetes
    // -----------------------------------------

    printf("\nGenerando %d paquetes...\n", NUM_PAQUETES);

    listaOriginal = generarPaquetes(NUM_PAQUETES);

    if (listaOriginal == NULL) {

        printf("Error: no se pudo generar la lista.\n");

        return;
    }

    printf("Paquetes generados correctamente.\n");

    // -----------------------------------------
    // Crear copias
    // -----------------------------------------

    printf("Creando copias para las pruebas...\n");

    listaBubble = copiarLista(listaOriginal);
    listaMerge = copiarLista(listaOriginal);

    if (listaBubble == NULL || listaMerge == NULL) {

        printf("Error al crear las copias.\n");

        liberarLista(&listaOriginal);
        liberarLista(&listaBubble);
        liberarLista(&listaMerge);

        return;
    }

    // =========================================
    // BUBBLE SORT
    // =========================================

    printf("\n---------------------------------------------\n");
    printf("FUERZA BRUTA - BUBBLE SORT\n");
    printf("---------------------------------------------\n");

    printf("Ordenando %d paquetes...\n", NUM_PAQUETES);

    inicio = obtenerTiempoMs();

    bubbleSort(listaBubble);

    fin = obtenerTiempoMs();

    tiempoBubble = (double)(fin - inicio);

    printf("Bubble Sort terminado.\n");
    printf("Tiempo: %.3f ms\n", tiempoBubble);

    // =========================================
    // MERGE SORT
    // =========================================

    printf("\n---------------------------------------------\n");
    printf("DIVIDIR Y CONQUISTAR - MERGE SORT\n");
    printf("---------------------------------------------\n");

    printf("Ordenando %d paquetes...\n", NUM_PAQUETES);

    inicio = obtenerTiempoMs();

    listaMerge = mergeSort(listaMerge);

    fin = obtenerTiempoMs();

    tiempoMerge = (double)(fin - inicio);

    printf("Merge Sort terminado.\n");
    printf("Tiempo: %.3f ms\n", tiempoMerge);

    // =========================================
    // BÚSQUEDA LINEAL
    // =========================================

    printf("\n---------------------------------------------\n");
    printf("BUSQUEDA LINEAL\n");
    printf("---------------------------------------------\n");

    seleccionarIdsBusqueda(ids, NUM_BUSQUEDAS);

    printf("Realizando %d busquedas...\n", NUM_BUSQUEDAS);

    inicio = obtenerTiempoMs();

    for (int i = 0; i < NUM_BUSQUEDAS; i++) {

        busquedaLineal(listaOriginal, ids[i]);
    }

    fin = obtenerTiempoMs();

    tiempoBusqueda = (double)(fin - inicio);

    printf("Busquedas terminadas.\n");
    printf("Tiempo total: %.3f ms\n", tiempoBusqueda);

    // =========================================
    // RESULTADOS
    // =========================================

    mostrarResultados(
        tiempoBubble,
        tiempoMerge,
        tiempoBusqueda
    );
}

// =====================
// OPCIÓN 2
// MOSTRAR PAQUETES
// =====================

void opcion2() {

    if (listaOriginal == NULL) {

        printf("\nPrimero debe ejecutar la simulacion.\n");

        return;
    }

    printf("\n=============================================\n");
    printf("             PRIMEROS PAQUETES\n");
    printf("=============================================\n");

    imprimirLista(listaOriginal, 10);
}

// =====================
// OPCIÓN 3
// BUSCAR PAQUETE
// =====================

void opcion3() {

    int id;
    Nodo *resultado;

    if (listaOriginal == NULL) {

        printf("\nPrimero debe ejecutar la simulacion.\n");

        return;
    }

    printf("\n=============================================\n");
    printf("             BUSQUEDA DE PAQUETE\n");
    printf("=============================================\n");

    printf("Ingrese el ID del paquete: ");

    if (scanf("%d", &id) != 1) {

        limpiarBuffer();

        printf("\nID invalido.\n");

        return;
    }

    limpiarBuffer();

    resultado = busquedaLineal(listaOriginal, id);

    if (resultado != NULL) {

        printf("\nPaquete encontrado:\n");
        printf("ID: %d\n", resultado->id);
        printf("Peso: %.2f kg\n", resultado->peso);
        printf("Prioridad: %d\n", resultado->prioridad);

    } else {

        printf("\nNo se encontro un paquete con ID %d.\n", id);
    }
}

// =====================
// OPCIÓN 4
// LIBERAR LISTAS
// =====================

void opcion4() {

    liberarLista(&listaOriginal);
    liberarLista(&listaBubble);
    liberarLista(&listaMerge);

    printf("\nMemoria liberada correctamente.\n");
}

// =====================
// CREAR NODO
// =====================

Nodo* crearNodo(int id, float peso, int prioridad) {

    Nodo *nuevo = (Nodo*)malloc(sizeof(Nodo));

    if (nuevo == NULL) {

        return NULL;
    }

    nuevo->id = id;
    nuevo->peso = peso;
    nuevo->prioridad = prioridad;
    nuevo->siguiente = NULL;

    return nuevo;
}

// =====================
// INSERTAR AL FINAL
// =====================

void insertarFinal(Nodo **lista,
                   int id,
                   float peso,
                   int prioridad) {

    Nodo *nuevo;
    Nodo *actual;

    nuevo = crearNodo(id, peso, prioridad);

    if (nuevo == NULL) {

        printf("Error: no se pudo reservar memoria.\n");

        return;
    }

    if (*lista == NULL) {

        *lista = nuevo;

        return;
    }

    actual = *lista;

    while (actual->siguiente != NULL) {

        actual = actual->siguiente;
    }

    actual->siguiente = nuevo;
}

// =====================
// GENERAR PAQUETES
// =====================

Nodo* generarPaquetes(int cantidad) {

    Nodo *lista = NULL;

    int *ids;

    ids = (int*)malloc(cantidad * sizeof(int));

    if (ids == NULL) {

        return NULL;
    }

    // Crear IDs únicos
    for (int i = 0; i < cantidad; i++) {

        ids[i] = i + 1;
    }

    // Barajar los IDs
    for (int i = cantidad - 1; i > 0; i--) {

        int j = rand() % (i + 1);

        int temporal = ids[i];

        ids[i] = ids[j];
        ids[j] = temporal;
    }

    // Crear los paquetes
    for (int i = 0; i < cantidad; i++) {

        float peso;
        int prioridad;

        peso = 0.5f + ((float)rand() / RAND_MAX) * 49.5f;

        prioridad = 1 + rand() % 5;

        insertarFinal(
            &lista,
            ids[i],
            peso,
            prioridad
        );
    }

    free(ids);

    return lista;
}

// =====================
// IMPRIMIR LISTA
// =====================

void imprimirLista(Nodo *lista, int cantidad) {

    Nodo *actual = lista;

    int contador = 0;

    printf("\n%-10s %-15s %-10s\n",
           "ID",
           "PESO (kg)",
           "PRIORIDAD");

    printf("---------------------------------------------\n");

    while (actual != NULL && contador < cantidad) {

        printf("%-10d %-15.2f %-10d\n",
               actual->id,
               actual->peso,
               actual->prioridad);

        actual = actual->siguiente;

        contador++;
    }
}

// =====================
// LIBERAR LISTA
// =====================

void liberarLista(Nodo **lista) {

    Nodo *actual;
    Nodo *siguiente;

    if (*lista == NULL) {

        return;
    }

    actual = *lista;

    while (actual != NULL) {

        siguiente = actual->siguiente;

        free(actual);

        actual = siguiente;
    }

    *lista = NULL;
}

// =====================
// COPIAR LISTA
// =====================

Nodo* copiarLista(Nodo *lista) {

    Nodo *copia = NULL;
    Nodo *actual = lista;

    while (actual != NULL) {

        insertarFinal(
            &copia,
            actual->id,
            actual->peso,
            actual->prioridad
        );

        actual = actual->siguiente;
    }

    return copia;
}

// =====================
// BÚSQUEDA LINEAL
// =====================

Nodo* busquedaLineal(Nodo *lista, int id) {

    Nodo *actual = lista;

    while (actual != NULL) {

        if (actual->id == id) {

            return actual;
        }

        actual = actual->siguiente;
    }

    return NULL;
}

// =====================
// BUBBLE SORT
// =====================
//
// Se intercambia la información de los nodos
// lógicamente, sin modificar los enlaces.
// =====================

void bubbleSort(Nodo *lista) {

    int huboCambio;

    Nodo *actual;
    Nodo *ultimo = NULL;

    if (lista == NULL) {

        return;
    }

    do {

        huboCambio = 0;

        actual = lista;

        while (actual->siguiente != ultimo) {

            if (actual->id > actual->siguiente->id) {

                // Intercambiar ID
                int tempId = actual->id;

                actual->id = actual->siguiente->id;
                actual->siguiente->id = tempId;

                // Intercambiar peso
                float tempPeso = actual->peso;

                actual->peso = actual->siguiente->peso;
                actual->siguiente->peso = tempPeso;

                // Intercambiar prioridad
                int tempPrioridad = actual->prioridad;

                actual->prioridad =
                    actual->siguiente->prioridad;

                actual->siguiente->prioridad =
                    tempPrioridad;

                huboCambio = 1;
            }

            actual = actual->siguiente;
        }

        ultimo = actual;

    } while (huboCambio);
}

// =====================
// MERGE SORT
// =====================

Nodo* mergeSort(Nodo *lista) {

    Nodo *mitad;
    Nodo *izquierda;
    Nodo *derecha;

    if (lista == NULL ||
        lista->siguiente == NULL) {

        return lista;
    }

    mitad = dividirLista(lista);

    izquierda = mergeSort(lista);

    derecha = mergeSort(mitad);

    return mezclarListas(izquierda, derecha);
}

// =====================
// DIVIDIR LISTA
// =====================

Nodo* dividirLista(Nodo *lista) {

    Nodo *lento = lista;
    Nodo *rapido = lista->siguiente;

    while (rapido != NULL &&
           rapido->siguiente != NULL) {

        lento = lento->siguiente;

        rapido = rapido->siguiente->siguiente;
    }

    Nodo *mitad = lento->siguiente;

    lento->siguiente = NULL;

    return mitad;
}

// =====================
// MEZCLAR DOS LISTAS
// =====================

Nodo* mezclarListas(Nodo *lista1,
                    Nodo *lista2) {

    Nodo *resultado = NULL;
    Nodo **ultimo = &resultado;

    while (lista1 != NULL &&
           lista2 != NULL) {

        if (lista1->id <= lista2->id) {

            *ultimo = lista1;

            lista1 = lista1->siguiente;

        } else {

            *ultimo = lista2;

            lista2 = lista2->siguiente;
        }

        ultimo = &((*ultimo)->siguiente);
    }

    if (lista1 != NULL) {

        *ultimo = lista1;

    } else {

        *ultimo = lista2;
    }

    return resultado;
}

// =====================
// SELECCIONAR IDS
// =====================

void seleccionarIdsBusqueda(int ids[],
                            int cantidad) {

    for (int i = 0; i < cantidad; i++) {

        // Los IDs válidos están entre 1 y NUM_PAQUETES
        ids[i] = 1 + rand() % NUM_PAQUETES;
    }
}

// =====================
// MOSTRAR RESULTADOS
// =====================

void mostrarResultados(double tiempoBubble,
                       double tiempoMerge,
                       double tiempoBusqueda) {

    printf("\n=============================================\n");
    printf("              RESULTADOS FINALES\n");
    printf("=============================================\n");

    printf("\nCantidad de paquetes: %d\n", NUM_PAQUETES);

    printf("Cantidad de busquedas: %d\n", NUM_BUSQUEDAS);

    printf("\n---------------------------------------------\n");
    printf("TIEMPOS DE ORDENAMIENTO\n");
    printf("---------------------------------------------\n");

    printf("Bubble Sort : %.3f ms\n", tiempoBubble);
    printf("Merge Sort  : %.3f ms\n", tiempoMerge);

    printf("\n---------------------------------------------\n");
    printf("TIEMPO DE BUSQUEDA\n");
    printf("---------------------------------------------\n");

    printf("Busqueda Lineal: %.3f ms\n", tiempoBusqueda);

    printf("\n=============================================\n");
    printf("             ANALISIS EMPIRICO\n");
    printf("=============================================\n");

    if (tiempoBubble < tiempoMerge) {

        printf("Bubble Sort fue mas rapido en esta ejecucion.\n");

    } else if (tiempoMerge < tiempoBubble) {

        printf("Merge Sort fue mas rapido en esta ejecucion.\n");

    } else {

        printf("Ambos algoritmos tuvieron el mismo tiempo.\n");
    }

    printf("\n");

    if (tiempoMerge > 0 &&
        tiempoBubble > 0) {

        printf("Diferencia Bubble/Merge: %.2f veces.\n",
               tiempoBubble / tiempoMerge);
    }

    printf("\nLa busqueda lineal recorrio la lista nodo por nodo\n");
    printf("para realizar %d consultas.\n", NUM_BUSQUEDAS);

    printf("\nLa medicion anterior representa el comportamiento\n");
    printf("empirico obtenido durante la simulacion.\n");

    printf("\n=============================================\n");
}

// =====================
// CANTIDAD DE NODOS
// =====================

int obtenerCantidad(Nodo *lista) {

    int cantidad = 0;

    Nodo *actual = lista;

    while (actual != NULL) {

        cantidad++;

        actual = actual->siguiente;
    }

    return cantidad;
}

// =====================
// TIEMPO EN MILISEGUNDOS
// =====================

long obtenerTiempoMs() {

    return (long)(
        (double)clock() * 1000.0 / CLOCKS_PER_SEC
    );
}

// =====================
// LIMPIAR BUFFER
// =====================

void limpiarBuffer() {

    int c;

    while ((c = getchar()) != '\n' &&
           c != EOF);
}