#ifndef __WEB_SOCKET__
#define __WEB_SOCKET__

#include <websocketpp/logger/basic.hpp>
#include <websocketpp/logger/levels.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/close.hpp>
#include "cws_proxy.h"
#include "network.h"
#include "debug_log.h"
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

typedef websocketpp::server<websocketpp::config::asio> server;
typedef server::connection_ptr connection_ptr;

class Proxy_Callback {
public:
	virtual bool on_proxy_recv(connection_hdl hdl, char* buf, int size, bool bString) = 0;
	virtual bool on_proxy_close(connection_hdl hdl) = 0;
	virtual bool web_socket_close(CWs_Proxy *proxy) = 0;
};

class Web_Socket : public Proxy_Callback {
typedef std::map<connection_hdl, CWs_Proxy*> HDL_PROXY_MAP;
public:
	Web_Socket();
	~Web_Socket();
	bool on_proxy_recv(connection_hdl hdl, char* buf, int size, bool bString);
	bool on_proxy_close(connection_hdl hdl);
	bool web_socket_close(CWs_Proxy *proxy);
	CWs_Proxy *get_proxy_by_hdl(connection_hdl hdl);
	void run();
private:
	void on_open(connection_hdl hdl);
	void on_message(connection_hdl hdl, server::message_ptr msg);
	void on_close(connection_hdl hdl);
private:
	HDL_PROXY_MAP proxy_map_;
	CConfig configs;
	server m_server;
};

#endif
