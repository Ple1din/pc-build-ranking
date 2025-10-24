# Proyecto Avance 2 – Estructuras  
**Tema:** Ranking de PC Builds  

Este proyecto organiza, ordena y filtra configuraciones de PC (builds) para distintos usos: gaming, edición, animación, etc.  
Aplica **Programación Orientada a Objetos (POO)**, **algoritmos de ordenamiento eficientes**, y **lectura/escritura de archivos CSV** para almacenar la información de manera persistente.

---

## 1. Descripción general del proyecto

El sistema “Ranking de PC Builds” permite gestionar una colección de configuraciones de computadoras mediante la creación, almacenamiento y ordenamiento de builds por distintos criterios, utilizando algoritmos y estructuras de datos eficientes.  

Cada build incluye datos como:
- Nombre de la configuración
- CPU
- GPU
- RAM
- Precio total
- Score de rendimiento

Los datos se guardan en un archivo `builds.csv` para mantener la persistencia entre ejecuciones del programa.

---

## 2. Avance 1

En el primer avance se implementaron las bases del proyecto:

- Creación de la clase `Build` con sus atributos y métodos (`capturarDatos()`, `mostrarInfo()`, `toCSV()`, `fromCSV()`).
- Uso de un **vector de objetos `Build`** para almacenar las configuraciones registradas.
- Implementación de lectura y escritura en el archivo `builds.csv` mediante `ifstream` y `ofstream`.
- Menú básico para agregar y mostrar builds.
- Primer uso de un algoritmo de ordenamiento (`MergeSort`) aplicado al precio.

**Objetivo cumplido:** tener un programa funcional que almacenara, mostrara y organizara los datos usando estructuras básicas.

---

## 3. Avance 2

En este segundo avance, el proyecto evoluciona a un sistema más robusto que integra **estructuras de datos y algoritmos de ordenamiento múltiples**, además de un análisis formal de complejidad y toma de decisiones fundamentada.

### Cambios realizados respecto al avance anterior:
1. Se implementaron tres algoritmos de ordenamiento:
   - **MergeSort** (por precio)
   - **QuickSort** (por score de rendimiento)
   - **HeapSort** (por marca de CPU)
2. Se reforzó el uso del **vector** como estructura principal de almacenamiento.
3. Se implementó lectura y escritura de datos mediante archivos CSV.
4. Se mejoró la interfaz de texto para permitir al usuario seleccionar el tipo de ordenamiento.
5. Se incluyó un análisis completo de complejidad y justificación del uso de estructuras y algoritmos.

---

## 4. Cómo compilar y ejecutar el programa

### Opción 1: usando script automatizado  
Ejecuta el archivo `run.sh` (en Linux/Mac) o `run.bat` (en Windows).  
Estos scripts realizan automáticamente la compilación y ejecución del programa.

### Opción 2: manualmente desde la terminal  
Si los scripts no funcionan, puedes ejecutar los siguientes comandos:

**Compilación:**
g++ main.cpp Build.cpp CSVUtils.cpp Sorts.cpp -o pc_ranking
./pc_ranking
### Menú principal del programa

Ver todas las builds

Agregar una nueva build

Ordenar por precio (MergeSort)

Ordenar por score (QuickSort)

Ordenar por marca de CPU (HeapSort)

Salir

yaml
Copiar código

---

## 5. Cumplimiento de las sub-competencias

### SICT0301 – Evalúa los componentes

#### Análisis de complejidad correcto y completo

**Estructura principal:** `std::vector`

| Operación | Mejor caso | Promedio | Peor caso |
|------------|-------------|-----------|------------|
| Acceso directo (`[]`) | O(1) | O(1) | O(1) |
| Inserción al final (`push_back`) | O(1) | O(1) amortizado | O(n) |
| Recorrido / búsqueda secuencial | O(1) | O(n) | O(n) |
| Ordenamiento | O(n log n) | O(n log n) | O(n²) (según algoritmo aplicado) |

---

#### Algoritmos de ordenamiento implementados

| Algoritmo | Propósito | Estabilidad | Complejidad (Mejor) | Complejidad (Promedio) | Complejidad (Peor) |
|------------|------------|--------------|---------------------|------------------------|--------------------|
| MergeSort | Ordenar por precio | Estable | O(n log n) | O(n log n) | O(n log n) |
| QuickSort | Ordenar por score | No estable | O(n log n) | O(n log n) | O(n²) |
| HeapSort | Ordenar por marca de CPU | No estable | O(n log n) | O(n log n) | O(n log n) |

**Análisis:**

- **MergeSort** fue elegido para ordenar por precio por ser **estable**, manteniendo el orden de builds con igual costo.  
- **QuickSort** se usa para el score, priorizando su **rapidez promedio**.  
- **HeapSort** se usa para el CPU, garantizando **rendimiento constante O(n log n)** y sin requerir memoria adicional.

Estos algoritmos reflejan diferentes estrategias de eficiencia:

- **MergeSort:** estabilidad + consistencia.  
- **QuickSort:** rendimiento promedio alto.  
- **HeapSort:** rendimiento constante garantizado.  

---

### SICT0302 – Toma decisiones

#### Selección de estructuras de datos adecuadas

Se seleccionó `std::vector` como estructura principal debido a:

- Acceso aleatorio directo O(1).  
- Integración natural con algoritmos de la STL.  
- Alta eficiencia en recorridos secuenciales y ordenamientos.  
- Bajo consumo de memoria comparado con `list` o `deque`.  

**Justificación:**  
El proyecto requiere recorrer y ordenar con frecuencia el conjunto de builds, no insertar ni eliminar en medio del contenedor.  
Por lo tanto, `vector` es la estructura más eficiente y adecuada al contexto.

---

#### Selección del algoritmo de ordenamiento adecuado

| Criterio de orden | Algoritmo | Justificación |
|--------------------|------------|----------------|
| Precio | MergeSort | Necesita estabilidad (mantiene el orden de builds con igual precio). |
| Score | QuickSort | Máxima velocidad promedio, ideal para conjuntos medianos. |
| Marca de CPU | HeapSort | Garantiza O(n log n) en todos los casos, sin uso de memoria extra. |

**Justificación general:**  
Se utilizan tres algoritmos distintos porque cada uno ejemplifica una estrategia de ordenamiento distinta.  
Esta selección permite demostrar la correcta toma de decisiones técnicas según las necesidades de rendimiento y características de los datos.

---

### SICT0303 – Implementa acciones científicas

#### Mecanismos para consultar información de las estructuras

El programa ofrece múltiples formas de acceder y manipular la información:

- `mostrarInfo()` despliega los datos completos de cada build.  
- Los ordenamientos permiten obtener perspectivas diferentes (precio, score, CPU).  
- Se maneja un menú de interacción que permite al usuario seleccionar consultas específicas.  

Estas funciones aprovechan la estructura `vector` para ofrecer un acceso rápido y ordenado a los datos.

---

#### Mecanismos de lectura y escritura de archivos

El sistema implementa funciones para manejar archivos CSV:

- `cargarDesdeCSV()` → Lee las builds desde el archivo `builds.csv`.  
- `guardarEnCSV()` → Guarda nuevas builds sin sobrescribir las existentes.  

**Complejidad de las operaciones:**

| Operación | Complejidad |
|------------|--------------|
| Lectura secuencial | O(n) |
| Escritura (append) | O(1) |

Estos mecanismos permiten mantener la información actualizada y persistente entre ejecuciones del programa.

---

## 6. Próximos avances

En el siguiente avance se planea:

- Implementar un **árbol binario de búsqueda (BST)** para búsquedas por rango de precios.  
- Clasificar builds en **gama baja, media y alta** según el score.  
- Verificar compatibilidad de componentes (CPU ↔ motherboard, GPU ↔ fuente).  
- Incorporar validación automática y búsquedas más rápidas.
