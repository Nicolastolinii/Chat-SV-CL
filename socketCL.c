#ifndef _SOCKETCL_C_
#define _SOCKETCL_C_
#include "socketCL.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 0), &wsa);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    printf("Ip del servidor quiere conectar: ");
    scanf("%s", ip);
    printf("Por favor, ingrese su nombre: ");
    scanf("%s", name_user);

    struct sockaddr_in direccion;
    direccion.sin_family = AF_INET;
    direccion.sin_port = htons(PORT_SERVER_CLIENT);
    direccion.sin_addr.s_addr = inet_addr(ip);

    connect(sock, (struct sockaddr *)&direccion, sizeof(direccion));
    HANDLE hThread = (HANDLE)_beginthread(ReceiveMessages, 0, &sock);

    printf("\nMensaje a enviar(o escriba 'salir' para salir): \n\n");
    
    while (1)
    {
        printf(RED"\n%s: "RESET, name_user);
        fgets(mensajes, sizeof(mensajes), stdin);
        for (int i = 0; i < sizeof(mensajes); i++)
            if (mensajes[i] == '\n')
                mensajes[i] = '\0';
            
        char mensajeCompleto[200];
        snprintf(mensajeCompleto, sizeof(mensajeCompleto), "%s: %s", name_user, mensajes);
        send(sock, mensajeCompleto, sizeof(mensajeCompleto), 0);
        if (strcmp(mensajes, "salir\n") == 0)
            exit(0);
    }

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);

    closesocket(sock);
    WSACleanup();
    return 0;
}
void ReceiveMessages(void *param)
{
    int sock = *(int *)param;
    // char mensajer[200];
    char mensajeCompleto[200];

    while (1)
    {
         int bytesReceived = recv(sock, mensajeCompleto, sizeof(mensajeCompleto), 0);
        if (bytesReceived > 0)
        {
            mensajeCompleto[bytesReceived] = '\0';
            printf(GREEN"\n%s\n", mensajeCompleto, RESET);
            memset(mensajeCompleto, 0, 200);
        }
        else if (bytesReceived == 0)
        {
            // El socket se ha cerrado desde el otro extremo.
            printf("Conexión cerrada por el otro extremo.\n");
            break;
        }
        else
        {
            perror("Error en recv");
            break;
        }
    }
}
#endif