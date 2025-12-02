#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "components/Build.h"
#include "utils/BuildAVL.h"
#include "utils/Sorts.h"
#include "utils/utils.h"
#include "utils/CSVutils.h"

using std::cin;
using std::cout;
using std::string;
using std::vector;

const string BUILDS_FILE = "csv/builds.csv";
const string CPU_FILE    = "csv/cpus.csv";
const string GPU_FILE    = "csv/gpus.csv";
const string MB_FILE     = "csv/motherboards.csv";
const string RAM_FILE    = "csv/rams.csv";
const string STOR_FILE   = "csv/storages.csv";
const string PSU_FILE    = "csv/psus.csv";
const string COOLER_FILE = "csv/coolers.csv";
const string CASE_FILE   = "csv/cases.csv";

// ---------------------------------------------------------
//      CREADORES INTERACTIVOS PARA CADA COMPONENTE
// ---------------------------------------------------------

CPU* crearCPUInteractivo() {
    cout << "=== CREAR NUEVO CPU ===\n";

    cout << "Marca:\n";
    cout << "  1) AMD\n";
    cout << "  2) Intel\n";
    int opMarca = pedirEntero("Elige marca: ", 1, 2);

    string marca = (opMarca == 1 ? "AMD" : "Intel");

    string nombre = pedirLinea("Nombre comercial (ej. Ryzen 5 5600X): ");
    string modelo = pedirLinea("Modelo interno (ej. 5600X, i5-12400F): ");

    int nucleos = pedirEntero("Número de núcleos (ej. 6): ", 1, 128);
    int hilos   = pedirEntero("Número de hilos (>= núcleos): ", nucleos, 256);
    int tdp     = static_cast<int>(pedirDouble("TDP en Watts (ej. 65): ", 1.0));

    cout << "Soporte de memoria:\n";
    cout << "  1) Solo DDR4\n";
    cout << "  2) Solo DDR5\n";
    cout << "  3) DDR4 y DDR5\n";
    int opRam = pedirEntero("Elige opción: ", 1, 3);

    bool ddr4 = false, ddr5 = false;
    if (opRam == 1) ddr4 = true;
    else if (opRam == 2) ddr5 = true;
    else { ddr4 = true; ddr5 = true; }

    int pcie = pedirEntero("Versión PCIe (ej. 3, 4, 5): ", 1, 6);

    cout << "¿Soporta overclock?\n";
    cout << "  1) Sí\n";
    cout << "  2) No\n";
    int opOC = pedirEntero("Elige opción: ", 1, 2);
    bool soportaOC = (opOC == 1);

    double precio = pedirDouble("Precio aproximado (MXN): ", 0.0);
    double score  = pedirDouble("Score de rendimiento (ej. 0-100): ", 0.0);

    CPU* nuevo = new CPU(
        nombre,
        marca,
        modelo,
        "",           // socket (se infiere con inferirDatos)
        nucleos,
        hilos,
        tdp,
        ddr4,
        ddr5,
        pcie,
        soportaOC,
        precio,
        score
    );

    cout << "\nCPU creado correctamente:\n";
    nuevo->mostrarInfo();
    return nuevo;
}

GPU* crearGPUInteractivo() {
    cout << "=== CREAR NUEVA GPU ===\n";

    cout << "Marca:\n";
    cout << "  1) NVIDIA\n";
    cout << "  2) AMD\n";
    cout << "  3) Intel\n";
    cout << "  4) Otra\n";
    int opMarca = pedirEntero("Elige marca: ", 1, 4);

    string marca;
    if (opMarca == 1) marca = "NVIDIA";
    else if (opMarca == 2) marca = "AMD";
    else if (opMarca == 3) marca = "Intel";
    else marca = pedirLinea("Escribe la marca: ");

    string nombre = pedirLinea("Nombre comercial (ej. RTX 4070 SUPER): ");
    string modelo = pedirLinea("Modelo interno (ej. 4070 SUPER): ");

    int vramGB = pedirEntero("VRAM (en GB, ej. 8): ", 2, 48);

    cout << "Tipo de VRAM:\n";
    cout << "  1) GDDR5\n";
    cout << "  2) GDDR6\n";
    cout << "  3) GDDR6X\n";
    cout << "  4) Otro\n";
    int opVR = pedirEntero("Elige opción: ", 1, 4);

    string tipoVRAM;
    if (opVR == 1) tipoVRAM = "GDDR5";
    else if (opVR == 2) tipoVRAM = "GDDR6";
    else if (opVR == 3) tipoVRAM = "GDDR6X";
    else tipoVRAM = pedirLinea("Escribe el tipo de VRAM: ");

    int consumoW = pedirEntero("Consumo típico (Watts, ej. 200): ", 30, 800);

    double precio = pedirDouble("Precio aproximado (MXN): ", 0.0);
    double score  = pedirDouble("Score de rendimiento (ej. 0-100): ", 0.0);

    GPU* nueva = new GPU(
        nombre,
        marca,
        modelo,
        vramGB,
        tipoVRAM,
        consumoW,
        precio,
        score
    );

    cout << "\nGPU creada correctamente:\n";
    nueva->mostrarInfo();
    return nueva;
}

Motherboard* crearMotherboardInteractivo() {
    cout << "=== CREAR NUEVA MOTHERBOARD ===\n";
    const string marca  = pedirLinea("Marca (ej. ASUS, MSI, Gigabyte): ");
    const string nombre = pedirLinea("Nombre comercial (ej. TUF Gaming B550M-Plus): ");
    const string modelo = pedirLinea("Modelo (ej. B550M-PLUS): ");
    const double precio = pedirDouble("Precio aproximado (MXN): ", 0.0);
    const double score  = pedirDouble("Score de calidad (0-100): ", 0.0);

    auto* mb = new Motherboard(nombre, marca, modelo, precio, score);
    cout << "\nMotherboard creada correctamente:\n";
    mb->mostrarInfo();
    return mb;
}

PSU* crearPSUInteractivo() {
    cout << "=== CREAR NUEVA PSU ===\n";
    string marca  = pedirLinea("Marca (ej. Corsair, EVGA): ");
    string nombre = pedirLinea("Nombre comercial (ej. RM750x): ");
    string modelo = pedirLinea("Modelo interno (puede ser igual al nombre): ");

    int wattage = pedirEntero("Potencia (W, ej. 650): ", 300, 2000);

    cout << "Certificación:\n";
    cout << "  1) 80+ Bronze\n";
    cout << "  2) 80+ Silver\n";
    cout << "  3) 80+ Gold\n";
    cout << "  4) 80+ Platinum\n";
    cout << "  5) 80+ Titanium\n";
    cout << "  6) Otra\n";
    int opCert = pedirEntero("Elige opción: ", 1, 6);

    string certificacion;
    switch (opCert) {
        case 1: certificacion = "80+ Bronze"; break;
        case 2: certificacion = "80+ Silver"; break;
        case 3: certificacion = "80+ Gold"; break;
        case 4: certificacion = "80+ Platinum"; break;
        case 5: certificacion = "80+ Titanium"; break;
        default: certificacion = pedirLinea("Escribe la certificación: "); break;
    }

    cout << "Modularidad:\n";
    cout << "  1) Non-Modular\n";
    cout << "  2) Semi-Modular\n";
    cout << "  3) Full-Modular\n";
    int opMod = pedirEntero("Elige opción: ", 1, 3);

    string modularidad;
    if (opMod == 1) modularidad = "Non-Modular";
    else if (opMod == 2) modularidad = "Semi-Modular";
    else modularidad = "Full-Modular";

    double precio = pedirDouble("Precio aproximado (MXN): ", 0.0);
    double score  = pedirDouble("Score de calidad (0-100): ", 0.0);

    PSU* psu = new PSU(nombre, marca, modelo, wattage, certificacion, modularidad,
                       precio, score);
    cout << "\nPSU creada correctamente:\n";
    psu->mostrarInfo();
    return psu;
}

RAM* crearRAMInteractivo() {
    cout << "=== CREAR NUEVA RAM ===\n";
    string marca  = pedirLinea("Marca (ej. Corsair, G.Skill): ");
    string nombre = pedirLinea("Nombre comercial (ej. Vengeance LPX): ");
    string modelo = pedirLinea("Modelo: ");

    cout << "Tipo de RAM:\n";
    cout << "  1) DDR4\n";
    cout << "  2) DDR5\n";
    int opTipo = pedirEntero("Elige opción: ", 1, 2);
    string tipoRAM = (opTipo == 1 ? "DDR4" : "DDR5");

    int capacidad = pedirEntero("Capacidad por módulo (GB): ", 2, 256);
    int frecuencia = pedirEntero("Frecuencia (MHz, ej. 3200): ", 2133, 10000);

    double precio = pedirDouble("Precio aproximado (MXN): ", 0.0);
    double score  = pedirDouble("Score de rendimiento (0-100): ", 0.0);

    RAM* ram = new RAM(nombre, marca, modelo, tipoRAM, capacidad, frecuencia,
                       precio, score);
    cout << "\nRAM creada correctamente:\n";
    ram->mostrarInfo();
    return ram;
}

Storage* crearStorageInteractivo() {
    cout << "=== CREAR NUEVA UNIDAD DE ALMACENAMIENTO ===\n";
    string marca  = pedirLinea("Marca (ej. Samsung, WD, Seagate): ");
    string nombre = pedirLinea("Nombre comercial (ej. 970 EVO Plus): ");
    string modelo = pedirLinea("Modelo: ");

    cout << "Tipo de unidad:\n";
    cout << "  1) HDD\n";
    cout << "  2) SSD SATA\n";
    cout << "  3) M.2 SATA\n";
    cout << "  4) M.2 NVMe\n";
    int opTipo = pedirEntero("Elige opción: ", 1, 4);

    string tipoStorage;
    if (opTipo == 1) tipoStorage = "HDD";
    else if (opTipo == 2) tipoStorage = "SATA";
    else if (opTipo == 3) tipoStorage = "M.2-SATA";
    else tipoStorage = "M.2-NVMe";

    int capacidadGB = pedirEntero("Capacidad (GB, ej. 512): ", 120, 16000);
    int lecturaMBs  = pedirEntero("Lectura secuencial (MB/s): ", 50, 15000);
    int escrituraMBs= pedirEntero("Escritura secuencial (MB/s): ", 50, 15000);

    int pcieVersion = 0;
    if (tipoStorage == "M.2-NVMe") {
        pcieVersion = pedirEntero("Versión PCIe (3, 4 o 5): ", 3, 5);
    }

    double precio = pedirDouble("Precio aproximado (MXN): ", 0.0);
    double score  = pedirDouble("Score de rendimiento (0-100): ", 0.0);

    auto* s = new Storage(nombre, marca, modelo, tipoStorage,
                             capacidadGB, lecturaMBs, escrituraMBs,
                             pcieVersion, precio, score);
    cout << "\nUnidad creada correctamente:\n";
    s->mostrarInfo();
    return s;
}

Cooler* crearCoolerInteractivo() {
    cout << "=== CREAR NUEVO COOLER ===\n";
    string marca  = pedirLinea("Marca (ej. Cooler Master, Noctua): ");
    string nombre = pedirLinea("Nombre comercial: ");
    string modelo = pedirLinea("Modelo: ");

    cout << "Tipo de cooler:\n";
    cout << "  1) Air\n";
    cout << "  2) AIO\n";
    cout << "  3) Custom\n";
    int opTipo = pedirEntero("Elige opción: ", 1, 3);

    string tipoCooler;
    if (opTipo == 1) tipoCooler = "Air";
    else if (opTipo == 2) tipoCooler = "AIO";
    else tipoCooler = "Custom";

    int tdpSoportado = pedirEntero("TDP soportado (W, ej. 150): ", 65, 500);

    int tamanoRadiador = 0;
    int altura = 0;
    if (tipoCooler == "Air") {
        altura = pedirEntero("Altura del cooler (mm, ej. 155): ", 100, 220);
        tamanoRadiador = 0;
    } else {
        cout << "Tamaño de radiador:\n";
        cout << "  1) 120\n";
        cout << "  2) 240\n";
        cout << "  3) 280\n";
        cout << "  4) 360\n";
        int opRad = pedirEntero("Elige opción: ", 1, 4);
        if (opRad == 1) tamanoRadiador = 120;
        else if (opRad == 2) tamanoRadiador = 240;
        else if (opRad == 3) tamanoRadiador = 280;
        else tamanoRadiador = 360;
        altura = 0;
    }

    double precio = pedirDouble("Precio aproximado (MXN): ", 0.0);
    double score  = pedirDouble("Score de rendimiento (0-100): ", 0.0);

    auto* c = new Cooler(nombre, marca, modelo,
                           tipoCooler, tdpSoportado,
                           tamanoRadiador, altura,
                           precio, score);
    cout << "\nCooler creado correctamente:\n";
    c->mostrarInfo();
    return c;
}

Case* crearCaseInteractivo() {
    cout << "=== CREAR NUEVO GABINETE ===\n";
    string marca  = pedirLinea("Marca (ej. NZXT, Lian Li): ");
    string nombre = pedirLinea("Nombre comercial: ");
    string modelo = pedirLinea("Modelo: ");

    cout << "Form factor del gabinete:\n";
    cout << "  1) ATX\n";
    cout << "  2) mATX\n";
    cout << "  3) ITX\n";
    cout << "  4) FullTower\n";
    int opFF = pedirEntero("Elige opción: ", 1, 4);

    string formFactor;
    switch (opFF) {
        case 1: formFactor = "ATX"; break;
        case 2: formFactor = "mATX"; break;
        case 3: formFactor = "ITX"; break;
        default: formFactor = "FullTower"; break;
    }

    int alturaMaxCoolerAir = pedirEntero(
        "Altura máxima para cooler de aire (mm, ej. 165): ", 120, 220);
    int longitudMaxGPU = pedirEntero(
        "Longitud máxima de GPU (mm, ej. 360): ", 200, 500);

    cout << "Soporte radiador TOP:\n";
    cout << "  1) 0 (no soporta)\n";
    cout << "  2) 120\n";
    cout << "  3) 240\n";
    cout << "  4) 280\n";
    int opTop = pedirEntero("Elige opción: ", 1, 4);
    int radTop = (opTop == 1 ? 0 : (opTop == 2 ? 120 : (opTop == 3 ? 240 : 280)));

    cout << "Soporte radiador FRONT:\n";
    cout << "  1) 0 (no soporta)\n";
    cout << "  2) 240\n";
    cout << "  3) 280\n";
    cout << "  4) 360\n";
    int opFront = pedirEntero("Elige opción: ", 1, 4);
    int radFront;
    if (opFront == 1) radFront = 0;
    else if (opFront == 2) radFront = 240;
    else if (opFront == 3) radFront = 280;
    else radFront = 360;

    cout << "Soporte radiador REAR:\n";
    cout << "  1) 0 (no soporta)\n";
    cout << "  2) 120\n";
    int opRear = pedirEntero("Elige opción: ", 1, 2);
    int radRear = (opRear == 1 ? 0 : 120);

    double precio = pedirDouble("Precio aproximado (MXN): ", 0.0);
    double score  = pedirDouble("Score de calidad (0-100): ", 0.0);

    Case* ca = new Case(nombre, marca, modelo,
                        formFactor,
                        alturaMaxCoolerAir,
                        longitudMaxGPU,
                        radTop,
                        radFront,
                        radRear,
                        precio,
                        score);
    cout << "\nGabinete creado correctamente:\n";
    ca->mostrarInfo();
    return ca;
}

// ---------------------------------------------------------
//     HELPERS PARA RECONSTRUIR BUILDS DESDE CSV
// ---------------------------------------------------------

CPU* buscarCPU(const vector<CPU*>& cpus, const string& nombre) {
    if (nombre.empty()) return nullptr;
    for (CPU* c : cpus) {
        if (c && c->getNombre() == nombre) return c;
    }
    return nullptr;
}

GPU* buscarGPU(const vector<GPU*>& gpus, const string& nombre) {
    if (nombre.empty()) return nullptr;
    for (GPU* g : gpus) {
        if (g && g->getNombre() == nombre) return g;
    }
    return nullptr;
}

Motherboard* buscarMotherboard(const vector<Motherboard*>& mbs, const string& modelo) {
    if (modelo.empty()) return nullptr;
    for (Motherboard* m : mbs) {
        if (m && m->getModelo() == modelo) return m;
    }
    return nullptr;
}

PSU* buscarPSU(const vector<PSU*>& psus, const string& nombre) {
    if (nombre.empty()) return nullptr;
    for (PSU* p : psus) {
        if (p && p->getNombre() == nombre) return p;
    }
    return nullptr;
}

Cooler* buscarCooler(const vector<Cooler*>& coolers, const string& nombre) {
    if (nombre.empty()) return nullptr;
    for (Cooler* c : coolers) {
        if (c && c->getNombre() == nombre) return c;
    }
    return nullptr;
}

Case* buscarCase(const vector<Case*>& cases, const string& nombre) {
    if (nombre.empty()) return nullptr;
    for (Case* ca : cases) {
        if (ca && ca->getNombre() == nombre) return ca;
    }
    return nullptr;
}

RAM* buscarRAM(const vector<RAM*>& rams, const string& nombre) {
    if (nombre.empty()) return nullptr;
    for (RAM* r : rams) {
        if (r && r->getNombre() == nombre) return r;
    }
    return nullptr;
}

Storage* buscarStorage(const vector<Storage*>& storages, const string& nombre) {
    if (nombre.empty()) return nullptr;
    for (Storage* s : storages) {
        if (s && s->getNombre() == nombre) return s;
    }
    return nullptr;
}

Build* reconstruirBuildDesdeCSV(
    const string& linea,
    const vector<CPU*>& cpus,
    const vector<Motherboard*>& mbs,
    const vector<GPU*>& gpus,
    const vector<PSU*>& psus,
    const vector<Cooler*>& coolers,
    const vector<Case*>& cases,
    const vector<RAM*>& rams,
    const vector<Storage*>& storages)
{
    if (linea.empty()) return nullptr;

    std::stringstream ss(linea);

    string tipo;
    getline(ss, tipo, ',');
    if (tipo != "Build") {
        // línea que no corresponde a una build
        return nullptr;
    }

    string nombre;
    string precioStr, scoreStr;
    string cpuName, gpuName, mbModelo, psuName, coolerName, caseName;
    string ramsStr, storStr;

    getline(ss, nombre, ',');
    getline(ss, precioStr, ',');   // no los usamos directamente
    getline(ss, scoreStr, ',');    // pero podrías convertirlos si quieres

    getline(ss, cpuName, ',');
    getline(ss, gpuName, ',');
    getline(ss, mbModelo, ',');
    getline(ss, psuName, ',');
    getline(ss, coolerName, ',');
    getline(ss, caseName, ',');
    getline(ss, ramsStr, ',');
    std::getline(ss, storStr); // último campo (storages), hasta fin de línea

    // Buscar componentes en los catálogos
    CPU*        cpu    = buscarCPU(cpus, cpuName);
    Motherboard* mb    = buscarMotherboard(mbs, mbModelo);
    GPU*        gpu    = buscarGPU(gpus, gpuName);
    PSU*        psu    = buscarPSU(psus, psuName);
    Cooler*     cooler = buscarCooler(coolers, coolerName);
    Case*       casePC = buscarCase(cases, caseName);

    // Construimos la build con los componentes principales
    auto* b = new Build(nombre, cpu, mb, gpu, psu, cooler, casePC);

    // Agregar RAMs (nombres separados por '|')
    if (!ramsStr.empty()) {
        std::stringstream ssR(ramsStr);
        string token;
        while (std::getline(ssR, token, '|')) {
            if (token.empty()) continue;
            if (RAM* r = buscarRAM(rams, token)) b->addRAM(r);
        }
    }

    // Agregar Storages (nombres separados por '|')
    if (!storStr.empty()) {
        std::stringstream ssS(storStr);
        string token;
        while (std::getline(ssS, token, '|')) {
            if (token.empty()) continue;
            if (Storage* s = buscarStorage(storages, token)) b->addStorage(s);
        }
    }

    // addRAM/addStorage ya llaman a recalcularTotales,
    // así que precioTotal y scoreTotal quedan coherentes.
    return b;
}

// ---------------------------------------------------------
//          CARGAR Y GUARDAR BUILDS DESDE/EN CSV
// ---------------------------------------------------------

void cargarBuildsDesdeCSV(const string& filename,
                          vector<Build*>& builds,
                          BuildAVL& indicePrecio,
                          const vector<CPU*>& cpus,
                          const vector<Motherboard*>& mbs,
                          const vector<GPU*>& gpus,
                          const vector<PSU*>& psus,
                          const vector<Cooler*>& coolers,
                          const vector<Case*>& cases,
                          const vector<RAM*>& rams,
                          const vector<Storage*>& storages) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        cout << "Aviso: no se pudo abrir '" << filename << "'. "
             << "Se iniciará con lista de builds vacía.\n\n";
        return;
    }

    string linea;
    int count = 0;
    while (std::getline(file, linea)) {
        if (linea.empty()) continue;

        Build* b = reconstruirBuildDesdeCSV(
            linea,
            cpus, mbs, gpus, psus,
            coolers, cases, rams, storages
        );

        if (!b) continue;

        builds.push_back(b);
        indicePrecio.insert(b);
        ++count;
    }

    cout << "Se cargaron " << count << " builds desde '" << filename << "'.\n\n";
}


void guardarBuildsEnCSV(const string& filename,
                        const vector<Build*>& builds) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: no se pudo guardar en '" << filename << "'.\n";
        return;
    }

    for (Build* b : builds) {
        if (b) {
            file << b->toCSV() << "\n";
        }
    }

    cout << "Builds guardadas en '" << filename << "'.\n";
}

// Guardar cualquier tipo de componente que tenga toCSV()
template <typename T>
void guardarComponentesEnCSV(const string& filename,
                             const vector<T*>& comps) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: no se pudo guardar en '" << filename << "'.\n";
        return;
    }

    for (T* c : comps) {
        if (c) {
            file << c->toCSV() << "\n";
        }
    }

    cout << "Componentes guardados en '" << filename << "'.\n";
}

// ---------------------------------------------------------
//               MOSTRAR LISTAS DE COMPONENTES
// ---------------------------------------------------------

template <typename T>
void listarComponentes(const vector<T*>& comps, const string& titulo) {
    if (comps.empty()) {
        cout << "No hay " << titulo << " cargados.\n";
        return;
    }
    cout << "=== " << titulo << " (" << comps.size() << ") ===\n";
    for (size_t i = 0; i < comps.size(); ++i) {
        cout << "[" << i << "]\n";
        comps[i]->mostrarInfo();
        cout << "---------------------------\n";
    }
}

template <typename T>
T* elegirDeLista(const vector<T*>& comps,
                 const string& titulo,
                 bool permitirNinguno = true) {
    if (comps.empty()) {
        cout << "No hay " << titulo << " disponibles.\n";
        return nullptr;
    }

    listarComponentes(comps, titulo);

    int minIdx = permitirNinguno ? -1 : 0;
    int maxIdx = static_cast<int>(comps.size()) - 1;
    int idx;

    while (true) {
        cout << "Elige índice de " << titulo;
        if (permitirNinguno) {
            cout << " (-1 = ninguno)";
        }
        cout << ": ";
        if (cin >> idx && idx >= minIdx && idx <= maxIdx) {
            limpiarBuffer();
            if (idx == -1) return nullptr;
            return comps[idx];
        }
        cout << "Índice inválido.\n";
        limpiarBuffer();
    }
}

// ---------------------------------------------------------
//               MOSTRAR LISTAS DE BUILDS
// ---------------------------------------------------------

void mostrarResumenCorto(const Build* b, int idx = -1) {
    if (!b) return;
    if (idx >= 0) {
        cout << "[" << idx << "] ";
    }
    cout << b->getNombre()
         << " | Precio: $" << b->getPrecio()
         << " | Score: " << b->getScore()
         << "\n";
}

void listarBuildsBase(const vector<Build*>& builds) {
    if (builds.empty()) {
        cout << "No hay builds registradas.\n";
        return;
    }
    cout << "=== LISTA DE BUILDS (indice interno) ===\n";
    for (size_t i = 0; i < builds.size(); ++i) {
        mostrarResumenCorto(builds[i], static_cast<int>(i));
    }
}

int encontrarIndice(const vector<Build*>& builds, const Build* objetivo) {
    for (size_t i = 0; i < builds.size(); ++i) {
        if (builds[i] == objetivo) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void listarBuildsPorPrecio(const vector<Build*>& builds, const BuildAVL& indice) {
    if (builds.empty()) {
        cout << "No hay builds registradas.\n";
        return;
    }

    cout << "=== BUILDS ORDENADAS POR PRECIO (AVL, menor a mayor) ===\n";
    indice.inOrder([&builds](const Build* b) {
        int idx = encontrarIndice(builds, b);
        mostrarResumenCorto(b, idx);
    });
}

void listarBuildsPorScore(const vector<Build*>& builds, const BuildAVL& indice) {
    if (builds.empty()) {
        cout << "No hay builds registradas.\n";
        return;
    }

    // Obtener vector ordenado por precio desde el AVL (no importa el orden base)
    vector<Build*> v = indice.toVector();

    // Ordenar por score con MergeSort
    Sorts::mergeSortPorScore(v);

    cout << "=== BUILDS ORDENADAS POR SCORE (mayor a menor) ===\n";
    // Como mergeSort ordena de menor a mayor, recorremos al revés
    for (int i = static_cast<int>(v.size()) - 1; i >= 0; --i) {
        Build* b = v[i];
        int idx = encontrarIndice(builds, b);
        mostrarResumenCorto(b, idx);
    }
}

void listarBuildsRangoPrecio(const vector<Build*>& builds, const BuildAVL& indice) {
    if (builds.empty()) {
        cout << "No hay builds registradas.\n";
        return;
    }

    double minP = pedirDouble("Precio mínimo: ", 0.0);
    double maxP = pedirDouble("Precio máximo: ", 0.0);
    if (maxP < minP) {
        std::swap(minP, maxP);
    }

    auto resultado = indice.buscarPorRangoPrecio(minP, maxP);
    if (resultado.empty()) {
        cout << "No se encontraron builds en el rango $" << minP
             << " - $" << maxP << ".\n";
        return;
    }

    cout << "=== BUILDS EN RANGO DE PRECIO $" << minP << " - $" << maxP << " ===\n";
    for (Build* b : resultado) {
        int idx = encontrarIndice(builds, b);
        mostrarResumenCorto(b, idx);
    }
}

void verDetalleBuild(const vector<Build*>& builds) {
    if (builds.empty()) {
        cout << "No hay builds para mostrar.\n";
        return;
    }

    listarBuildsBase(builds);
    int idx = pedirEntero("Selecciona el índice de la build: ", 0,
                          static_cast<int>(builds.size()) - 1);

    Build* b = builds[idx];
    if (!b) {
        cout << "Build inválida.\n";
        return;
    }

    cout << "\n=== DETALLE DE BUILD ===\n";
    cout << "Nombre: " << b->getNombre() << "\n";
    cout << "Precio total: $" << b->getPrecio() << "\n";
    cout << "Score total: " << b->getScore() << "\n\n";

    cout << "---- COMPONENTES ----\n";
    if (b->getCPU()) cout << "CPU: " << b->getCPU()->getNombre() << "\n";
    if (b->getMotherboard()) cout << "Motherboard: " << b->getMotherboard()->getNombre() << "\n";
    if (b->getGPU()) cout << "GPU: " << b->getGPU()->getNombre() << "\n";
    if (b->getPSU()) cout << "PSU: " << b->getPSU()->getNombre() << "\n";
    if (b->getCooler()) cout << "Cooler: " << b->getCooler()->getNombre() << "\n";
    if (b->getCase()) cout << "Gabinete: " << b->getCase()->getNombre() << "\n";

    cout << "RAM:\n";
    for (auto* r : b->getRAMModules()) {
        cout << "   - " << r->getNombre() << "\n";
    }

    cout << "Almacenamiento:\n";
    for (auto* s : b->getStorages()) {
        cout << "   - " << s->getNombre() << "\n";
    }

    cout << "\n---- RESULTADO DE COMPATIBILIDAD ----\n";
    b->mostrarDetalle(); // usa la lógica de compatibilidad interna
}


// ---------------------------------------------------------
//           OPERACIONES CRUD SOBRE LAS BUILDS
// ---------------------------------------------------------

void agregarBuild(vector<Build*>& builds,
                  BuildAVL& indice,
                  const vector<CPU*>& cpus,
                  const vector<Motherboard*>& mbs,
                  const vector<GPU*>& gpus,
                  const vector<PSU*>& psus,
                  const vector<Cooler*>& coolers,
                  const vector<Case*>& cases,
                  const vector<RAM*>& ramsCatalog,
                  const vector<Storage*>& storCatalog) {
    cout << "=== AGREGAR NUEVA BUILD (USANDO COMPONENTES) ===\n";

    if (cpus.empty() || mbs.empty() || psus.empty()) {
        cout << "Error: se requieren al menos CPU, Motherboards y PSUs cargadas para crear una build.\n";
        return;
    }

    string nombre = pedirLinea("Nombre de la build: ");

    CPU* cpu = elegirDeLista(cpus, "CPUs", false);
    Motherboard* mb = elegirDeLista(mbs, "Motherboards", false);
    GPU* gpu = elegirDeLista(gpus, "GPUs (se puede omitir)", true);
    PSU* psu = elegirDeLista(psus, "PSUs", false);
    Cooler* cooler = elegirDeLista(coolers, "Coolers", true);
    Case* casePC = elegirDeLista(cases, "Gabinetes", true);

    auto* b = new Build(nombre, cpu, mb, gpu, psu, cooler, casePC);

    // Seleccionar RAM
    const int maxRAMSlots = mb->getSlotsRAM();
    if (!ramsCatalog.empty()) {
        int nRam = 0;
        nRam = pedirEntero("¿Cuántos módulos de RAM quieres agregar? (0-" + std::to_string(maxRAMSlots) + "): ",0,maxRAMSlots);
        for (int i = 0; i < nRam; ++i) {
            cout << "Selecciona RAM #" << (i + 1) << ":\n";
            if (RAM* r = elegirDeLista(ramsCatalog, "RAM", false)) b->addRAM(r);
        }
    } else {
        cout << "Aviso: no hay módulos RAM cargados.\n";
    }

    // Seleccionar Storage
    if (!storCatalog.empty()) {
        int nStor = 0;
        int formatStor = 0;
        int maxStorSlots = 0;
        string format = "";

        // Elegir formato
        formatStor = pedirEntero("¿Qué formato de almacenamiento quieres agregar? 1: SATA  2: M.2: ", 1, 2);
        if (formatStor == 1) {
            maxStorSlots = mb->getSataPorts();
            format = "SATA";
        } else {
            maxStorSlots = mb->getM2Slots();
            format = "M.2";
        }

        // Seleccionar cuántos
        nStor = pedirEntero("¿Cuántas unidades de almacenamiento quieres agregar? (0-"
                             + std::to_string(maxStorSlots) + "): ",
                             0, maxStorSlots );

        // Filtrar catálogo dependiendo del formato
        vector<Storage*> filtrados;
        for (auto* st : storCatalog) {
            if (formatStor == 1) {
                if (st->getTipoStorage() == "HDD" || st->getTipoStorage() == "SATA")
                    filtrados.push_back(st);
            } else {
                if (st->getTipoStorage().rfind("M.2", 0) == 0)  // "M.2-SATA", "M.2-NVMe"
                    filtrados.push_back(st);
            }
        }

        if (filtrados.empty()) {
            cout << "No hay unidades compatibles con el formato " << format << ".\n";
            return;
        }

        // Elegir los filtrados
        for (int i = 0; i < nStor; ++i) {
            cout << "Selecciona Storage #" << (i + 1) << ":\n";
            Storage* s = elegirDeLista(filtrados, "Storage", false);
            if (s) b->addStorage(s);
        }
    } else {
        cout << "Aviso: no hay unidades de almacenamiento cargadas.\n";
    }
    auto problemas = b->validarCompatibilidad();
    if (!problemas.empty()) {
        cout << "\n⚠ Se detectaron problemas de compatibilidad / bottleneck en la nueva build:\n";
        for (const auto& r : problemas) {
            string tipo;
            if (!r.compatible && r.bottleneck)      tipo = "INCOMPATIBLE + BOTTLENECK";
            else if (!r.compatible)                 tipo = "INCOMPATIBLE";
            else if (r.bottleneck)                  tipo = "BOTTLENECK";
            else                                    tipo = "ADVERTENCIA";

            cout << " - [" << tipo << "] " << r.message << "\n";
        }

        int cont = pedirEntero(
            "\n¿Deseas GUARDAR esta build a pesar de los problemas? (1=Sí, 0=No): ",
            0, 1
        );

        if (cont == 0) {
            delete b;
            cout << "Build descartada por incompatibilidad/bottleneck.\n";
            return;
        }
    }
    // Mostrar resumen y compatibilidad de la nueva build
    cout << "\nBuild creada:\n";
    b->mostrarDetalle();

    builds.push_back(b);
    indice.insert(b);

    cout << "Build agregada correctamente.\n";
}

void editarBuild(const vector<Build*>& builds,
                 BuildAVL& indice,
                 const vector<CPU*>& cpus,
                 const vector<Motherboard*>& mbs,
                 const vector<GPU*>& gpus,
                 const vector<PSU*>& psus,
                 const vector<Cooler*>& coolers,
                 const vector<Case*>& cases,
                 const vector<RAM*>& ramsCatalog,
                 const vector<Storage*>& storCatalog) {
    if (builds.empty()) {
        cout << "No hay builds para editar.\n";
        return;
    }

    cout << "=== EDITAR BUILD ===\n";
    listarBuildsBase(builds);
    int idx = pedirEntero("Selecciona el índice de la build a editar: ", 0,
                          static_cast<int>(builds.size()) - 1);

    Build* b = builds[idx];
    if (!b) {
        cout << "Build inválida.\n";
        return;
    }

    // 1) Sacar del AVL usando la clave vieja (precio+nombre actual)
    indice.remove(b);

    // 2) Guardar copia del estado anterior por si se cancela
    string oldNombre = b->getNombre();
    CPU* oldCPU = b->getCPU();
    Motherboard* oldMB = b->getMotherboard();
    GPU* oldGPU = b->getGPU();
    PSU* oldPSU = b->getPSU();
    Cooler* oldCooler = b->getCooler();
    Case* oldCase = b->getCase();

    vector<RAM*> oldRams = b->getRAMModules();
    vector<Storage*> oldStorages = b->getStorages();

    // 3) Editar nombre
    cout << "Nombre actual: " << oldNombre << "\n";
    string nuevoNombre = pedirLinea("Nuevo nombre (ENTER = conservar): ");
    if (!nuevoNombre.empty()) {
        b->setNombre(nuevoNombre);
    }

    // 4) Elegir nuevos componentes principales
    cout << "\nSelecciona los NUEVOS componentes para esta build.\n";
    cout << "(Si quieres mantener alguno igual, simplemente vuelve a elegirlo.)\n\n";

    CPU* newCPU = elegirDeLista(cpus, "CPUs", false);
    Motherboard* newMB = elegirDeLista(mbs, "Motherboards", false);
    GPU* newGPU = elegirDeLista(gpus, "GPUs (se puede omitir)", true);
    PSU* newPSU = elegirDeLista(psus, "PSUs", false);
    Cooler* newCooler = elegirDeLista(coolers, "Coolers", true);
    Case* newCase = elegirDeLista(cases, "Gabinetes", true);

    b->setCPU(newCPU);
    b->setMotherboard(newMB);
    b->setGPU(newGPU);
    b->setPSU(newPSU);
    b->setCooler(newCooler);
    b->setCase(newCase);

    // 5) RAM
    b->clearRAMModules();
    if (!ramsCatalog.empty()) {
        int nRam = pedirEntero("¿Cuántos módulos de RAM quieres en la build? (0-8): ", 0, 8);
        for (int i = 0; i < nRam; ++i) {
            cout << "Selecciona RAM #" << (i + 1) << ":\n";
            if (RAM* r = elegirDeLista(ramsCatalog, "RAM", false)) b->addRAM(r);
        }
    } else {
        cout << "Aviso: no hay RAM en el catálogo, se dejará sin RAM.\n";
    }

    // 6) Storage
    b->clearStorages();
    if (!storCatalog.empty()) {
        int nStor = pedirEntero("¿Cuántas unidades de almacenamiento quieres? (0-8): ", 0, 8);
        for (int i = 0; i < nStor; ++i) {
            cout << "Selecciona Storage #" << (i + 1) << ":\n";
            if (Storage* s = elegirDeLista(storCatalog, "Storage", false)) b->addStorage(s);
        }
    } else {
        cout << "Aviso: no hay unidades de almacenamiento en el catálogo.\n";
    }

    // 7) Recalcular totales con los nuevos componentes
    b->recalcularTotales();

    // 8) Validar compatibilidad y bottlenecks
    auto problemas = b->validarCompatibilidad();
    if (!problemas.empty()) {
        cout << "\n⚠ Se detectaron problemas de compatibilidad / bottleneck:\n";
        for (const auto& r : problemas) {
            string tipo;
            if (!r.compatible && r.bottleneck)      tipo = "INCOMPATIBLE + BOTTLENECK";
            else if (!r.compatible)                 tipo = "INCOMPATIBLE";
            else if (r.bottleneck)                  tipo = "BOTTLENECK";
            else                                    tipo = "ADVERTENCIA";

            cout << " - [" << tipo << "] " << r.message << "\n";
        }

        int cont = pedirEntero(
            "\n¿Deseas GUARDAR esta build a pesar de los problemas? (1=Sí, 0=No): ",
            0, 1
        );

        if (cont == 0) {
            // 9) Revertir al estado anterior
            b->setNombre(oldNombre);
            b->setCPU(oldCPU);
            b->setMotherboard(oldMB);
            b->setGPU(oldGPU);
            b->setPSU(oldPSU);
            b->setCooler(oldCooler);
            b->setCase(oldCase);

            b->clearRAMModules();
            for (auto* r : oldRams) {
                b->addRAM(r);
            }

            b->clearStorages();
            for (auto* s : oldStorages) {
                b->addStorage(s);
            }

            b->recalcularTotales();
            indice.insert(b);  // reinsertar con los valores viejos

            cout << "Cambios descartados. La build se mantiene como antes.\n";
            return;
        }
    }

    // 10) Insertar versión EDITADA en el AVL
    indice.insert(b);
    cout << "Build actualizada exitosamente.\n";
}


void eliminarBuild(vector<Build*>& builds, BuildAVL& indice) {
    if (builds.empty()) {
        cout << "No hay builds para eliminar.\n";
        return;
    }

    cout << "=== ELIMINAR BUILD ===\n";
    listarBuildsBase(builds);
    int idx = pedirEntero("Selecciona el índice de la build a eliminar: ", 0,
                          static_cast<int>(builds.size()) - 1);

    Build* b = builds[idx];
    if (!b) {
        cout << "Build inválida.\n";
        return;
    }

    indice.remove(b);
    delete b; // liberar memoria de la build
    builds.erase(builds.begin() + idx);

    cout << "Build eliminada correctamente.\n";
}

// ---------------------------------------------------------
//              MENÚ PARA VER / AGREGAR COMPONENTES
// ---------------------------------------------------------

void menuComponentes(vector<CPU*>& cpus,
                     vector<Motherboard*>& mbs,
                     vector<GPU*>& gpus,
                     vector<PSU*>& psus,
                     vector<Cooler*>& coolers,
                     vector<Case*>& cases,
                     vector<RAM*>& rams,
                     vector<Storage*>& storages) {
    bool salir = false;
    while (!salir) {
        cout << "\n===== CATÁLOGO DE COMPONENTES =====\n";
        cout << "1) Ver CPUs\n";
        cout << "2) Agregar CPU\n";
        cout << "3) Ver Motherboards\n";
        cout << "4) Agregar Motherboard\n";
        cout << "5) Ver GPUs\n";
        cout << "6) Agregar GPU\n";
        cout << "7) Ver PSUs\n";
        cout << "8) Agregar PSU\n";
        cout << "9) Ver Coolers\n";
        cout << "10) Agregar Cooler\n";
        cout << "11) Ver Gabinetes (Cases)\n";
        cout << "12) Agregar Gabinete\n";
        cout << "13) Ver RAM\n";
        cout << "14) Agregar RAM\n";
        cout << "15) Ver Storage\n";
        cout << "16) Agregar Storage\n";
        cout << "0) Regresar\n";

        int op = pedirEntero("Elige una opción: ", 0, 16);
        cout << "\n";

        switch (op) {
            case 1: {
                cout << "=== CPUs ===\n";
                cout << "Opciones de ordenamiento:\n";
                cout << "1) Precio\n2) Score\n3) Marca\n4) Sin ordenar\n";
                int opCPU = pedirEntero("Elige opción: ", 1, 4);
                if (opCPU == 1) Sorts::ordenarPorPrecio(cpus);
                else if (opCPU == 2) Sorts::ordenarPorScore(cpus);
                else if (opCPU == 3) Sorts::ordenarPorMarca(cpus);
                listarComponentes(cpus, "CPUs");
                pausar();
                break;
            }
            case 2: {
                if (CPU* nuevo = crearCPUInteractivo()) {
                    cpus.push_back(nuevo);
                    cout << "CPU agregado al catálogo.\n";
                }
                pausar();
                break;
            }
            case 3: {
                cout << "=== Motherboards ===\n";
                cout << "Opciones de ordenamiento:\n";
                cout << "1) Precio\n2) Score\n3) Marca\n4) Sin ordenar\n";
                int opMB = pedirEntero("Elige opción: ", 1, 4);
                if (opMB == 1) Sorts::ordenarPorPrecio(mbs);
                else if (opMB == 2) Sorts::ordenarPorScore(mbs);
                else if (opMB == 3) Sorts::ordenarPorMarca(mbs);
                listarComponentes(mbs, "Motherboards");
                pausar();
                break;
            }
            case 4: {
                if (Motherboard* nuevaMB = crearMotherboardInteractivo()) {
                    mbs.push_back(nuevaMB);
                    cout << "Motherboard agregada al catálogo.\n";
                }
                pausar();
                break;
            }
            case 5: {
                cout << "=== GPUs ===\n";
                cout << "Opciones de ordenamiento:\n";
                cout << "1) Precio\n2) Score\n3) Marca\n4) Sin ordenar\n";
                int opGPU = pedirEntero("Elige opción: ", 1, 4);
                if (opGPU == 1) Sorts::ordenarPorPrecio(gpus);
                else if (opGPU == 2) Sorts::ordenarPorScore(gpus);
                else if (opGPU == 3) Sorts::ordenarPorMarca(gpus);
                listarComponentes(gpus, "GPUs");
                pausar();
                break;
            }
            case 6: {
                if (GPU* nuevaGPU = crearGPUInteractivo()) {
                    gpus.push_back(nuevaGPU);
                    cout << "GPU agregada al catálogo.\n";
                }
                pausar();
                break;
            }
            case 7:{
                cout << "=== PSUs ===\n";
                cout << "Opciones de ordenamiento:\n";
                cout << "1) Precio\n2) Score\n3) Marca\n4) Sin ordenar\n";
                int opPSU = pedirEntero("Elige opción: ", 1, 4);
                if (opPSU == 1) Sorts::ordenarPorPrecio(psus);
                else if (opPSU == 2) Sorts::ordenarPorScore(psus);
                else if (opPSU == 3) Sorts::ordenarPorMarca(psus);
                listarComponentes(psus, "PSUs");
                pausar();
                break;
            }
            case 8: {
                if (PSU* nuevaPSU = crearPSUInteractivo()) {
                    psus.push_back(nuevaPSU);
                    cout << "PSU agregada al catálogo.\n";
                }
                pausar();
                break;
            }
            case 9: {
                cout << "=== Coolers ===\n";
                cout << "Opciones de ordenamiento:\n";
                cout << "1) Precio\n2) Score\n3) Marca\n4) Sin ordenar\n";
                int opCoolers = pedirEntero("Elige opción: ", 1, 4);
                if (opCoolers == 1) Sorts::ordenarPorPrecio(coolers);
                else if (opCoolers == 2) Sorts::ordenarPorScore(coolers);
                else if (opCoolers == 3) Sorts::ordenarPorMarca(coolers);
                listarComponentes(coolers, "Coolers");
                pausar();
                break;
            }
            case 10: {
                if (Cooler* nuevoCooler = crearCoolerInteractivo()) {
                    coolers.push_back(nuevoCooler);
                    cout << "Cooler agregado al catálogo.\n";
                }
                pausar();
                break;
            }
            case 11: {
                cout << "=== Gabinetes ===\n";
                cout << "Opciones de ordenamiento:\n";
                cout << "1) Precio\n2) Score\n3) Marca\n4) Sin ordenar\n";
                int opGabinetes = pedirEntero("Elige opción: ", 1, 4);
                if (opGabinetes == 1) Sorts::ordenarPorPrecio(cases);
                else if (opGabinetes == 2) Sorts::ordenarPorScore(cases);
                else if (opGabinetes == 3) Sorts::ordenarPorMarca(cases);
                listarComponentes(cases, "Gabinetes");
                pausar();
                break;
            }
            case 12: {
                if (Case* nuevoCase = crearCaseInteractivo()) {
                    cases.push_back(nuevoCase);
                    cout << "Gabinete agregado al catálogo.\n";
                }
                pausar();
                break;
            }
            case 13: {
                cout << "=== RAMs ===\n";
                cout << "Opciones de ordenamiento:\n";
                cout << "1) Precio\n2) Score\n3) Marca\n4) Sin ordenar\n";
                int opRAM = pedirEntero("Elige opción: ", 1, 4);
                if (opRAM == 1) Sorts::ordenarPorPrecio(rams);
                else if (opRAM == 2) Sorts::ordenarPorScore(rams);
                else if (opRAM == 3) Sorts::ordenarPorMarca(rams);
                listarComponentes(rams, "RAMs");
                pausar();
                break;
            }
            case 14: {
                if (RAM* nuevaRAM = crearRAMInteractivo()) {
                    rams.push_back(nuevaRAM);
                    cout << "RAM agregada al catálogo.\n";
                }
                pausar();
                break;
            }
            case 15: {
                cout << "=== Storages ===\n";
                cout << "Opciones de ordenamiento:\n";
                cout << "1) Precio\n2) Score\n3) Marca\n4) Sin ordenar\n";
                int opStorage = pedirEntero("Elige opción: ", 1, 4);
                if (opStorage == 1) Sorts::ordenarPorPrecio(storages);
                else if (opStorage == 2) Sorts::ordenarPorScore(storages);
                else if (opStorage == 3) Sorts::ordenarPorMarca(storages);
                listarComponentes(storages, "Storages");
                pausar();
                break;
            }
            case 16: {
                if (Storage* nuevoStor = crearStorageInteractivo()) {
                    storages.push_back(nuevoStor);
                    cout << "Unidad de almacenamiento agregada al catálogo.\n";
                }
                pausar();
                break;
            }
            case 0:
                salir = true;
                break;
        }
    }
}

template<typename T>
T* elegirFiltrado(vector<T*>& lista,
                  function<bool(T*)> filtroCompat,
                  function<CompatResult(T*)> advertidor)
{
    vector<pair<T*, CompatResult>> filtrada;

    // Filtrar
    for (auto* elemento : lista) {
        if (!elemento) continue;

        if (!filtroCompat(elemento))
            continue; // incompatible duro

        CompatResult r = advertidor(elemento);
        filtrada.push_back({elemento, r});
    }

    if (filtrada.empty()) {
        cout << "No hay componentes compatibles.\n";
        return nullptr;
    }

    // Menú de ordenamiento
    cout << "\nOpciones de ordenamiento:\n";
    cout << "1) Precio\n";
    cout << "2) Score\n";
    cout << "3) Marca\n";
    cout << "4) Sin ordenar\n";
    int op;
    cout << "Elige opción: ";
    cin >> op;

    vector<T*> ordenada;
    for (auto& p : filtrada) ordenada.push_back(p.first);

    if (op == 1) Sorts::ordenarPorPrecio(ordenada);
    else if (op == 2) Sorts::ordenarPorScore(ordenada);
    else if (op == 3) Sorts::ordenarPorMarca(ordenada);

    // Mostrar lista
    cout << "\n=== COMPONENTES DISPONIBLES ===\n";
    for (int i = 0; i < static_cast<int>(ordenada.size()); i++) {
        auto* e = ordenada[i];
        CompatResult r = advertidor(e);

        cout << "[" << i << "] ";
        e->mostrarInfo();

        // Asumiendo que CompatResult tiene:

        if (r.bottleneck)
            cout << "   ⚠ Advertencia: " << r.message << "\n";

        cout << "--------------------------\n";
    }

    // Elegir
    int idx;
    cout << "Elige índice: ";
    cin >> idx;

    if (idx < 0 || idx >= ordenada.size()) return nullptr;
    return ordenada[idx];
}

// ---------------------------------------------------------
//                       MENÚ PRINCIPAL
// ---------------------------------------------------------

void mostrarMenu() {
    cout << "\n====== RANKING DE PC BUILDS ======\n";
    cout << "1) Ver builds (indice base)\n";
    cout << "2) Ver builds ordenadas por PRECIO (AVL)\n";
    cout << "3) Ver builds ordenadas por SCORE (MergeSort)\n";
    cout << "4) Buscar builds por rango de PRECIO (AVL)\n";
    cout << "5) Ver DETALLE y compatibilidad de una build\n";
    cout << "6) Agregar nueva build (usando componentes)\n";
    cout << "7) Editar build\n";
    cout << "8) Eliminar build\n";
    cout << "9) Ver catálogo de componentes\n";
    cout << "10) Guardar builds en CSV\n";
    cout << "0) Salir\n";
}

int main() {
    // Asegurar que los CSV existan (si no, se crean vacíos)
    cout << "Cargado1...\n";
    crearCSVSiNoExiste(BUILDS_FILE);
    crearCSVSiNoExiste(CPU_FILE);
    crearCSVSiNoExiste(GPU_FILE);
    crearCSVSiNoExiste(MB_FILE);
    crearCSVSiNoExiste(RAM_FILE);
    crearCSVSiNoExiste(STOR_FILE);
    crearCSVSiNoExiste(PSU_FILE);
    crearCSVSiNoExiste(COOLER_FILE);
    crearCSVSiNoExiste(CASE_FILE);

    vector<Build*> builds;
    BuildAVL indicePrecio;
    cout << "Cargado2...\n";
    // Catálogos de componentes
    vector<CPU*> cpus;
    vector<Motherboard*> motherboards;
    vector<GPU*> gpus;
    vector<PSU*> psus;
    vector<Cooler*> coolers;
    vector<Case*> cases;
    vector<RAM*> rams;
    vector<Storage*> storages;

    // Cargar componentes desde CSV (si existen)
    cout << "Cargando CPU...\n";
    cargarCPUs(CPU_FILE, cpus);
    cout << "Cargado CPU...\n";
    cout << "Cargando Motherboards...\n";
    cargarMotherboards(MB_FILE, motherboards);
    cout << "Cargado Motherboards...\n";
    cout << "Cargando GPUs...\n";
    cargarGPUs(GPU_FILE, gpus);
    cout << "Cargado GPUs...\n";
    cout << "Cargando PSUs...\n";
    cargarPSUs(PSU_FILE, psus);
    cout << "Cargado PSUs...\n";
    cout << "Cargando Coolers...\n";
    cargarCoolers(COOLER_FILE, coolers);
    cout << "Cargado Coolers...\n";
    cout << "Cargando Cases...\n";
    cargarCases(CASE_FILE, cases);
    cout << "Cargado Cases...\n";
    cout << "Cargando RAM...\n";
    cargarRAMs(RAM_FILE, rams);
    cout << "Cargado RAM...\n";
    cout << "Cargando Storages...\n";
    cargarStorages(STOR_FILE, storages);
    cout << "Cargado Storages...\n";

    // Cargar builds agregadas previamente (reconstruyendo componentes)
    cargarBuildsDesdeCSV(BUILDS_FILE,
                     builds,
                     indicePrecio,
                     cpus,
                     motherboards,
                     gpus,
                     psus,
                     coolers,
                     cases,
                     rams,
                     storages);


    bool salir = false;
    while (!salir) {
        mostrarMenu();
        int opcion = pedirEntero("Elige una opción: ", 0, 10);

        cout << "\n";
        switch (opcion) {
            case 1:
                listarBuildsBase(builds);
                pausar();
                break;
            case 2:
                listarBuildsPorPrecio(builds, indicePrecio);
                pausar();
                break;
            case 3:
                listarBuildsPorScore(builds, indicePrecio);
                pausar();
                break;
            case 4:
                listarBuildsRangoPrecio(builds, indicePrecio);
                pausar();
                break;
            case 5:
                verDetalleBuild(builds);
                pausar();
                break;
            case 6:
                agregarBuild(builds, indicePrecio,
                             cpus, motherboards, gpus, psus,
                             coolers, cases, rams, storages);
                pausar();
                break;
            case 7:
                editarBuild(builds, indicePrecio,
                            cpus, motherboards, gpus, psus,
                            coolers, cases, rams, storages);
                pausar();
                break;

            case 8:
                eliminarBuild(builds, indicePrecio);
                pausar();
                break;
            case 9:
                menuComponentes(cpus, motherboards, gpus, psus,
                                coolers, cases, rams, storages);
                break;
            case 10:
                guardarBuildsEnCSV(BUILDS_FILE, builds);
                pausar();
                break;
            case 0:
                salir = true;
                break;
        }
    }

    // Guardar automáticamente al salir
    guardarBuildsEnCSV(BUILDS_FILE, builds);
    guardarComponentesEnCSV(CPU_FILE, cpus);
    guardarComponentesEnCSV(GPU_FILE, gpus);
    guardarComponentesEnCSV(MB_FILE, motherboards);
    guardarComponentesEnCSV(RAM_FILE, rams);
    guardarComponentesEnCSV(STOR_FILE, storages);
    guardarComponentesEnCSV(PSU_FILE, psus);
    guardarComponentesEnCSV(COOLER_FILE, coolers);
    guardarComponentesEnCSV(CASE_FILE, cases);

    // Liberar memoria de builds
    for (Build* b : builds) {
        delete b;
    }

    // Liberar memoria de componentes
    for (CPU* c : cpus) delete c;
    for (Motherboard* m : motherboards) delete m;
    for (GPU* g : gpus) delete g;
    for (PSU* p : psus) delete p;
    for (Cooler* c : coolers) delete c;
    for (Case* ca : cases) delete ca;
    for (RAM* r : rams) delete r;
    for (Storage* s : storages) delete s;

    cout << "Saliendo del programa. ¡Hasta luego!\n";
    return 0;
}