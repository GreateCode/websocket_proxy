#ifndef __PTHREAD_POOL__
#define __PTHREAD_POOL__

#include "pthread_sync.h"
#include <list>

#define THREAD_NUM 5

class Pthread_Pool {
private:
	Pthread_Pool();
	~Pthread_Pool();
public:
	static Pthread_Pool *get_pthread_pool_instance();
	static void *worker(void *arg);
	int append(Task *tsk);
	void run();
private:
	static Pthread_Pool *pool_;
	Pthread_Sync *sync_;
	std::list<Task *> worklist_;
	pthread_t *pthread_;
	bool stop_;
};

#define PTHREAD_POOL Pthread_Pool::get_pthread_pool_instance()

#endif
