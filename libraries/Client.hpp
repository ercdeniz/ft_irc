#pragma once

#include "Server.hpp"

class Client
{
    private:
        string _username;
        string _nickname;
        bool _hasPass;

    public:
        Client();
        void setHasPass(bool hasPass);
        bool getHasPass() const;
};