#pragma once

#include "Print.hpp"
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/poll.h>

using namespace std;

#define MAX_CLIENTS 0
class Server
{
private:
    sockaddr_in _serverAddr;
    pollfd _pollfds[MAX_CLIENTS];
    int _port;
    int _clientCount;
    string _password;
    
public:
    Server(int port, string password);
    void socketGenerate();
    void start();

};
