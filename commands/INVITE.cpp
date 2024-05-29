#include "../includes/Server.hpp"

/**
 * @brief Kullanıcıyı Kanala Davet Etme İşlevi
 *
 * Bu işlev, bir kullanıcıyı belirli bir IRC kanalına davet etmek için kullanılır.
 * Davet edilen kullanıcı, kanalın üyesi olmasa bile kanala katılma izni alır.
 * Kanal operatörleri, bu komutu kullanarak diğer kullanıcıları davet edebilirler.
 *
 * Komut: INVITE
 * Parametreler: <nickname> <channel>
 * Sayısal Yanıtlar:
 * - RPL_INVITE: Davet işlemi başarılı bir şekilde gerçekleştiğinde.
 * - ERR_CHANOPRIVSNEEDED: Davet işlemi için yeterli yetkiye sahip olunmadığında.
 * - ERR_NOSUCHNICK: Belirtilen kullanıcı bulunamadığında.
 * - ERR_NOSUCHCHANNEL: Belirtilen kanal bulunamadığında.
 *
 * Örnekler:
 *
 * - :Angel INVITE Wiz #Dust: Angel kullanıcısı Wiz'i #Dust kanalına davet eder.
 * - INVITE Wiz #Twilight_Zone: Wiz'i #Twilight_Zone kanalına davet eder.
 */

void Server::Invite(std::vector<std::string>& params, Client& cli)
{
    passChecker(cli);
    if (isChannelExist(params[1]) == 0) {
        Server::writeMessage(cli._clientFd, ERR_NOSUCHCHANNEL(params[1], params[0]));
        return ;
    }
    size_t flag = 0;
    for (cliIt it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->_nick == params[0])
        {
            flag = 1;
            Channel chan = getChannel(params[1]);
            if (cli._nick != chan._opNick) {
                Server::writeMessage(cli._clientFd, ERR_CHANOPRIVSNEEDED(cli._nick, params[1]));
                return ;
            }
            Server::writeMessage(it->_clientFd, RPL_INVITE(cli._nick, cli._ipAddr, chan._name, params[1]));
        }
    }
    if (flag == 0)
        Server::writeMessage(cli._clientFd, ERR_NOSUCHNICK);
}
