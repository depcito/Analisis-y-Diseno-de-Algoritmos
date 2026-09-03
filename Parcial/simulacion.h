#ifndef SIMULACION_H
#define SIMULACION_H

#include "nodo.h"

/* ISSUE 5 - Generacion de datos aleatorios (carga masiva de prueba) */
void generar_lista_aleatoria(Nodo **cabeza, int cantidad);

/* ISSUE 6 - Medicion de tiempos y reporte empirico final */
void medir_ordenamientos(Nodo **cabeza);
void medir_busquedas(Nodo *cabeza, int id_buscado);
void mostrar_reporte_final(void);

#endif
