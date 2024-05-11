#include "../libraries/Client.hpp"

Client::Client()
{
    _username = "";
    _nickname = "";
    _hasPass = false;
}

void Client::setHasPass(bool hasPass)
{
    _hasPass = hasPass;
}

bool Client::getHasPass() const
{
    return _hasPass;
}