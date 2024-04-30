#pragma once

#include <iostream>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/poll.h>

using namespace std;

#define MAX_CLIENTS 0
class Server
{
private:
    sockaddr_in _serverAddr;
    pollfd _pollfds[MAX_CLIENTS];
    int _port;
    int _client_count;
    string _password;
    
public:
    Server(int port, string password);
    void start();
    int get_client_count();

};
