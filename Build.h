/*
* Build.h
 * Clase que representa una Build de PC
 */

#ifndef BUILD_H
#define BUILD_H

#include <iostream>
#include <string>

using namespace std;

class Build {
private:
    string nombre;
    string cpu;
    string gpu;
    int ram;         // en GB
    double precio;   // en pesos
    double score;    // puntuacion de rendimiento

public:
    // --- Constructores ---
    Build();  // constructor vacío
    Build(string n, string c, string g, int r, double p, double s);

    // --- Métodos para capturar y mostrar datos ---
    void capturarDatos();
    void mostrarInfo() const;

    // --- Getters (necesarios para ordenar) ---
    double getPrecio() const { return precio; }
    double getScore() const { return score; }
    string getCPU() const { return cpu; }
    string getGPU() const { return gpu; }

    // --- Métodos para CSV ---
    string toCSV() const;
    void fromCSV(const string &linea);
};

#endif
