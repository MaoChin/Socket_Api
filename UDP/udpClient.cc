#include <cassert>
#include <cstring>
#include <iostream>
#include <string>
#include <unistd.h>

// net
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "Log.hpp"

static void Usage(const std::string name) {
  std::cout << "Usage:\n\t" << name << " ip port" << std::endl;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    Usage(argv[0]);
    exit(3);
  }
  std::string ip = argv[1];
  uint16_t port = atoi(argv[2]);

	// 基于文件描述符
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  assert(sockfd > 0);

  logMessage(FATAL, "socket create success:%d", sockfd);

  // 客户端不用自己bind，让操作系统搞就好了
  struct sockaddr_in server;
  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(ip.c_str());
  server.sin_port = htons(port);

  std::string outbuffer;
  while (true) {
    std::cout << "please enter# ";
    std::getline(std::cin, outbuffer);
    sendto(sockfd, outbuffer.c_str(), outbuffer.size(), 0,
           (const struct sockaddr *)&server, sizeof(server));
  }
	close(sockfd);
}
