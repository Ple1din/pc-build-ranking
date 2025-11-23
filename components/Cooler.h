#ifndef COOLER_H
#define COOLER_H

#include "Component.h"
#include "CPU.h"
#include "Motherboard.h"
#include <iostream>
#include <sstream>



class Cooler : public Component {
private:
    // Datos almacenados (CSV)
    string tipoCooler;     // "Air", "AIO", "Custom"
    int tdpSoportado;      // 120W, 150W, 250W...
    int tamanoRadiador;    // 0 (si es Air), o 120/240/280/360
    int altura;            // altura en mm (solo para Air)

    // Datos inferidos (no se guardan)
    string socketSoportado;
    string calidadCooler;

public:

    Cooler(string nombre = "", string marca = "", string modelo = "",
           string tipoCooler = "Air", int tdpSoportado = 120,
           int tamanoRadiador = 0, int altura = 150,
           double precio = 0, double score = 0)
        : Component("Cooler", nombre, marca, modelo, "", precio, score),
          tipoCooler(tipoCooler),
          tdpSoportado(tdpSoportado),
          tamanoRadiador(tamanoRadiador),
          altura(altura)
    {
        inferirDatos();
    }

    // ============================================================
    //                  INFERIR SOCKET Y CALIDAD
    // ============================================================
    void inferirDatos() {
        inferirSocket();
        inferirCalidad();
    }

    void inferirSocket() {
        string n = nombre + " " + modelo;

        // Coolers más comunes soportan estos sockets
        if (n.find("AM4") != string::npos || n.find("AM5") != string::npos)
            socketSoportado = "AM4/AM5";
        else if (n.find("LGA1700") != string::npos)
            socketSoportado = "LGA1700";
        else if (n.find("LGA1200") != string::npos || n.find("1151") != string::npos)
            socketSoportado = "LGA1200/1151";
        else {
            // Caso estándar para coolers modernos
            socketSoportado = "AM4/AM5/LGA1700";
        }
    }

    void inferirCalidad() {
        if (marca == "Noctua")
            calidadCooler = "Premium";
        else if (marca == "Cooler Master" || marca == "DeepCool" || marca == "BeQuiet")
            calidadCooler = "Media-Alta";
        else if (marca == "AeroCool" || marca == "Gamdias")
            calidadCooler = "Media";
        else
            calidadCooler = "Baja";
    }

    // ============================================================
    //                    COMPATIBILIDAD ↔ CPU
    // ============================================================
    CompatResult compatCPU(const CPU* cpu) const {
        if (!cpu)
            return {false, false, "CPU nula"};

        string socCPU = cpu->getSocket();

        // Socket check (simple)
        if (socketSoportado.find(socCPU) == string::npos)
            return {false, false, "Socket incompatible con la CPU"};

        // TDP check
        int tdpCPU = cpu->getTDP();

        if (tdpCPU > tdpSoportado)
            return {false, false, "Cooler insuficiente para el TDP de la CPU"};

        if (tdpCPU > tdpSoportado * 0.8)
            return {true, true, "Compatible, pero con riesgo de altas temperaturas"};

        return {true, false, "Compatible"};
    }

    // ============================================================
    //                COMPATIBILIDAD ↔ MOTHERBOARD
    // ============================================================
    CompatResult compatMotherboard(const Motherboard* mb) const {
        if (!mb)
            return {true, false, "Compatible"};

        string socMB = mb->getSocket();

        if (socketSoportado.find(socMB) == string::npos)
            return {false, false, "Cooler incompatible con el socket del motherboard"};

        // Clearances (simples pero realistas)
        if (tipoCooler == "Air" && altura > 160 && mb->getFormFactor() == "mITX")
            return {true, true, "Cooler muy alto para Motherboard ITX"};

        return {true, false, "Compatible"};
    }

    // ============================================================
    //                     MOSTRAR INFO
    // ============================================================
    void mostrarInfo() const override {
        cout << "--- Cooler ---\n";
        cout << "Nombre: " << nombre << "\n";
        cout << "Marca: " << marca << "\n";
        cout << "Modelo: " << modelo << "\n";
        cout << "Tipo: " << tipoCooler << "\n";
        cout << "TDP soportado: " << tdpSoportado << " W\n";

        if (tipoCooler == "AIO")
            cout << "Radiador: " << tamanoRadiador << " mm\n";
        else
            cout << "Altura: " << altura << " mm\n";

        cout << "Socket: " << socketSoportado << "\n";
        cout << "Calidad: " << calidadCooler << "\n";
        cout << "Precio: $" << precio << "\n";
        cout << "Score: " << score << "\n";
    }

    // ============================================================
    //                        CSV MINIMALISTA
    // ============================================================
    string toCSV() const override {
        stringstream ss;
        ss << "Cooler,"
           << nombre << ","
           << marca << ","
           << modelo << ","
           << tipoCooler << ","
           << tdpSoportado << ","
           << tamanoRadiador << ","
           << altura << ","
           << precio << ","
           << score;
        return ss.str();
    }

    void fromCSV(const string& line) override {
        string temp;
        stringstream ss(line);

        getline(ss, categoria, ',');
        getline(ss, nombre, ',');
        getline(ss, marca, ',');
        getline(ss, modelo, ',');
        getline(ss, tipoCooler, ',');

        getline(ss, temp, ','); tdpSoportado = stoi(temp);
        getline(ss, temp, ','); tamanoRadiador = stoi(temp);
        getline(ss, temp, ','); altura = stoi(temp);
        getline(ss, temp, ','); precio = stod(temp);
        getline(ss, temp, ','); score = stod(temp);

        inferirDatos();
    }

    // ============================================================
    //                          GETTERS
    // ============================================================
    string getTipo() const { return tipoCooler; }
    int getTDPSoportado() const { return tdpSoportado; }
    int getAltura() const { return altura; }
    int getRadiador() const { return tamanoRadiador; }
    string getSocketSoportado() const { return socketSoportado; }
};

#endif // COOLER_H
