//Copyright 2021 Dailiang
//License GPL 3.0
//Author Dailiang

#ifndef TIMER_H_
#define TIMER_H_

#include <sys/time.h>
//#include <multiset>

class clientDataBase;

//using std::multiset;

namespace TimerSpace{
using timeProc = void(*)(int fd, clientDataBase *clientData);
using finalFileProc = void(*)(clientDataBase *clientData);

class Timer{
private:
	long long id;
	struct timeval;

public:
	timeProc proc;

	bool operator<(Timer &);
	bool operator>(Timer &);
};

}//TimerSpace

#endif
