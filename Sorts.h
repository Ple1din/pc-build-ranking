/*
* Sorts.h
 * Algoritmos de ordenamiento para las builds
 */

#ifndef SORTS_H
#define SORTS_H

#include <vector>
#include "Build.h"

using namespace std;

// Ordenar por precio (MergeSort)
void mergeSort(vector<Build> &v, int inicio, int fin);

// Ordenar por score de rendimiento (QuickSort)
void quickSort(vector<Build> &v, int inicio, int fin);

// Ordenar por marca de CPU/GPU (BubbleSort)
void bubbleSort(vector<Build> &v);

#endif
