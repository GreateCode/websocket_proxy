#ifndef __PTHREAD_SYNC__
#define __PTHREAD_SYNC__

#include <semaphore.h>
#include <pthread.h>

class Pthread_Sync {
public:
	Pthread_Sync();
	~Pthread_Sync();
	bool wait();
	bool post();
	bool lock();
	bool unlock();
private:
	sem_t sem_;
	pthread_mutex_t mutex_;
};

#define PTHREAD_SYNC Pthread_Sync pthread_sync_;
#define WAIT pthread_sync_.wait();
#define POST pthread_sync_.post();
#define LOCK pthread_sync_.lock();
#define UNLOCK pthread_sync_.unlock();

#endif
