#ifndef SERVER_H_
#define SERVER_H_

#include <arpa/inet.h>
#include <list>
#include <stdarg.h>
#include <ctime>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <cstdlib>
//#include "ae.h"
#include <atomic>
#include "anet.h"
#include "ae.h"

using namespace std;

static int anetTcpServer(int port, const char * ip);
class LoopEvent;

enum LogLevel{
	 LOG_DEBUG, 
	 LOG_INFO, 
	 LOG_NOTICE, 
	 LOG_WARNING
};

void serverLog(LogLevel level, const char *fmt, ...);

class Server{
public:
	friend class LoopEvent;
	static Server & getServer(){
		static Server server;
		return server;
	}
	
	~Server();
	Server(const Server&) = delete;
	Server& operator=(const Server &) = delete;

private:
	int port;
	char bindaddr[INET6_ADDRSTRLEN];
	int fd;
	char * logfile;//日志文件
	int logfd;
	LogLevel loglevel;
	int maxNum; //最大连接数量
	LoopEvent* le;

	bool stop;

	Server();

public:
	friend void serverLog(LogLevel level, const char *fmt, ...);
	void initServer();
	void loadServerConfig(char *filename);
	bool isStop(){return stop;}
	void start();
	void DelFileEvent(int fd){le->aeDelFileEvent(fd);}
	void RegFileEvent(int fd, int mask, fileProc rfileProc, fileProc wfileProc, clientDataBase *clientData, finalFileProc finalProc){
		le->aeRegFileEvent(fd, mask, rfileProc, wfileProc, clientData, finalProc);
	}
};

#endif
