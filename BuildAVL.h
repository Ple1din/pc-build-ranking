#ifndef BUILD_AVL_H
#define BUILD_AVL_H

#include <vector>
#include <functional>
#include <string>
#include <algorithm>
#include "components/Build.h"

// Árbol AVL de builds ordenado por:
// 1) Precio total (Build::getPrecio())
// 2) Nombre (Build::getNombre()) en caso de empate.
//
// NOTA IMPORTANTE:
//  - El AVL NO es dueño de las Build* (no las deletea).
//  - Solo administra los nodos del árbol.

class BuildAVL {
private:
    struct Node {
        Build* build;
        Node* left;
        Node* right;
        int height;

        Node(Build* b)
            : build(b), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;

    // ----------- utilidades internas -----------

    inline int height(Node* n) const {
        return n ? n->height : 0;
    }

    inline int getBalance(Node* n) const {
        if (!n) return 0;
        return height(n->left) - height(n->right);
    }

    inline Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        // Rotación
        x->right = y;
        y->left = T2;

        // Actualizar alturas
        y->height = 1 + std::max(height(y->left), height(y->right));
        x->height = 1 + std::max(height(x->left), height(x->right));

        return x;
    }

    inline Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        // Rotación
        y->left = x;
        x->right = T2;

        // Actualizar alturas
        x->height = 1 + std::max(height(x->left), height(x->right));
        y->height = 1 + std::max(height(y->left), height(y->right));

        return y;
    }

    // Comparador de builds para el orden del árbol
    // (precio, y si empatan, nombre)
    inline bool lessBuild(Build* a, Build* b) const {
        if (a == b) return false;
        if (!a) return true;   // null < no-null (por seguridad)
        if (!b) return false;  // no-null > null

        double pa = a->getPrecio();
        double pb = b->getPrecio();

        if (pa < pb) return true;
        if (pa > pb) return false;

        // Si el precio es igual, ordenar por nombre
        return a->getNombre() < b->getNombre();
    }

    // ------------- funciones recursivas -------------

    inline Node* insertRec(Node* node, Build* b) {
        if (!node) return new Node(b);

        if (lessBuild(b, node->build)) {
            node->left = insertRec(node->left, b);
        } else if (lessBuild(node->build, b)) {
            node->right = insertRec(node->right, b);
        } else {
            // Misma clave (precio+nombre): ignorar duplicado
            return node;
        }

        // Actualizar altura
        node->height = 1 + std::max(height(node->left), height(node->right));

        // Balancear
        int balance = getBalance(node);

        // Caso IZQ-IZQ
        if (balance > 1 && lessBuild(b, node->left->build)) {
            return rotateRight(node);
        }

        // Caso DER-DER
        if (balance < -1 && lessBuild(node->right->build, b)) {
            return rotateLeft(node);
        }

        // Caso IZQ-DER
        if (balance > 1 && lessBuild(node->left->build, b)) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // Caso DER-IZQ
        if (balance < -1 && lessBuild(b, node->right->build)) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    inline Node* minNode(Node* node) const {
        if (!node) return nullptr;
        while (node->left) node = node->left;
        return node;
    }

    inline Node* maxNode(Node* node) const {
        if (!node) return nullptr;
        while (node->right) node = node->right;
        return node;
    }

    inline Node* removeRec(Node* node, Build* b) {
        if (!node) return nullptr;

        if (lessBuild(b, node->build)) {
            node->left = removeRec(node->left, b);
        } else if (lessBuild(node->build, b)) {
            node->right = removeRec(node->right, b);
        } else {
            // Encontramos el nodo a eliminar
            if (!node->left || !node->right) {
                Node* temp = node->left ? node->left : node->right;
                delete node;
                return temp;
            } else {
                // Dos hijos: sucesor en in-order (mínimo de la derecha)
                Node* temp = minNode(node->right);
                node->build = temp->build;
                node->right = removeRec(node->right, temp->build);
            }
        }

        if (!node) return nullptr;

        // Actualizar altura
        node->height = 1 + std::max(height(node->left), height(node->right));

        // Re-balancear
        int balance = getBalance(node);

        // IZQ-IZQ
        if (balance > 1 && getBalance(node->left) >= 0) {
            return rotateRight(node);
        }

        // IZQ-DER
        if (balance > 1 && getBalance(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // DER-DER
        if (balance < -1 && getBalance(node->right) <= 0) {
            return rotateLeft(node);
        }

        // DER-IZQ
        if (balance < -1 && getBalance(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    inline void clearRec(Node* node) {
        if (!node) return;
        clearRec(node->left);
        clearRec(node->right);
        delete node;
    }

    inline void inOrderRec(Node* node,
                           const std::function<void(Build*)>& visit) const {
        if (!node) return;
        inOrderRec(node->left, visit);
        visit(node->build);
        inOrderRec(node->right, visit);
    }

    inline void rangoPrecioRec(Node* node,
                               double minP, double maxP,
                               std::vector<Build*>& out) const {
        if (!node) return;

        double p = node->build->getPrecio();

        if (p > minP) {
            rangoPrecioRec(node->left, minP, maxP, out);
        }

        if (p >= minP && p <= maxP) {
            out.push_back(node->build);
        }

        if (p < maxP) {
            rangoPrecioRec(node->right, minP, maxP, out);
        }
    }

    inline int countRec(Node* node) const {
        if (!node) return 0;
        return 1 + countRec(node->left) + countRec(node->right);
    }

    inline void fillVectorRec(Node* node, std::vector<Build*>& v) const {
        if (!node) return;
        fillVectorRec(node->left, v);
        v.push_back(node->build);
        fillVectorRec(node->right, v);
    }

public:
    // ----------- interfaz pública -----------

    BuildAVL() : root(nullptr) {}

    ~BuildAVL() {
        clearRec(root);
    }

    bool empty() const {
        return root == nullptr;
    }

    // Inserta una build en el árbol (por precio+nombre)
    void insert(Build* b) {
        root = insertRec(root, b);
    }

    // Elimina una build exacta (mismo puntero)
    void remove(Build* b) {
        root = removeRec(root, b);
    }

    // Recorre el árbol en orden (de menor a mayor precio)
    void inOrder(const std::function<void(Build*)>& visit) const {
        inOrderRec(root, visit);
    }

    // Devuelve todas las builds en [minP, maxP]
    std::vector<Build*> buscarPorRangoPrecio(double minP, double maxP) const {
        std::vector<Build*> out;
        rangoPrecioRec(root, minP, maxP, out);
        return out;
    }

    // Devuelve un vector con TODAS las builds en orden por precio
    std::vector<Build*> toVector() const {
        std::vector<Build*> v;
        v.reserve(countRec(root));
        fillVectorRec(root, v);
        return v;
    }

    // Build más barata / más cara
    Build* getMasBarata() const {
        Node* m = minNode(root);
        return m ? m->build : nullptr;
    }

    Build* getMasCara() const {
        Node* m = maxNode(root);
        return m ? m->build : nullptr;
    }
};

#endif // BUILD_AVL_H
