#一个简单的makefile
SOURCESALL = $(wildcard *.cpp)
HEADS = $(wildcard *.h)
OBJECTSALL = $(patsubst %.cpp,%.o,$(SOURCESALL))
CLIENT = cli_main.o
OBJECTS = $(filter-out $(CLIENT),$(OBJECTSALL))
LIBS = -pthread
FLAGS = -g

CC = g++

all: server client

server:$(OBJECTS)
	$(CC) $^ -o $@ $(LIBS) $(FLAGS)

$(OBJECTS):%.o:%.cpp
	$(CC) -c $< -o $@ $(FLAGS)

client:cli_main.o
	$(CC) $< -o $@ $(FLAGS)

.PHONY:clean 
clean:
	rm -rf $(OBJECTSALL) 
	
