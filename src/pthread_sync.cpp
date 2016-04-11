#include "pthread_sync.h"

Pthread_Sync::Pthread_Sync(){
	sem_init(&sem_, 0, 0);
	pthread_mutex_init(&mutex_, NULL);
}

Pthread_Sync::~Pthread_Sync(){
	sem_destroy(&sem_);
	pthread_mutex_destroy(&mutex_);
}

bool Pthread_Sync::wait(){
	return sem_wait(&sem_) == 0;
}

bool Pthread_Sync::post(){
	return sem_post(&sem_) == 0;
}


bool Pthread_Sync::lock(){
	return pthread_mutex_lock(&mutex_) == 0;
}


bool Pthread_Sync::unlock(){
	return pthread_mutex_unlock(&mutex_) == 0;
}

