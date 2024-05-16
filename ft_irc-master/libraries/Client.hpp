#pragma once

#include "Server.hpp"

class Client
{
    private:
        string _username;
        string _nickname;
        bool _hasPass;
        bool _isNc;

    public:
        Client();
        void setUsername(string username);
        string getUsername() const;
        void setNickname(string nickname);
        string getNickname() const;
        void setHasPass(bool hasPass);
        bool getHasPass() const;
        void setIsNc(bool isNc);
        bool getIsNc() const;
};