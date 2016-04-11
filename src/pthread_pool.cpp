#include "pthread_pool.h"

Pthread_Pool *Pthread_Pool::pool_ = NULL;

Pthread_Pool::Pthread_Pool():
	sync_(new Pthread_Sync()),
	worklist_(),
	pthread_(),
	stop_(false)
{
	pthread_ = new pthread_t[THREAD_NUM];
	for(int i = 0; i < THREAD_NUM; i++){
		pthread_create(pthread_[i], NULL, worker, this);
		pthread_detach(pthread_[i]);
	}
}

Pthread_Pool::~Pthread_Pool(){
	delete [] pthread_;
}

Pthread_Pool *get_pthread_pool_instance(){
	if(pool_ == NULL){
		pool_ = new Pthread_Pool();
	}
	return pool_;
}

void *Pthread_Pool::worker(void *arg){
	Pthread_Pool *pool = (Pthread_Pool *)arg;
	pool->run();
	return pool;
}

int Pthread_Pool::run(){
	task *tsk = NULL;
	while(!stop_){
		sync_.wait();
		sync_.lock();
		tsk = worklist_.front();
		worklist_.pop_front();
		sync.unlock();
		tsk->process();
	}
	return 0;
}

int Pthread_Pool::append(Task *tsk){
	sync_.lock();
	worklist_.push_back(tsk);
	sync_.unlock();
	sync_.post();
	return 0;
}

