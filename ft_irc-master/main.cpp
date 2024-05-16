#include "libraries/Server.hpp"

bool is_digit(string str)
{
    int len = str.length();
    for (int i = 0; i < len; i++)
        if (!isdigit(str[i]))
            return false;
    return true;
}

Server *server = NULL;

void Server::sendMessage(int index)
{
    printFd(_pollfds[index].fd, "Server shutting down...", RED);
}

void singalHandler(int signum)
{
    println("\nSignal received: " + to_string(signum), RED);
    for (int i = 0; i < MAX_CLIENTS; i++)
        server->sendMessage(i);
    delete server;
    server = NULL;
    exit(signum);
}

int main(int ac, char** av) {

    if(ac == 3)
    { 
        try
        {
            if (!is_digit(av[1]) || !is_digit(av[2]))
                throw runtime_error("Port and password must be digits");
            server = new Server(atoi(av[1]), av[2]);
            signal(SIGINT, singalHandler);
            signal(SIGTSTP, singalHandler);
            server->start();
        }
        catch(const std::exception& e)
        {
            printlnErr(string(e.what()) + ": " + strerror(errno), RED);
        }
        
    }
    else
    {
        println("Usage: ./server <port> <password>", RED); 
        return 1;
    }
    return 0;
}