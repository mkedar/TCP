#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

struct message {
    char username[100];
    char message[100];
};

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    short port = 8080;
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    listen(serverSocket, 5);

    printf("Listening on port %d...\n", port);

    struct message user_message;
    memset(&user_message, 0, sizeof(user_message));

    while (strcmp(user_message.message, "q") != 0) {
        int clientSocket = accept(serverSocket, NULL, NULL);
        recv(clientSocket, &user_message, sizeof(user_message), 0);
        printf("%s: %s (%lu bytes)\n", user_message.username, user_message.message, sizeof(user_message));
        if (strcmp(user_message.message, "q") == 0) {
            break;
        }
        memset(&user_message, 0, sizeof(user_message));
        close(clientSocket);
    }

    printf("Program terminated\n");
    close(serverSocket);

    return 0;
}
