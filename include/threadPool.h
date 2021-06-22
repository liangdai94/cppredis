#ifndef PHTREAD_POOL_H_
#define PHTREAD_POOL_H_

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <algorithm>
#include <vector>
#include <thread>
#include <functional>
#include <unistd.h>

namespace threadPools{
using namespace std;
using worker = void(*)(void);

//暂时只支持定量的线程数量，后期尝试动态增加减少线程数量
class thread_pool{
private:
	//atomic_bool done;
	vector<thread> pool;
	//worker work;
	
public:
	thread_pool(int num = 4, worker func = nullptr){
		for(int i = 0; i < num; i++){
			pool.push_back(thread(func));
		}
	}

	~thread_pool(){
		for_each(pool.begin(), pool.end(), mem_fn(&thread::join));
	}

	thread_pool(const thread_pool&) = delete;
	thread_pool& operator=(const thread_pool &) = delete;

	static thread_pool & getThreadPool(int num = 0, worker work = nullptr){
		static thread_pool pool(num, work);
		return pool;
	}
};

}//namespace threadPools

#endif
