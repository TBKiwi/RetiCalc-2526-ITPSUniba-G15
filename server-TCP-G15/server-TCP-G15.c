#include <stdio.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    typedef SOCKET socket_t;
    typedef int socklen_t;
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

    socket_t server = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server, (struct sockaddr*)&addr, sizeof(addr));
    listen(server, 5);

    printf("Server in ascolto sulla porta %d...\n", PORT);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);

        socket_t client = accept(server, (struct sockaddr*)&client_addr, &len);

        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip));
        printf("Ricevuti dati dal client con indirizzo: %s\n", ip);

        char buffer[BUF_SIZE];
        
        recv(client, buffer, BUF_SIZE, 0);

        int n = recv(client, buffer, BUF_SIZE, 0);
        buffer[n] = '\0';

        printf("Stringa ricevuta: %s\n", buffer);

        
        char risposta[BUF_SIZE];
        int j = 0;
        for (int i = 0; buffer[i]; i++) {
            char c = tolower(buffer[i]);
            if (!strchr("aeiouAEIOU", c))
                risposta[j++] = buffer[i];
        }
        risposta[j] = '\0';

        send(client, risposta, strlen(risposta), 0);

        close_socket(client);
    }

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}

