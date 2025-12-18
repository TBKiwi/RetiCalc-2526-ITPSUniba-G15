#include <stdio.h>
#include <string.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    typedef SOCKET socket_t;
    #define close_socket closesocket
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    typedef int socket_t;
    #define close_socket close
#endif

#define PORT 8080
#define BUF_SIZE 1024

int main() {
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
#endif

    socket_t sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    connect(sock, (struct sockaddr*)&server, sizeof(server));

    send(sock, "Hello", 5, 0);

    char buffer[BUF_SIZE];
    printf("Inserisci una stringa: ");
    fgets(buffer, BUF_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0;

    send(sock, buffer, strlen(buffer), 0);

    int n = recv(sock, buffer, BUF_SIZE, 0);
    buffer[n] = '\0';

    printf("Risposta dal server: %s\n", buffer);

    close_socket(sock);

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}

