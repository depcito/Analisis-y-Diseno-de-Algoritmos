#ifndef MENU_H
#define MENU_H

/* Muestra las opciones disponibles del menú principal. */
void mostrar_menu(void);

/* Ejecuta la opción seleccionada por el usuario.
   'continuar' controla si el programa sigue ejecutándose. */
void ejecutar_opcion(int opcion, int *continuar);

#endif