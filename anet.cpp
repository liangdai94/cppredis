#include "anet.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string>

using namespace std;

int anetAccept(string & err, int sockfd, char *ip, int *port){
	sockaddr_in client;
	socklen_t clientLen = sizeof(client);
	int fd;

	while(true){
		fd = accept(sockfd, (struct sockaddr*)&client, &clientLen);
		if (fd == -1) {
            if (errno == EINTR)
                continue;
            else {
                //anetSetError(err, "accept: %s\n", strerror(errno));
                err += strerror(errno);
                return fd;
            }
        }
        break;
    }

    if (ip) strcpy(ip,inet_ntoa(client.sin_addr));
    if (port) *port = ntohs(client.sin_port);
    return fd;
}
