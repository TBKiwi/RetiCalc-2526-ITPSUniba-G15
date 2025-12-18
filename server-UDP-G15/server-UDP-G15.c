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
    #include <netdb.h>
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

    socket_t sock = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    printf("Server UDP in ascolto sulla porta %d\n", PORT);

    while (1) {
        char buffer[BUF_SIZE];
        int n = recvfrom(sock, buffer, BUF_SIZE, 0, (struct sockaddr*)&client_addr, &client_len);
        if (n < 0) break;
        buffer[n] = '\0';

        
        if (strcmp(buffer, "Hello") == 0)
            continue;

        
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip));

        char hostname[NI_MAXHOST];
        if (getnameinfo((struct sockaddr*)&client_addr, client_len, hostname, sizeof(hostname), NULL, 0, 0) != 0) {
            strcpy(hostname, "Hostname non trovato");
        }

        printf("Ricevuti dei dati dal client nome: %s\nIndirizzo: %s\n", hostname, ip);

        char risposta[BUF_SIZE];
        int j = 0;
        for (int i = 0; buffer[i]; i++) {
            char c = tolower(buffer[i]);
            if (!strchr("aeiouAEIOU", c))
                risposta[j++] = buffer[i];
        }
        risposta[j] = '\0';

        sendto(sock, risposta, strlen(risposta), 0, (struct sockaddr*)&client_addr, client_len);
    }

#ifdef _WIN32
    WSACleanup();
#endif
    close_socket(sock);
    return 0;
}
