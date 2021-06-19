 #include "ae.h"
#include "server.h"

int setnonblocking(int fd){
	int old_option = fcntl(fd, F_GETFL);
	int new_option = old_option | O_NONBLOCK;
	fcntl(fd, F_SETFL, new_option);
	return new_option;
}

FileEvent::FileEvent(int mask, fileProc rfileProc, fileProc wfileProc, clientDataBase *clientData, finalFileProc finalProc):mask(mask),\
			rfileProc(rfileProc), wfileProc(wfileProc),finalProc(finalProc),clientData(clientData){
}

LoopEvent::LoopEvent(int maxNum = 1024){
	ev = new epoll_event[maxNum];
	epollfd =  epoll_create(5);

}

int LoopEvent::aeProcessEvents(){
	serverLog(LOG_NOTICE, "in looping.....");
	sleep(2);
	return 0;
}

//将事件注册到主循环体
bool LoopEvent::aeRegFileEvent(int fd, int mask, fileProc rfileProc, fileProc wfileProc, clientDataBase *clientData, finalFileProc finalProc){
	FileEvent * file = new FileEvent(mask, rfileProc, wfileProc, clientData, finalProc);
	//加入epoll
	if(file == nullptr)
		return false;
	else{
		this->events[fd] = file; 
		this->curEventNum++;
	}

	addfd(epollfd, fd, mask);
	return true;
}

void LoopEvent::aeDelFileEvent (int fd){
	FileEvent * file = this->events[fd];
	this->curEventNum--;
	this->events.erase(fd);
	close(fd);
	delete file;
}

void addfd(int epollfd, int fd, int mask, bool oneshot){
	epoll_event event;
	event.data.fd = fd;
	event.events = mask | EPOLLET; //ET工作模式
	if(oneshot){
		event.events |= EPOLLONESHOT;
	}

	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
	setnonblocking(fd);
}


