/*
 * Proyecto: Ranking de PC Builds
 * En este programa podemos registrar builds de PC, verlas y ordenarlas por diferentes criterios usando algoritmos vistos en clase.
 */

#include <iostream>
#include <vector>
#include <string>
#include "Build.h"      // Clase Build
#include "CSVUtils.h"   // Funciones para leer y escribir CSV
#include "Sorts.h"      // Algoritmos de ordenamiento

using namespace std;

// --- Función para mostrar el menú ---
void mostrarMenu() {
    cout << "\n===== MENU PRINCIPAL =====\n";
    cout << "1. Ver todas las builds\n";
    cout << "2. Agregar una nueva build\n";
    cout << "3. Ordenar por precio (MergeSort, O(n log n))\n";
    cout << "4. Ordenar por score de rendimiento (QuickSort, O(n log n) promedio)\n";
    cout << "5. Ordenar por marca de CPU (HeapSort, O(n log n))\n";
    cout << "0. Salir\n";
    cout << "Seleccione una opcion: ";
}

int main() {
    vector<Build> builds;

    // Cargar builds que ya existan desde el CSV
    cargarDesdeCSV("builds.csv", builds);

    int opcion;

    do {
        mostrarMenu();
        cin >> opcion;
        cin.ignore(); // limpiar el enter que deja cin

        switch (opcion) {
        case 1:
            cout << "\n--- Lista de Builds ---\n";
            if (builds.empty()) {
                cout << "No hay builds registradas.\n";
            } else {
                for (const auto &b : builds) {
                    b.mostrarInfo();
                    cout << "------------------------\n";
                }
            }
            break;

        case 2: {
            Build nueva;
            cout << "\n--- Registrar nueva Build ---\n";
            nueva.capturarDatos();             // Captura desde teclado
            builds.push_back(nueva);           // Se agrega al vector
            guardarEnCSV("builds.csv", nueva); // Se guarda en el CSV
            cout << ">> Build agregada correctamente.\n";
            break;
        }

        case 3:
            if (builds.size() > 1) {
                mergeSort(builds, 0, builds.size() - 1); // Ordena por precio
                cout << ">> Builds ordenadas por precio.\n";
            } else {
                cout << "No hay suficientes builds para ordenar.\n";
            }
            break;

        case 4:
            if (builds.size() > 1) {
                quickSort(builds, 0, builds.size() - 1); // Ordena por score
                cout << ">> Builds ordenadas por score.\n";
            } else {
                cout << "No hay suficientes builds para ordenar.\n";
            }
            break;

        case 5:
            if (builds.size() > 1) {
                heapSort(builds); // Ordena por marca de CPU con HeapSort
                cout << ">> Builds ordenadas por marca de CPU (HeapSort, O(n log n)).\n";
            } else {
                cout << "No hay suficientes builds para ordenar.\n";
            }
            break;

        case 0:
            cout << "Saliendo del programa...\n";
            break;

        default:
            cout << "Opcion invalida. Intente de nuevo.\n";
        }

    } while (opcion != 0);

    return 0;
}
