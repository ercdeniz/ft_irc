#pragma once

#include "Client.hpp"

class Channel
{
private:
    string _channelName;
    string _channelPassword;
    vector<Client> _channelClients;
    int _limit;

public:
    Channel(string channelName, string channelPassword = "", int limit = -1)
        : _channelName(channelName), _channelPassword(channelPassword), _limit(limit) {}
    string getChannelName() const { return _channelName; }
    void setChannelName(string channelName) { _channelName = channelName; }
    string getChannelPassword() const { return _channelPassword; }
    void setChannelPassword(string channelPassword) { _channelPassword = channelPassword; }
    int getLimit() const { return _limit; }
    void setLimit(int limit) { _limit = limit; }
};