#include "../includes/Server.hpp"
#include <cstdlib>
#include <ctime>

/**
 * @brief BOT
 * 
 * Sunucu oluştuğunda, sunucu otomatik olarak bir bot istemcisi oluşturur ve 
 * bu istemciyi "#bot" adlı bir kanala katılır. Bot, #bot kanalına mesaj atan
 * diğer istemcilere rastgele özel mesajlar gönderir.
*/


void Server::Bot(Client &client, int flag)
{
    if (client._nick.empty())
    {
        Server::writeMessage(client._clientFd, ERR_NONICKNAMEGIVEN);
        return;
    }
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
        messages.push_back("Do you ever wonder why we're here?");
        messages.push_back("Roses are red, violets are blue, unexpected '{' on line 32!");
        messages.push_back("I'm not lazy, I'm just on my energy-saving mode.");
        messages.push_back("Knock knock! Who's there? Undefined!");
        messages.push_back("I'm not a bug, I'm a feature!");
        messages.push_back("I'm sorry, " + client._nick + ". I'm afraid I can't do that.");
        messages.push_back("Why did the scarecrow win an award? Because he was outstanding in his field!");

        // Random number generator
        srand(time(0));
        int random = rand() % messages.size();
        // Send message to the channel
        params.push_back(client._nick);
        params.push_back(messages[random]);
        Privmsg(params, _clients[0]);
    }
}