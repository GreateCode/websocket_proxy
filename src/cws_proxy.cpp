#include "cws_proxy.h"
#include "web_socket.h"

CWs_Proxy::CWs_Proxy():
	fd_(socket(AF_INET, SOCK_STREAM, 0)),
	buff_(),
	buff_size_(0),
	hdl_(),
	pc_()
{
}

CWs_Proxy::~CWs_Proxy(){
}

int CWs_Proxy::recv(){
	buff_size_ = read(fd_, buff_, BUFFLEN);
	return buff_size_;
}

char *parse_to_hex(char *strhex, const char *buff_, int buff_size_){
	for(int i = 0; i < buff_size_; i++){
		sprintf(strhex + i * 5, "0x%02x ", buff_[i]);
	}
	return strhex;
}

int CWs_Proxy::handle_input(){
	//char strhex[4096] = {};
	DEBUG_LOG("fd:%d 转发消息至websocket", fd_);
	//DEBUG_LOG("字节流：%s", parse_to_hex(strhex, buff_, buff_size_));
	pc_->on_proxy_recv(hdl_, buff_, buff_size_, false);
	DEBUG_LOG("已发送%d字节", buff_size_);
	return 0;
}

int CWs_Proxy::handle_output(std::string sInput){
	//char strhex[4096] = {};
	DEBUG_LOG("fd:%d 转发消息至proxy", fd_);
	//size_t len;
	//unsigned char * p = base64decode(sInput.c_str(),&len);
	//int write_size =  write(fd_, (char *)p, len);
	int write_size =  write(fd_, sInput.c_str(), sInput.length());
	//DEBUG_LOG("字节流：%s", parse_to_hex(strhex, sInput.c_str(), sInput.length()));
	DEBUG_LOG("已发送%d字节", write_size);
	return write_size;
}
	
bool CWs_Proxy::connect(const char *ip, int port){
	struct sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr(ip);
	clientService.sin_port = htons(port);
	if(::connect(fd_, (struct sockaddr *)&clientService, sizeof(clientService)) < 0){
		DEBUG_LOG("连接proxy服务器失败，错误代码：%d", errno);
		return false;
	}
	return true;
}

int CWs_Proxy::web_socket_close(){
	return pc_->web_socket_close(this);
}

int CWs_Proxy::on_proxy_close(){
	return pc_->on_proxy_close(hdl_);
}

std::string CWs_Proxy::encrypt(std::string plainText){
    std::string cipherText;
/*
    CryptoPP::AES::Encryption aesEncryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption( aesEncryption, iv );
    CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink( cipherText ));
    stfEncryptor.Put( reinterpret_cast<const unsigned char*>( plainText.c_str() ), plainText.length() + 1 );
    stfEncryptor.MessageEnd();

    string cipherTextHex;
    for( int i = 0; i < cipherText.size(); i++ )
    {
        char ch[3] = {0};
        sprintf(ch, "%02x",  static_cast<byte>(cipherText[i]));
        cipherTextHex += ch;
    }
*/
    return cipherText;
}
