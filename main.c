#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080

void menu() {
    printf("Seleccione una opcion:\n");
    printf("1. Generar nombre de usuario\n");
    printf("2. Generar contrasenia\n");
    printf("3. Salir\n");
}

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char request[1024];

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("No se pudo crear socket : %d", WSAGetLastError());
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (serv_addr.sin_addr.s_addr == INADDR_NONE) {
        printf("\n Dirección no válida / No soportada \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\n Error de conexión \n");
        return -1;
    }

    int opcion;
    int largo;
    while (1) {
        menu();
        scanf("%d", &opcion);

        if (opcion == 3) {
            printf("Saliendo\n");
            break;
        }

        printf("Ingrese la longitud deseada: ");
        scanf("%d", &largo);

        if (opcion == 1) {
            snprintf(request, sizeof(request), "U%d", largo);  // "U" para usuario
        } else if (opcion == 2) {
            snprintf(request, sizeof(request), "P%d", largo);  // "P" para contrasenia
        } else {
            printf("Opcion invalida\n");
            continue;
        }

        send(sock, request, strlen(request), 0);

        int valread = recv(sock, buffer, sizeof(buffer), 0);
        buffer[valread] = '\0';
        printf("Respuesta del servidor: %s\n", buffer);
    }

    closesocket(sock);
    WSACleanup();

    return 0;
}
