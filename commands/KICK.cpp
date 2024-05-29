#include "../includes/Server.hpp"

/**
 * @brief Kullanıcıyı Kanaldan Atma İşlevi
 *
 * Bu işlev, bir kullanıcıyı zorla bir kanaldan çıkarmak için kullanılır.
 * Sadece kanal operatörleri, başka bir kullanıcıyı kanaldan atabilir. Her
 * KICK mesajı alan sunucu, göndericinin gerçekten kanal operatörü olduğunu
 * doğrular ve ardından kullanıcıyı kanaldan çıkarır.
 *
 * Komut: KICK
 * Parametreler: <channel> <user> [<comment>]
 * Sayısal Yanıtlar:
 * - RPL_KICK: Kullanıcı başarıyla kanaldan atıldığında.
 * - RPL_ENDOFNAMES: Kanaldaki kullanıcı listesinin sonu.
 * - ERR_NEEDMOREPARAMS: Yetersiz parametre sağlandığında.
 * - ERR_NOSUCHCHANNEL: Belirtilen kanal mevcut değilse.
 * - ERR_CHANOPRIVSNEEDED: Kullanıcının yeterli yetkiye sahip olmadığı durumlarda.
 * - ERR_CHANOPKICK: Kanal operatörü, kendisini kanaldan atamaz.
 * Örnekler:
 *
 * - :Angel KICK #Dust Wiz: Angel kullanıcısı, Wiz'i #Dust kanalından atar.
 * - KICK #Twilight_Zone Wiz: Wiz'i #Twilight_Zone kanalından atar.
 */


void Server::Kick(std::vector<std::string> &params, Client &cli)
{
    if (!passChecker(cli))
        return;
        
    if (params.size() < 2)
    {
        Server::writeMessage(cli._clientFd, ERR_NEEDMOREPARAMS(cli._nick, params[0]));
        return;
    }
    else if (params[1] == cli._nick)
    {
        Server::writeMessage(cli._clientFd, ERR_CHANOPKICK(cli._nick, params[0]));
        return;
    }
    else
    {
        int flag = 0;
        for (chanIt it = _channels.begin(); it != _channels.end(); ++it)
        {
            if (it->_name == params[0])
            {
                if (cli._nick != it->_opNick)
                {
                    Server::writeMessage(cli._clientFd, ERR_CHANOPRIVSNEEDED(cli._nick, params[0]));
                    return;
                }
                for (cliIt it2 = it->_channelClients.begin(); it2 != it->_channelClients.end(); ++it2)
                {
                    flag = 1;
                    if (it2->_nick == params[1])
                    {
                        std::string msg = "";
                        if (params.size() == 3)
                            msg = params[2];
                        Server::writeMessage(it2->_clientFd, RPL_KICK(it->_opNick, params[0], params[1], msg));
                        Server::writeMessage(it2->_clientFd, RPL_ENDOFNAMES(it2->_nick, params[0]));
                        it->_channelClients.erase(it2);
                        showRightGui(cli, *it);
                        return;
                    }
                }
            }
        }
        if (flag == 0)
            Server::writeMessage(cli._clientFd, ERR_NOSUCHCHANNEL(cli._nick, params[0]));
        else if (flag == 1)
            Server::writeMessage(cli._clientFd, ": 431 " + cli._nick + " " + params[0] + " : " + params[1] + " not in this channel\r\n");
    }
}