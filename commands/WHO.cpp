#include "../includes/Server.hpp"

/**
 * @brief Kullanıcı Bilgisi Sorgulama İşlevi
 *
 * WHO mesajı, istemci tarafından verilen <name> parametresi ile eşleşen bilgi
 * listesini döndürmek için kullanılır. <name> parametresi belirtilmemişse, tüm
 * görünür kullanıcılar (görünmez olmayan kullanıcılar - kullanıcı modu +i olanlar
 * ve istekte bulunan istemci ile ortak bir kanala sahip olmayanlar) listelenir.
 * Aynı sonuca "<name>" olarak "0" veya mümkün olan her girişi eşleştirecek herhangi
 * bir joker karakter kullanılarak da ulaşılabilir.
 *
 * WHO'ya iletilen <name>, kanal <name> bulunamazsa, kullanıcıların ana bilgisayar,
 * sunucu, gerçek ad ve takma adına eşleştirilir.
 *
 *
 * Komut: WHO
 * Parametreler: [<name>]
 * Sayısal Yanıtlar:
 * - ERR_NOSUCHCHANNEL: Belirtilen kanal mevcut değ
 *
 * Örnekler:
 * - WHO #channel: #channel kanalındaki kullanıcılar hakkında bilgi döndürür.
 */


void Server::Who(std::vector<std::string>& params, Client& cli)
{
    if (!passChecker(cli))
        return;
    if (params.size() > 1)
        return ;
    if (isChannelExist(params[0]) == false)
        Server::writeMessage(cli._clientFd,ERR_NOSUCHCHANNEL(params[0], cli._nick));
    for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        if (it->_name == params[0])
        {
            Server::writeMessage(cli._clientFd, "Users in this channel:\r\n");
            for (cliIt it2 = it->_channelClients.begin(); it2 != it->_channelClients.end(); ++it2) {
                Server::writeMessage(cli._clientFd, "* " + it2->_nick + "\r\n");
            }
            return;
        }
    }
}
