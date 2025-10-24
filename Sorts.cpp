/*
 * Sorts.cpp
 * Implementación de los algoritmos de ordenamiento
 * - MergeSort: por Precio (estable)  — O(n log n), espacio O(n)
 * - QuickSort: por Score (promedio)  — O(n log n) promedio, O(n^2) peor, espacio O(log n)
 * - HeapSort : por Marca de CPU      — O(n log n) en todos los casos, espacio O(1), no estable
 */

#include "Sorts.h"
#include <string>
#include <utility> // swap
#include <cctype>
#include <algorithm> // reverse
using namespace std;

// =============================
// MERGESORT por Precio (estable)
// =============================
static void merge(vector<Build> &v, int inicio, int medio, int fin) {
    int n1 = medio - inicio + 1;
    int n2 = fin - medio;

    vector<Build> izquierda(n1);
    vector<Build> derecha(n2);

    for (int i = 0; i < n1; i++) izquierda[i] = v[inicio + i];
    for (int j = 0; j < n2; j++) derecha[j] = v[medio + 1 + j];

    int i = 0, j = 0, k = inicio;
    while (i < n1 && j < n2) {
        if (izquierda[i].getPrecio() <= derecha[j].getPrecio()) {
            v[k++] = izquierda[i++];
        } else {
            v[k++] = derecha[j++];
        }
    }
    while (i < n1) v[k++] = izquierda[i++];
    while (j < n2) v[k++] = derecha[j++];
}

void mergeSort(vector<Build> &v, int inicio, int fin) {
    if (inicio < fin) {
        int medio = inicio + (fin - inicio) / 2;
        mergeSort(v, inicio, medio);
        mergeSort(v, medio + 1, fin);
        merge(v, inicio, medio, fin);
    }
}

// =============================
// QUICKSORT por Score (rápido en promedio)
// =============================
static int particion(vector<Build> &v, int inicio, int fin) {
    double pivote = v[fin].getScore();
    int i = inicio - 1;

    for (int j = inicio; j < fin; j++) {
        if (v[j].getScore() <= pivote) {
            i++;
            swap(v[i], v[j]);
        }
    }

    swap(v[i + 1], v[fin]);
    return (i + 1);
}

void quickSort(vector<Build> &v, int inicio, int fin) {
    if (inicio < fin) {
        int pi = particion(v, inicio, fin);
        quickSort(v, inicio, pi - 1);
        quickSort(v, pi + 1, fin);
    }
}

// =============================
// HEAPSORT por Nombre de CPU (orden alfabético A–Z)
// =============================

static bool cpuLess(const Build& A, const Build& B) {
    // Compara directamente las cadenas (orden alfabético)
    string a = A.getCPU();
    string b = B.getCPU();

    // Convertimos ambas a minúsculas para comparación consistente
    transform(a.begin(), a.end(), a.begin(),
              [](unsigned char c){ return tolower(c); });
    transform(b.begin(), b.end(), b.begin(),
              [](unsigned char c){ return tolower(c); });

    return a < b;
}

// --- HeapSort ---
static void heapify(vector<Build> &v, int n, int i) {
    int mayor = i;
    int izq = 2 * i + 1;
    int der = 2 * i + 2;

    if (izq < n && cpuLess(v[mayor], v[izq]))
        mayor = izq;
    if (der < n && cpuLess(v[mayor], v[der]))
        mayor = der;

    if (mayor != i) {
        swap(v[i], v[mayor]);
        heapify(v, n, mayor);
    }
}

void heapSort(vector<Build> &v) {
    int n = static_cast<int>(v.size());
    if (n <= 1) return;

    // construir heap máximo
    for (int i = n / 2 - 1; i >= 0; --i)
        heapify(v, n, i);

    // extraer elementos
    for (int i = n - 1; i > 0; --i) {
        swap(v[0], v[i]);
        heapify(v, i, 0);
    }

    // invertir para obtener orden ascendente (A → Z)
    reverse(v.begin(), v.end());
}