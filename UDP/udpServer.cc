#include <iostream>
#include <cstring>
#include <string>

#include <stdlib.h>
#include <unistd.h>
#include <cerrno>

// net
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "Log.hpp"

class udpServer {
public:
  udpServer(std::string ip, uint16_t port)
      : ip_(ip), port_(port), sockfd_(-1) {}

  ~udpServer() {
    if (sockfd_ > 0)
      close(sockfd_);
  }

	void init()
	{
		// 基于文件描述符
		sockfd_ = socket(AF_INET, SOCK_DGRAM, 0);
		if(sockfd_ < 0)
		{
			logMessage(FATAL, "socket:%s:%d", strerror(errno), sockfd_);
			exit(1);
		}
		logMessage(DEBUG, "socket create success:%d", sockfd_);

		// 填充sock_addr
		struct sockaddr_in local;
		// or  memset
		// bzero(&local, sizeof(local));
		memset(&local, 0, sizeof(local));
		local.sin_port = htons(port_);
		local.sin_family = AF_INET;
		local.sin_addr.s_addr = inet_addr(ip_.c_str());

		if(bind(sockfd_, (const struct sockaddr*)&local, sizeof(local)) < 0)
		{
			logMessage(FATAL, "bind:%s:%d", strerror(errno), sockfd_);
			exit(2);
		}
		logMessage(DEBUG, "bind success:%d", sockfd_);
	}

	void start()
	{
		// 输入缓冲区
		char inbuffer[1024];
		// 输出缓冲区
		char outbuffer[1024];
		while(true)
		{
			struct sockaddr_in server;
			memset(&server, 0, sizeof(server));
			socklen_t len = sizeof(server);

			// std::cout << "server start!" << std::endl;

			ssize_t s = recvfrom(sockfd_, inbuffer, sizeof(inbuffer)-1, 0, (struct sockaddr*)&server, &len);
			if(s < 0)
			{
				logMessage(NOTICE, "recvfrom:%s:%d", strerror(errno), sockfd_);
				continue;
			}
			inbuffer[s] = '\0';
			// 提取客户端的IP和Port
			std::string server_ip = inet_ntoa(server.sin_addr);
			uint16_t server_port = htons(server.sin_port);

			logMessage(DEBUG, "receive from:[%s:%d]# %s", server_ip.c_str(), server_port, inbuffer);
			handlerObject(server_ip, server_port, inbuffer);
			// 如果需要给客户端回复消息
			sendto(sockfd_, outbuffer, sizeof(outbuffer), 0, (const struct sockaddr*)&server, sizeof(server));
		}
	}

private:
	void handlerObject(const std::string &server_ip, const uint16_t server_port, char* inbuffer)
	{
		// ...

	}

private:
  std::string ip_;
  uint16_t port_;
  int sockfd_;
};

static void Usage(const std::string name)
{
	std::cout << "Usage:\n\t" << name << " ip port" << std::endl;
}

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		Usage(argv[0]);
		exit(3);
	}
	std::string ip = argv[1];
	uint16_t port = atoi(argv[2]);

	udpServer svr(ip, port);
	svr.init();
	svr.start();

	return 0;
}
