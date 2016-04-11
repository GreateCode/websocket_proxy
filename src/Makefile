CC = g++
FLAG = -g 
INCLUDE = -I /root/ws_simple/ -I /root/ws_simple/cryptopp/ -I /root/ws_simple/ws_master
LIBDIR =
LIB = -lboost_system -lboost_chrono -lcryptopp -lpthread
BIN = ../bin
TARGET = ws_sample
SRCS = b64.cpp CConfig.cpp pthread_sync.cpp network.cpp cws_proxy.cpp \
		web_socket.cpp debug_log.cpp main.cpp

$(TARGET):$(SRCS:.cpp=.o)
	$(CC) $(FLAG) $(LIBDIR) $(LIB) -o $@ $^
	-mv -f $@ $(BIN)
	-rm -f *.o *.d

%.o:%.cpp
	$(CC) $(FLAG) $(INCLUDE) -c -o $@ $<

clean:
	-rm -f *.o *.d
