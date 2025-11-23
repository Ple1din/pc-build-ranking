#ifndef PSU_H
#define PSU_H

#include "Component.h"
#include "CPU.h"
#include "GPU.h"
#include "Motherboard.h"
#include <iostream>
#include <sstream>



class PSU : public Component {
private:
    // Datos almacenados (CSV)
    int wattage;              // Potencia total
    string certificacion;     // 80+ Bronze/Gold/etc.
    string modularidad;       // Non, Semi, Full

    // Datos inferidos (no se guardan)
    double eficiencia;        // segun certificación (aprox)
    string calidadGeneral;    // High / Mid / Low

public:

    PSU(string nombre = "", string marca = "", string modelo = "",
        int wattage = 500, string certificacion = "80+ Bronze",
        string modularidad = "Non-Modular",
        double precio = 0, double score = 0)
        : Component("PSU", nombre, marca, modelo, "", precio, score),
          wattage(wattage),
          certificacion(certificacion),
          modularidad(modularidad)
    {
        inferirCalidad();
    }

    // ============================================================
    //                   INFERIR CALIDAD Y EFICIENCIA
    // ============================================================
    void inferirCalidad() {
        string n = nombre + " " + marca + " " + modelo;

        // Eficiencia (aproximación estándar)
        if (certificacion.find("Titanium") != string::npos) eficiencia = 0.94;
        else if (certificacion.find("Platinum") != string::npos) eficiencia = 0.92;
        else if (certificacion.find("Gold") != string::npos) eficiencia = 0.90;
        else if (certificacion.find("Silver") != string::npos) eficiencia = 0.87;
        else if (certificacion.find("Bronze") != string::npos) eficiencia = 0.85;
        else eficiencia = 0.80; // 80+ white o genéricas

        // Calidad basada en marca (simple pero útil)
        if (marca == "Corsair" || marca == "Seasonic" || marca == "EVGA" ||
            marca == "Cooler Master") {
            calidadGeneral = "Alta";
        }
        else if (marca == "Cougar" || marca == "Aerocool" || marca == "Gigabyte") {
            calidadGeneral = "Media";
        }
        else {
            calidadGeneral = "Baja"; // Marcas genéricas
        }
    }

    // ============================================================
    //                    COMPATIBILIDAD PSU ↔ CPU
    // ============================================================
    CompatResult compatCPU(const CPU* cpu) const {
        if (!cpu) return {false, false, "CPU nula"};

        int cpuTDP = cpu->getTDP(); // debes agregar getTDP y tdp en CPU.h
        if (cpuTDP == 0)
            return {true, false, "Compatible"};

        if (wattage < cpuTDP + 100)
            return {true, true, "PSU podría ser insuficiente para picos de carga del CPU"};

        return {true, false, "Compatible con CPU"};
    }

    // ============================================================
    //                    COMPATIBILIDAD PSU ↔ GPU
    // ============================================================
    CompatResult compatGPU(const GPU* gpu) const {
        if (!gpu) return {true, false, "Compatible"};

        int gpuW = gpu->getConsumoW();

        if (wattage < gpuW + 150)
            return {true, true, "PSU al límite para la GPU (sin margen seguro)"};

        return {true, false, "Compatible con GPU"};
    }

    // ============================================================
    //             COMPATIBILIDAD COMPLETA PARA BUILD
    // ============================================================
    CompatResult compatBuild(const CPU* cpu, const GPU* gpu, int otrosWatts) const {
        int cpuTDP = cpu ? cpu->getTDP() : 0;
        int gpuW   = gpu ? gpu->getConsumoW() : 0;

        int consumoTotal = cpuTDP + gpuW + otrosWatts;

        // Recomendación profesional: 30% de margen
        int recomendado = (int)(consumoTotal * 1.3);

        if (wattage < consumoTotal)
            return {false, true, "PSU insuficiente (no cubre el consumo total)"};

        if (wattage < recomendado)
            return {true, true, "PSU compatible, pero sin el margen recomendado"};

        return {true, false, "PSU adecuada con buen margen"};
    }

    // ============================================================
    //                   COMPATIBILIDAD MOTHERBOARD
    // ============================================================
    CompatResult compatMotherboard(const Motherboard* mb) const {
        if (!mb) return {true, false, "Compatible"};

        // PSU siempre compatible, pero con advertencias
        if (wattage < 400 && mb->getVRMQuality() == "Alta")
            return {true, true, "PSU muy baja para motherboard de gama alta"};

        return {true, false, "Compatible"};
    }

    // ============================================================
    //                        MOSTRAR INFO
    // ============================================================
    void mostrarInfo() const override {
        cout << "--- PSU ---\n";
        cout << "Nombre: " << nombre << "\n";
        cout << "Marca: " << marca << "\n";
        cout << "Modelo: " << modelo << "\n";
        cout << "Wattage: " << wattage << " W\n";
        cout << "Certificación: " << certificacion << "\n";
        cout << "Modularidad: " << modularidad << "\n";
        cout << "Calidad: " << calidadGeneral << "\n";
        cout << "Eficiencia: " << eficiencia * 100 << "%\n";
        cout << "Precio: $" << precio << "\n";
        cout << "Score: " << score << "\n";
    }

    // ============================================================
    //                        CSV MINIMALISTA
    // ============================================================
    string toCSV() const override {
        stringstream ss;
        ss << "PSU,"
           << nombre << ","
           << marca << ","
           << modelo << ","
           << wattage << ","
           << certificacion << ","
           << modularidad << ","
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

        getline(ss, temp, ','); wattage = stoi(temp);
        getline(ss, certificacion, ',');
        getline(ss, modularidad, ',');
        getline(ss, temp, ','); precio = stod(temp);
        getline(ss, temp, ','); score = stod(temp);

        inferirCalidad();
    }

    // ============================================================
    //                        GETTERS
    // ============================================================
    int getWattage() const { return wattage; }
    string getCertificacion() const { return certificacion; }
    string getModularidad() const { return modularidad; }
    double getEficiencia() const { return eficiencia; }
    string getCalidad() const { return calidadGeneral; }
};

#endif // PSU_H
