#一个简单的makefile
SOURCESALL = $(wildcard *.cpp)
HEADS = $(wildcard *.h)
OBJECTSALL = $(patsubst %.cpp,%.o,$(SOURCESALL))
CLIENT = cli_main.o
OBJECTS = $(filter-out $(CLIENT),$(OBJECTSALL))

CC = g++

all: server client

server:$(OBJECTS)
	$(CC) $^ -o $@

$(OBJECTS):%.o:%.cpp
	$(CC) -c $< -o $@

client:cli_main.o
	$(CC) $< -o $@

.PHONY:clean 
clean:
	rm -rf $(OBJECTSALL) 
	
