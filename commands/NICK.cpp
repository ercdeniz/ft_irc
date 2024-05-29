#include "../includes/Server.hpp"

/**
 * @brief Kullanıcı Takma Adı Değiştirme İşlevi
 *
 * 
 * NICK mesajı, kullanıcıya bir takma ad vermek veya önceki takma adını
 * değiştirmek için kullanılır.
 * 
 * Bir NICK mesajı, başka bir istemci için zaten aynı takma adı bilen bir
 * sunucuya ulaştığında, bir takma adı çakışması meydana gelir. Takma adı
 * çakışması sonucunda, sunucunun veritabanından tüm takma adlarının örnekleri
 * kaldırılır ve bir KILL komutu, takma adını tüm diğer sunucu veritabanlarından
 * kaldırmak için gönderilir. Takma adı çakışmasına neden olan NICK mesajı bir
 * takma adı değiştirme işlemiyse, o zaman orijinal (eski) takma ad da kaldırılmalıdır.
 *
 *
 * Komut: NICK
 * Parametreler: <newnick>
 * Sayısal Yanıtlar:
 * - ERR_NEEDMOREPARAMS: Komut için yeterli parametre girilmediğinde.
 * - ERR_NONICKNAMEGIVEN: Yeni bir takma ad belirtilmediğinde.
 * - ERR_NICKNAMEINUSE: Belirtilen takma adı zaten kullanımda olduğunda.
 * - ERR_NICKNAMEEMPTY: Takma adı boş olduğunda.
 *
 * Örnekler:
 *
 * - NICK Wiz: Kullanıcının takma adını "Wiz" olarak değiştirir.
 */


int Server::CheckNickDepends(std::vector<std::string>& params, Client& cli)
{
    if (cli._isCap == NC && !passChecker(cli))
            return (0);
    if (!(params.size() == 1 && !isNickExist(params[0]) && !params[0].empty() && params[0].size() <= 9)) {
        if (params.size() != 1)
            Server::writeMessage(cli._clientFd, ERR_NEEDMOREPARAMS(cli._nick, params[0]));
        else if (params[0].size() > 9)
            Server::writeMessage(cli._clientFd, ERR_NONICKNAMEGIVEN);
        else if (params[0].empty())
            Server::writeMessage(cli._clientFd, ERR_NICKNAMEEMPTY(cli._nick));
        else
            Server::writeMessage(cli._clientFd, ERR_NICKNAMEINUSE(params[0]));
        return (0);
    }
    return (1);
}

void Server::Nick(std::vector<std::string>& params, Client& cli)
{
    if (!CheckNickDepends(params, cli))
        return ;
    std::string oldNick = cli._nick;
    cli._nick = params[0];
    if(cli._isCap == NC && cli._bot == 0 && !cli._nick.empty())
        Bot(cli, 0);  
    for (chanIt it = _channels.begin(); it != _channels.end(); ++it) {
        for (cliIt cit = it->_channelClients.begin(); cit != it->_channelClients.end(); ++cit) {
            if (oldNick == cit->_nick)
            {
                if (oldNick == it->_opNick)
                    it->_opNick = params[0];
                cit->_nick = params[0];
                break;
            }
        }
        Server::writeMessage(cli._clientFd, RPL_NICK(oldNick, cli._user, cli._ipAddr, params[0]));
        showRightGui(cli, *it);
    }
    Server::writeMessage(cli._clientFd, RPL_NICK(oldNick, cli._user, cli._ipAddr, params[0]));
}
