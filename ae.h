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

using namespace std;

using fileProc = void(*)(int fd, void * clientData, int mask);
using finalFileProc = void(*)(void *clientData);
//typedef void fileProc(struct aeEventLoop *eventLoop, int fd, void *clientData, MASKTYPE mask);

class clientDataBase{
private:
	char * recvBuf;
	size_t recvUsed;
	size_t recvLen;
	char * sendBuf;
	size_t sendUsed;
	size_t sendLen;
public:
	virtual ~clientDataBase(){
		delete recvBuf;
		recvBuf = nullptr;
		delete sendBuf;
		sendBuf = nullptr;
	}
	clientDataBase(){
	}
};

class ClientData:public clientDataBase{
};

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
private:
	int mask;
	fileProc rfileProc;
	fileProc wfileProc;
	void *clientData;
	finalFileProc finalProc;

public:
	FileEvent(int mask, fileProc rfileProc, fileProc wfileProc, void *clientData, finalFileProc finalProc);
};

class TimeEvent{
};

class LoopEvent{
private:
	//list<FileEvent*> events;
	unordered_map<int, FileEvent*> events;
	int curEventNum;

	int epollfd;
	epoll_event* ev;

public:
	int aeProcessEvents();
	bool aeRegFileEvent(int fd, int mask, fileProc rfileProc, fileProc wfileProc, void *clientData, finalFileProc finalProc);
	void aeDelFileEventint (int fd, int mask);
	LoopEvent(int maxNum);
};

void addfd(int epollfd, int fd, int mask, bool oneshot = true);


#endif
