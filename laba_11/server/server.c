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
    int server_socket, client_socket;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char message[MAX_MSG_LEN];
    struct passwd *pw = getpwuid(getuid());

    // Создание сокета
    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }

    // Настройка опции SO_REUSEADDR
    int enable = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1) {
        perror("Ошибка при настройке опции SO_REUSEADDR");
        exit(EXIT_FAILURE);
    }

    // Установка параметров сервера
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_PATH);

    // Привязка сокс-файла
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Ошибка при привязке сокета");
        exit(EXIT_FAILURE);
    }

    // Ожидание подключения клиента
    if (listen(server_socket, 5) == -1) {
        perror("Ошибка при ожидании подключения клиента");
        exit(EXIT_FAILURE);
    }

    printf("Ожидание подключения клиента...\n");

    // Принимаем соединение
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
    if (client_socket == -1) {
        perror("Ошибка при принятии соединения");
        exit(EXIT_FAILURE);
    }

    printf("Клиент подключен.\n");

    while (1) {
        memset(message, 0, MAX_MSG_LEN);

        // Чтение сообщения от клиента
        ssize_t bytes_received = recv(client_socket, message, MAX_MSG_LEN, 0);
        if (bytes_received <= 0) {
            perror("Ошибка при чтении сообщения");
            break;
        }

        printf("%s", message);

        // Отправка сообщения клиенту
        printf("Введите сообщение: ");
        fgets(message, MAX_MSG_LEN, stdin);

        // Отправляем сообщение с указанием пользователя
        char full_message[MAX_MSG_LEN + 20];
        sprintf(full_message, "%s: %s", pw->pw_name, message);
        send(client_socket, full_message, strlen(full_message), 0);
    }

    // Закрытие сокс-файла
    close(client_socket);
    close(server_socket);
    unlink(SOCKET_PATH);

    return 0;
}
