#include "../libraries/Server.hpp"

void Server::QUIT(int i)
{
	println("Client disconnected!\n-> fd: " + convertString(_pollfds[i].fd), RED);
	printFd(_pollfds[i].fd, "Goodbye!", RED);
	close(_pollfds[i].fd);
	clients[i - 1].setHasPass(false);
	_pollfds[i].fd = -1;
	_clientCount--;
}