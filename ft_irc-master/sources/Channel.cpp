#include "../libraries/Server.hpp"
#include "../libraries/Channel.hpp"

Channel::Channel()
{
	
}

string Channel::getName()const
{
	return _channelName;
}
void Channel::setName(string name)
{
	_channelName = name;
	
}

void Channel::setChannelPass(string pass)
{
	_channelPass = pass;
}

int Channel::getLimit()const
{
	return _limit;
}