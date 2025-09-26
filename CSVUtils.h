/*
* CSVUtils.h
 * Funciones para manejar el archivo CSV de las builds
 */

#ifndef CSVUTILS_H
#define CSVUTILS_H

#include <vector>
#include <string>
#include "Build.h"

using namespace std;

// Cargar builds existentes desde el archivo CSV
void cargarDesdeCSV(const string &nombreArchivo, vector<Build> &builds);

// Guardar una nueva build al final del CSV
void guardarEnCSV(const string &nombreArchivo, const Build &build);

#endif