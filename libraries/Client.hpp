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
        bool _isActive;
        int _fd;
        
    public:
        Client() : _username(""), _nickname(""), _hasPass(false), _isNc(true), _isActive(true), _fd(-1) {}
        void setUsername(string username) { _username = username; }
        string getUsername() const { return _username; }
        void setNickname(string nickname) { _nickname = nickname; }
        string getNickname() const { return _nickname; }
        void setHasPass(bool hasPass) { _hasPass = hasPass; }
        bool getHasPass() const { return _hasPass; }
        void setIsNc(bool isNc) { _isNc = isNc; }
        bool getIsNc() const { return _isNc; }
        void setIsActive(bool isActive) { _isActive = isActive; }
        bool getIsActive() const { return _isActive; }
        void setFd(int fd) { _fd = fd; }
        int getFd() const { return _fd; }
};