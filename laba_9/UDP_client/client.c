#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> 

#define MAX_BUFFER_SIZE 1024

int main(int argc, char **argv){
 
  if (argc != 2) {
    printf("Usage: %s <port>\n", argv[0]);
    exit(0);
  }
 
  char *ip = "192.168.203.223";
  int port = atoi(argv[1]);

  int sockfd;
  struct sockaddr_in addr;
  socklen_t addr_size;

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  memset(&addr, '\0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip);

  FILE *file_to_send = fopen("file_to_send.txt", "rb");

  if (file_to_send == NULL) {
    perror("[-] Error opening the file");
    exit(1);
  }

  fseek(file_to_send, 0, SEEK_END);
  int file_size = ftell(file_to_send);
  fseek(file_to_send, 0, SEEK_SET);

  // Отправляем размер файла серверу
  sendto(sockfd, &file_size, sizeof(int), 0, (struct sockaddr*)&addr, sizeof(addr));

  char buffer[MAX_BUFFER_SIZE];
  int bytes_sent;
  
  while (1) {
    int bytes_read = fread(buffer, 1, MAX_BUFFER_SIZE, file_to_send);

    if (bytes_read <= 0) {
      break;
    }

    bytes_sent = sendto(sockfd, buffer, bytes_read, 0, (struct sockaddr*)&addr, sizeof(addr));

    if (bytes_sent < 0) {
      perror("[-]sendto error");
      break;
    }
  }

  fclose(file_to_send);

  printf("[+] File sent successfully\n");

  close(sockfd);

  return 0;
}
