#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include <iostream>
using namespace std;

struct CompatResult {
    bool compatible;
    bool bottleneck;
    string message;
};

class Component {
protected:
    string categoria;
    string nombre;
    string marca;
    string modelo;
    string sufijo;
    double precio;
    double score;

public:
    Component(string categoria = "", string nombre = "", string marca = "",
              string modelo = "", string sufijo = "",
              double precio = 0, double score = 0)
        : categoria(categoria), nombre(nombre), marca(marca),
          modelo(modelo), sufijo(sufijo),
          precio(precio), score(score) {}

    virtual ~Component() {}


    virtual bool esCompatibleCon(const Component* other) const {
        // Por defecto asumimos que NO aplica compatibilidad genérica.
        return true;
    }

    // Métodos generales
    virtual void mostrarInfo() const = 0;
    virtual string toCSV() const = 0;
    virtual void fromCSV(const string& line) = 0;

    // Getters
    string getCategoria() const { return categoria; }
    string getNombre() const { return nombre; }
    string getMarca() const { return marca; }
    string getModelo() const { return modelo; }
    double getPrecio() const { return precio; }
    double getScore() const { return score; }
};

#endif