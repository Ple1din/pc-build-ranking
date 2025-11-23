#ifndef MOTHERBOARD_H
#define MOTHERBOARD_H

#include "Component.h"
#include "CPU.h"
#include <iostream>
#include <sstream>



class Motherboard : public Component {
private:
    // ---------------------------------------------------
    // Datos inferidos NO guardados en CSV (eficientes)
    // ---------------------------------------------------
    string socket;
    string chipset;
    string tipoRAM;
    int slotsRAM;
    int maxFrecuenciaRAM;

    int pcieVersion;

    int sataPorts;
    int m2Slots;
    int m2PcieVersion;
    bool m2SATACompat;

    bool soportaOC;
    int fasesVRM;
    bool requiereBIOSUpdate;

    string formFactor; // ATX, mATX, ITX

public:

    Motherboard(string nombre = "", string marca = "", string modelo = "",
                double precio = 0, double score = 0)
        : Component("Motherboard", nombre, marca, modelo, "", precio, score),
          socket(""), chipset(""), tipoRAM(""),
          slotsRAM(2), maxFrecuenciaRAM(2666),
          pcieVersion(3),
          sataPorts(4), m2Slots(1), m2PcieVersion(3), m2SATACompat(true),
          soportaOC(false), fasesVRM(6), requiereBIOSUpdate(false),
          formFactor("ATX")
    {
        extraerChipset();
        inferirDatos();
    }

    // ============================================================
    //                EXTRACCIÓN SIMPLE DE CHIPSET
    // ============================================================
    void extraerChipset() {
        string combos[] = {
            "A320","B350","X370","B450","X470","B550","X570",
            "A620","B650","X670",
            "H410","B460","H470","Z490","B560","H570","Z590",
            "H610","B660","H670","Z690","B760","Z790"
        };

        for (auto& c : combos) {
            if (modelo.find(c) != string::npos || nombre.find(c) != string::npos) {
                chipset = c;
                return;
            }
        }

        chipset = "DESCONOCIDO";
    }

    // ============================================================
    //            INFERIR TODA LA INFORMACIÓN REAL
    // ============================================================
    void inferirDatos() {
        inferirFormFactor();
        inferirSocket();
        inferirRAM();
        inferirPCIe();
        inferirOC();
        inferirVRM();
        inferirAlmacenamiento();
        inferirBIOS();
    }

    void inferirFormFactor() {
        string n = nombre + " " + modelo;

        if (n.find("ITX") != string::npos || n.find("Mini") != string::npos)
            formFactor = "ITX";
        else if (n.find("mATX") != string::npos || n.find("Micro") != string::npos)
            formFactor = "mATX";
        else
            formFactor = "ATX";
    }

    void inferirSocket() {
        if (chipset == "A320" || chipset == "B350" || chipset == "X370" ||
            chipset == "B450" || chipset == "X470" || chipset == "B550" ||
            chipset == "X570") socket = "AM4";

        if (chipset == "A620" || chipset == "B650" || chipset == "X670")
            socket = "AM5";

        if (chipset == "H410" || chipset == "B460" || chipset == "H470" ||
            chipset == "Z490" || chipset == "B560" || chipset == "H570" ||
            chipset == "Z590") socket = "LGA1200";

        if (chipset == "H610" || chipset == "B660" || chipset == "H670" ||
            chipset == "Z690" || chipset == "B760" || chipset == "Z790")
            socket = "LGA1700";

        if (socket == "")
            socket = "DESCONOCIDO";
    }

    void inferirRAM() {
        if (socket == "AM4" || socket == "LGA1200") {
            tipoRAM = "DDR4";
            maxFrecuenciaRAM = 3200;
        }
        else if (socket == "AM5") {
            tipoRAM = "DDR5";
            maxFrecuenciaRAM = 6000;
        }
        else if (socket == "LGA1700") {
            if (chipset == "H610" || chipset == "B660" || chipset == "B760")
                tipoRAM = "DDR4";
            else
                tipoRAM = "DDR5";

            maxFrecuenciaRAM = (tipoRAM == "DDR4" ? 3600 : 6400);
        }
    }

    void inferirPCIe() {
        if (chipset == "A320" || chipset == "B350" || chipset == "X370" ||
            chipset == "B450" || chipset == "X470")
            pcieVersion = 3;

        if (chipset == "B550" || chipset == "X570")
            pcieVersion = 4;

        if (chipset == "A620") pcieVersion = 4;
        if (chipset == "B650" || chipset == "X670") pcieVersion = 5;

        if (chipset == "Z490" || chipset == "B560" || chipset == "Z590")
            pcieVersion = 4;

        if (chipset == "Z690" || chipset == "Z790")
            pcieVersion = 5;
        if (chipset == "H610" || chipset == "B660" || chipset == "B760")
            pcieVersion = 4;
    }

    void inferirOC() {
        soportaOC =
            chipset[0] == 'X' ||
            chipset[0] == 'Z' ||
            chipset == "B550" ||
            chipset == "B650";
    }

    void inferirVRM() {
        if (chipset[0] == 'A') fasesVRM = 6;
        else if (chipset[0] == 'B') fasesVRM = 8;
        else if (chipset[0] == 'H') fasesVRM = 8;
        else if (chipset[0] == 'X') fasesVRM = 12;
        else if (chipset[0] == 'Z') fasesVRM = 14;
        else fasesVRM = 6;
    }

    void inferirAlmacenamiento() {
        if (chipset == "A320") { m2Slots = 1; m2PcieVersion = 3; m2SATACompat = true; }
        if (chipset == "B550") { m2Slots = 2; m2PcieVersion = 4; m2SATACompat = true; }
        if (chipset == "X570") { m2Slots = 2; m2PcieVersion = 4; m2SATACompat = true; }

        if (chipset == "A620") { m2Slots = 2; m2PcieVersion = 4; }
        if (chipset == "B650") { m2Slots = 3; m2PcieVersion = 5; }
        if (chipset == "X670") { m2Slots = 4; m2PcieVersion = 5; }
    }

    void inferirBIOS() {
        requiereBIOSUpdate =
            chipset == "B350" || chipset == "X370" || chipset == "B450" || chipset == "X470";
    }

    // ============================================================
    //                COMPATIBILIDAD CPU ↔ MOTHERBOARD
    // ============================================================
    CompatResult compatCPU(const CPU* cpu) const {
        if (!cpu) return {false, false, "CPU nula"};

        if (cpu->getSocket() != socket)
            return {false, false, "Socket incompatible (CPU " + cpu->getSocket() +
                                  " vs MB " + socket + ")"};

        if (tipoRAM == "DDR4" && !cpu->soportaDDR4())
            return {false, false, "MB usa DDR4, CPU no soporta DDR4"};

        if (tipoRAM == "DDR5" && !cpu->soportaDDR5())
            return {false, false, "MB usa DDR5, CPU no soporta DDR5"};

        bool bottleneck = pcieVersion < cpu->getPCIeVersionCPU();
        string msg = bottleneck
            ? "Compatible (PCIe bottleneck)"
            : "Compatible";


        if (cpu->getSoportaOC() && !soportaOC) {
            bottleneck = true;
            msg += " (CPU con OC pero motherboard no soporta OC)";
        }

        return {true, bottleneck, msg};
    }

    // ============================================================
    //                MOSTRAR INFO
    // ============================================================
    void mostrarInfo() const override {
        cout << "--- Motherboard ---\n";
        cout << "Nombre: " << nombre << "\n";
        cout << "Marca: " << marca << "\n";
        cout << "Modelo: " << modelo << "\n";
        cout << "Chipset: " << chipset << "\n";
        cout << "Socket: " << socket << "\n";
        cout << "FormFactor: " << formFactor << "\n";
        cout << "RAM: " << tipoRAM << " (" << slotsRAM << " slots, hasta "
             << maxFrecuenciaRAM << " MHz)\n";
        cout << "PCIe: Gen " << pcieVersion << "\n";
        cout << "M.2: " << m2Slots << " slots (PCIe Gen " << m2PcieVersion << ")\n";
        cout << "SATA: " << sataPorts << " puertos\n";
        cout << "OC: " << (soportaOC ? "Sí" : "No") << "\n";
        cout << "VRM: " << fasesVRM << " fases (" << getVRMQuality() << ")\n";
        cout << "BIOS Update: " << (requiereBIOSUpdate ? "Requerido" : "No") << "\n";
        cout << "Precio: $" << precio << "\n";
        cout << "Score: " << score << "\n";
    }

    // ============================================================
    //                CSV MINIMALISTA
    // ============================================================
    string toCSV() const override {
        stringstream ss;
        ss << "Motherboard,"
           << nombre << ","
           << marca << ","
           << modelo << ","
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

        getline(ss, temp, ','); precio = stod(temp);
        getline(ss, temp, ','); score = stod(temp);

        extraerChipset();
        inferirDatos();
    }

    // ============================================================
    //                       GETTERS PARA TODO
    // ============================================================
    string getSocket() const { return socket; }
    string getChipset() const { return chipset; }
    string getTipoRAM() const { return tipoRAM; }
    int getSlotsRAM() const { return slotsRAM; }
    int getMaxFrecuenciaRAM() const { return maxFrecuenciaRAM; }

    int getPCIeVersion() const { return pcieVersion; }

    int getSataPorts() const { return sataPorts; }
    int getM2Slots() const { return m2Slots; }
    int getM2PcieVersion() const { return m2PcieVersion; }
    bool getM2SATACompat() const { return m2SATACompat; }

    bool getSoportaOC() const { return soportaOC; }
    int getFasesVRM() const { return fasesVRM; }

    string getVRMQuality() const {
        if (fasesVRM >= 12) return "Alta";
        if (fasesVRM >= 8) return "Media";
        return "Baja";
    }

    bool getBIOSUpdateRequired() const { return requiereBIOSUpdate; }
    string getFormFactor() const { return formFactor; }
};

#endif // MOTHERBOARD_H
