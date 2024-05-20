#include "../libraries/Server.hpp"

void Server::USER(int fdIndex, vector<string> args)
{
    if(!clients[fdIndex - 1]->getUsername().empty())
    {
        printFd(_pollfds[fdIndex].fd, "You already have a username", CYAN);
        return;
    }
    if(args.size() != 2)
    {
        printFd(_pollfds[fdIndex].fd, "Usage: USER <username>", RED);
        return;
    }
    string username = trim(args[1], " \n\r\t");
    if(username.length() > 20 || username.length() == 0)
    {
        printFd(_pollfds[fdIndex].fd, "Username must be 1-20 characters", RED);
        return;
    }
    clients[fdIndex - 1]->setUsername(trim(args[1], "\n"));
    printFd(_pollfds[fdIndex].fd, "Username accepted", GREEN);
}