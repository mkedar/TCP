#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

struct message {
    char username[100];
    char message[100];
};

void sendMessageOverTCP(const char *serverIP, int serverPort, const struct message *user_message) {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("socket");
        return;
    }

    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);
    inet_pton(AF_INET, serverIP, &serverAddress.sin_addr);

    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("connect");
        close(clientSocket);
        return;
    }

    send(clientSocket, user_message, sizeof(struct message), 0);
    printf("Message sent: %s\n", user_message->message);
    close(clientSocket);
}

int main() {
    char username[100];
    char message[100];
    short port = 8080;
    printf("Sending message on port %d...\n",port);


    printf("Please enter your desired username: ");
    scanf("%s", username);

    while (1) {
        printf("Enter a message (or 'q' to quit): ");
        scanf(" %[^\n]", message);

        struct message user_message;
        strcpy(user_message.username, username);
        strcpy(user_message.message, message);

        sendMessageOverTCP("127.0.0.1", port, &user_message);
        if (strcmp(message, "q") == 0) {
            break;
        }
    }

    printf("Program terminated\n");
    return 0;
}
