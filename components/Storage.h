#ifndef STORAGE_H
#define STORAGE_H

#include "Component.h"
#include "CPU.h"
#include "Motherboard.h"
#include <iostream>
#include <sstream>



class Storage : public Component {
private:
    // Datos almacenados (CSV)
    string tipoStorage;     // HDD, SATA, M.2-SATA, M.2-NVMe
    int capacidadGB;
    int lecturaMBs;
    int escrituraMBs;
    int pcieVersion;        // Solo si NVMe (3,4,5). Para SSD/HDD = 0

public:

    Storage(string nombre = "", string marca = "", string modelo = "",
            string tipoStorage = "SATA", int capacidadGB = 256,
            int lecturaMBs = 500, int escrituraMBs = 450,
            int pcieVersion = 0,
            double precio = 0, double score = 0)
        : Component("Storage", nombre, marca, modelo, "", precio, score),
          tipoStorage(tipoStorage),
          capacidadGB(capacidadGB),
          lecturaMBs(lecturaMBs),
          escrituraMBs(escrituraMBs),
          pcieVersion(pcieVersion)
    {
        inferirDatos();
    }

    // ============================================================
    //               INFERIR TIPO, NVMe, VELOCIDADES
    // ============================================================
    void inferirDatos() {
        detectarTipo();
        inferirPCIe();
        inferirVelocidades();
    }

    // ---------------------- TIPO REAL ----------------------------
    void detectarTipo() {
        string n = nombre + " " + modelo;

        if (n.find("HDD") != string::npos || n.find("RPM") != string::npos) {
            tipoStorage = "HDD";
            return;
        }

        if (n.find("NVMe") != string::npos || n.find("Gen4") != string::npos ||
            n.find("Gen5") != string::npos || n.find("PCIe") != string::npos) {
            tipoStorage = "M.2-NVMe";
            return;
        }

        if (n.find("M.2") != string::npos) {
            tipoStorage = "M.2-SATA"; // Por descarte
            return;
        }

        if (n.find("SSD") != string::npos) {
            tipoStorage = "SATA";
            return;
        }
    }

    // ---------------------- PCIe VERSION -------------------------
    void inferirPCIe() {
        if (tipoStorage != "M.2-NVMe") {
            pcieVersion = 0;
            return;
        }

        string n = nombre + modelo;

        if (n.find("Gen5") != string::npos || n.find("990") != string::npos)
            pcieVersion = 5;
        else if (n.find("Gen4") != string::npos || n.find("980") != string::npos ||
                 n.find("SN770") != string::npos || n.find("SN850") != string::npos)
            pcieVersion = 4;
        else
            pcieVersion = 3; // por defecto
    }

    // ---------------------- VELOCIDADES --------------------------
    void inferirVelocidades() {
        if (lecturaMBs > 0 && escrituraMBs > 0) return;

        if (tipoStorage == "HDD") {
            lecturaMBs = 180;
            escrituraMBs = 160;
        }
        else if (tipoStorage == "SATA") {
            lecturaMBs = 550;
            escrituraMBs = 520;
        }
        else if (tipoStorage == "M.2-SATA") {
            lecturaMBs = 550;
            escrituraMBs = 520;
        }
        else if (tipoStorage == "M.2-NVMe") {
            if (pcieVersion == 3) {
                lecturaMBs = 3000;
                escrituraMBs = 2500;
            }
            else if (pcieVersion == 4) {
                lecturaMBs = 7000;
                escrituraMBs = 6000;
            }
            else if (pcieVersion == 5) {
                lecturaMBs = 10000;
                escrituraMBs = 9000;
            }
        }
    }

    // ============================================================
    //          COMPATIBILIDAD STORAGE ↔ MOTHERBOARD
    // ============================================================
    CompatResult compatMotherboard(const Motherboard* mb) const {
        if (!mb)
            return {false, false, "Motherboard nula"};

        bool bottleneck = false;
        string msg = "Compatible";

        // HDD y SSD SATA necesitan puertos SATA
        if (tipoStorage == "HDD" || tipoStorage == "SATA") {
            if (mb->getSataPorts() == 0)
                return {false, false, "Motherboard no tiene puertos SATA"};
        }

        // M.2-SATA necesita slot M.2 y compat SATA
        if (tipoStorage == "M.2-SATA") {
            if (mb->getM2Slots() == 0)
                return {false, false, "Motherboard no tiene slots M.2"};
            if (!mb->getM2SATACompat())
                return {false, false, "Slot M.2 no soporta SATA"};
        }

        // NVMe
        if (tipoStorage == "M.2-NVMe") {
            if (mb->getM2Slots() == 0)
                return {false, false, "Motherboard sin M.2 para NVMe"};

            // PCIe Gen bottleneck
            if (pcieVersion > mb->getM2PcieVersion()) {
                bottleneck = true;
                msg = "NVMe limitado por PCIe de la motherboard";
            }
        }

        return {true, bottleneck, msg};
    }

    // ============================================================
    //              COMPATIBILIDAD STORAGE ↔ CPU
    // ============================================================
    CompatResult compatCPU(const CPU* cpu) const {
        if (!cpu)
            return {false, false, "CPU nula"};

        if (tipoStorage != "M.2-NVMe")
            return {true, false, "Compatible"};

        bool bottleneck = false;
        string msg = "Compatible";

        // CPU limita NVMe si PCIe CPU < PCIe NVMe
        if (cpu->getPCIeVersionCPU() < pcieVersion) {
            bottleneck = true;
            msg = "NVMe limitado por PCIe del CPU";
        }

        return {true, bottleneck, msg};
    }

    // ============================================================
    //                     MOSTRAR INFORMACIÓN
    // ============================================================
    void mostrarInfo() const override {
        cout << "--- Storage ---\n";
        cout << "Nombre: " << nombre << "\n";
        cout << "Marca: " << marca << "\n";
        cout << "Modelo: " << modelo << "\n";
        cout << "Tipo: " << tipoStorage << "\n";
        cout << "Capacidad: " << capacidadGB << " GB\n";
        cout << "Lectura: " << lecturaMBs << " MB/s\n";
        cout << "Escritura: " << escrituraMBs << " MB/s\n";
        if (tipoStorage == "M.2-NVMe")
            cout << "PCIe: Gen " << pcieVersion << "\n";
        cout << "Precio: $" << precio << "\n";
        cout << "Score: " << score << "\n";
    }

    // ============================================================
    //                        CSV MINIMALISTA
    // ============================================================
    string toCSV() const override {
        stringstream ss;
        ss << "Storage,"
           << nombre << ","
           << marca << ","
           << modelo << ","
           << tipoStorage << ","
           << capacidadGB << ","
           << lecturaMBs << ","
           << escrituraMBs << ","
           << pcieVersion << ","
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
        getline(ss, tipoStorage, ',');

        getline(ss, temp, ','); capacidadGB = stoi(temp);
        getline(ss, temp, ','); lecturaMBs = stoi(temp);
        getline(ss, temp, ','); escrituraMBs = stoi(temp);
        getline(ss, temp, ','); pcieVersion = stoi(temp);

        getline(ss, temp, ','); precio = stod(temp);
        getline(ss, temp, ','); score = stod(temp);

        inferirDatos();
    }

    // ============================================================
    //                        GETTERS PRÁCTICOS
    // ============================================================
    string getTipoStorage() const { return tipoStorage; }
    int getCapacidad() const { return capacidadGB; }
    int getLectura() const { return lecturaMBs; }
    int getEscritura() const { return escrituraMBs; }
    int getPCIeVersion() const { return pcieVersion; }
};

#endif // STORAGE_H
