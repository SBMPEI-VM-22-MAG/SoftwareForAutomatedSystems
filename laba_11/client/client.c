#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pwd.h>

#define SOCKET_PATH "/home/kinwend17/laba_11/chat_socket"
#define MAX_MSG_LEN 1024

int main() {
    int client_socket;
    struct sockaddr_un server_addr;
    char message[MAX_MSG_LEN];
    struct passwd *pw = getpwuid(getuid());

    if (pw == NULL) {
        perror("Ошибка при получении информации о текущем пользователе");
        exit(EXIT_FAILURE);
    }

    // Создание сокета
    client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }

    // Установка параметров сервера
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_PATH);

    // Подключение к серверу
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Ошибка при подключении к серверу");
        exit(EXIT_FAILURE);
    }

    while (1) {
        memset(message, 0, MAX_MSG_LEN);

        // Отправка сообщения серверу
        printf("Введите сообщение: ");
        fgets(message, MAX_MSG_LEN, stdin);

        // Формирование сообщения с указанием пользователя
        char full_message[MAX_MSG_LEN + 20];
        sprintf(full_message, "%s: %s", pw->pw_name, message);
        send(client_socket, full_message, strlen(full_message), 0);

        // Чтение ответа от сервера
        ssize_t bytes_received = recv(client_socket, message, MAX_MSG_LEN, 0);
        if (bytes_received <= 0) {
            perror("Ошибка при чтении ответа");
            break;
        }

        printf("%s", message);
    }

    // Закрытие сокета клиента
    close(client_socket);

    return 0;
}
