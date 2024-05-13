#include "../libraries/Server.hpp"

void Server::NICK(int i, vector<string> args)
{
    if(!clients[i - 1].getNickname().empty())
    {
        printFd(_pollfds[i].fd, "You already have a nickname", CYAN);
        return;
    }
    if(args.size() != 2)
    {
        printFd(_pollfds[i].fd, "Usage: NICK <nickname>", RED);
        return;
    }
    clients[i - 1].setNickname(trim(args[1], "\n"));
    printFd(_pollfds[i].fd, "Nickname accepted", GREEN);
}