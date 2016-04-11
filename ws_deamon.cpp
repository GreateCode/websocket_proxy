#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdarg.h>
#include <time.h>

#define NOFILE 2048
#define WAIT_TCP_TIMEOUT 30

int pid = 0;

void log_file(const char *fmt, ...){
	char msg[2048] = {};
	va_list argp;
	va_start(argp, fmt);
	vsprintf(msg + 1024, fmt, argp);
	va_end(argp);
	
	time_t tim = 0;
	time(&tim);
	tm *t = localtime(&tim); 
	sprintf(msg, "[%02d-%02d-%02d %02d:%02d:%02d] %s\n", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, msg + 1024);
	FILE *fp = fopen("./log/deamon.log", "ab+");
	if(fp == NULL){
		exit(1);
	}
	fputs(msg, fp);
	fflush(fp);
	fclose(fp);
}

void kill_sub_process(int arg){
	log_file("ws_sample killed");
	kill(pid, SIGKILL);
	exit(arg);
}

void init_daemon() {
	int pid;
 
	if ((pid = fork()) == -1) 
		exit(1);

	if (pid != 0) 
		exit(0);
 
	if (setsid() == -1) 
		exit(1);
 
	if ((pid = fork()) == -1) 
		exit(1);
	
	if (pid != 0) 
		exit(0);
 
	for (int i = 0; i < NOFILE; i++)
		close(i);
 
	if (chdir("./") == -1) 
		exit(1);
 
	if (umask(0) == -1) 
		exit(1);

	signal(SIGTERM, kill_sub_process);
}

int main(){
	init_daemon();
	int ret = 0;
	while(1){
		if((pid = fork()) == -1){
			exit(1);
		}
		if(pid != 0){
			wait(&ret);
			log_file("ws_sample %d exit status: %d", pid, ret);
			sleep(WAIT_TCP_TIMEOUT);
		}
		else{
			execl("./ws_sample", "ws_sample", NULL);
			log_file("fail to exec sub program");
			exit(1);
		}
	}
	return 0;
}
