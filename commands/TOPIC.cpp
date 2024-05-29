#include "../includes/Server.hpp"

/**
 * @brief Kanal Konusunu Değiştirme veya Görüntüleme İşlevi
 *
 * TOPIC komutu, bir kanalın konusunu değiştirmek veya görüntülemek için
 * kullanılır. Eğer <topic> belirtilmemişse, kanal için <channel> konusu
 * döndürülür. Eğer <topic> parametresi mevcutsa, kanal modları bu eylemi
 * izin veriyorsa, o kanalın konusu değiştirilir.
 *
 * Komut: TOPIC
 * Parametreler: <channel> [<topic>]
 * Sayısal Yanıtlar:
 * - RPL_NOTOPIC: Kanalda henüz bir konu belirlenmediğinde.
 * - RPL_TOPIC: Kanalın mevcut konusu sorgulandığında.
 * - ERR_NEEDMOREPARAMS: Yetersiz parametre sağlandığında.
 * - ERR_CHANOPRIVSNEEDED: Kanal modları bu eyleme izin vermiyorsa.
 * - ERR_NOSUCHCHANNEL: Belirtilen kanal mevcut değilse.
 * Örnekler:
 * - TOPIC #channel: #channel kanalının mevcut konusunu sorgular.
 * - TOPIC #channel New topic: #channel kanalının konusunu "New topic" olarak değiştirir.
 */


std::string getTopic(std::vector<std::string>& params)
{
    std::string ret = "";
    if (params[1][0] == ':')
    {
        for (size_t i = 1; i < params.size(); ++i) {
            ret += params[i];
            ret += " ";
        }
    }
    else
        ret = params[1];
    return (ret);
}

void Server::Topic(std::vector<std::string>& params, Client& cli)
{
    if (!passChecker(cli))
        return;
    if (params.size() <= 1) {
        Server::writeMessage(cli._clientFd, ERR_NEEDMOREPARAMS(cli._nick, params[0]));
        return ;
    }
    size_t flag = 0;
    for (chanIt it = _channels.begin(); it != _channels.end(); ++it) {
        if (params[0] == it->_name) {
            flag = 1;
            if (params.size() == 1 || params[1] == ":") {
                Server::writeMessage(cli._clientFd, RPL_NOTOPIC(it->_opNick, params[0]));
                return ;
            }
            else if (params.size() >= 2) {
                if (it->_opNick != cli._nick) {
                    Server::writeMessage(cli._clientFd, ERR_CHANOPRIVSNEEDED(cli._nick, params[0]));
                    return ;
                }
                it->_topic = getTopic(params);
                std::string top = it->_topic;
                top.erase(0, 1);
                Server::writeAllMessage(it->getFds(), RPL_TOPIC(cli._nick, cli._ipAddr, params[0], top));
                return ;
            }
        }
    }
    if (flag == 0)
        Server::writeMessage(cli._clientFd, ERR_NOSUCHCHANNEL(cli._nick, params[0]));
}
