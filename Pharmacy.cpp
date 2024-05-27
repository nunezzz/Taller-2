#include "Pharmacy.h"
#include <iostream>
#include <queue>
using namespace std;

Pharmacy::Pharmacy() : contLlegada(0) {}

int Pharmacy::getNextNumAtencion() {
    return ++contLlegada;
}

void Pharmacy::agregarCliente(const Client& client) {
    if (client.type == "NORMAL") {
        filaNormal.push(client);
    } else {
        // Agregar cliente preferencial manteniendo el orden de prioridad
        int priority = client.getPriority();
        if (priority == 1) {
            filaPreferencial.push(client); // Tercera Edad
        } else if (priority == 2) {
            priority_queue<Client> tempQueue;
            while (!filaPreferencial.empty() && filaPreferencial.top().getPriority() == 1) {
                tempQueue.push(filaPreferencial.top());
                filaPreferencial.pop();
            }
            filaPreferencial.push(client);
            while (!tempQueue.empty()) {
                filaPreferencial.push(tempQueue.top());
                tempQueue.pop();
            }
        } else if (priority == 3) {
            priority_queue<Client> tempQueue;
            while (!filaPreferencial.empty() && filaPreferencial.top().getPriority() == 1) {
                tempQueue.push(filaPreferencial.top());
                filaPreferencial.pop();
            }
            while (!filaPreferencial.empty() && filaPreferencial.top().getPriority() == 2) {
                tempQueue.push(filaPreferencial.top());
                filaPreferencial.pop();
            }
            filaPreferencial.push(client);
            while (!tempQueue.empty()) {
                filaPreferencial.push(tempQueue.top());
                tempQueue.pop();
            }
        }
    }
}

Client Pharmacy::atenderCliente() {
    if (!filaPreferencial.empty()) {
        Client cliente = filaPreferencial.top();
        filaPreferencial.pop();
        return cliente;
    } else if (!filaNormal.empty()) {
        Client cliente = filaNormal.front();
        filaNormal.pop();
        return cliente;
    } else {
        cout << "No hay clientes en la fila." << endl;
        return Client("", "", 0); // Devolver un cliente vacío
    }
}