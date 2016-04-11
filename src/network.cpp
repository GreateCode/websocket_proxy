#include "network.h"

Network *Network::network_ = NULL;
bool Network::stop_ = false;

Network::Network():
	epollfd(0),
	proxy_map_(),
	pthread_(),
	drop_proxy_list_(),
	register_proxy_list_()
{
	init();
	pthread_create(&pthread_, NULL, worker, this);
	pthread_detach(pthread_);
}

Network::~Network(){

}
	
int Network::init(){
	epollfd = epoll_create(MAXEVENTSSIZE);
	return 0;
}

int Network::setnonblocking(int fd){
	int flags;
    if ((flags = fcntl(fd, F_GETFL, 0)) == -1)
        flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

Network *Network::get_share_network(){
	if(network_ == NULL)
		network_ = new Network();
	return network_;
}

void Network::close_network(){
	if(network_)
		delete network_;
	network_ = NULL;
	stop_ = true;
}

void *Network::worker(void *arg){
	Network *network = (Network *)arg;
	network->run();
	return network;
}

void Network::run(){
	while(!stop_){
		process_proxy();
		epoll_loop(TIMEOUT);
	}
}

void Network::epoll_loop(int timeout){
	struct epoll_event events[MAXEVENTSSIZE];
	int nfds = 0;
	int fd = 0;
	nfds = epoll_wait(epollfd, events, MAXEVENTSSIZE, timeout);
	for(int i = 0; i < nfds; i++){
		if(events[i].events & EPOLLIN){
			if((fd = events[i].data.fd) < 0)
				continue;
			CWs_Proxy *proxy = get_proxy_by_fd(fd);
			if(proxy == NULL)
				continue;
			if(proxy->recv() <= 0){
				proxy->on_proxy_close();
				degister_proxy(proxy);
			}
			else{
				proxy->handle_input();
			}
		}
	}
}

void Network::ctl_event(int fd, bool flag){
	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = flag ? EPOLLIN : 0;
	epoll_ctl(epollfd, flag ? EPOLL_CTL_ADD : EPOLL_CTL_DEL, fd, &ev);
	if(flag){
		DEBUG_LOG("fd:%d 加入epoll机制", fd);
		setnonblocking(fd);
	}
	else{
		DEBUG_LOG("fd:%d 移除epoll机制", fd);
		close(fd);
	}
}

void Network::register_proxy(CWs_Proxy *proxy){
	int fd = proxy->getfd();
	proxy_map_[fd] = proxy;
	ctl_event(fd, true);
}

void Network::degister_proxy(CWs_Proxy *proxy){
	int fd = proxy->getfd();
	proxy_map_.erase(fd);
	ctl_event(fd, false);
	proxy->web_socket_close();
}

CWs_Proxy *Network::get_proxy_by_fd(int fd){
	PROXYMAP::iterator iter = proxy_map_.find(fd);
	if(iter != proxy_map_.end())
		return iter->second;
	else
		return NULL;
}

void Network::process_proxy(){
	LOCK
	while(!drop_proxy_list_.empty()){
		CWs_Proxy *proxy = drop_proxy_list_.front();
		degister_proxy(proxy);
		drop_proxy_list_.pop_front();
	}
	while(!register_proxy_list_.empty()){
		CWs_Proxy *proxy = register_proxy_list_.front();
		register_proxy(proxy);
		register_proxy_list_.pop_front();
	}
	UNLOCK
}

void Network::set_drop_proxy(CWs_Proxy *proxy){
	LOCK
	drop_proxy_list_.push_back(proxy);
	UNLOCK
}

void Network::set_register_proxy(CWs_Proxy *proxy){
	LOCK
	register_proxy_list_.push_back(proxy);
	UNLOCK
}
