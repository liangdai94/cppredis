//Copyright 2021 Dailiang
//License GPL 3.0
//Author Dailiang

#ifndef TIMER_H_
#define TIMER_H_

#include<time.h>
#include<set>
#include<functional>
#include<mutex>
#include<memory>
#include<initializer_list>

using TimerCallback = std::function<void()>;
using std::shared_ptr;
using std::make_shared;

class Timer{
public:
	Timer();
	~Timer(){}
	Timer(int repeatedTimes, long long interval, const TimerCallback & timerCallback);

	void run(){ m_callback(); }
	long long getId() const{return m_id;}

	bool isExpired();
	int& getRepeatedTimes(){return m_repeatedTimes;}

	static long long generateId();

private:
	long long m_id;	//定时器id，唯一标识
	time_t m_expiredTime;//定时器到期时间
	int m_repeatedTimes;//重复剩余次数，-1为重复触发
	TimerCallback m_callback;

	static long long s_initialId;	//生成一个唯一ID
	static std::mutex s_mutex;		//保护s_initialId
};

class Timercompare{
public:
	bool operator()(const shared_ptr<Timer> lhs, const shared_ptr<Timer> rhs){
		return lhs->getId() < rhs->getId();
	}
};

class TimerManager{
public:
	TimerManager() = default;
	~TimerManager() = default;

	/*添加定时器
	*@param in 1 定时器重复次数
	*@param in 2 触发间隔
	*@param in 3 定时器触发回调函数
	*return 返回创建定时器的id
	*/
	long long addTimer(int reapeatedTimes, long long interval, const TimerCallback & timerCallback);
	/*
	*移除timerId定时器，成功返回true，否则false
	*/
	bool removeTimer(long long timerId);
	/*
	*检测定时器是否到期，到期则触发定时器函数
	*/
	void checkAndHandleTimers(); 

private:
	std::set<shared_ptr<Timer>, Timercompare> m_listTimers;
};


#endif

