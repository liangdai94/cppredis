//Copyright 2021 Dailiang
//License GPL 3.0
//Author Dailiang

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <unordered_set>
#include <string>
#include <iostream>

using namespace std;

class Command{
public:
	unordered_set<string> comTable;

	static Command & getComTable(){
		static Command table;
		return table;
	}

	Command(const Command&) = delete;
	Command& operator=(const Command &) = delete;

private:
	Command(){
		comTable = {"set", "get"};
	}
};

static int createTcpLink(int port, const char *ip){
	sockaddr_in server;
	socklen_t len = sizeof(server);
	int ret = 0;
	int yes;

	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	ret = inet_pton(AF_INET, ip, &server.sin_addr);

	if(ret != 1){
		fprintf(stderr, "create tcp link error\n");
		exit(1);
	}

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	ret = connect(sockfd, (struct sockaddr *)&server, len);

	if(ret < 0){
		fprintf(stderr, "create connect error\n");
		exit(1);
	}

	//关闭nagle算法，避免粘包
	if (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes)) == -1)
    {
        fprintf(stderr, "close nagle error\n");
    }

	return sockfd;
}

int main(int argc, char * * argv){
	int port = 0;
	int ch;
	char ip[INET6_ADDRSTRLEN] = {0};
	int ret;
	char buf[128] = {0};
	if(argc <= 2){
		fprintf(stderr, "usage: cli_main [-h host] [-p port] cmd arg1 arg2 arg3 ... argN\n");
		exit(1);
	}
	while ((ch = getopt(argc, argv, "h:p:")) != -1){
		switch(ch){
			case 'h':
				//memcpy(ip, "127.0.0.1", strlen("127.0.0.1"));
				snprintf(ip, INET6_ADDRSTRLEN, "%s", optarg);
				break;
			case 'p':
				port = atoi(optarg);
				break;
			case '?':
				fprintf(stderr, "Unknown option: %c\n",(char)optopt);
				break;
		}
	}

	//if(ip[0] == '\0') memcpy(ip, "127.0.0.1", strlen("127.0.0.1"));
	Command & cmd = Command::getComTable();
	int fd = createTcpLink(port, ip);
	while(true){
		printf("client> ");
		memset(buf, 0, sizeof(buf));
		cin.getline(buf, sizeof(buf));
		ret = write(fd, buf, strlen(buf));
		memset(buf, 0, sizeof(buf));
		ret = read(fd, buf, sizeof(buf));
		printf("client> %s\n", buf);
	}
}