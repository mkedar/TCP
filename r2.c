#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 32768  // Adjusted buffer size

int main() {
    int server_fd, client_fd, port;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t client_len;

    // Prompt user for port number
    printf("Enter port number to listen on: ");
    scanf("%d", &port);

    // Create server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Cannot create socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen on the socket
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Listening on port %d...\n", port);

    // Infinite loop to handle incoming connections
    while (1) {
        client_len = sizeof(client_addr);
        if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len)) < 0) {
            perror("Accept failed");
            continue;
        }

        printf("Connected to client: %s\n", inet_ntoa(client_addr.sin_addr));

        // Read and print the data
        ssize_t bytes_read;
        while ((bytes_read = recv(client_fd, buffer, BUFFER_SIZE, 0)) > 0) {
            fwrite(buffer, sizeof(char), bytes_read, stdout);
        }

        if (bytes_read < 0) {
            perror("Read error");
        }

        close(client_fd);
    }

    close(server_fd);
    return 0;
}
