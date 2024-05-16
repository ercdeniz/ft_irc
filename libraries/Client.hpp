#pragma once

#include <vector>
#include <iostream>

using std::string;
using std::vector;
using std::cout;

class Channel;

class Client
{
    private:
        string _username;
        string _nickname;
        bool _hasPass;
        bool _isNc;

    public:
        vector<Channel> channelsClient;
        Client() : _username(""), _nickname(""), _hasPass(false), _isNc(true) {}
        void setUsername(string username) { _username = username; }
        string getUsername() const { return _username; }
        void setNickname(string nickname) { _nickname = nickname; }
        string getNickname() const { return _nickname; }
        void setHasPass(bool hasPass) { _hasPass = hasPass; }
        bool getHasPass() const { return _hasPass; }
        void setIsNc(bool isNc) { _isNc = isNc; }
        bool getIsNc() const { return _isNc; }
};