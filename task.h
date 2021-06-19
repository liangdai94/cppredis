#ifndef TASK_H_
#define TASK_H_

using fileProc = void(*)(int fd, void * clientData, int mask);
using finalFileProc = void(*)(void *clientData);

class TaskBase{
private:
	int fd;
	char * recvbuf;
	char * sendbuf;
};

#endif
