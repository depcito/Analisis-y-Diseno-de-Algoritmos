#include <stdio.h>
#include "menu.h"

static void limpiar_buffer_entrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* descartar caracteres restantes */
    }
}

int main(void) {
    int opcion;
    int continuar = 1;

    while (continuar) {
        mostrar_menu();

        if (scanf("%d", &opcion) != 1) {
            limpiar_buffer_entrada();
            printf("\nEntrada invalida. Intente nuevamente.\n");
            continue;
        }
        limpiar_buffer_entrada();

        ejecutar_opcion(opcion, &continuar);
    }

    return 0;
}
