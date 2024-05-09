#include "../libraries/Server.hpp"

void Server::QUIT(int i)
{
	println("Client disconnected.\n fd: " + convertString(_pollfds[i].fd), RED);
	close(_pollfds[i].fd);
	_pollfds[i].fd = -1;
	_clientCount--;
}