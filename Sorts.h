#ifndef SORTS_H
#define SORTS_H

#include <vector>
#include <algorithm>
using namespace std;

class Sorts {
public:

    // -------------------------------
    //  ORDENAMIENTOS CON std::sort
    // -------------------------------
    template<typename T>
    static void ordenarPorPrecio(vector<T*>& v) {
        sort(v.begin(), v.end(), [](T* a, T* b){
            return a->getPrecio() < b->getPrecio();
        });
    }

    template<typename T>
    static void ordenarPorScore(vector<T*>& v) {
        sort(v.begin(), v.end(), [](T* a, T* b){
            return a->getScore() < b->getScore();
        });
    }

    template<typename T>
    static void ordenarPorMarca(vector<T*>& v) {
        sort(v.begin(), v.end(), [](T* a, T* b){
            return a->getMarca() < b->getMarca();
        });
    }


    // ===========================================
    //              MERGESORT POR SCORE
    // ===========================================

    template<typename T>
    static void mergeSortPorScore(vector<T*>& v) {
        if (v.size() <= 1) return;
        mergeSortScoreRec(v, 0, v.size() - 1);
    }

private:

    template<typename T>
    static void mergeSortScoreRec(vector<T*>& v, int left, int right) {
        if (left >= right) return;

        int mid = (left + right) / 2;
        mergeSortScoreRec(v, left, mid);
        mergeSortScoreRec(v, mid + 1, right);
        mergeScore(v, left, mid, right);
    }

    template<typename T>
    static void mergeScore(vector<T*>& v, int left, int mid, int right) {
        vector<T*> temp;
        temp.reserve(right - left + 1);

        int i = left;
        int j = mid + 1;

        // Mezcla ordenada por Score (ascendente)
        while (i <= mid && j <= right) {
            if (v[i]->getScore() <= v[j]->getScore()) {
                temp.push_back(v[i]);
                i++;
            } else {
                temp.push_back(v[j]);
                j++;
            }
        }

        // Copiar restos
        while (i <= mid) {
            temp.push_back(v[i]);
            i++;
        }
        while (j <= right) {
            temp.push_back(v[j]);
            j++;
        }

        // Regresar al vector original
        for (int k = 0; k < temp.size(); ++k) {
            v[left + k] = temp[k];
        }
    }
};

#endif