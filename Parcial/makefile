# Makefile - Centro de distribucion (listas enlazadas, ordenamiento y busqueda)
#
# Uso:
#   make        compila y genera el ejecutable "simulacion"
#   make run    compila si hace falta y ejecuta el programa
#   make clean  borra el ejecutable y los archivos objeto (.o)

CC     = gcc
CFLAGS = -O2 -Wall

OBJETOS = main.o paquetes.o ordenamiento.o busqueda.o

# Objetivo por defecto: lo que se ejecuta al escribir solo "make"
all: simulacion

# Enlace final: une los archivos objeto en el ejecutable
simulacion: $(OBJETOS)
	$(CC) $(CFLAGS) -o simulacion $(OBJETOS)

# Regla general: compila cada .c en su .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Dependencias: si cambia un .h, se recompilan los modulos que lo incluyen
main.o: main.c paquetes.h ordenamiento.h busqueda.h
paquetes.o: paquetes.c paquetes.h
ordenamiento.o: ordenamiento.c ordenamiento.h paquetes.h
busqueda.o: busqueda.c busqueda.h paquetes.h

# Compila (si hace falta) y ejecuta
run: simulacion
	./simulacion

# Limpia los archivos generados
clean:
	rm -f simulacion $(OBJETOS)

.PHONY: all run clean