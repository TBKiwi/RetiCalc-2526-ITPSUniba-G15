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
    #include <netdb.h>
    #include <unistd.h>
    typedef int socket_t;
    #define close_socket close
#endif

#define BUF_SIZE 1024

int main() {
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
#endif

    socket_t sock = socket(AF_INET, SOCK_DGRAM, 0);

    char hostname[256];
    int port;

    
    printf("Inserisci nome host: ");
    fgets(hostname, sizeof(hostname), stdin);
    hostname[strcspn(hostname, "\n")] = 0;

    printf("Inserisci porta server: ");
    scanf("%d", &port);
    getchar();

    
    struct addrinfo hints = {0}, *res = NULL;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo(hostname, NULL, &hints, &res) != 0) {
        printf("Errore risoluzione hostname\n");
        return 1;
    }

    struct sockaddr_in server_addr = *(struct sockaddr_in*)res->ai_addr;
    server_addr.sin_port = htons(port);
    freeaddrinfo(res);

    
    sendto(sock, "Hello", 5, 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

    
    char buffer[BUF_SIZE];
    printf("Inserisci stringa: ");
    fgets(buffer, BUF_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0;

    sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

    
    struct sockaddr_in from_addr;
    socklen_t from_len = sizeof(from_addr);
    int n = recvfrom(sock, buffer, BUF_SIZE, 0, (struct sockaddr*)&from_addr, &from_len);
    buffer[n] = '\0';

    
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &from_addr.sin_addr, ip, sizeof(ip));

    char hostname_resp[NI_MAXHOST];
    if (getnameinfo((struct sockaddr*)&from_addr, from_len, hostname_resp, sizeof(hostname_resp), NULL, 0, 0) != 0) {
        strcpy(hostname_resp, "Hostname non trovato");
    }

    printf("Stringa %s\nRicevuta dal server nome:%s\nIndirizzo:%s\n", buffer, hostname_resp, ip);

    close_socket(sock);
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
