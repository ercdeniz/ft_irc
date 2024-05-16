#include "../libraries/Server.hpp"

void Server::QUIT(int i)
{
	println("Client disconnected!\n-> fd: " + convertString(_pollfds[i].fd), RED);
	printFd(_pollfds[i].fd, "Goodbye!", RED);
	close(_pollfds[i].fd);
	clients[i - 1].setHasPass(false);
	clients[i - 1].setUsername("");
	clients[i - 1].setNickname("");
	clients[i - 1].channelsClient.clear();
	/* for(size_t x = 0; x < channelsServer.size(); x++)
	{
		if(!channelsServer[x]._channelClients[i - 1].getNickname().compare(clients[i - 1].getNickname()))
		{
			channelsServer[x]._channelClients.erase(channelsServer[x]._channelClients.begin() + i - 1);
			println("Client removed from channel\n-> channel: " + channelsServer[x].getChannelName(), RED);
		}
	} */
	_pollfds[i].fd = -1;
	_clientCount--;
}