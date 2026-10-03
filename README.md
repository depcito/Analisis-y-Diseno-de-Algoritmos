# Centro de distribución: listas enlazadas, ordenamiento y búsqueda en C
## Integrantes:
Jeronimo Machado Leal
Samuel Hernando Echeverry Castrillon
Emanuel Ossa
Sebastian Augusto Gudiño Pachon

# Este Proyecto fue diseñado con ayuda de inteligencia artifial 
## Compilación y ejecución

El proyecto se compila y ejecuta desde la terminal de Linux (probado en el flujo de trabajo con WSL)escribe `make` para compilar y generar el ejecutable `simulacion`, y `./simulacion` para correr la simulación (opcionalmente con una semilla propia, por ejemplo `./simulacion 12345`; por defecto usa 2026). También puedes usar `make run`, que compila si hace falta y ejecuta el programa en un solo paso, y `make clean` para borrar el ejecutable y los archivos objeto y empezar de cero. Ten en cuenta que Selection Sort con 50.000 paquetes tarda varios segundos por repetición, y la carga de cada lista también demora por la validación de IDs, por lo que la ejecución completa puede tardar un buen rato.

### Cómo cambiar los parámetros de la simulación

Todos los parámetros están en el módulo `main.c`, en los `#define` de las **líneas 20 a 27**, justo después de los `#include`; basta con editar el valor, guardar y volver a ejecutar `make run`. Allí puedes cambiar: `NUM_PAQUETES` (línea 20, cantidad de paquetes, 50.000 por defecto), `RANGO_ID` (línea 21, rango máximo de los IDs aleatorios; debe ser mayor que `NUM_PAQUETES`), `SEMILLA_DEFECTO` (línea 22, semilla usada si no se pasa una por línea de comandos), `REPETICIONES_ORDEN` (línea 23, repeticiones de cada ordenamiento), `RONDAS_BUSQUEDA` (línea 24, rondas de búsqueda), `BUSQUEDAS_POR_RONDA` (línea 25, búsquedas en cada ronda), `REPETICIONES_BINARIA` (línea 26, veces que se repite el lote de la búsqueda binaria para poder medirla) y `NODOS_A_IMPRIMIR` (línea 27, cuántos nodos iniciales y finales se muestran de cada lista ordenada). La semilla también se puede cambiar sin editar el código: `./simulacion 12345`.

## Descripción

Aplicación en C para el parcial de Diseño de Algoritmos (2026B). Almacena paquetes en una lista enlazada simple, los ordena con dos paradigmas distintos (fuerza bruta y dividir y conquistar) y compara empíricamente dos formas de buscarlos por ID, midiendo tiempos reales en milisegundos.

## Estructura del proyecto

| Archivo | Responsabilidad |
|---|---|
| `paquetes.h / paquetes.c` | Estructuras `Nodo` y `Lista` y operaciones básicas: crear, insertar, imprimir y liberar. |
| `ordenamiento.h / ordenamiento.c` | Comparación de nodos, Selection Sort y Merge Sort sobre la lista. |
| `busqueda.h / busqueda.c` | Búsqueda lineal, índice (arreglo de punteros) y búsqueda binaria. |
| `main.c` | Simulación, mediciones con `clock()` y reporte en consola. |

## Estructuras de datos

- **`Nodo`**: `int id` (único), `float peso`, `int prioridad` (1 a 5) y `Nodo *siguiente`.
- **`Lista`**: `cabeza`, `cola` y `tamano`. Se usa un struct y no solo un `Nodo *` porque el puntero a la cola permite insertar al final sin recorrer la lista. El campo se llama `tamano` (sin ñ) porque los identificadores de C deben ser ASCII.
- **`Indice`**: `Nodo **nodos` y `int tamano`. Es un arreglo auxiliar de punteros a los nodos de la lista; los nodos pertenecen a la lista, el índice solo los apunta.

## Decisiones de diseño por módulo

### Lista (`paquetes`)

- **Inserción al final en O(1)** gracias al puntero a la cola.
- **`insertar` valida que el ID no esté repetido**, recorriendo la lista. Esto hace que cargar 50.000 paquetes sea más lento, y se aceptó a propósito. Esa carga no entra en ningún cronómetro.
- **`insertar` devuelve 1 o 0** (éxito o fallo) sin distinguir la causa: ID repetido o falta de memoria.
- **`imprimirLista(lista, n)`** muestra los primeros `n` y los últimos `n` nodos, para no inundar la consola con 50.000 líneas. Si la lista tiene `2n` nodos o menos, la imprime completa.
- **`crearLista()`** devuelve la lista vacía por valor.

### Ordenamiento (`ordenamiento`)

- **Criterio único**: prioridad ascendente y, en caso de empate, ID ascendente. Vive en `comparar(a, b)`, que devuelve negativo, cero o positivo, y la usan los dos algoritmos.
- **Selection Sort (fuerza bruta)**: en cada pasada busca el mínimo de la parte sin ordenar, lo desenlaza y lo agrega al final de la lista ordenada. Solo reenlaza nodos; no copia datos ni usa arreglos.
- **Merge Sort (dividir y conquistar)**: divide con punteros lento y rápido, ordena cada mitad recursivamente y mezcla de forma iterativa con un nodo centinela. Tampoco usa arreglos temporales.
  - La mezcla es iterativa porque una mezcla recursiva apila una llamada por nodo y con 50.000 nodos podría desbordar la pila. La recursión del propio Merge Sort solo llega a unos 16 niveles.
- **Firma**: `selectionSort(Lista *)` y `mergeSort(Lista *)`. Ambas dejan `cabeza`, `cola` y `tamano` correctos al terminar.
- No hay función que verifique el orden; la corrección se comprueba mirando los primeros y últimos nodos impresos.

### Búsqueda (`busqueda`)

- **Búsqueda lineal**: recorre la lista desde el inicio. Devuelve `Nodo *` (o `NULL` si el ID no existe).
- **Solución mixta para el punto 3.3.2**: una lista enlazada no permite acceso directo, así que la búsqueda binaria se hace sobre un `Indice`, un arreglo de punteros a los nodos ordenado por ID.
  - `construirIndice` recorre la lista, guarda un puntero por nodo y los ordena con un Merge Sort propio (`static`, dentro de `busqueda.c`). Como la lista está ordenada por prioridad y no por ID, el índice necesita su propio orden.
  - Ese Merge Sort sí usa un arreglo temporal, pero solo para el índice; nunca para ordenar la lista.
  - Si falla `malloc`, `construirIndice` devuelve un índice vacío (`tamano = 0`, `nodos = NULL`).
- **Búsqueda binaria recursiva**, con punto medio `bajo + (alto - bajo) / 2`. Su profundidad es de unos 16 niveles, sin riesgo para la pila. Devuelve `Nodo *` o `NULL`.
- **`liberarIndice`** libera solo el arreglo; los nodos se liberan con la lista.

## Simulación y medición (`main.c`)

- **50.000 paquetes**, con ID aleatorio en [1, 1.000.000] (unicidad verificada con un arreglo de marcas), peso aleatorio entre 1.0 y 1000.0 kg y prioridad aleatoria de 1 a 5.
- **Semilla fija por defecto (2026)**, que se puede cambiar con un argumento de línea de comandos. Se imprime en el reporte para poder reproducir la corrida.
- **Mismos datos para ambos ordenamientos**: cada repetición regenera la lista con la misma semilla.
- **Ordenamiento**: 3 repeticiones por algoritmo, cronometrando solo el ordenamiento con `clock()`. Se reportan mínimo, máximo y promedio en ms.
- **Búsquedas**: 5 rondas de 1.000 IDs existentes elegidos al azar del índice. Ambas búsquedas reciben exactamente los mismos IDs en cada ronda, y se preparan antes de cronometrar.
  - La lineal se mide con un solo lote por ronda y corre sobre la lista ordenada con Merge Sort, la misma de la que sale el índice.
  - La binaria tarda microsegundos, menos que la resolución típica de `clock()`. Por eso se repite el lote 1.000 veces y se divide el tiempo entre las repeticiones, para que sea comparable con la lineal.
- **Generador aleatorio**: `rand()` solo garantiza 15 bits en algunos sistemas (en Windows, `RAND_MAX` es 32.767), lo que impediría sortear 50.000 IDs únicos. Por eso `aleatorio30()` combina dos llamadas para obtener 30 bits.

## Reporte en consola

Una línea por medición (tiempos por repetición y por ronda, y mínimo, máximo y promedio de cada algoritmo) y una conclusión automática para el ordenamiento y otra para la búsqueda:

- **Más rápido**: el de menor tiempo promedio.
- **Más consistente**: el de menor diferencia entre máximo y mínimo.

Los mensajes de `main.c` están escritos sin tildes para que se vean bien en consolas de Windows.

## Compilación y ejecución

```bash
gcc -O2 -Wall -o simulacion main.c paquetes.c ordenamiento.c busqueda.c
./simulacion          # semilla por defecto (2026)
./simulacion 12345    # semilla personalizada
```

Selection Sort con 50.000 nodos tarda varios segundos por repetición, y la carga de cada lista también (por la validación de IDs), así que la ejecución completa puede tardar un buen rato.

## Equipo y responsabilidades

*(Completar: integrantes y qué módulo desarrolló cada uno. Recordar que todos los miembros deben publicar las evidencias en el buzón de Eafit Interactiva.)*

## Reporte de simulación

*(Completar con los tiempos que arroje la ejecución y un análisis breve de los hallazgos del equipo.)*

# Este Proyecto fue diseñado con ayuda de inteligencia artifial