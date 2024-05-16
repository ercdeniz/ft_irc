#include "../libraries/Server.hpp"

void Server::PASS(int fdIndex, vector<string> args)
{
    if (clients[fdIndex - 1].getHasPass())
    {
        printFd(_pollfds[fdIndex].fd, "Password already accepted", CYAN);
        return;
    }
    else
    {
        if (args.size() != 2)
        {
            printFd(_pollfds[fdIndex].fd, "Usage: PASS <password>", RED);
            return;
        }
        size_t len = args[1].length();
        if (!args[1].compare(0, len, _password))
        {
            clients[fdIndex - 1].setHasPass(true);
            this->_pollfds[fdIndex].events = POLLIN;
            println("Password accepted\n-> fd: " + convertString(_pollfds[fdIndex].fd), GREEN);
            printFd(_pollfds[fdIndex].fd, "Password accepted", GREEN);
        }
        else
        {
            println("Password rejected\n-> fd: " + convertString(_pollfds[fdIndex].fd), RED);
            printFd(_pollfds[fdIndex].fd, "Password rejected", RED);
        }
    }
}