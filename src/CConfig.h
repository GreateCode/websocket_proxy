/*
 * CConfig.h
 *
 *  Created on: 2015年7月26日
 *      Author: phenix
 */

#ifndef CCONFIG_H_
#define CCONFIG_H_
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <string>
using namespace std;
class CConfig {
public:
	CConfig();
	CConfig(string ini_file);
	void config_init(boost::property_tree::ptree pt);

	char proxy_ip[16];
	int proxy_port;
	int ws_port;
};

#endif /* CCONFIG_H_ */
