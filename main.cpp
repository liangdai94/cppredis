#include "server.h"
#include <cstdlib>

int main(int argc, char ** argv){
	Server & server = Server::getServer();

	if(argc == 2){
		server.loadServerConfig(argv[1]);
		serverLog(LOG_DEBUG, "loading config success.....");
	}
	else if (argc > 2){
		fprintf(stderr,"Usage: ./erver [/path/to/redis.conf]\n");
		exit(-1);
	}
	else{
		serverLog(LOG_NOTICE, "using fault value for server.....");
	}
	server.initServer();

	serverLog(LOG_NOTICE, "init server finished.....");
	//aeMain();
	//aeDeleteEventLoop(server.el);
	server.start();
	server.~Server();
	return 0;
}

