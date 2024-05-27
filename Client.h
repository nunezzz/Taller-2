# pragma once 
# include <iostream>
#include <string>
#include <vector>
#include "Product.h"

using namespace std;

class Client
{
    public:
        string type; 
        string name;
        int numAtencion;
        std::vector<Product> shoppingList; // Lista de productos que el cliente desea comprar

    public:
    Client (string, string, int);
        string getType();
        void setType(string);
        string getName();
        void setName(string);
        int getNumAtencion();
        void setNumAtencion(int);

        // Sobrecarga del operador < para ser usado en priority_queue
        bool operator < (const Client& other) const;

public:
    int getPriority() const;
    void addProduct(const Product& product); // Método para agregar productos a la lista de compras
};