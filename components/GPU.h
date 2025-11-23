#ifndef GPU_H
#define GPU_H

#include "Component.h"
#include "CPU.h"
#include "Motherboard.h"
#include <iostream>
#include <sstream>
#include <cctype>

class GPU : public Component {
private:
    // Datos CSV
    int vramGB;
    string tipoVRAM;
    int consumoW;

    // Datos inferidos
    int pcieVersionGPU;
    string arquitectura;
    string familia;
    string sufijoGPU;

public:

    GPU(string nombre = "", string marca = "", string modelo = "",
        int vramGB = 8, string tipoVRAM = "GDDR6", int consumoW = 150,
        double precio = 0, double score = 0)
        : Component("GPU", nombre, marca, modelo, "", precio, score),
          vramGB(vramGB), tipoVRAM(tipoVRAM), consumoW(consumoW),
          pcieVersionGPU(3), arquitectura(""), familia(""), sufijoGPU("")
    {
        inferirDatos();
    }

    // ============================================================
    //      UTILIDAD SEGURA PARA EXTRAER NÚMEROS DE UNA CADENA
    // ============================================================
    int safeToInt(const string& s) const {
        string digits = "";
        for (char c : s)
            if (isdigit(c)) digits += c;

        if (digits.empty()) return 0;
        return stoi(digits);
    }

    // ============================================================
    //                 INFERENCIA COMPLETA DE GPU
    // ============================================================
    void inferirDatos() {
        detectarMarca();
        extraerSufijo();
        inferirArquitectura();
        inferirPCIe();
    }

    // ------------------ Detectar marca ------------------
    void detectarMarca() {
        string n = nombre;

        if (marca == "NVIDIA" || n.find("RTX") != string::npos || n.find("GTX") != string::npos)
            marca = "NVIDIA";

        else if (marca == "AMD" || n.find("RX") != string::npos || n.find("Radeon") != string::npos)
            marca = "AMD";

        else if (marca == "Intel" || n.find("ARC") != string::npos)
            marca = "Intel";
    }

    // ------------------ Sufijos GPU ------------------
    void extraerSufijo() {
        string n = nombre;

        if (n.find("SUPER") != string::npos) sufijoGPU = "SUPER";
        else if (n.find("Ti") != string::npos) sufijoGPU = "Ti";
        else if (n.find("XT") != string::npos) sufijoGPU = "XT";
        else if (n.find("XTX") != string::npos) sufijoGPU = "XTX";
        else sufijoGPU = "";
    }

    // ------------------ Arquitectura ------------------
    void inferirArquitectura() {
        int num = safeToInt(modelo);

        if (marca == "NVIDIA") {
            if (num < 1000) { familia = "GT";      arquitectura = "Kepler/Maxwell"; }
            else if (num < 2000) { familia = "GTX"; arquitectura = "Pascal"; }
            else if (num < 3000) { familia = "RTX 2000"; arquitectura = "Turing"; }
            else if (num < 4000) { familia = "RTX 3000"; arquitectura = "Ampere"; }
            else { familia = "RTX 4000/5000"; arquitectura = "Ada Lovelace"; }
        }

        else if (marca == "AMD") {
            if (num < 5000) { familia = "RX 400/500"; arquitectura = "Polaris"; }
            else if (num < 6000) { familia = "RX 5000"; arquitectura = "RDNA1"; }
            else if (num < 7000) { familia = "RX 6000"; arquitectura = "RDNA2"; }
            else { familia = "RX 7000"; arquitectura = "RDNA3"; }
        }

        else if (marca == "Intel") {
            familia = "ARC A-Series";
            arquitectura = "Alchemist";
        }
    }

    // ------------------ PCIe ------------------
    void inferirPCIe() {
        int num = safeToInt(modelo);

        if (marca == "NVIDIA") {
            if (num < 2000) pcieVersionGPU = 3;
            else if (num < 3000) pcieVersionGPU = 3;
            else pcieVersionGPU = 4;
        }

        else if (marca == "AMD") {
            if (num < 5000) pcieVersionGPU = 3;
            else pcieVersionGPU = 4;
        }

        else if (marca == "Intel") {
            pcieVersionGPU = 4;
        }
    }

    // ============================================================
    //                     COMPATIBILIDAD
    // ============================================================
    CompatResult compatCPU(const CPU* cpu) const {
        if (!cpu) return {false, false, "CPU nula"};

        bool bottleneck = false;
        string msg = "Compatible";

        if (score > cpu->getScore() * 2) {
            bottleneck = true;
            msg = "CPU hace cuello de botella";
        }

        if (cpu->getScore() > score * 1.5) {
            bottleneck = true;
            msg = "GPU hace cuello de botella";
        }

        return {true, bottleneck, msg};
    }

    CompatResult compatMotherboard(const Motherboard* mb) const {
        if (!mb) return {false, false, "Motherboard nula"};

        bool bottleneck = false;
        string msg = "Compatible";

        if (pcieVersionGPU > mb->getPCIeVersion()) {
            bottleneck = true;
            msg = "Compatible con cuello de botella PCIe";
        }

        return {true, bottleneck, msg};
    }

    // ============================================================
    //                        MOSTRAR INFO
    // ============================================================
    void mostrarInfo() const override {
        cout << "--- GPU ---\n";
        cout << "Nombre: " << nombre << "\n";
        cout << "Marca: " << marca << "\n";
        cout << "Modelo: " << modelo << " " << sufijoGPU << "\n";
        cout << "VRAM: " << vramGB << " GB " << tipoVRAM << "\n";
        cout << "Consumo: " << consumoW << " W\n";
        cout << "Arquitectura: " << arquitectura << "\n";
        cout << "PCIe: Gen " << pcieVersionGPU << "\n";
        cout << "Precio: $" << precio << "\n";
        cout << "Score: " << score << "\n";
    }

    // ============================================================
    //                          CSV
    // ============================================================
    string toCSV() const override {
        stringstream ss;
        ss << "GPU,"
           << nombre << ","
           << marca << ","
           << modelo << ","
           << vramGB << ","
           << tipoVRAM << ","
           << consumoW << ","
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

        getline(ss, temp, ','); vramGB = stoi(temp);
        getline(ss, tipoVRAM, ',');
        getline(ss, temp, ','); consumoW = stoi(temp);
        getline(ss, temp, ','); precio = stod(temp);
        getline(ss, temp, ','); score = stod(temp);

        inferirDatos();
    }

    // GETTERS
    int getVRAM() const { return vramGB; }
    int getConsumoW() const { return consumoW; }
    int getPCIeVersionGPU() const { return pcieVersionGPU; }
    string getFamilia() const { return familia; }
    string getArquitectura() const { return arquitectura; }
};

#endif // GPU_H
