#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>  // Включаем заголовочный файл для функции close

#define MAX_BUFFER_SIZE 1024

int main(int argc, char **argv){
 
  if (argc != 2) {
    printf("Usage: %s <port>\n", argv[0]);
    exit(0);
  }
 
  char *ip = "127.0.0.1";
  int port = atoi(argv[1]);
 
  int sockfd;
  struct sockaddr_in server_addr, client_addr;
  char buffer[MAX_BUFFER_SIZE];
  socklen_t addr_size;
  int n;
 
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    perror("[-]socket error");
    exit(1);
  }
 
  memset(&server_addr, '\0', sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr(ip);
 
  n = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (n < 0){
    perror("[-]bind error");
    exit(1);
  }
 
  addr_size = sizeof(client_addr);
  int file_size;
  FILE *received_file = fopen("received_file.txt", "wb");

  if (received_file == NULL) {
    perror("[-] Error opening the file");
    exit(1);
  }
  
  // Передаем размер файла клиенту
  recvfrom(sockfd, &file_size, sizeof(int), 0, (struct sockaddr*)&client_addr, &addr_size);

  while (file_size > 0) {
    int bytes_received = recvfrom(sockfd, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &addr_size);

    if (bytes_received < 0) {
      perror("[-]recvfrom error");
      break;
    }

    fwrite(buffer, 1, bytes_received, received_file);
    file_size -= bytes_received;
  }

  fclose(received_file);

  printf("[+] File received and saved as 'received_file.txt'\n");

  close(sockfd);

  return 0;
}
