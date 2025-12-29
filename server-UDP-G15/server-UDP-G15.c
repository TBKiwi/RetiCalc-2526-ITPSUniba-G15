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

#define LISTENING_PORT 8080
#define MAX_BUFFER_SIZE 1024

int main() {
#ifdef _WIN32
    WSADATA wsa_data;
    WSAStartup(MAKEWORD(2,2), &wsa_data);
#endif

    socket_t server_socket = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in local_address;
    memset(&local_address, 0, sizeof(local_address));
    local_address.sin_family = AF_INET;
    local_address.sin_port = htons(LISTENING_PORT);
    local_address.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr*)&local_address, sizeof(local_address));

    printf("Servizio UDP attivo sulla porta %d\n", LISTENING_PORT);

    while (1) {
        struct sockaddr_in client_address;
        socklen_t client_address_len = sizeof(client_address);
        char input_buffer[MAX_BUFFER_SIZE];

        int bytes_received = recvfrom(server_socket, input_buffer, MAX_BUFFER_SIZE, 0,(struct sockaddr*)&client_address, &client_address_len);

        if (bytes_received < 0)
            break;

        input_buffer[bytes_received] = '\0';

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_address.sin_addr, client_ip, sizeof(client_ip));

        char client_hostname[NI_MAXHOST];
        if (getnameinfo((struct sockaddr*)&client_address, client_address_len,client_hostname, sizeof(client_hostname), NULL, 0, 0) != 0) {
            strcpy(client_hostname, "Sconosciuto");
        }

        printf("Messaggio da: %s Indirizzo: %s\n", client_hostname, client_ip);

        if (strcmp(input_buffer, "Hello") != 0) {
            char output_buffer[MAX_BUFFER_SIZE];
            int output_index = 0;

            for (int input_index = 0; input_buffer[input_index] != '\0'; input_index++) {
                char current_char = input_buffer[input_index];
                char lower_char = tolower((unsigned char)current_char);

                if (strchr("aeiou", lower_char) == NULL) {
                    output_buffer[output_index++] = current_char;
                }
            }

            output_buffer[output_index] = '\0';

            sendto(server_socket, output_buffer, strlen(output_buffer), 0,(struct sockaddr*)&client_address, client_address_len);
        }
    }

    close_socket(server_socket);

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
