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
    string nick = trim(args[1], " \n\r\t");
    if(nick.length() > 20 || nick.length() == 0)
    {
        printFd(_pollfds[fdIndex].fd, "Nickname must be 1-20 characters", RED);
        return;
    }
    for (size_t i = 0; i < _clientCount; i++)
    {
        if(!nick.compare(clients[i]->getNickname()))
        {
            printFd(_pollfds[fdIndex].fd, "out of use nick", RED);
            return;
        }
    }
    clients[fdIndex - 1]->setNickname(trim(nick, "\n"));
    printFd(_pollfds[fdIndex].fd, "Nickname accepted", GREEN);
}