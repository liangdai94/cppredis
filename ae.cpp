#include "ae.h"
#include "server.h"
#include <utility> //pair

int setnonblocking(int fd){
	int old_option = fcntl(fd, F_GETFL);
	int new_option = old_option | O_NONBLOCK;
	fcntl(fd, F_SETFL, new_option);
	return new_option;
}

FileEvent::FileEvent(int mask, fileProc rfileProc, fileProc wfileProc, clientDataBase *clientData, finalFileProc finalProc):mask(mask),\
			rfileProc(rfileProc), wfileProc(wfileProc),finalProc(finalProc),clientData(clientData){
}

LoopEvent::LoopEvent(int maxNum = 1024):evNum(maxNum){
	ev = new epoll_event[maxNum];
	epollfd =  epoll_create(5);

}

LoopEvent::~LoopEvent(){
	
	for(auto it_pos = events.begin();it_pos!=events.end();)
	{
		if( nullptr != it_pos->second )
		{
			//map.erase(it_pos++);
			auto tmp = it_pos++;
			aeDelFileEvent(it_pos->first);//迭代器失效，必须先加
			it_pos = tmp;
		}
		else
		{
			it_pos++;
		}
	
	}
	delete [] ev;
}

//
int LoopEvent::aeProcessEvents(){
	Server & server = Server::getServer();
	int ret = 0;
	serverLog(LOG_NOTICE, "in looping.....");
	if(events.size() != 0){
		int timeout = 100; //ms
		ret = epoll_wait(epollfd, ev, evNum, timeout);
		if(ret < 0){
			serverLog(LOG_NOTICE, "epoll wait 0 fd.....");
		}
		for(int i = 0; i < ret; i++){
			int sockfd = ev[i].data.fd;
			if(sockfd == server.fd){
				FileEvent * tmp = events[sockfd];
				tmp->rfileProc(sockfd, nullptr, 0);
			}
			else{
				fireFd.push(sockfd); //将事件压入就绪队列，等待线程池处理
			}
		}
	}
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


