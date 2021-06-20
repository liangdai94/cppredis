#ifndef AE_H_
#define AE_H_

#include <list>
#include <unistd.h>
#include <unordered_map>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <queue>
#include "threadSafequeue.h"

using namespace std;
//typedef void fileProc(struct aeEventLoop *eventLoop, int fd, void *clientData, MASKTYPE mask);

class clientDataBase{
private:
	char * buf;
	int len;
	int used;
public:
	virtual ~clientDataBase(){
		delete [] buf;
	}
	clientDataBase(int len = 1024):len(len),used(0){
		buf = new char[1024];
	}
	char * data(){return buf;}
	char * back(){return buf + used;}
	int capacity(){return len;}
	int size(){return used;}
	clientDataBase & operator=(const clientDataBase &)=delete;
	clientDataBase(const clientDataBase&) = delete;
};

class ClientData:public clientDataBase{
};

using fileProc = void(*)(int fd, clientDataBase *clientData, int mask);
using finalFileProc = void(*)(clientDataBase *clientData);

class FileEventBase{
public:
	virtual ~FileEventBase(){}

private:
	fileProc rfileProc;
	fileProc wfileProc;
	void *clientData;
	finalFileProc finalProc;
};

class FileEvent{
public:
	int mask;
	fileProc rfileProc;
	fileProc wfileProc;
	clientDataBase *clientData;
	finalFileProc finalProc;

public:
	FileEvent(int mask, fileProc rfileProc, fileProc wfileProc, clientDataBase *clientData, finalFileProc finalProc);
};

class TimeEvent{
};

class LoopEvent{
private:
	friend class Server;
	//list<FileEvent*> events;
	unordered_map<int, FileEvent*> events;
	int curEventNum;  //events.size()

	int epollfd;
	epoll_event* ev;
	int evNum;
	

public:
	int aeProcessEvents();
	bool aeRegFileEvent(int fd, int mask, fileProc rfileProc, fileProc wfileProc, clientDataBase *clientData, finalFileProc finalProc);
	void aeDelFileEvent (int fd);
	LoopEvent(int maxNum);
	~LoopEvent();
	friend threadsafe_queue<int> & getFireFd();
	friend void worker(void);
	threadsafe_queue<int> fireFd; //就绪队列
};

void addfd(int epollfd, int fd, int mask, bool oneshot = false);

#endif

