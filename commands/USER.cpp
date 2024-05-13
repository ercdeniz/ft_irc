#include "../libraries/Server.hpp"

void Server::USER(int i, vector<string> args)
{
    if(!clients[i - 1].getUsername().empty())
    {
        printFd(_pollfds[i].fd, "You already have a username", CYAN);
        return;
    }
    if(args.size() != 2)
    {
        printFd(_pollfds[i].fd, "Usage: USER <username>", RED);
        return;
    }
    clients[i - 1].setUsername(args[1]);
    printFd(_pollfds[i].fd, "Username accepted", GREEN);
}