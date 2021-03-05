SERVER = server.cpp
CLIENT = client.cpp

KEYS = -lboost_system -lboost_thread -lpthread
LIB = /usr/lib/

TARGET_SERVER = server
TARGET_CLIENT = client

.PHONY: all clean

all: server client

server:
	g++ -o $(TARGET_SERVER) $(SERVER) -L $(LIB) $(KEYS)

client:
	g++ -o $(TARGET_CLIENT) $(CLIENT) -L $(LIB) $(KEYS)

clean:
	rm -rf $(TARGET_SERVER)
	rm -rf $(TARGET_CLIENT)
