#ifndef __CWS_PROXY__
#define __CWS_PROXY__

#include <websocketpp/common/connection_hdl.hpp>
#include "cryptopp/aes.h"
#include "cryptopp/aes.h"
#include "cryptopp/base64.h"
#include "cryptopp/filters.h"
#include "cryptopp/modes.h"
#include "b64.h"
#include "CConfig.h"
#include "debug_log.h"
using websocketpp::connection_hdl;

#define BUFFLEN 65535

class Proxy_Callback;

class CWs_Proxy {
public:
	CWs_Proxy();
	~CWs_Proxy();
	int recv();
	int handle_input();
	int handle_output(std::string sInput);
	bool connect(const char *ip, int port);
	int web_socket_close();
	int on_proxy_close();
	inline int getfd();
	inline connection_hdl gethdl();
	inline void set_proxy_attr(connection_hdl hdl, Proxy_Callback *pc);
private:
	std::string encrypt(std::string plainText);
private:
	int fd_;
	char buff_[BUFFLEN];
	int buff_size_;
	connection_hdl hdl_;
	Proxy_Callback *pc_;
};

inline int CWs_Proxy::getfd(){
	return fd_;
}

inline connection_hdl CWs_Proxy::gethdl(){
	return hdl_;
}

inline void CWs_Proxy::set_proxy_attr(connection_hdl hdl, Proxy_Callback *pc){
	hdl_ = hdl;
	pc_ = pc;
}


#endif
