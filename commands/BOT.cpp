#include "../includes/Server.hpp"
#include <cstdlib>
#include <ctime>

void Server::Bot(Client &client, int flag)
{
    std::vector<std::string> params;

    if (flag == 0)
    {
        std::vector<std::string> join;
        join.push_back("#bot");
        (this->*_commands["JOIN"])(join, client);
        client._bot = 1;
        params.push_back(client._nick);
        params.push_back("Welcome to the bot channel! " + client._nick);
        Notice(params, _clients[0]);
    }
    else if (flag == 1)
    {
        // Random messages
        std::vector<std::string> messages;
        messages.push_back("Hayırlı Cumalar!");
        messages.push_back("Hello! I'm a bot!");
        messages.push_back("HelloWorld(\"print\")");
        messages.push_back("How are you " + client._nick + "?");

        // Random number generator
        srand(time(0));
        int random = rand() % 4;

        // Send message to the channel
        params.push_back(client._nick);
        params.push_back(messages[random]);
        Privmsg(params, _clients[0]);
    }
}