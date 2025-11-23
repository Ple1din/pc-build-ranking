#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <algorithm>

// Ajusta esta ruta según tu proyecto:
//  - "components/Build.h" si está en carpeta components/
//  - "Build.h" si está en la raíz
#include "components/Build.h"
#include "BuildAVL.h"
#include "Sorts.h"

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

// ---------------------------------------------------------
//                  UTILIDADES GENERALES
// ---------------------------------------------------------

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
    string marca  = pedirLinea("Marca (ej. ASUS, MSI, Gigabyte): ");
    string nombre = pedirLinea("Nombre comercial (ej. TUF Gaming B550M-Plus): ");
    string modelo = pedirLinea("Modelo (ej. B550M-PLUS): ");
    double precio = pedirDouble("Precio aproximado (MXN): ", 0.0);
    double score  = pedirDouble("Score de calidad (0-100): ", 0.0);

    Motherboard* mb = new Motherboard(nombre, marca, modelo, precio, score);
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

    Storage* s = new Storage(nombre, marca, modelo, tipoStorage,
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

    Cooler* c = new Cooler(nombre, marca, modelo,
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
//     CARGAR COMPONENTES DESDE CSV (CPU, GPU, etc.)
// ---------------------------------------------------------

void cargarCPUs(const string& filename, vector<CPU*>& cpus) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        cout << "Aviso: no se pudo abrir '" << filename
             << "' (CPUs). Se iniciará vacío.\n";
        return;
    }
    string linea;
    int count = 0;
    while (std::getline(file, linea)) {
        if (linea.empty()) continue;
        CPU* c = new CPU();
        c->fromCSV(linea);
        cpus.push_back(c);
        ++count;
    }
    cout << "Se cargaron " << count << " CPUs desde '" << filename << "'.\n";
}

void cargarGPUs(const string& filename, vector<GPU*>& gpus) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        cout << "Aviso: no se pudo abrir '" << filename
             << "' (GPUs). Se iniciará vacío.\n";
        return;
    }
    string linea;
    int count = 0;
    while (std::getline(file, linea)) {
        if (linea.empty()) continue;
        GPU* g = new GPU();
        g->fromCSV(linea);
        gpus.push_back(g);
        ++count;
    }
    cout << "Se cargaron " << count << " GPUs desde '" << filename << "'.\n";
}

void cargarMotherboards(const string& filename, vector<Motherboard*>& mbs) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        cout << "Aviso: no se pudo abrir '" << filename
             << "' (Motherboards). Se iniciará vacío.\n";
        return;
    }
    string linea;
    int count = 0;
    while (std::getline(file, linea)) {
        if (linea.empty()) continue;
        Motherboard* m = new Motherboard();
        m->fromCSV(linea);
        mbs.push_back(m);
        ++count;
    }
    cout << "Se cargaron " << count << " motherboards desde '" << filename << "'.\n";
}

void cargarRAMs(const string& filename, vector<RAM*>& rams) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        cout << "Aviso: no se pudo abrir '" << filename
             << "' (RAM). Se iniciará vacío.\n";
        return;
    }
    string linea;
    int count = 0;
    while (std::getline(file, linea)) {
        if (linea.empty()) continue;
        RAM* r = new RAM();
        r->fromCSV(linea);
        rams.push_back(r);
        ++count;
    }
    cout << "Se cargaron " << count << " módulos RAM desde '" << filename << "'.\n";
}

void cargarStorages(const string& filename, vector<Storage*>& storages) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        cout << "Aviso: no se pudo abrir '" << filename
             << "' (Storage). Se iniciará vacío.\n";
        return;
    }
    string linea;
    int count = 0;
    while (std::getline(file, linea)) {
        if (linea.empty()) continue;
        Storage* s = new Storage();
        s->fromCSV(linea);
        storages.push_back(s);
        ++count;
    }
    cout << "Se cargaron " << count << " unidades de almacenamiento desde '"
         << filename << "'.\n";
}

void cargarPSUs(const string& filename, vector<PSU*>& psus) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        cout << "Aviso: no se pudo abrir '" << filename
             << "' (PSUs). Se iniciará vacío.\n";
        return;
    }
    string linea;
    int count = 0;
    while (std::getline(file, linea)) {
        if (linea.empty()) continue;
        PSU* p = new PSU();
        p->fromCSV(linea);
        psus.push_back(p);
        ++count;
    }
    cout << "Se cargaron " << count << " PSUs desde '" << filename << "'.\n";
}

void cargarCoolers(const string& filename, vector<Cooler*>& coolers) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        cout << "Aviso: no se pudo abrir '" << filename
             << "' (Coolers). Se iniciará vacío.\n";
        return;
    }
    string linea;
    int count = 0;
    while (std::getline(file, linea)) {
        if (linea.empty()) continue;
        Cooler* c = new Cooler();
        c->fromCSV(linea);
        coolers.push_back(c);
        ++count;
    }
    cout << "Se cargaron " << count << " coolers desde '" << filename << "'.\n";
}

void cargarCases(const string& filename, vector<Case*>& cases) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        cout << "Aviso: no se pudo abrir '" << filename
             << "' (Cases). Se iniciará vacío.\n";
        return;
    }
    string linea;
    int count = 0;
    while (std::getline(file, linea)) {
        if (linea.empty()) continue;
        Case* c = new Case();
        c->fromCSV(linea);
        cases.push_back(c);
        ++count;
    }
    cout << "Se cargaron " << count << " gabinetes desde '" << filename << "'.\n";
}

// ---------------------------------------------------------
//          CARGAR Y GUARDAR BUILDS DESDE/EN CSV
// ---------------------------------------------------------

void cargarBuildsDesdeCSV(const string& filename,
                          vector<Build*>& builds,
                          BuildAVL& indicePrecio) {
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
        Build* b = new Build();
        b->fromCSV(linea); // nombre, precioTotal, scoreTotal
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

int encontrarIndice(const vector<Build*>& builds, Build* objetivo) {
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
    indice.inOrder([&builds](Build* b) {
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
    b->mostrarDetalle();  // usa toda la lógica de compatibilidad
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

    Build* b = new Build(nombre, cpu, mb, gpu, psu, cooler, casePC);

    // Seleccionar RAM
    int nRam = 0;
    if (!ramsCatalog.empty()) {
        nRam = pedirEntero("¿Cuántos módulos de RAM quieres agregar? (0-8): ", 0, 8);
        for (int i = 0; i < nRam; ++i) {
            cout << "Selecciona RAM #" << (i + 1) << ":\n";
            RAM* r = elegirDeLista(ramsCatalog, "RAM", false);
            if (r) b->addRAM(r);
        }
    } else {
        cout << "Aviso: no hay módulos RAM cargados.\n";
    }

    // Seleccionar Storage
    int nStor = 0;
    if (!storCatalog.empty()) {
        nStor = pedirEntero("¿Cuántas unidades de almacenamiento quieres agregar? (0-8): ", 0, 8);
        for (int i = 0; i < nStor; ++i) {
            cout << "Selecciona Storage #" << (i + 1) << ":\n";
            Storage* s = elegirDeLista(storCatalog, "Storage", false);
            if (s) b->addStorage(s);
        }
    } else {
        cout << "Aviso: no hay unidades de almacenamiento cargadas.\n";
    }

    // Mostrar resumen y compatibilidad de la nueva build
    cout << "\nBuild creada:\n";
    b->mostrarDetalle();

    builds.push_back(b);
    indice.insert(b);

    cout << "Build agregada correctamente.\n";
}

void editarBuild(vector<Build*>& builds, BuildAVL& indice) {
    if (builds.empty()) {
        cout << "No hay builds para editar.\n";
        return;
    }

    cout << "=== EDITAR BUILD (solo nombre por ahora) ===\n";
    listarBuildsBase(builds);
    int idx = pedirEntero("Selecciona el índice de la build a editar: ", 0,
                          static_cast<int>(builds.size()) - 1);

    Build* b = builds[idx];
    if (!b) {
        cout << "Build inválida.\n";
        return;
    }

    string nuevoNombre = pedirLinea("Nuevo nombre (deja vacío para conservar): ");
    if (nuevoNombre.empty()) {
        cout << "Nombre sin cambios.\n";
        return;
    }

    // Sacamos del AVL porque cambia la clave (nombre influye si se empata en precio)
    indice.remove(b);

    double precio = b->getPrecio();
    double score  = b->getScore();
    string lineaCSV = "Build," + nuevoNombre + "," +
                      std::to_string(precio) + "," +
                      std::to_string(score);
    b->fromCSV(lineaCSV);

    indice.insert(b);

    cout << "Build editada correctamente.\n";
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
            case 1:
                listarComponentes(cpus, "CPUs");
                pausar();
                break;
            case 2: {
                CPU* nuevo = crearCPUInteractivo();
                if (nuevo) {
                    cpus.push_back(nuevo);
                    cout << "CPU agregado al catálogo.\n";
                }
                pausar();
                break;
            }
            case 3:
                listarComponentes(mbs, "Motherboards");
                pausar();
                break;
            case 4: {
                Motherboard* nuevaMB = crearMotherboardInteractivo();
                if (nuevaMB) {
                    mbs.push_back(nuevaMB);
                    cout << "Motherboard agregada al catálogo.\n";
                }
                pausar();
                break;
            }
            case 5:
                listarComponentes(gpus, "GPUs");
                pausar();
                break;
            case 6: {
                GPU* nuevaGPU = crearGPUInteractivo();
                if (nuevaGPU) {
                    gpus.push_back(nuevaGPU);
                    cout << "GPU agregada al catálogo.\n";
                }
                pausar();
                break;
            }
            case 7:
                listarComponentes(psus, "PSUs");
                pausar();
                break;
            case 8: {
                PSU* nuevaPSU = crearPSUInteractivo();
                if (nuevaPSU) {
                    psus.push_back(nuevaPSU);
                    cout << "PSU agregada al catálogo.\n";
                }
                pausar();
                break;
            }
            case 9:
                listarComponentes(coolers, "Coolers");
                pausar();
                break;
            case 10: {
                Cooler* nuevoCooler = crearCoolerInteractivo();
                if (nuevoCooler) {
                    coolers.push_back(nuevoCooler);
                    cout << "Cooler agregado al catálogo.\n";
                }
                pausar();
                break;
            }
            case 11:
                listarComponentes(cases, "Gabinetes");
                pausar();
                break;
            case 12: {
                Case* nuevoCase = crearCaseInteractivo();
                if (nuevoCase) {
                    cases.push_back(nuevoCase);
                    cout << "Gabinete agregado al catálogo.\n";
                }
                pausar();
                break;
            }
            case 13:
                listarComponentes(rams, "RAM");
                pausar();
                break;
            case 14: {
                RAM* nuevaRAM = crearRAMInteractivo();
                if (nuevaRAM) {
                    rams.push_back(nuevaRAM);
                    cout << "RAM agregada al catálogo.\n";
                }
                pausar();
                break;
            }
            case 15:
                listarComponentes(storages, "Storage");
                pausar();
                break;
            case 16: {
                Storage* nuevoStor = crearStorageInteractivo();
                if (nuevoStor) {
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
    for (int i = 0; i < (int)ordenada.size(); i++) {
        auto* e = ordenada[i];
        CompatResult r = advertidor(e);

        cout << "[" << i << "] ";
        e->mostrarInfo();

        // Asumiendo que CompatResult tiene:
        // bool warning;
        // std::string message;
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
    cout << "7) Editar build (nombre)\n";
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

    // Cargar builds agregadas previamente (sin componentes enlazados)
    cargarBuildsDesdeCSV(BUILDS_FILE, builds, indicePrecio);

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
                editarBuild(builds, indicePrecio);
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