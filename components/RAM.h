#ifndef RAM_H
#define RAM_H

#include "Component.h"
#include "CPU.h"
#include "Motherboard.h"
#include <sstream>
#include <iostream>


class RAM : public Component {
private:
    // Datos mínimos necesarios (almacenados en CSV)
    string tipoRAM;        // DDR4 / DDR5
    int capacidadGB;       // Capacidad por módulo
    int frecuenciaMHz;     // Frecuencia efectiva

public:

    RAM(string nombre = "", string marca = "", string modelo = "",
        string tipoRAM = "DDR4", int capacidadGB = 8, int frecuenciaMHz = 2666,
        double precio = 0, double score = 0)
        : Component("RAM", nombre, marca, modelo, "", precio, score),
          tipoRAM(tipoRAM),
          capacidadGB(capacidadGB),
          frecuenciaMHz(frecuenciaMHz)
    {
        inferirDatos();
    }

    // ============================================================
    //                    INFERENCIAS SIMPLES
    // ============================================================
    void inferirDatos() {
        // Si el nombre dice DDR4 o DDR5, ajustamos tipoRAM
        if (nombre.find("DDR4") != string::npos) tipoRAM = "DDR4";
        if (nombre.find("DDR5") != string::npos) tipoRAM = "DDR5";

        // Si modelo contiene "3200" o "3600", ajustar frecuencia
        for (size_t i = 0; i < modelo.size(); i++) {
            if (isdigit(modelo[i])) {
                int num = stoi(modelo.substr(i));
                if (num >= 2000 && num <= 8000) {
                    frecuenciaMHz = num;
                    break;
                }
            }
        }
    }

    // ============================================================
    //               COMPATIBILIDAD RAM ↔ CPU
    // ============================================================
    CompatResult compatCPU(const CPU* cpu) const {
        if (!cpu)
            return {false, false, "CPU nula"};

        // Tipo RAM (DDR4 / DDR5)
        if (tipoRAM == "DDR4" && !cpu->soportaDDR4())
            return {false, false, "RAM DDR4 incompatible con CPU"};

        if (tipoRAM == "DDR5" && !cpu->soportaDDR5())
            return {false, false, "RAM DDR5 incompatible con CPU"};

        // Frecuencia
        bool bottleneck = false;
        string msg = "Compatible";

        // CPU no define frecuencia máxima exacta, pero podemos aplicar regla general
        // DDR4 CPU limit typical ≈ 3200–3600
        // DDR5 CPU limit typical ≈ 5600–6000

        if (tipoRAM == "DDR4" && frecuenciaMHz > 3600) {
            bottleneck = true;
            msg += " (frecuencia limitada por CPU)";
        }

        if (tipoRAM == "DDR5" && frecuenciaMHz > 6000) {
            bottleneck = true;
            msg += " (frecuencia limitada por CPU)";
        }

        return {true, bottleneck, msg};
    }

    // ============================================================
    //            COMPATIBILIDAD RAM ↔ MOTHERBOARD
    // ============================================================
    CompatResult compatMotherboard(const Motherboard* mb) const {
        if (!mb)
            return {false, false, "Motherboard nula"};

        // Tipo RAM debe coincidir EXACTO
        if (tipoRAM != mb->getTipoRAM())
            return {false, false, "RAM " + tipoRAM +
                                  " incompatible con motherboard (" +
                                  mb->getTipoRAM() + ")"};

        // Frecuencia soportada
        bool bottleneck = false;
        string msg = "Compatible";

        if (frecuenciaMHz > mb->getMaxFrecuenciaRAM()) {
            bottleneck = true;
            msg += " (frecuencia limitada por motherboard)";
        }

        return {true, bottleneck, msg};
    }

    // ============================================================
    //                  MOSTRAR INFORMACIÓN
    // ============================================================
    void mostrarInfo() const override {
        cout << "--- RAM ---\n";
        cout << "Nombre: " << nombre << "\n";
        cout << "Marca: " << marca << "\n";
        cout << "Modelo: " << modelo << "\n";
        cout << "Tipo: " << tipoRAM << "\n";
        cout << "Capacidad por módulo: " << capacidadGB << " GB\n";
        cout << "Frecuencia: " << frecuenciaMHz << " MHz\n";
        cout << "Precio: $" << precio << "\n";
        cout << "Score: " << score << "\n";
    }

    // ============================================================
    //                     CSV MINIMALISTA
    // ============================================================
    string toCSV() const override {
        stringstream ss;
        ss << "RAM,"
           << nombre << ","
           << marca << ","
           << modelo << ","
           << tipoRAM << ","
           << capacidadGB << ","
           << frecuenciaMHz << ","
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
        getline(ss, tipoRAM, ',');

        getline(ss, temp, ','); capacidadGB = stoi(temp);
        getline(ss, temp, ','); frecuenciaMHz = stoi(temp);
        getline(ss, temp, ','); precio = stod(temp);
        getline(ss, temp, ','); score = stod(temp);

        inferirDatos();
    }

    // ============================================================
    //        GETTERS (solo los necesarios para compatibilidad)
    // ============================================================
    string getTipoRAM() const { return tipoRAM; }
    int getCapacidadGB() const { return capacidadGB; }
    int getFrecuenciaMHz() const { return frecuenciaMHz; }
};

#endif // RAM_H
