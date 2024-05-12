#include "../libraries/Server.hpp"

void Server::PASS(int i, vector<string> args)
{
    if (clients[i - 1].getHasPass())
    {
        printFd(_pollfds[i].fd, "Password already accepted", CYAN);
        return;
    }
    else
    {
        if (args.size() != 2)
        {
            printFd(_pollfds[i].fd, "Usage: PASS <password>", RED);
            return;
        }
        size_t len = args[1].length();
        if (!args[1].compare(0, len - 1, this->_password))
        {
            clients[i - 1].setHasPass(true);
            this->_pollfds[i].events = POLLIN;
            println("Password accepted\n-> fd: " + convertString(_pollfds[i].fd), GREEN);
            printFd(_pollfds[i].fd, "Password accepted", GREEN);
        }
        else
        {
            println("Password rejected\n-> fd: " + convertString(_pollfds[i].fd), RED);
            printFd(_pollfds[i].fd, "Password rejected", RED);
        }
    }
}