#include "Client.h"
#include <queue>
#include <string>

class Pharmacy {
    int contLlegada; // Contador llegada
    queue<Client> filaNormal;
    std::priority_queue<Client> filaPreferencial; // Cola preferencial

public:
    Pharmacy();
    int getNextNumAtencion();
    void agregarCliente(const Client& client);
    Client atenderCliente();
};