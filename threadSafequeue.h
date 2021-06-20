#ifndef THREADSAFEQUEUE_H_
#define THREADSAFEQUEUE_H_

#include <queue>
#include <condition_variable>
#include <memory>	//shared_ptr
#include <mutex>  // std::mutex, std::lock_guard

using namespace std;

template<typename T>
class threadsafe_queue{
private:
	mutable mutex mut;
	queue<shared_ptr<T>> data_queue;
	condition_variable data_cond;

public:
	threadsafe_queue(){}
	bool empty(){
		lock_guard<mutex> lk(mut);
		return data_queue.empty();
	}

	void push(T new_value){
		shared_ptr<T> data(make_shared<T>(move(new_value)));
		lock_guard<mutex> lk(mut);
		data_queue.push(data);
		data_cond.notify_one();
	}

	//不提供直接返回值，避免赋值比较大的类型时产生异常
	void wait_and_pop(T &value){
		unique_lock<mutex> lk(mut);
		data_cond.wait(lk, [this]{return !data_queue.empty();});
		value = move(*data_queue.front());
		data_queue.pop();
	}

	bool try_pop(T &value){
		unique_lock<mutex> lk(mut);
		if(data_queue.empty())
			return false;
		value = move(*data_queue.front());
		data_queue.pop();
		return true;
	}

	shared_ptr<T> wait_and_pop(void){
		lock_guard<mutex> lk(mut);
		data_cond.wait(lk, [this]{return !data_queue.empty();});
		shared_ptr<T> data = data_queue.front();
		data_queue.pop();
		return data;
	}

	shared_ptr<T> try_pop(void){
		lock_guard<mutex> lk(mut);
		if(data_queue.empty())
			return shared_ptr<T>();
		shared_ptr<T> data = data_queue.front();
		data_queue.pop();
		return data;
	}
};

#endif
