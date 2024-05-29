#include "../includes/Server.hpp"

/**
 * @brief Kullanıcıya Not Gönderme İşlevi
 *
 * NOTICE mesajı, kullanıcıya bir mesaj göndermek için kullanılır. NOTICA ve
 * PRIVMSG arasındaki fark, bir NOTICE mesajına otomatik yanıtların asla
 * gönderilmemesidir. Bu kural sunucular için de geçerlidir - sunucular, bir
 * NOTICE mesajını alır almaz istemciye herhangi bir hata yanıtı göndermemelidir.
 * Bu kuralın amacı, bir istemcinin aldığı bir şeye otomatik olarak yanıt göndermemesi
 * durumunda bir döngüye girmesini önlemektir. Bu genellikle, sürekli bir şekilde
 * yanıt gönderen otomatlar (yapay zeka veya diğer etkileşimli programlarla
 * kontrol edilen istemciler) tarafından kullanılır.
 *
 * Komut: NOTICE
 * Parametreler: <nickname> <text>
 * Sayısal Yanıtlar:
 * - RPL_NOTICE: Mesaj başarıyla gönderildiğinde.
 * - ERR_NOSUCHNICK: Belirtilen kullanıcı bulunamadığında.
 * - ERR_NEEDMOREPARAMS: Yetersiz parametre sağlandığında.
 * 
 * Örnekler:
 * - NOTICE Angel :Hello!		: Angel kullanıcısına "Hello!" mesajını gönderir.
 * 
 * NOT: Yanıtlar ve örnekler için PRIVMSG'ye bakınız.
 */


void Server::Notice(std::vector<std::string>& params, Client& cli)
{
    if (!passChecker(cli))
        return;
    if (params[0][0] == '#') {
        Server::writeMessage(cli._clientFd, "Cannot notice a channel\r\n");
        return ;
    }
    if (params.size() < 2) {
        Server::writeMessage(cli._clientFd, ERR_NEEDMOREPARAMS(cli._nick, "NOTICE"));
        return ;
    }
    if (cli._nick == params[0]) {
        Server::writeMessage(cli._clientFd, "Cannot notice yourself\r\n");
        return ;
    }
    size_t flag = 0;
    for (cliIt it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->_nick == params[0])
        {
            if (params[1][0] == ':')
                getAfterColon(params);
            flag = 1;
            it->_messageBox.push_back(RPL_NOTICE(cli._nick, params[0], params[1]));
            FD_SET(it->_clientFd, &_writeFds);
            break;
        }
    }
    if (flag == 0) {
        Server::writeMessage(cli._clientFd, ERR_NOSUCHNICK);
    }
}
