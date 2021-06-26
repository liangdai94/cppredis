//Copyright 2021 Dailiang
//License GPL 3.0
//Author Dailiang

#include"timer.h"

long long Timer::s_initialId = 0;
std::mutex Timer::s_mutex{};

Timer::Timer(int repeatedTimes, long long interval, const TimerCallback & timerCallback){
	m_repeatedTimes = repeatedTimes;
	m_expiredTime = interval + (long long)time(nullptr);
	m_callback = timerCallback;
	m_id = Timer::generateId();
}

bool Timer::isExpired(){
	long long now = time(nullptr);
	return now >= m_expiredTime;
}

long long Timer::generateId(){
	long long tmpId;
	Timer::s_mutex.lock();
	++s_initialId;
	Timer::s_mutex.unlock();
	tmpId = s_initialId;

	return tmpId;
}

long long TimerManager::addTimer(int reapeatedTimes, long long interval, const TimerCallback & timerCallback){
	//Timer * pTimer = new Timer(reapeatedTimes, interval, timerCallback);
	shared_ptr<Timer> pTimer = make_shared<Timer>(reapeatedTimes, interval, timerCallback);
	m_listTimers.insert(pTimer);
	return pTimer->getId();
}

bool TimerManager::removeTimer(long long timerId){
	//Timer * deleteTimer;
	
	for(auto iter = m_listTimers.begin(); iter != m_listTimers.end(); iter++){
		if(timerId == (*iter)->getId()){
			this->m_listTimers.erase(iter);
			return true;
		}
	}

	return false;
}

void TimerManager::checkAndHandleTimers(){
	//Timer * timer;

	for(auto iter = m_listTimers.begin(); iter != m_listTimers.end();){
		if((*iter)->isExpired()){
			(*iter)->run();
			if((*iter)->getRepeatedTimes() > 0){
				(*iter)->getRepeatedTimes()--;
				if((*iter)->getRepeatedTimes() == 0){
					iter = m_listTimers.erase(iter);
				}
				else{
					iter++;
				}
			}
			else{
				iter++;
			}
		}
		else{
			break;
		}
	}

	return;
}

