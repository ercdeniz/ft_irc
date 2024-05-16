#pragma once

#include "Print.hpp"
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <vector>
#include "Client.hpp"
#include <string.h>
#include <errno.h>

using namespace std;

#define BUFFER 1000000
#define MAX_CLIENTS 3 + 1 // client + server
class Channel;
class Client;
class Server
{
private:
    sockaddr_in _serverAddr;
    pollfd _pollfds[MAX_CLIENTS];
    int _port;
    int _clientCount;
    string _password;
public:
	vector<Channel>channels;
    vector<Client> clients;
    Server(int port, string password);
    void socketGenerate();
    void start();
    void isRevent();
    void receiveData();
    void handleCommand(int fdIndex, char *buf);
    //commands
    void QUIT(int fdIndex);
    void PASS(int fdIndex, vector<string> args);
    void USER(int fdIndex, vector<string> args);
    void NICK(int fdIndex, vector<string> args);
	void JOIN(int fdIndex, vector<string> args);

    // utils
    void sendMessage(int index);
    static string trim(const string& str, const string& whitespaces);
    static vector<string> splitString(const string &str, char delimiter);
    static string toUpper(const string &str);

	//channel
};
