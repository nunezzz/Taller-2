# include <iostream>
# include "Client.h"
using namespace std;

Client :: Client (string type,string name, int numAtencion)
{
    this -> type = type;
    this -> name = name;
    this -> numAtencion = numAtencion;
}
string Client :: getType(){return this -> type;}
void Client :: setType(string type){this -> type = type;}
string Client :: getName(){return this-> name;}
void Client :: setName(string name){this -> name = name;}
int Client :: getNumAtencion(){return this-> numAtencion;}
void Client :: setNumAtencion(int numAtencion){this -> numAtencion = numAtencion;}

// Comparar clientes por su numero de atencion para determinar la prioridad
bool Client::operator < (const Client& other) const {
    if (this->getPriority() == other.getPriority()) {
        return this->numAtencion > other.numAtencion; // Mayor número de llegada tiene menor prioridad
    }
    return this->getPriority() > other.getPriority(); // Menor valor de prioridad tiene mayor prioridad
}

int Client::getPriority() const {
    if (type == "TERCERA EDAD") return 1;
    if (type == "DISCAPACITADO") return 2;
    if (type == "EMBARAZADA") return 3;
    return 4;// NORMAL

}

void Client::addProduct(const Product& product) {
    shoppingList.push_back(product);
}