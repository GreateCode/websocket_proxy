#include "debug_log.h"

void DEBUG_LOG(const char *msg, ...){
	char message[256] = {0};
	va_list args;
	va_start(args, msg);
	vsprintf(message, msg, args);
	va_end(args);
	LOG::get_share_log()->write_log(message);
}

LOG *LOG::m_log = NULL;

LOG::LOG(){
#ifdef __DEBUG_TO_LOG__
	fp = NULL;
	sprintf(filepath, "./log/debuglog_");
	tim = 0;
	t = NULL; 

	time(&tim); 
	t = localtime(&tim); 
	sprintf(filepath + 15, "%02d_%02d",t->tm_mon + 1, t->tm_mday); 
	fp = fopen(filepath, "a+");
#endif
}

LOG::~LOG(){
#ifdef __DEBUG_TO_LOG__
	fclose(fp);
#endif
}

LOG *LOG::get_share_log(){
	if(m_log == NULL){
		m_log = new LOG();
	}
	return m_log;
}

void LOG::write_log(const char *msg){
	time(&tim); 
	t = localtime(&tim); 
	sprintf(message, "[%02d:%02d:%02d] %s\n", t->tm_hour, t->tm_min, t->tm_sec, msg);
#ifdef __DEBUG_TO_LOG__
	fwrite(message, strlen(message), 1, fp);
	fflush(fp);
#else
	printf("\n%s", message);
#endif
}

