#include "../libraries/Server.hpp"
#include "../libraries/Channel.hpp"

void Server::JOIN(int fdIndex, vector<string> args)
{
	if (args.size() < 2)
	{
		printFd(_pollfds[fdIndex].fd, "Usage: JOIN <channel>", BLUE);
		return;
	}
	string channelName = args[1];
	Channel *channel ;
	channel->setName(channelName);
	channel->setChannelPass("");
	if (args.size() == 3)
	{
		if(args[2].find(' ') == string::npos)
		{
			printFd(_pollfds[fdIndex].fd, "Channel name can't contain spaces", RED);
			return;
		}
		channel->setChannelPass(args[2]);
	}
	//if (limit)
		//channel.setLimit(limit);
	if(args[1][0] != '#')
	{
		printFd(_pollfds[fdIndex].fd, "Channel name start with #", RED);
		return;
	}
	
	
	//channel.addClient(fdIndex);
	//channel.sendMessage(fdIndex, "joined channel " + channelName);
}