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

LOG::LOG():
	tim(0),
	t(NULL),
	fp(NULL),
	filepath(),
	message(),
	last_log_time()
{
#ifdef __DEBUG_TO_LOG__
	create_log_file();
#endif
}

LOG::~LOG(){
#ifdef __DEBUG_TO_LOG__
	fclose(fp);
#endif
}

void LOG::create_log_file(){
	if(fp != NULL)
		fclose(fp);

	sprintf(filepath, "./log/debuglog_");
	time(&tim); 
	t = localtime(&tim);
	memcpy(&last_log_time, t, sizeof(struct tm));
	sprintf(filepath + 15, "%02d_%02d",t->tm_mon + 1, t->tm_mday); 
	fp = fopen(filepath, "a+");
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
	if(t->tm_mday != last_log_time.tm_mday || t->tm_mon != last_log_time.tm_mon
		|| t->tm_year != last_log_time.tm_year)
		create_log_file();
	fwrite(message, strlen(message), 1, fp);
	fflush(fp);
#else
	printf("\n%s", message);
#endif
}

