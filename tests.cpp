/*
 * tests.cpp
 * Casos de prueba para el proyecto de Ranking de PC Builds
 *
 * Objetivo:
 *  - Probar que las funciones de la clase Build,
 *    el manejo de CSV y los algoritmos de ordenamiento
 *    funcionan correctamente.
 */

#include <iostream>
#include <vector>
#include <string>
#include "Build.h"
#include "CSVUtils.h"
#include "Sorts.h"

using namespace std;

// Función auxiliar para mostrar builds
void mostrarVector(const vector<Build> &v) {
    for (const auto &b : v) {
        b.mostrarInfo();
        cout << "----------------------\n";
    }
}

int main() {
    cout << "===== PRUEBAS DEL PROYECTO =====\n";

    // 1. Crear builds manualmente
    vector<Build> builds;
    builds.push_back(Build("Gaming Entry", "AMD Ryzen 5 5600", "RTX 3060", 16, 15000, 75));
    builds.push_back(Build("Edicion Pro", "Intel i7 12700", "RTX 4070", 32, 28000, 90));
    builds.push_back(Build("Oficina", "Intel i5 10400", "GTX 1650", 8, 12000, 55));
    builds.push_back(Build("Streaming", "AMD Ryzen 7 5800X", "RTX 4060 Ti", 16, 22000, 82));

    cout << "\n--- Builds iniciales ---\n";
    mostrarVector(builds);

    // 2. Probar MergeSort por precio
    cout << "\n--- Ordenadas por precio (MergeSort) ---\n";
    mergeSort(builds, 0, builds.size() - 1);
    mostrarVector(builds);

    // 3. Probar QuickSort por score
    cout << "\n--- Ordenadas por score (QuickSort) ---\n";
    quickSort(builds, 0, builds.size() - 1);
    mostrarVector(builds);

    // 4. Probar BubbleSort por marca de CPU
    cout << "\n--- Ordenadas por marca de CPU (BubbleSort) ---\n";
    bubbleSort(builds);
    mostrarVector(builds);

    // 5. Probar escritura en CSV
    cout << "\n--- Guardando builds en archivo CSV ---\n";
    for (const auto &b : builds) {
        guardarEnCSV("builds.csv", b);
    }
    cout << "Se han guardado las builds en builds.csv\n";

    // 6. Probar lectura desde CSV
    cout << "\n--- Cargando builds desde archivo CSV ---\n";
    vector<Build> buildsCargadas;
    cargarDesdeCSV("builds.csv", buildsCargadas);
    mostrarVector(buildsCargadas);

    cout << "\n===== FIN DE LAS PRUEBAS =====\n";
    return 0;
}
