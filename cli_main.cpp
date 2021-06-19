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

	return sockfd;
}

int main(int argc, char * * argv){
	int port = 0;
	int ch;
	char ip[INET6_ADDRSTRLEN] = {0};
	int ret;
	if(argc <= 2){
		fprintf(stderr, "usage: redis-cli [-h host] [-p port] cmd arg1 arg2 arg3 ... argN\n");
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
		ret = send(fd, "1111111", strlen("1111111"));
		printf("send %d\n", ret);
		sleep(1);
	}
}