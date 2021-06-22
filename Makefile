#一个简单的makefile
HEADDIR = ./include
SRCDIR = ./src
BINDIR = ./bin
OBJDIR = ./obj

SOURCESALL = $(wildcard $(SRCDIR)/*.cpp)
SOURCENODIR = $(notdir $(SOURCESALL))
HEADS = $(wildcard $(HEADDIR)/*.h)

OBJECTSALL = $(patsubst %.cpp, $(OBJDIR)/%.o, $(SOURCENODIR))
CLIENT = $(OBJDIR)/cli_main.o
OBJECTS = $(filter-out $(CLIENT),$(OBJECTSALL))
LIBS = -pthread
FLAGS = -g -I$(HEADDIR)

CC = g++

all: server client

server:$(OBJECTS)
	$(CC) $^ -o $(BINDIR)/$@ $(LIBS) $(FLAGS)

$(OBJECTS):$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) -c $< -o $@ $(FLAGS)

client:$(SRCDIR)/cli_main.cpp
	$(CC) $< -o $(BINDIR)/$@ $(FLAGS)

.PHONY:clean 
clean:
	rm -rf $(OBJECTSALL) 
	
