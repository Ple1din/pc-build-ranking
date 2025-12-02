# Proyecto Avance 2 – Estructuras  
**Tema:** Ranking de PC Builds  

Este proyecto implementa un sistema completo para administrar configuraciones de computadoras (PC Builds), integrando estructuras de datos avanzadas, mecanismos de compatibilidad entre componentes, ordenamientos eficientes y persistencia mediante archivos externos. Este avance consolida el sistema con la inclusión de un Árbol AVL implementado desde cero, un MergeSort propio, ampliación del sistema de compatibilidad y guardado automático del inventario.

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

## 4. Avance 3

Este avance consolida el sistema integrando estructuras y algoritmos avanzados, así como guardado persistente automático.

Las funcionalidades agregadas son:

#Árbol AVL

El AVL reemplaza al vector como estructura principal para almacenar las builds.
Cada inserción mantiene el árbol balanceado mediante:

-Cálculo de alturas

-Factor de balance

-Rotaciones simples (LL, RR)

-Rotaciones dobles (LR, RL)

-Funcionalidades soportadas:

-Insertar builds en orden por precio

-Buscar por rangos de precios

-Recorrer en orden ascendente

-Mostrar builds completamente ordenadas sin usar sort

Razón de uso:

El AVL garantiza:

-O(log n) en inserciones, búsquedas y eliminaciones

-Ordenamiento permanente sin necesidad de relanzar algoritmos

-Excelente desempeño incluso con muchos elementos

#MergeSort como algoritmo de ordenamiento secundario

Aunque el AVL ya ordena por precio, se requiere ordenamiento por score.
Para ello se implementó un MergeSort desde cero, con:

-División recursiva

-Fusión ordenada

-Estabilidad

-Control explícito de índices

Uso:

-Se extrae el inventario desde el AVL hacia un vector.

-Se aplica MergeSort para ordenar ese vector por score.

Razón:

-MergeSort garantiza estabilidad y rendimiento constante incluso en el peor caso, algo necesario para rankings de builds.

#Guardado automático del inventario del usuario

Al finalizar el programa, se guarda el inventario en builds_guardado.csv.

Al iniciar el programa:

-Si existe builds_guardado.csv, se carga y el usuario continúa donde lo dejó.

-Si no existe, se carga la base por defecto.

Esto simula un sistema de guardado de progreso típico de aplicaciones reales.

#Módulo completo de compatibilidad entre componentes

El sistema valida automáticamente compatibilidad entre todas las piezas:

Compatibilidad	Verifica
CPU ↔ Motherboard	Socket, generación, chipset
RAM ↔ Motherboard	Tipo DDRx, frecuencia máxima
Storage ↔ Motherboard	PCIe / SATA, soporte NVMe
GPU ↔ PSU	Consumo eléctrico, margen seguro
Motherboard ↔ Case	Form factor (ATX, mATX, ITX)
Cooler ↔ CPU	Tamaño, socket soportado

Estas reglas introducen realismo y aseguran builds funcionales.

El sistema pasó de ser estático a dinámico y escalable:

Lectura de catálogos desde archivos CSV

Ya no se crean componentes manualmente. En su lugar, el programa carga catálogos completos de:

-CPUs

-GPUs

-Motherboards

-RAM

-Storage

-PSUs

-Cases

-Coolers

Esto vuelve el sistema modular y actualizable sin modificar el código.

#Búsqueda y filtrado de componentes

El usuario puede explorar catálogos completos y seleccionar los componentes adecuados para formar una build.

#Compatibilidad entre componentes

Cada clase (CPU, Motherboard, GPU, etc.) incluye funciones que evalúan compatibilidad realista basándose en:

-Socket

-Chipset

-PCIe

-TDP

-Frecuencias de RAM

-Factor de forma

-Límites de potencia

-Slots M.2

-Soporte de overclock

#Menú ampliado

Se agregó un sistema estructurado de opciones para registrar, consultar y filtrar builds y componentes.


---


## 5. Cómo compilar y ejecutar el programa

### Opción 1: usando script automatizado  
Ejecuta el archivo `run.sh` (en Linux/Mac) o `run.bat` (en Windows).  
Estos scripts realizan automáticamente la compilación y ejecución del programa.

### Opción 2: manualmente desde la terminal  
Si los scripts no funcionan, puedes ejecutar los siguientes comandos:

**Compilación:**
g++ main.cpp -o pc_ranking
./pc_ranking

---

## 6. Cumplimiento de las sub-competencias

### SICT0301 – Evalúa los componentes

#### Análisis de complejidad

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
| MergeSort | Ordenar por score | Estable | O(n log n) | O(n log n) | O(n log n) |

**Razones técnicas para seleccionarlo:**

-Garantiza estabilidad (importante para builds con igual score).

-No cae jamás a O(n²).

-Permite ordenar vectores de gran tamaño sin pérdida de rendimiento.

-Es más predecible que Quicksort en casos adversos.  

#Orden implícito mediante Árbol AVL

El AVL ordena por precio sin ejecutar algoritmos de ordenamiento explícitos.

Ordenamiento efectivo:

El recorrido inorder entrega el inventario en O(n) completamente ordenado.

Complejidad de mantenimiento:
Operación	Complejidad
Insertar	O(log n)
Buscar	O(log n)
Eliminar	O(log n)
Recorrer	O(n)

---

### SICT0302 – Toma decisiones

#### Selección de estructuras de datos adecuadas

Se seleccionó `std::vector` como estructura principal debido a:

- Acceso aleatorio directo O(1).  
- Integración natural con algoritmos de la STL.  
- Alta eficiencia en recorridos secuenciales y ordenamientos.  

**Justificación:**  
El proyecto requiere recorrer y ordenar con frecuencia el conjunto de builds, no insertar ni eliminar en medio del contenedor.  
Por lo tanto, `vector` es la estructura más eficiente y adecuada al contexto.

---
##Árbol AVL

Es la estructura principal del programa.

Justificación técnica:

Los datos deben mantenerse ordenados en todo momento.

El inventario se modifica frecuentemente, por lo que un árbol balanceado es superior a reordenar vectores repetidamente.

Permite búsquedas por rango y por precio con grandes mejoras de rendimiento.

---

##Manejo de archivos

Se usan ifstream y ofstream.

Complejidad:
Operación	Complejidad
Leer catálogos	O(n)
Guardar inventario	O(n)
Cargar inventario previo	O(n)

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
#Consulta de estructuras

El sistema usa:

-Recorridos inorder

-Filtros por atributos

-Listados completos

-Ordenamientos específicos

-Cada función corresponde a las fortalezas de su estructura.

#Lectura de archivos

-Carga catálogos extensos sin impactar el desempeño.

-Se validan los campos de cada CSV.

-Se manejan errores y líneas mal formateadas.

#Escritura de archivos

-Se registra cada build del usuario.

-Se conserva el progreso a través de sesiones.

-Formato compatible con otras aplicaciones (CSV estándar).
---
