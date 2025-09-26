/*
 * Sorts.cpp
 * Implementación de los algoritmos de ordenamiento
 */

#include "Sorts.h"
#include <string>

using namespace std;

// MergeSort por Precio
void merge(vector<Build> &v, int inicio, int medio, int fin) {
    int n1 = medio - inicio + 1;
    int n2 = fin - medio;

    vector<Build> izquierda(n1);
    vector<Build> derecha(n2);

    // Copiar datos a subarreglos
    for (int i = 0; i < n1; i++)
        izquierda[i] = v[inicio + i];

    for (int j = 0; j < n2; j++)
        derecha[j] = v[medio + 1 + j];

    // Mezclar los subarreglos
    int i = 0, j = 0, k = inicio;
    while (i < n1 && j < n2) {
        if (izquierda[i].getPrecio() <= derecha[j].getPrecio()) {
            v[k] = izquierda[i];
            i++;
        } else {
            v[k] = derecha[j];
            j++;
        }
        k++;
    }

    // Copiar elementos restantes
    while (i < n1) {
        v[k] = izquierda[i];
        i++;
        k++;
    }

    while (j < n2) {
        v[k] = derecha[j];
        j++;
        k++;
    }
}

void mergeSort(vector<Build> &v, int inicio, int fin) {
    if (inicio < fin) {
        int medio = inicio + (fin - inicio) / 2;
        mergeSort(v, inicio, medio);
        mergeSort(v, medio + 1, fin);
        merge(v, inicio, medio, fin);
    }
}

// QuickSort por Score
int particion(vector<Build> &v, int inicio, int fin) {
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

//BubbleSort por Marca
void bubbleSort(vector<Build> &v) {
    int n = v.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            // Compara la marca de CPU (alfabéticamente)
            if (v[j].getCPU() > v[j + 1].getCPU()) {
                swap(v[j], v[j + 1]);
            }
        }
    }
}
