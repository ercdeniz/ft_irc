#include "../includes/Server.hpp"

/**
 * @brief Özel Mesaj Gönderme İşlevi
 *
 * PRIVMSG komutu, kullanıcılar arasında özel mesaj göndermek için kullanılır.
 * <receiver>, mesajın alıcısının takma adıdır. <receiver>, virgüllerle
 * ayrılmış bir isim veya kanal listesi de olabilir.
 * 
 * <receiver> parametresi ayrıca bir host maskesi (#mask) veya sunucu maskesi
 * ($mask) de olabilir. Her iki durumda da sunucu, maskeye uyan bir sunucu veya
 * ana bilgisayara sahip olanlara sadece PRIVMSG gönderir. Maske en az bir (1)
 * "." içermeli ve son "."'den sonraki joker karakterleri içermemelidir. Bu
 * gereklilik, "#*" veya "$*" gibi tüm kullanıcılara yayın yapılmasını önlemek
 * içindir; tecrübelerden, bu sorumlulukla ve doğru bir şekilde kullanıldığından
 * daha fazla kötüye kullanılmıştır. Joker karakterleri '*', '?' karakterleridir.
 * Bu PRIVMSG komutuna ek, yalnızca Operatörlere özeldir.
 *
 * Komut: PRIVMSG
 * Parametreler: <receiver> <message>
 * Sayısal Yanıtlar:
 * - RPL_PRIVMSG: Mesaj başarıyla gönderildiğinde.
 * - ERR_NOSUCHNICK: Belirtilen kullanıcı bulunamadığında.
 * - ERR_NOSUCHCHANNEL: Belirtilen kanal bulunamadığında.
 * - ERR_NEEDMOREPARAMS: Yetersiz parametre sağlandığında.
 *
 * Örnekler:
 * - PRIVMSG Wiz :Hello, are you there?: Wiz kullanıcısına "Hello, are you there?" mesajını gönderir.
 * - PRIVMSG #channel :Hi everyone!: #channel kanalına "Hi everyone!" mesajını gönderir.
 */


void Server::toChannel(std::vector<std::string>& params, Client& cli)
{
    if (params[0] == "#bot")
        Bot(cli, 1);
    if (isChannelExist(params[0]) == 0) {
        Server::writeMessage(cli._clientFd, ERR_NOSUCHCHANNEL(params[0], params[1]));
        return ;
    }
    if (clientIsInThere(cli, params[0]) == 1)
    {
        if (params[1][0] == ':') {
            getAfterColon(params);
        }
        for (cliIt it = _clients.begin(); it != _clients.end(); ++it) {
            if (it->_nick != cli._nick && clientIsInThere(*it, params[0]) == 1) {
                it->_messageBox.push_back(RPL_PRIVMSG(cli._nick, params[0], params[1]));
                FD_SET(it->_clientFd, &_writeFds);
            }
        }
    }
}

void Server::toClient(std::vector<std::string>& params, Client& cli)
{
    int flag = 0;

    for (cliIt it = _clients.begin(); it != _clients.end(); ++it) {
        if (params[0] == it->_nick) {
            if (params[1][0] == ':') {
               getAfterColon(params);
            }
            it->_messageBox.push_back(RPL_PRIVMSG(cli._nick, params[0], params[1]));
            FD_SET(it->_clientFd, &_writeFds);
            flag = 1;
            return ;
        }
    }
    if (flag == 0)
        Server::writeMessage(cli._clientFd, ERR_NOSUCHNICK);
}

void Server::Privmsg(std::vector<std::string>& params, Client& cli)
{
    if (!passChecker(cli))
        return;
    if (params.size() < 2)
    {
        Server::writeMessage(cli._clientFd, ERR_NEEDMOREPARAMS(cli._nick, params[0]));
        return ;
    }
    else if (params[0][0] == '#')
        toChannel(params, cli);
    else
        toClient(params, cli);
}
