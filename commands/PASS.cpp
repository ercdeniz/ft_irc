#include "../libraries/Server.hpp"

void Server::PASS(int i, vector<string> args)
{
    if (args.size() != 2)
    {
        printlnErr("Usage: PASS <password>", RED);
        return;
    }
    size_t len = args[1].length();
    if (!args[1].compare(0,len,this->_password))
    {
        this->_pollfds[i].events = POLLIN;
        println("Password accepted", GREEN);
    }
    else
    {
        this->_pollfds[i].events = 0;
        println("Password rejected", RED);
    }
}