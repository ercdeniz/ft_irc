#include "../libraries/Server.hpp"

void Server::KICK(int fdIndex, vector<string> args)
{
    vector<string>::iterator it = find(clients[fdIndex - 1]->clientChannels.begin(), clients[fdIndex - 1]->clientChannels.end(), args[1]);

    if (it != clients[fdIndex - 1]->clientChannels.end())
    {
        clients[fdIndex - 1]->clientChannels.erase(it);
    printFd(_pollfds[fdIndex].fd, " KICKED", GREEN);
    }
    else
        printFd(_pollfds[fdIndex].fd, "You are not in this channel", RED);
}