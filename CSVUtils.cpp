/*
* CSVUtils.cpp
 * Implementación de las funciones para CSV
 */

#include "CSVUtils.h"
#include <fstream>   // ifstream y ofstream
#include <iostream>

using namespace std;

// Cargar builds desde el archivo CSV
void cargarDesdeCSV(const string &nombreArchivo, vector<Build> &builds) {
    ifstream archivo(nombreArchivo);

    if (!archivo.is_open()) {
        cout << "No se encontro el archivo " << nombreArchivo
             << ". Se creara cuando se agregue la primera build.\n";
        return; // Si no existe, simplemente salimos
    }

    string linea;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue; // Saltar lineas vacías

        Build b;
        b.fromCSV(linea);            // Llenar la build desde la linea
        builds.push_back(b);
    }

    archivo.close();
}

// Guardar nueva build al final del CSV
void guardarEnCSV(const string &nombreArchivo, const Build &build) {
    ofstream archivo(nombreArchivo, ios::app); // ios::app = agregar al final

    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo " << nombreArchivo << endl;
        return;
    }

    archivo << build.toCSV() << "\n"; // Guardamos en formato CSV
    archivo.close();
}