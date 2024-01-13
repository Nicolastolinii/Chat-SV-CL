#ifndef _SOCKETSV_C_
#define _SOCKETSV_C_
#include "socketSV.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

threadParams *dataThread(SOCKET clSocket, char *ipCliente)
{
    threadParams *params = (threadParams *)malloc(sizeof(threadParams));
    strcpy(params->ipClient, ipCliente);
    params->clientSocket = clSocket;
    return params;
};

Data *clInfo(unsigned short puerto, unsigned short puertoCliente, char *ipCliente)
{

    Data *struct_datos = (Data *)malloc(sizeof(Data));
    struct_datos->puerto = puerto;
    strcpy(struct_datos->ipClient, ipCliente);
    struct_datos->puertoClient = puertoCliente;
    return struct_datos;
} /*funcion para inicializar espacio en memoria y rellenar los datos del struct*/

int main()
{
    struct sockaddr_in clientAddr;
    //"clientaddr" struct para la info del sock
    Data *datos = clInfo(PORT_SERVER_CLIENT, ntohs(clientAddr.sin_port), inet_ntoa(clientAddr.sin_addr)); /*inicializamos un puntero a la funcion "info" y rellenamos con los datos correspondientes*/

    /*Los HANDLE se utilizan para hacer referencia a objetos del sistema operativo, como hilos, procesos, archivos, semáforos, etc.*/
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);                                 /*WSAStartup es una función de la API de Windows Sockets (Winsock) que se utiliza para inicializar la biblioteca de sockets de Windows en una aplicación.*/
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Crear el socket

    /*Configuramos el socket*/
    struct sockaddr_in serverAddr; // Estructura para la info necesaria para configurar el socket
    serverAddr.sin_family      = AF_INET;
    serverAddr.sin_port        = htons(datos->puerto);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    /*Configuramos el socket*/
    bind(server_socket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)); // vincula(bind) el socket a una direccion ip y puerto especifico.
    listen(server_socket, MAX_CLIENTS);// Cantidad de conexiones que va aceptar

    printf("puerto en escucha %d...\n", datos->puerto);
    while (1)
    {
        int clientAddrSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(server_socket, (struct sockaddr *)&clientAddr, &clientAddrSize); // Acepta la conexion entrante.
        printf("ClientSocket: %lld\n", clientSocket);

        if (clientSocket == INVALID_SOCKET)
        {
            perror("Error conexion no aceptada");
            continue;
        }
        printf(RED "[" RESET "Server" RED "]" RESET "Cliente conectado - [IP]: %s, [Puerto]: %d\n",
               inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
        if (num_clients < 10) // si el num de clientes no excede el limite.
        {
            client_sockets[num_clients] = clientSocket; // se almacena un nuevo sock en el arreglo
            num_clients++;

            client_threads[num_clients - 1] = (HANDLE)_beginthread(HandleClient, 0, dataThread(clientSocket, inet_ntoa(clientAddr.sin_addr))); // se crea un nuevo hilo para manejar este nuevo cliente.
        }
        else
        {
            printf("Número máximo de conexiones\n");
            closesocket(server_socket);
        }
    }

    closesocket(server_socket);
    free(datos);
    WSACleanup();
    return 0;
}

void HandleClient(void *param)
{
    threadParams *params = (threadParams *)param;
    SOCKET socket = params->clientSocket; // Recibimos el socket por parametro de la estructura.
    char *ipClient = params->ipClient;
    // printf("IP del cliente: %s\n", ipClient);
    // printf("Socket del cliente: %lld\n", socket);

    char mensajes[MAX_BUFFER_SERVER_CLIENT];
    memset(mensajes, 0, sizeof(mensajes));

    while (1)
    {
        // printf("IP del cliente: %s\n", params->ipClient);
        // printf("Socket del cliente: %lld\n", params->clientSocket);
        int bytesReceived = recv(socket, mensajes, sizeof(mensajes), 0); /*recibimos datos del cliente y lo guardamos en el bufer "mensajes", "byteReceived" almacena la cantidad de bytes recibidos en el momento*/
        if (bytesReceived > 0)
        {
            mensajes[bytesReceived] = '\0'; // caracter NULO al final de la cadena
            printf(GREEN "[*]%s %s\n" RESET, ipClient, mensajes);

            for (int i = 0; i < num_clients; i++)
            /*se itera sobre todos los clientes conectados y si el socket del cliente actual no es el mismo que el del cliente que envio el msj, se usa "send" para enviar el msj al cliente actual*/
            {
                if (client_sockets[i] != socket) //
                {
                    send(client_sockets[i], mensajes, sizeof(mensajes), 0);
                }
            }
        }
        else if (bytesReceived == 0)
        {
            printf("Conexión cerrada por el otro extremo.\n");
            break;
        }
        else
        {
            int recvError = WSAGetLastError();
            fprintf(stderr, "Error en recv: %d\n", recvError);
            break;
        }
    }

    for (int i = 0; i < num_clients; i++)
    /*Se busca el socket del cliente que se manejo en esta funcion dentro del arreglo de clientes y se elimina para que no se intente enviar msj a un cliente que ya no esta conectado*/
    {
        if (client_sockets[i] == socket)
        {
            for (; i < num_clients - 1; i++)
            {
                client_sockets[i] = client_sockets[i + 1];
            }
            num_clients--;
            break;
        }
    }

    closesocket(socket);
}
#endif