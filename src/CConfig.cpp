/*
 * CConfig.cpp
 *
 *  Created on: 2015年7月26日
 *      Author: phenix
 */

#include "CConfig.h"
CConfig::CConfig()
{
	string ini_file = "./ws.ini";
	  boost::property_tree::ptree pt;
	    if(access(ini_file.c_str(),0) ==0)
	    {
	        boost::property_tree::ini_parser::read_ini(ini_file,pt);
	        config_init(pt);
	    }
}
CConfig::CConfig(string ini_file)
{
    boost::property_tree::ptree pt;
    if(access(ini_file.c_str(),0) ==0)
    {
        boost::property_tree::ini_parser::read_ini(ini_file,pt);
        config_init(pt);
    }
}

void CConfig::config_init(boost::property_tree::ptree  pt)
{
    memset(proxy_ip,0,sizeof(proxy_ip));
    ws_port = pt.get<int>("wsproxy.ws_port");
    string ip = pt.get<string>("wsproxy.proxy_ip");
    strcpy(proxy_ip,ip.c_str());
    proxy_port = pt.get<int>("wsproxy.proxy_port");
}
