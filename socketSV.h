#ifndef _SOCKETSV_H_
#define _SOCKETSV_H_
#include "global.h"

#define MAX_CLIENTS 16

SOCKET client_sockets[10]; // Crea un arreglo de 10 Sockets.
int num_clients = 0;       // Contador de cantidad de Clientes

typedef struct threadParams
{
    SOCKET clientSocket; // donde vamos a guardar el socket para pasarle al thread
    char ipClient[16];   // ip del cliente
} threadParams;          // estructura para pasarle al thread

typedef struct Data
{
    unsigned short puerto;       // puerto por el que se van a conectar al SV
    char ipClient[16];           // ip del cliente que se conecta
    unsigned short puertoClient; // puerto por el q cliente que se conecta
} Data;                          // estructura para datos del cliente.

void HandleClient(void *param);
threadParams *dataThread(SOCKET clSocket, char *ipCliente);
Data *clInfo(unsigned short puerto, unsigned short puertoCliente, char *ipCliente);

static HANDLE client_threads[10]; // Crea un arreglo de hilos

#include "socketSV.c"
#endif