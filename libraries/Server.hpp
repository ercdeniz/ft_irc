#pragma once

#include "Print.hpp"
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <vector>

using namespace std;

#define BUFFER 1000000
#define MAX_CLIENTS 10
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
    void handleCommand(int i, char *buf);
    //commands
    void QUIT(int i);
    void PASS(int i, vector<string> args);

};
