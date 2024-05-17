#include "../libraries/Server.hpp"

void Server::NICK(int fdIndex, vector<string> args)
{
    if(!clients[fdIndex - 1]->getNickname().empty())
    {
        printFd(_pollfds[fdIndex].fd, "You already have a nickname", CYAN);
        return;
    }
    if(args.size() != 2)
    {
        printFd(_pollfds[fdIndex].fd, "Usage: NICK <nickname>", RED);
        return;
    }
    for (size_t i = 0; i < _clientCount; i++)
    {
        if(!args[1].compare(clients[i]->getNickname()))
        {
            printFd(_pollfds[fdIndex].fd, "out of use nick", RED);
            return;
        }
    }
    clients[fdIndex - 1]->setNickname(trim(args[1], "\n"));
    printFd(_pollfds[fdIndex].fd, "Nickname accepted", GREEN);
}