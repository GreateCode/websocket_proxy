#include "web_socket.h"

Web_Socket::Web_Socket(){
	m_server.init_asio();

	m_server.set_open_handler(bind(&Web_Socket::on_open, this, ::_1));
	m_server.set_close_handler(bind(&Web_Socket::on_close, this, ::_1));
	m_server.set_message_handler(bind(&Web_Socket::on_message, this, ::_1, ::_2));
}

Web_Socket::~Web_Socket()
{
}
	
bool Web_Socket::on_proxy_recv(connection_hdl hdl, char* buf, int size, bool bString){
	m_server.send(hdl, buf, size, bString ? websocketpp::frame::opcode::TEXT : websocketpp::frame::opcode::BINARY);
	return true;
}

bool Web_Socket::web_socket_close(CWs_Proxy *proxy){
	proxy_map_.erase(proxy->gethdl());
	DEBUG_LOG("proxy删除，fd：%d", proxy->getfd());
	delete proxy;
	return true;
}

bool Web_Socket::on_proxy_close(connection_hdl hdl){
	CWs_Proxy *proxy = get_proxy_by_hdl(hdl);
	if(proxy == NULL)
		return false;
	m_server.send(hdl, std::string("proxy connect closed!"),websocketpp::frame::opcode::TEXT);
	m_server.close(hdl, websocketpp::close::status::try_again_later, std::string("proxy connect closed!"));
	web_socket_close(proxy);
	return true;
}
	
void Web_Socket::on_open(connection_hdl hdl){
	CWs_Proxy* proxy = new CWs_Proxy();
	if(proxy == NULL){
		DEBUG_LOG("创建CWs_Proxy失败");
		return;
	}
	proxy->set_proxy_attr(hdl, this);

	if (!proxy->connect(configs.proxy_ip, configs.proxy_port))
	{
		m_server.send(hdl, std::string("proxy connect failed!"),websocketpp::frame::opcode::TEXT);
		m_server.close(hdl, websocketpp::close::status::try_again_later, std::string("proxy connect failed!"));
		delete proxy;
		return;
	}

	proxy_map_[hdl] = proxy;
	NETWORK->set_register_proxy(proxy);
	DEBUG_LOG("新连接建立！fd是%d", proxy->getfd());
}
	
void Web_Socket::on_message(connection_hdl hdl, server::message_ptr msg){
	CWs_Proxy *proxy = get_proxy_by_hdl(hdl);
	if(proxy == NULL){
		DEBUG_LOG("转发连接不存在");
		return;
	}
	
	std::string sInput;
	if(msg->get_opcode() == websocketpp::frame::opcode:: TEXT) {
		sInput = msg->get_payload();
	}
	else if(msg->get_opcode() == websocketpp::frame::opcode:: BINARY){
		sInput = msg->get_raw_payload();
	}
	proxy->handle_output(sInput);
}

void Web_Socket::on_close(connection_hdl hdl){
	CWs_Proxy *proxy = get_proxy_by_hdl(hdl);
	if(proxy == NULL)
		return;
	NETWORK->set_drop_proxy(proxy);
	DEBUG_LOG("连接断开，fd是%d", proxy->getfd());
}

CWs_Proxy *Web_Socket::get_proxy_by_hdl(connection_hdl hdl){
	HDL_PROXY_MAP::iterator iter = proxy_map_.find(hdl);
	if(iter != proxy_map_.end()){
		return iter->second;
	}
	return NULL;
}

void Web_Socket::run(){
	m_server.clear_access_channels(websocketpp::log::alevel::all);
	m_server.listen(websocketpp::lib::asio::ip::tcp::v4(), configs.ws_port);
	m_server.start_accept();
	DEBUG_LOG("websocketpp开始运行···");
	m_server.run();
}
