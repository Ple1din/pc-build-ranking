#ifndef CASE_H
#define CASE_H

#include "Component.h"
#include "GPU.h"
#include "Cooler.h"
#include "Motherboard.h"
#include <iostream>
#include <sstream>



class Case : public Component {
private:
    // Datos almacenados (CSV)
    string formFactorCase;   // ATX, mATX, ITX, FullTower
    int alturaMaxCoolerAir;  // mm
    int longitudMaxGPU;      // mm
    int radiadorTop;         // 0, 120, 240, 280
    int radiadorFront;       // 0, 240, 280, 360
    int radiadorRear;        // 0, 120

    // Datos inferidos (no guardados)
    string airflow;          // Alto / Medio / Bajo
    string calidadCase;      // Alta / Media / Baja

public:

    Case(string nombre = "", string marca = "", string modelo = "",
         string formFactor = "ATX",
         int alturaMaxCoolerAir = 160,
         int longitudMaxGPU = 330,
         int radiadorTop = 240,
         int radiadorFront = 360,
         int radiadorRear = 120,
         double precio = 0, double score = 0)
         : Component("Case", nombre, marca, modelo, "", precio, score),
           formFactorCase(formFactor),
           alturaMaxCoolerAir(alturaMaxCoolerAir),
           longitudMaxGPU(longitudMaxGPU),
           radiadorTop(radiadorTop),
           radiadorFront(radiadorFront),
           radiadorRear(radiadorRear)
    {
        inferirDatos();
    }

    // ============================================================
    //                 INFERIR AIRFLOW Y CALIDAD
    // ============================================================
    void inferirDatos() {
        string n = nombre + " " + modelo;

        // Airflow
        if (n.find("Mesh") != string::npos || n.find("Airflow") != string::npos)
            airflow = "Alto";
        else if (n.find("H510") != string::npos || n.find("Elite") != string::npos)
            airflow = "Bajo"; // NZXT clásica con mal airflow
        else
            airflow = "Medio";

        // Calidad
        if (marca == "NZXT" || marca == "Corsair" || marca == "Lian Li" || marca == "Phanteks")
            calidadCase = "Alta";
        else if (marca == "Cooler Master" || marca == "DeepCool" || marca == "Fractal Design")
            calidadCase = "Media";
        else
            calidadCase = "Baja";
    }

    // ============================================================
    //                COMPATIBILIDAD ↔ MOTHERBOARD
    // ============================================================
    CompatResult compatMotherboard(const Motherboard* mb) const {
        if (!mb)
            return {false, false, "Motherboard nula"};

        string f = mb->getFormFactor();

        if (formFactorCase == "ATX") {
            // ATX case soporta ATX, mATX, ITX
            return {true, false, "Compatible"};
        }

        if (formFactorCase == "mATX") {
            if (f == "ATX")
                return {false, false, "Case mATX no soporta motherboards ATX"};
            return {true, false, "Compatible"};
        }

        if (formFactorCase == "ITX") {
            if (f != "ITX")
                return {false, false, "Case ITX solo soporta motherboards ITX"};
            return {true, false, "Compatible"};
        }

        return {true, false, "Compatible"};
    }

    // ============================================================
    //                COMPATIBILIDAD ↔ GPU
    // ============================================================
    CompatResult compatGPU(const GPU* gpu) const {
        if (!gpu)
            return {true, false, "Compatible"};

        // Podrías definir largo según modelo (opcional)
        // Por ahora, se pide al usuario el largo correcto.
        int largoGPU = gpu->getScore();
        // NOTA: REEMPLAZA getScore() por getLongitudGPU() cuando lo agreguemos

        // Pero por ahora como placeholder:
        largoGPU = 300; // suposición estándar (puedes cambiarlo)

        if (largoGPU > longitudMaxGPU)
            return {false, false, "La GPU no cabe en este case"};

        if (largoGPU > longitudMaxGPU - 10)
            return {true, true, "La GPU cabe muy justa, airflow comprometido"};

        return {true, false, "Compatible"};
    }

    // ============================================================
    //                COMPATIBILIDAD ↔ COOLER
    // ============================================================
    CompatResult compatCooler(const Cooler* cooler) const {
        if (!cooler)
            return {true, false, "Compatible"};

        if (cooler->getTipo() == "Air") {
            if (cooler->getAltura() > alturaMaxCoolerAir)
                return {false, false, "El cooler de aire no cabe por altura"};

            if (cooler->getAltura() > alturaMaxCoolerAir - 5)
                return {true, true, "Cooler muy justo, posible problema de cierre de panel"};

            return {true, false, "Compatible"};
        }

        // AIO
        int rad = cooler->getRadiador();

        if (rad == 0)
            return {true, false, "Compatible"};

        // Verificar radiador
        if (rad == radiadorTop || rad == radiadorFront || rad == radiadorRear)
            return {true, false, "Compatible"};

        return {false, false, "Radiador del AIO no soportado por el case"};
    }

    // ============================================================
    //                       MOSTRAR INFO
    // ============================================================
    void mostrarInfo() const override {
        cout << "--- Case ---\n";
        cout << "Nombre: " << nombre << "\n";
        cout << "Marca: " << marca << "\n";
        cout << "Modelo: " << modelo << "\n";
        cout << "Factor de forma: " << formFactorCase << "\n";
        cout << "Altura máx cooler aire: " << alturaMaxCoolerAir << " mm\n";
        cout << "Longitud máx GPU: " << longitudMaxGPU << " mm\n";
        cout << "Radiador TOP: " << radiadorTop << " mm\n";
        cout << "Radiador FRONT: " << radiadorFront << " mm\n";
        cout << "Radiador REAR: " << radiadorRear << " mm\n";
        cout << "Airflow: " << airflow << "\n";
        cout << "Calidad del case: " << calidadCase << "\n";
        cout << "Precio: $" << precio << "\n";
        cout << "Score: " << score << "\n";
    }

    // ============================================================
    //                          CSV
    // ============================================================
    string toCSV() const override {
        stringstream ss;
        ss << "Case,"
           << nombre << ","
           << marca << ","
           << modelo << ","
           << formFactorCase << ","
           << alturaMaxCoolerAir << ","
           << longitudMaxGPU << ","
           << radiadorTop << ","
           << radiadorFront << ","
           << radiadorRear << ","
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
        getline(ss, formFactorCase, ',');

        getline(ss, temp, ','); alturaMaxCoolerAir = stoi(temp);
        getline(ss, temp, ','); longitudMaxGPU = stoi(temp);
        getline(ss, temp, ','); radiadorTop = stoi(temp);
        getline(ss, temp, ','); radiadorFront = stoi(temp);
        getline(ss, temp, ','); radiadorRear = stoi(temp);

        getline(ss, temp, ','); precio = stod(temp);
        getline(ss, temp, ','); score = stod(temp);

        inferirDatos();
    }

    // ============================================================
    //                         GETTERS
    // ============================================================
    string getFormFactor() const { return formFactorCase; }
    int getAlturaMaxCooler() const { return alturaMaxCoolerAir; }
    int getLongitudMaxGPU() const { return longitudMaxGPU; }
    int getRadiadorTop() const { return radiadorTop; }
    int getRadiadorFront() const { return radiadorFront; }
    int getRadiadorRear() const { return radiadorRear; }
    string getAirflow() const { return airflow; }
    string getCalidad() const { return calidadCase; }
};

#endif // CASE_H
