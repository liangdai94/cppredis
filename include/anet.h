//Copyright 2021 Dailiang
//License GPL 3.0
//Author Dailiang

#ifndef ANET_H_
#define ANET_H_

#include <string>

using std::string;

int anetAccept(string &err, int serversock, char *ip, int *port);
int anetTcpNoDelay(string &err, int fd);

#endif
