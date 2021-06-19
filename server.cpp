#include "ae.h"
#include "anet.h"
#include "server.h"
#include <cstring>
#include <string>
#include <memory>

Server::Server(){
	port = 9898;
	loglevel = LogLevel::LOG_DEBUG;
	maxNum = 65535;
	stop = false;
}

Server::~Server(){
}

static int anetTcpServer(int port, const char * ip){
	int ret = 0;
	sockaddr_in address;
	int sockfd;

	if(port < 1 || port > 65535){
		serverLog(LOG_WARNING, "error port %d", port);
		exit(-1);
	}

	ret = inet_pton(AF_INET, ip, &address.sin_addr);

	if(ret != 1){
		serverLog(LOG_WARNING, "inet_pton error");
		exit(-1);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	address.sin_family = AF_INET;
	address.sin_port = htons(port);

	ret = bind(sockfd, (struct sockaddr *)&address, sizeof(address));

	ret |= listen(sockfd, 5);

	if(ret != 0){
		serverLog(LOG_WARNING, "bind or listen error");
		exit(-1);
	}

	//setnonblocking(sockfd);

	return sockfd;
}

//处理客户端发来的命令
static void readQueryFromClient(int fd, clientDataBase *clientData, int mask){
	int ret = 0;
	Server & server = Server::getServer();
	ret = read(fd, clientData->data(), clientData->bufLen());
	if(ret == 0){
		server.DelFileEvent(fd);
		serverLog(LOG_DEBUG, "Client closed connection");
		return ;	
	}
	else if(ret < 0){
		if (errno == EAGAIN){
			ret = 0;
		}
		else{
			server.DelFileEvent(fd);
			serverLog(LOG_DEBUG, "Client closed connection");
			return ;
		}
	}
	else{
		
	}

	return;
}


//服务器接受连接
static void acceptHandler(int fd, clientDataBase *privdata, int mask){
	string err;
	char ip[INET6_ADDRSTRLEN] = {0};
	int port = 0;
	Server & server = Server::getServer();
	ClientData * client = new ClientData();
	
	int clientfd = anetAccept(err, fd, ip, &port);

	if(clientfd < 0){
		serverLog(LOG_WARNING, "Accepting client connection: %s", err.c_str());
		return;
	}		
	serverLog(LOG_DEBUG, "Accepted %s:%d", ip, port);

	//将新收到的连接注册到主循环体
	if(anetTcpNoDelay(err, fd) != 0){
		serverLog(LOG_WARNING, "anetTcpNoDelay error: %s", err.c_str());
		return;
	}
	server.RegFileEvent(clientfd, EPOLLIN | EPOLLOUT, readQueryFromClient, nullptr, client, nullptr);
}

void Server::initServer(){
	Server & server = Server::getServer();

	server.le = new LoopEvent(server.maxNum);

	if(bindaddr[0] == '\0'){
		memcpy(bindaddr, "127.0.0.1", strlen("127.0.0.1"));
	}
	
	server.fd = anetTcpServer(server.port, server.bindaddr);
	server.RegFileEvent(server.fd, EPOLLIN, acceptHandler, nullptr, nullptr, nullptr);
}

void Server::loadServerConfig(char *filename){
}

void Server::start(){
	while(!stop){
		le->aeProcessEvents();
	}
}

void serverLog(LogLevel level, const char *fmt, ...){
	va_list ap;
    FILE *fp;
    const Server& server = Server::getServer();

    fp = (server.logfile == NULL) ? stdout : fopen(server.logfile,"a");
    if (!fp) return;

    va_start(ap, fmt);
    if (level >= server.loglevel) {
        const char *c = ".-*";
        char buf[64];
        time_t now;

        now = time(NULL);
        strftime(buf,64,"%d %b %H:%M:%S",gmtime(&now));
        fprintf(fp,"%s %c ",buf,c[level]);
        vfprintf(fp, fmt, ap);
        fprintf(fp,"\n");
        fflush(fp);
    }
    va_end(ap);

    if (server.logfile) fclose(fp);
}




