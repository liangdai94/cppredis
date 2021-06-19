#ifndef ANET_H_
#define ANET_H_

#include <string>

using namespace std;

int anetAccept(string &err, int serversock, char *ip, int *port);
int anetTcpNoDelay(string &err, int fd);

#endif
