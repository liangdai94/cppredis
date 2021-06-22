//Copyright 2021 Dailiang
//License GPL 3.0
//Author Dailiang

#ifndef SERVER_H_
#define SERVER_H_

#include <arpa/inet.h>
#include <stdarg.h>
#include <ctime>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <cstdlib>
#include <atomic>
#include <list>
#include <unordered_map>
#include <vector>
#include <array>
#include "anet.h"
#include "threadPool.h"
#include "threadSafequeue.h"
#include "ae.h"

//using namespace std;
using std::array;
using std::unordered_map;
using std::string;

static int anetTcpServer(int port, const char * ip);
void worker(void);

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
	using DATABASE = unordered_map<string, string>;

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
	int threadNum;

	//using DATABASE = unordered_map<string, vector<char>>;
	array<DATABASE, 8> databases;

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
	LoopEvent* getEventLoop(){return le;}
	DATABASE & getDatabase(int n){return databases[n];}
	friend void worker(void);
};

#endif
