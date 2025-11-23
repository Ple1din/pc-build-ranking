#ifndef CPU_H
#define CPU_H

#include "Component.h"
#include <sstream>
#include <iostream>
using namespace std;

class CPU : public Component {
private:
    string socket;
    int generacion;
    int nucleos;
    int hilos;
    int tdp;
    bool soporteDDR4;
    bool soporteDDR5;
    int pcieVersionCPU;
    bool soportaOC;

public:

    CPU(string nombre = "", string marca = "", string modelo = "",
        string socket = "", int nucleos = 6, int hilos = 12,
        int tdp = 65, bool soporteDDR4 = true, bool soporteDDR5 = false,
        int pcieVersionCPU = 4, bool soportaOC = false,
        double precio = 0, double score = 0)
        : Component("CPU", nombre, marca, modelo, "", precio, score),
          socket(socket), generacion(0),
          nucleos(nucleos), hilos(hilos), tdp(tdp),
          soporteDDR4(soporteDDR4), soporteDDR5(soporteDDR5),
          pcieVersionCPU(pcieVersionCPU), soportaOC(soportaOC)
    {
        inferirDatos();
    }



    // ============================================================
    //                      INFERENCIA INTEL / AMD
    // ============================================================
    void inferirDatos() {
        if (marca == "AMD")
            inferirAMD();
        else if (marca == "Intel")
            inferirIntel();
    }

    // ---------- AMD ----------
    void inferirAMD() {
        // modelo: "5600X", "5800H", etc.
        if (modelo.length() >= 4 && isdigit(modelo[0])) {
            int num = stoi(modelo.substr(0, 4));  // 5600 → 5600
            generacion = num / 1000;              // gen = 5
        } else generacion = 5;

        // Socket
        if (generacion <= 5) socket = "AM4";
        else socket = "AM5";

        // RAM
        if (socket == "AM4") {
            soporteDDR4 = true;
            soporteDDR5 = false;
        } else {
            soporteDDR4 = false;
            soporteDDR5 = true;
        }

        // PCIe
        if (generacion <= 5) pcieVersionCPU = 4;
        else pcieVersionCPU = 5;

        // OC soporte
        soportaOC = true;
    }

    // ---------- INTEL ----------
    void inferirIntel() {
        // Ej: i5-12400F → 12400 → gen 12
        string numeros = "";
        for (char c : modelo)
            if (isdigit(c)) numeros += c;

        if (numeros.size() >= 2)
            generacion = stoi(numeros.substr(0, 2));
        else generacion = 12;

        // Socket
        if (generacion <= 11)
            socket = "LGA1200";
        else
            socket = "LGA1700";

        // RAM
        if (socket == "LGA1200") {
            soporteDDR4 = true;
            soporteDDR5 = false;
        } else {
            soporteDDR4 = true;
            soporteDDR5 = true;
        }

        // PCIe
        if (generacion <= 11) pcieVersionCPU = 3;
        else pcieVersionCPU = 5;

        // OC en Intel solo en modelos que terminan en K
        soportaOC = (!modelo.empty() && modelo.back() == 'K');
    }



    // ============================================================
    //                      MOSTRAR INFO
    // ============================================================
    void mostrarInfo() const override {
        cout << "--- CPU ---\n";
        cout << "Nombre: " << nombre << "\n";
        cout << "Marca: " << marca << "\n";
        cout << "Modelo: " << modelo << "\n";
        cout << "Socket: " << socket << "\n";
        cout << "Generación: " << generacion << "\n";
        cout << "Núcleos: " << nucleos << "\n";
        cout << "Hilos: " << hilos << "\n";
        cout << "TDP: " << tdp << " W\n";
        cout << "DDR4: " << (soporteDDR4 ? "Sí" : "No") << "\n";
        cout << "DDR5: " << (soporteDDR5 ? "Sí" : "No") << "\n";
        cout << "PCIe versión: " << pcieVersionCPU << "\n";
        cout << "Soporta OC: " << (soportaOC ? "Sí" : "No") << "\n";
        cout << "Precio: $" << precio << "\n";
        cout << "Score: " << score << "\n";
    }

    // ============================================================
    //                         CSV
    // ============================================================
    string toCSV() const override {
        stringstream ss;
        ss << "CPU,"
           << nombre << ","
           << marca << ","
           << modelo << ","
           << socket << ","
           << generacion << ","
           << nucleos << ","
           << hilos << ","
           << tdp << ","
           << soporteDDR4 << ","
           << soporteDDR5 << ","
           << pcieVersionCPU << ","
           << soportaOC << ","
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
        getline(ss, socket, ',');

        getline(ss, temp, ','); generacion = stoi(temp);
        getline(ss, temp, ','); nucleos = stoi(temp);
        getline(ss, temp, ','); hilos = stoi(temp);
        getline(ss, temp, ','); tdp = stoi(temp);
        getline(ss, temp, ','); soporteDDR4 = stoi(temp);
        getline(ss, temp, ','); soporteDDR5 = stoi(temp);
        getline(ss, temp, ','); pcieVersionCPU = stoi(temp);
        getline(ss, temp, ','); soportaOC = stoi(temp);

        getline(ss, temp, ','); precio = stod(temp);
        getline(ss, temp, ','); score = stod(temp);

        inferirDatos();
    }

    // ============================================================
    //                       GETTERS
    // ============================================================
    string getSocket() const { return socket; }
    int getGeneracion() const { return generacion; }
    int getTDP() const { return tdp; }
    int getPCIeVersionCPU() const { return pcieVersionCPU; }
    bool soportaDDR4() const { return soporteDDR4; }
    bool soportaDDR5() const { return soporteDDR5; }
    bool getSoportaOC() const { return soportaOC; }
};

#endif
