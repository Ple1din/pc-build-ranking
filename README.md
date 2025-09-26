# Ranking de PC Builds
Proyecto para organizar, ordenar y filtrar configuraciones de PC (builds) para distintos usos: gaming, edición, animación, etc.  
Aplica **POO, algoritmos de ordenamiento, búsqueda con árbol y validaciones de compatibilidad**.

---

## SICT0302B: Toma decisiones

### Selecciona y usa una estructura lineal adecuada al problema
Uso un **`std::vector`** para almacenar las builds porque permite:
- Acceso aleatorio y rápido a cualquier build.
- Fácil integración con los algoritmos de ordenamiento.
- Inserción de nuevas builds al final de la colección.

En el futuro se podrá migrar a una lista doblemente enlazada si es necesario mantener orden de inserción y eliminación frecuente.

Cada **Build** es un objeto que contiene:
- Nombre de la build
- CPU (marca, modelo, socket, score)
- GPU (marca, modelo, VRAM, soporte DLSS/FSR, score)
- RAM (capacidad y velocidad)
- Motherboard (socket y soporte PCIe)
- Fuente de poder (wattage)
- Precio total
- Score promedio de rendimiento
- Calificación de usuarios

### Selecciona un algoritmo de ordenamiento adecuado al problema
Para este problema se usan varios algoritmos según el criterio:

- Para ordenar builds por **precio total** → **Merge Sort**, porque es estable y eficiente en datos grandes y no depende de que la lista esté parcialmente ordenada.
- Para ordenar builds por **score promedio de rendimiento** → **Quick Sort**, por su rapidez en la práctica cuando el pivote está bien elegido.
- Para ordenar por **marca de CPU o GPU** → **Bubble Sort**, por fines didácticos ya que se usa en listas pequeñas.

Las funciones de ordenamiento se encuentran en `src/utils/Sorts.h`.

### Usa un árbol adecuado para resolver un problema
Uso un **Árbol Binario de Búsqueda (BST)** para organizar las builds por **precio total**.  
Permite:
- Buscar rápidamente builds dentro de un rango de presupuesto.
- Recorrer en orden ascendente o descendente según se requiera.

Las funciones para crear el árbol, insertar nodos y buscar builds por precio se encuentran en `src/utils/BST.h`.

---

## SICT0301B: Evalúa los componentes

### Presenta Casos de Prueba correctos y completos para todas las funciones y procedimientos del programa
Los casos de prueba se encuentran en `tests/pruebas.cpp` e incluyen:
- Inserción y visualización de builds en el vector.
- Ordenamiento por precio, score y marca.
- Acceso al BST por presupuesto.
- Búsqueda de builds por nombre y filtros por marca o RAM.

### Hace un análisis de complejidad correcto y completo para todo el programa y sus componentes

#### Vector de builds
- Acceso a una build por índice: **O(1)**
- Búsqueda secuencial por atributo: **O(n)**
- Inserción al final: **O(1)** amortizado
- Eliminación (si se implementa): **O(n)** en peor caso

#### Ordenamientos
- Merge Sort: **O(n log n)**
- Quick Sort: **O(n log n)** promedio, **O(n²)** peor caso
- Bubble Sort: **O(n²)**

#### Árbol BST
- Crear el árbol: **O(n log n)** promedio
- Insertar un nodo: **O(log n)** promedio
- Buscar builds por precio: **O(log n)** promedio

---

## SICT0303B: Implementa acciones científicas

### Implementa mecanismos para consultar información de las estructuras correctos y útiles dentro de un programa
El programa permite:
- Buscar builds por nombre (en el vector) — opción en el menú principal.
- Mostrar reportes ordenados de builds por precio, score o marca — opción en el menú.
- Obtener builds dentro de un rango de precio usando el BST — opción en el menú.

### Implementa mecanismos de lectura de archivos correctos y útiles dentro de un programa
Las builds registradas se cargan desde un archivo `builds.csv` al iniciar el programa.

### Implementa mecanismos de escritura de archivos correctos y útiles dentro de un programa
Las nuevas builds se agregan al final de `builds.csv` con la función `agregarBuild()`, para que no tengan que ser recapturadas cada vez que se corre el programa.

---

## Ejemplo de Uso
1. Registrar builds con sus componentes y datos básicos.
2. Consultar builds existentes.
3. Ordenar builds por:
   - Precio
   - Score de rendimiento
   - Marca de CPU o GPU
4. Filtrar builds por requisitos (ejemplo: solo con GPU NVIDIA y RAM ≥ 32 GB).
5. Buscar builds por presupuesto máximo usando el BST.

---

## Próximos Avances
- Validaciones de compatibilidad entre componentes:
  - Socket CPU ↔ Motherboard
  - Compatibilidad PCIe GPU ↔ Motherboard
  - Requerimiento de potencia de la fuente de poder
- Clasificación automática de las builds en **Gama Baja / Media / Alta** según score promedio.
- Calificación de builds por usuarios.
