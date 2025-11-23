//
// Created by Yael on 23/11/2025.
//
// ---------------------------------------------------------
//                  UTILIDADES GENERALES
// ---------------------------------------------------------

#ifndef UTILS_H
#define UTILS_H

void crearCSVSiNoExiste(const string& filename) {
    // Intentar abrir para lectura
    std::ifstream f(filename);
    if (f.is_open()) {
        // Ya existe, no hacemos nada
        return;
    }
    f.close();

    // Intentar crearlo vacío
    std::ofstream out(filename);
    if (!out.is_open()) {
        cout << "Error: no se pudo crear el archivo '"
             << filename
             << "'. Verifica que exista la carpeta correspondiente (por ejemplo 'csv/').\n";
        return;
    }

    // Si quieres, aquí podrías escribir un encabezado, pero tú estás usando solo líneas de datos,
    // así que lo dejamos completamente vacío.
    out.close();

    cout << "Archivo '" << filename << "' no existía. Se creó vacío.\n";
}

void limpiarBuffer() {
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int pedirEntero(const string& mensaje, int minVal, int maxVal) {
    int opcion;
    while (true) {
        cout << mensaje;
        if (cin >> opcion) {
            if (opcion >= minVal && opcion <= maxVal) {
                limpiarBuffer();
                return opcion;
            }
        } else {
            cout << "Entrada inválida. Intenta de nuevo.\n";
            cin.clear();
        }
        limpiarBuffer();
    }
}

double pedirDouble(const string& mensaje, double minVal = 0.0) {
    double valor;
    while (true) {
        cout << mensaje;
        if (cin >> valor) {
            if (valor >= minVal) {
                limpiarBuffer();
                return valor;
            } else {
                cout << "Debe ser >= " << minVal << ".\n";
            }
        } else {
            cout << "Entrada inválida. Intenta de nuevo.\n";
            cin.clear();
        }
        limpiarBuffer();
    }
}

string pedirLinea(const string& mensaje) {
    cout << mensaje;
    string linea;
    std::getline(cin, linea);
    return linea;
}

void pausar() {
    cout << "\nPresiona ENTER para continuar...";
    std::cin.get();
}
#endif //UTILS_H
