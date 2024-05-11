#include "../libraries/Server.hpp"

void Server::PASS(int i, vector<string> args)
{
    println("Command:" + args[1], CYAN);
    println("HasPass:" + to_string(clients[i].getHasPass()), CYAN);

    if (clients[i].getHasPass())
    {
        println("Password already accepted", CYAN);
        printFd(_pollfds[i].fd, "Password already accepted", CYAN);
        return;
    }
    else
    {
        if (args.size() != 2)
        {
            printlnErr("Usage: PASS <password>", RED);
            printFd(_pollfds[i].fd, "Usage: PASS <password>", RED);
            return;
        }
        size_t len = args[1].length();
        if (!args[1].compare(0, len - 1, this->_password))
        {
            clients[i].setHasPass(true);
            this->_pollfds[i].events = POLLIN;
            println("Password accepted", GREEN);
            printFd(_pollfds[i].fd, "Password accepted", GREEN);
        }
        else
        {
            println("Password rejected", RED);
            printFd(_pollfds[i].fd, "Password rejected", RED);
        }
    }
}