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

#define BUFFER_MAX 1024

int main() {
#ifdef _WIN32
    WSADATA wsa_data;
    WSAStartup(MAKEWORD(2,2), &wsa_data);
#endif

    socket_t client_socket = socket(AF_INET, SOCK_DGRAM, 0);

    char server_host[256];
    int server_port;

    printf("Inserisci nome host: ");
    fgets(server_host, sizeof(server_host), stdin);

    char *newline_ptr = strchr(server_host, '\n');
    if (newline_ptr) *newline_ptr = '\0';

    printf("Inserisci porta server: ");
    scanf("%d", &server_port);
    while (getchar() != '\n');

    struct addrinfo hints, *resolved_addr = NULL;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo(server_host, NULL, &hints, &resolved_addr) != 0) {
        printf("Impossibile risolvere l'hostname\n");
        return 1;
    }

    struct sockaddr_in server_address;
    memcpy(&server_address, resolved_addr->ai_addr, sizeof(struct sockaddr_in));
    server_address.sin_port = htons(server_port);
    freeaddrinfo(resolved_addr);

    char *handshake_message = "Hello";
    sendto(
        client_socket,
        handshake_message,
        strlen(handshake_message),
        0,
        (struct sockaddr*)&server_address,
        sizeof(server_address)
    );

    char message_buffer[BUFFER_MAX];
    printf("Inserisci stringa: ");
    fgets(message_buffer, BUFFER_MAX, stdin);

    newline_ptr = strchr(message_buffer, '\n');
    if (newline_ptr) *newline_ptr = '\0';

    sendto(
        client_socket,
        message_buffer,
        strlen(message_buffer),
        0,
        (struct sockaddr*)&server_address,
        sizeof(server_address)
    );

    struct sockaddr_in sender_address;
    socklen_t sender_address_len = sizeof(sender_address);

    int bytes_received = recvfrom(
        client_socket,
        message_buffer,
        BUFFER_MAX,
        0,
        (struct sockaddr*)&sender_address,
        &sender_address_len
    );

    if (bytes_received > 0) {
        message_buffer[bytes_received] = '\0';

        char server_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &sender_address.sin_addr, server_ip, sizeof(server_ip));

        char server_hostname[NI_MAXHOST];
        if (getnameinfo(
                (struct sockaddr*)&sender_address,sender_address_len,server_hostname,sizeof(server_hostname),NULL,0,0) != 0) {
            strcpy(server_hostname, "N/A");
        }

        printf("Stringa %s\nRicevuta dal server nome: %s\nIndirizzo: %s\n",
               message_buffer, server_hostname, server_ip);
    }

    close_socket(client_socket);

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
