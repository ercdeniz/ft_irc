#include "../libraries/Server.hpp"

void Server::QUIT(int fdIndex)
{
	println("Client disconnected!\n-> fd: " + convertString(_pollfds[fdIndex].fd), RED);
	printFd(_pollfds[fdIndex].fd, "Goodbye!", RED);
	close(_pollfds[fdIndex].fd);
	clients[fdIndex - 1]->setHasPass(false);
	clients[fdIndex - 1]->setUsername("");
	clients[fdIndex - 1]->setNickname("");

	_pollfds[fdIndex].fd = -1;
	_clientCount--;
}