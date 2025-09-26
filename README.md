# Proyecto Avance 1 – Algoritmos de Ordenamiento  
**Tema:** Ranking de PC Builds

Este proyecto organiza, ordena y filtra configuraciones de PC (builds) para distintos usos: gaming, edición, animación, etc.  
Aplica **POO, algoritmos de ordenamiento y búsqueda con árbol**, además de lectura y escritura de archivos.

---

## 1. Avance del proyecto
En esta primera etapa el objetivo fue que los datos de las builds pudieran **almacenarse y organizarse dentro de un programa funcional**, usando las estructuras y algoritmos vistos en clase.

- Elegí un **`std::vector`** para almacenar las builds porque:
  - Permite acceder rápido a cualquier build usando su índice.
  - Se integra fácilmente con los algoritmos de ordenamiento.
  - Es práctico para insertar nuevas builds al final.

Cada **Build** es un objeto con:
- Nombre de la build  
- CPU (marca, modelo, socket, score)  
- GPU (marca, modelo, VRAM, soporte DLSS/FSR, score)  
- RAM (capacidad y velocidad)  
- Motherboard (socket y soporte PCIe)  
- Fuente de poder (wattage)  
- Precio total  
- Score promedio de rendimiento  
- Calificación de usuarios  

Para las búsquedas por presupuesto, implementé un **Árbol Binario de Búsqueda (BST)**, lo que permite encontrar builds en un rango de precio y recorrerlas en orden ascendente o descendente.

---

## 2. Cómo usar el programa
1. Al iniciar, el programa **carga las builds registradas** desde el archivo `builds.csv`.
2. En el **menú principal** puedes:
   - Registrar nuevas builds con sus componentes.
   - Consultar las builds existentes.
   - Ordenarlas por:
     - Precio total
     - Score promedio de rendimiento
     - Marca de CPU o GPU
   - Filtrar builds según requisitos (por ejemplo, GPU NVIDIA y RAM ≥ 32 GB).
   - Buscar builds dentro de un **presupuesto máximo usando el BST**.
3. Cada vez que agregas una nueva build, esta se guarda al final de `builds.csv` para no tener que recapturarla.

 **Ejemplo rápido:**  
- Registras una build llamada **“Gaming Entry”**.  
- Usas la opción para ordenar las builds por **precio** y ves dónde queda ubicada.  
- Luego buscas builds con **presupuesto menor a $20,000** usando el BST.

---

## 3. Cumplimiento de las sub-competencias

### SICT0302 – Toma decisiones
- **Estructura lineal:** usé `std::vector` porque es eficiente para acceder y recorrer las builds y funciona bien con los algoritmos de ordenamiento.
- **Algoritmos de ordenamiento:**  
  - Por **precio total → Merge Sort**, porque es estable y eficiente en datos grandes.  
  - Por **score promedio → Quick Sort**, que es rápido en promedio si el pivote se elige bien.  
  - Por **marca de CPU o GPU → Bubble Sort**, solo con fines didácticos ya que las listas son pequeñas.  
- **Árbol BST:** usado para búsquedas por rango de precios. Se inserta y consulta de manera eficiente.

---

### SICT0301 – Evalúa los componentes
**Casos de prueba:** en `tests/pruebas.cpp` se verifican:  
- Inserción y visualización de builds en el vector.  
- Ordenamiento por precio, score y marca.  
- Acceso al BST por presupuesto.  
- Búsqueda de builds por nombre y filtros por atributos.

**Complejidad de los componentes:**  
- Acceso a una build en el vector: **O(1)**  
- Inserción al final del vector: **O(1)** amortizado  
- Búsqueda secuencial: **O(n)**  
- Eliminación (si se implementa): **O(n)**  
- **Ordenamiento:**  
  - Merge Sort: **O(n log n)**  
  - Quick Sort: **O(n log n)** promedio, **O(n²)** peor caso  
  - Bubble Sort: **O(n²)**  
- **BST:**  
  - Crear árbol: **O(n log n)** promedio  
  - Insertar nodo: **O(log n)** promedio  
  - Buscar por precio: **O(log n)** promedio  

---

## 4. Próximos avances
Para los siguientes pasos planeo:
- Validar la **compatibilidad entre componentes** (socket CPU ↔ motherboard, GPU ↔ PCIe, potencia de la fuente).  
- Clasificar automáticamente las builds en **Gama Baja / Media / Alta** según su score promedio.  
- Permitir que los usuarios califiquen las builds y mostrar los resultados.

---
