#ifndef __NETWORK__
#define __NETWORK__

#include <arpa/inet.h>
#include <sys/types.h>  
#include <sys/socket.h> 
#include <sys/epoll.h> 
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <list>
#include "pthread_sync.h"
#include "cws_proxy.h"

#define MAXEVENTSSIZE 20
#define TIMEOUT 100

class Network{
typedef std::map<int, CWs_Proxy *> PROXYMAP;
typedef std::list<CWs_Proxy *> PROXYLIST;
private:
	Network();
	~Network();
	int init();
	int setnonblocking(int fd);
	void ctl_event(int fd, bool flag);
	void run();
	void epoll_loop(int timeout);
	static void *worker(void *arg);
	void register_proxy(CWs_Proxy *proxy);
	void degister_proxy(CWs_Proxy *proxy);
public:
	static Network *get_share_network();
	static void close_network();
	CWs_Proxy *get_proxy_by_fd(int fd);
	void process_proxy();
	void set_drop_proxy(CWs_Proxy *proxy);
	void set_register_proxy(CWs_Proxy *proxy);
private:
	int epollfd;
	static Network *network_;
	PTHREAD_SYNC
	pthread_t pthread_;
	static bool stop_;
	PROXYMAP proxy_map_;
	PROXYLIST drop_proxy_list_;
	PROXYLIST register_proxy_list_;
};

#define NETWORK Network::get_share_network()

#endif

