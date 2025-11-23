//
// Created by Yael on 23/11/2025.
//
// ---------------------------------------------------------
//     CARGAR COMPONENTES DESDE CSV (CPU, GPU, etc.)
// ---------------------------------------------------------
#ifndef CSVUTILS_H
#define CSVUTILS_H
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
#endif //CSVUTILS_H
