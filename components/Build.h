#ifndef BUILD_H
#define BUILD_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "Component.h"
#include "CPU.h"
#include "GPU.h"
#include "Motherboard.h"
#include "RAM.h"
#include "Storage.h"
#include "PSU.h"
#include "Cooler.h"
#include "Case.h"

using namespace std;

/*
    Clase Build:
    Representa una PC completa armada a partir de componentes.
    No es dueña de los componentes (no hace delete), solo guarda punteros.
*/
class Build {
private:
    string nombre;

    CPU* cpu;
    Motherboard* motherboard;
    GPU* gpu;
    vector<RAM*> rams;
    vector<Storage*> storages;
    PSU* psu;
    Cooler* cooler;
    Case* casePC;

    double precioTotal;
    double scoreTotal;

public:
    // ---------------------------------------------------------
    //                  CONSTRUCTORES
    // ---------------------------------------------------------
    Build(string nombre = "",
          CPU* cpu = nullptr,
          Motherboard* motherboard = nullptr,
          GPU* gpu = nullptr,
          PSU* psu = nullptr,
          Cooler* cooler = nullptr,
          Case* casePC = nullptr);

    // Agregar módulos de RAM y unidades de almacenamiento
    void addRAM(RAM* ram);
    void addStorage(Storage* storage);

    // Recalcular precioTotal y scoreTotal
    void recalcularTotales();

    // Validar compatibilidad entre TODOS los componentes
    // Devuelve una lista de resultados (errores + cuellos de botella)
    vector<CompatResult> validarCompatibilidad() const;

    // Mostrar resumen y detalle
    void mostrarResumen() const;
    void mostrarDetalle() const;

    // Getters para ordenamiento
    double getPrecio() const { return precioTotal; }
    double getScore() const { return scoreTotal; }
    string getNombre() const { return nombre; }

    // CSV de builds (no guarda todos los componentes, solo resumen)
    string toCSV() const;
    void fromCSV(const string& linea);

    // Getters de componentes (útiles para GUI o lógica extra)
    CPU* getCPU() const { return cpu; }
    Motherboard* getMotherboard() const { return motherboard; }
    GPU* getGPU() const { return gpu; }
    const vector<RAM*>& getRAMModules() const { return rams; }
    const vector<Storage*>& getStorages() const { return storages; }
    PSU* getPSU() const { return psu; }
    Cooler* getCooler() const { return cooler; }
    Case* getCase() const { return casePC; }

    // Setters básicos por si quieres construir la build por partes
    void setCPU(CPU* c) { cpu = c; }
    void setMotherboard(Motherboard* m) { motherboard = m; }
    void setGPU(GPU* g) { gpu = g; }
    void setPSU(PSU* p) { psu = p; }
    void setCooler(Cooler* c) { cooler = c; }
    void setCase(Case* c) { casePC = c; }
};

// =========================================================
//          IMPLEMENTACIÓN INLINE (HEADER-ONLY)
// =========================================================

// ---------------------------------------------------------
//                  CONSTRUCTOR
// ---------------------------------------------------------
inline Build::Build(string nombre,
                    CPU* cpu,
                    Motherboard* motherboard,
                    GPU* gpu,
                    PSU* psu,
                    Cooler* cooler,
                    Case* casePC)
    : nombre(nombre),
      cpu(cpu),
      motherboard(motherboard),
      gpu(gpu),
      psu(psu),
      cooler(cooler),
      casePC(casePC),
      precioTotal(0.0),
      scoreTotal(0.0)
{
    recalcularTotales();
}

// ---------------------------------------------------------
//          AGREGAR RAM Y STORAGE
// ---------------------------------------------------------
inline void Build::addRAM(RAM* ram) {
    if (ram) {
        rams.push_back(ram);
        recalcularTotales();
    }
}

inline void Build::addStorage(Storage* storage) {
    if (storage) {
        storages.push_back(storage);
        recalcularTotales();
    }
}

// ---------------------------------------------------------
//          RECALCULAR PRECIO Y SCORE
// ---------------------------------------------------------
inline void Build::recalcularTotales() {
    precioTotal = 0.0;
    scoreTotal  = 0.0;

    int contribScore = 0;

    auto addComp = [&](Component* c, double pesoScore = 1.0) {
        if (!c) return;
        precioTotal += c->getPrecio();
        scoreTotal  += c->getScore() * pesoScore;
        contribScore++;
    };

    // CPU y GPU tienen más impacto, puedes ajustar pesos si quieres
    if (cpu) {
        precioTotal += cpu->getPrecio();
        scoreTotal  += cpu->getScore() * 1.0;
        contribScore++;
    }

    if (gpu) {
        precioTotal += gpu->getPrecio();
        scoreTotal  += gpu->getScore() * 1.0;
        contribScore++;
    }

    if (motherboard) addComp(motherboard, 0.3);
    if (psu)         addComp(psu, 0.2);
    if (cooler)      addComp(cooler, 0.2);
    if (casePC)      addComp(casePC, 0.1);

    for (auto* r : rams)      addComp(r, 0.4);
    for (auto* s : storages) addComp(s, 0.3);

    if (contribScore > 0)
        scoreTotal /= contribScore;
}

// ---------------------------------------------------------
//          VALIDAR COMPATIBILIDAD COMPLETA
// ---------------------------------------------------------
inline vector<CompatResult> Build::validarCompatibilidad() const {
    vector<CompatResult> resultados;

    auto agregar = [&](const string& prefijo, const CompatResult& r) {
        if (!r.compatible || r.bottleneck) {
            CompatResult rr = r;
            rr.message = prefijo + rr.message;
            resultados.push_back(rr);
        }
    };

    // CPU ↔ Motherboard
    if (cpu && motherboard) {
        agregar("CPU - Motherboard: ", motherboard->compatCPU(cpu));
    }

    // RAM ↔ CPU y RAM ↔ Motherboard
    for (auto* ram : rams) {
        if (cpu)         agregar("RAM - CPU: ", ram->compatCPU(cpu));
        if (motherboard) agregar("RAM - Motherboard: ", ram->compatMotherboard(motherboard));
    }

    // GPU ↔ CPU y GPU ↔ Motherboard
    if (gpu) {
        if (cpu)         agregar("GPU - CPU: ", gpu->compatCPU(cpu));
        if (motherboard) agregar("GPU - Motherboard: ", gpu->compatMotherboard(motherboard));
    }

    // Storage ↔ Motherboard y Storage ↔ CPU (NVMe)
    for (auto* st : storages) {
        if (motherboard) agregar("Storage - Motherboard: ", st->compatMotherboard(motherboard));
        if (cpu)         agregar("Storage - CPU: ", st->compatCPU(cpu));
    }

    // PSU ↔ CPU, GPU, Motherboard, Build
    if (psu) {
        if (cpu)         agregar("PSU - CPU: ", psu->compatCPU(cpu));
        if (gpu)         agregar("PSU - GPU: ", psu->compatGPU(gpu));
        if (motherboard) agregar("PSU - Motherboard: ", psu->compatMotherboard(motherboard));

        // Calcular consumo aproximado para informar a la PSU
        int otrosWatts = 0;
        if (motherboard) otrosWatts += 50;
        if (cooler)      otrosWatts += 10;
        if (casePC)      otrosWatts += 5;
        otrosWatts += static_cast<int>(rams.size()) * 5;
        otrosWatts += static_cast<int>(storages.size()) * 5;

        agregar("PSU - Build: ", psu->compatBuild(cpu, gpu, otrosWatts));
    }

    // Cooler ↔ CPU y Cooler ↔ Motherboard
    if (cooler) {
        if (cpu)         agregar("Cooler - CPU: ", cooler->compatCPU(cpu));
        if (motherboard) agregar("Cooler - Motherboard: ", cooler->compatMotherboard(motherboard));
    }

    // Case ↔ Motherboard, GPU, Cooler
    if (casePC) {
        if (motherboard) agregar("Case - Motherboard: ", casePC->compatMotherboard(motherboard));
        if (gpu)         agregar("Case - GPU: ", casePC->compatGPU(gpu));
        if (cooler)      agregar("Case - Cooler: ", casePC->compatCooler(cooler));
    }

    return resultados;
}

// ---------------------------------------------------------
//          MOSTRAR RESUMEN Y DETALLE
// ---------------------------------------------------------
inline void Build::mostrarResumen() const {
    cout << "=== Build: " << nombre << " ===\n";
    cout << "Precio total: $" << precioTotal << "\n";
    cout << "Score total: " << scoreTotal << "\n";

    cout << "CPU: "     << (cpu         ? cpu->getNombre()         : "N/A") << "\n";
    cout << "GPU: "     << (gpu         ? gpu->getNombre()         : "N/A") << "\n";
    cout << "MB: "      << (motherboard ? motherboard->getModelo() : "N/A") << "\n";
    cout << "RAM: "     << rams.size() << " modulo(s)\n";
    cout << "Storage: " << storages.size() << " unidad(es)\n";
    cout << "PSU: "     << (psu         ? psu->getNombre()         : "N/A") << "\n";
    cout << "Cooler: "  << (cooler      ? cooler->getNombre()      : "N/A") << "\n";
    cout << "Case: "    << (casePC      ? casePC->getNombre()      : "N/A") << "\n";
}

inline void Build::mostrarDetalle() const {
    mostrarResumen();
    cout << "\n--- Compatibilidad ---\n";
    auto res = validarCompatibilidad();
    if (res.empty()) {
        cout << "Sin problemas detectados.\n";
    } else {
        for (const auto& r : res) {
            cout << (r.compatible ? "[OK] " : "[X] ")
                 << (r.bottleneck ? "(Bottleneck) " : "")
                 << r.message << "\n";
        }
    }
    cout << "----------------------\n";
}

// ---------------------------------------------------------
//          CSV: GUARDAR Y CARGAR BUILDS
// ---------------------------------------------------------
//
// Formato simple (no guarda todos los detalles de componentes):
// Build,<nombre>,<precioTotal>,<scoreTotal>
//
inline string Build::toCSV() const {
    // Une nombres de componentes en un solo string separados por '|'
    auto joinNombres = [](const auto& vec) {
        string result;
        for (size_t i = 0; i < vec.size(); ++i) {
            if (i > 0) result += '|';
            if (vec[i]) result += vec[i]->getNombre();
        }
        return result;
    };

    string ramsStr    = joinNombres(rams);
    string storesStr  = joinNombres(storages);

    stringstream ss;
    ss << "Build,"
       << nombre << ","
       << precioTotal << ","
       << scoreTotal << ","
       // Componentes principales (pueden ir vacíos si no hay)
       << (cpu         ? cpu->getNombre()         : "") << ","
       << (gpu         ? gpu->getNombre()         : "") << ","
       << (motherboard ? motherboard->getModelo() : "") << ","
       << (psu         ? psu->getNombre()         : "") << ","
       << (cooler      ? cooler->getNombre()      : "") << ","
       << (casePC      ? casePC->getNombre()      : "") << ","
       // Listas dinámicas
       << ramsStr << ","
       << storesStr;

    return ss.str();
}

inline void Build::fromCSV(const string& linea) {
    string tipo;
    string temp;
    stringstream ss(linea);

    getline(ss, tipo, ',');     // "Build"
    getline(ss, nombre, ',');
    getline(ss, temp, ','); precioTotal = stod(temp);
    getline(ss, temp, ','); scoreTotal  = stod(temp);

    // Componentes quedan en nullptr; se esperaría que otro
    // sistema enlazara esta build a componentes reales.
}


#endif // BUILD_H
