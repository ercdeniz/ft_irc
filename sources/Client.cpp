#include "../libraries/Client.hpp"

Client::Client()
{
    _username = "";
    _nickname = "";
    _hasPass = false;
    _isNc = true;
}

void Client::setHasPass(bool hasPass)
{
    _hasPass = hasPass;
}

bool Client::getHasPass() const
{
    return _hasPass;
}

void Client::setIsNc(bool isNc)
{
    _isNc = isNc;
}

bool Client::getIsNc() const
{
    return _isNc;
}