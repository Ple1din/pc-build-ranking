/*
* Build.cpp
 * Implementación de la clase Build
 */

#include "Build.h"
#include <sstream>  // para leer/escribir CSV

// --- Constructor vacío ---
Build::Build() {
    nombre = "";
    cpu = "";
    gpu = "";
    ram = 0;
    precio = 0.0;
    score = 0.0;
}

// --- Constructor con parámetros ---
Build::Build(string n, string c, string g, int r, double p, double s) {
    nombre = n;
    cpu = c;
    gpu = g;
    ram = r;
    precio = p;
    score = s;
}

// --- Capturar datos desde consola ---
void Build::capturarDatos() {
    cout << "Nombre de la build: ";
    getline(cin, nombre);

    cout << "CPU (ej. Ryzen 5 5600): ";
    getline(cin, cpu);

    cout << "GPU (ej. RTX 3060): ";
    getline(cin, gpu);

    cout << "Cantidad de RAM (GB): ";
    cin >> ram;

    cout << "Precio total (en pesos): ";
    cin >> precio;

    cout << "Score de rendimiento (0-100): ";
    cin >> score;

    cin.ignore(); // limpiar buffer
}

// --- Mostrar información de la build ---
void Build::mostrarInfo() const {
    cout << "Nombre: " << nombre << endl;
    cout << "CPU: " << cpu << endl;
    cout << "GPU: " << gpu << endl;
    cout << "RAM: " << ram << " GB" << endl;
    cout << "Precio: $" << precio << endl;
    cout << "Score: " << score << endl;
}

// --- Convertir a línea CSV ---
string Build::toCSV() const {
    stringstream ss;
    ss << nombre << "," << cpu << "," << gpu << ","
       << ram << "," << precio << "," << score;
    return ss.str();
}

// --- Cargar datos desde una línea CSV ---
void Build::fromCSV(const string &linea) {
    stringstream ss(linea);
    string temp;

    getline(ss, nombre, ',');
    getline(ss, cpu, ',');
    getline(ss, gpu, ',');

    getline(ss, temp, ',');
    ram = stoi(temp);

    getline(ss, temp, ',');
    precio = stod(temp);

    getline(ss, temp, ',');
    score = stod(temp);
}
