#pragma once

#include "Print.hpp"
#include "Channel.hpp"
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <vector>
#include <map>
#include <string.h>
#include <errno.h>

using namespace std;

#define BUFFER 1000000
#define MAX_CLIENTS 3 + 1 // clients + server

class Server
{
private:
    sockaddr_in _serverAddr;
    pollfd _pollfds[MAX_CLIENTS];
    int _port;
    size_t _clientCount;
    string _password;
public:
    vector<Client*> clients;
    vector<Channel> _channels;
    Server(int port, string password);
    void socketGenerate();
    void start();
    void isRevent();
    void receiveData();
    void handleCommand(int fdIndex, char *buf);
    void otherCommands(int fdIndex, vector<string> args);
    //commands
    void QUIT(int fdIndex);
    void PASS(int fdIndex, vector<string> args);
    void USER(int fdIndex, vector<string> args);
    void NICK(int fdIndex, vector<string> args);
    void JOIN(int fdIndex, vector<string> args);

    // utils
    static void argControl(char **av);
    void sendMessage(int index);
    static string trim(const string& str, const string& whitespaces);
    static vector<string> splitString(const string &str, char delimiter);
    static string toUpper(const string &str);
};
