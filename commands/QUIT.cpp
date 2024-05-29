#include "../includes/Server.hpp"

/**
 * @brief Oturumu Sonlandırma İşlevi
 *
 * QUIT komutu, bir istemci oturumunu sonlandırmak için kullanılır. Sunucu, QUIT
 * mesajı gönderen bir istemciye bağlantıyı kapatmalıdır. Eğer bir "Çıkış
 * Mesajı" belirtilmişse, bu, varsayılan mesaj olan takma adı yerine gönderilecektir.
 * Netsplitler (iki sunucunun bağlantısının kesilmesi) meydana geldiğinde, çıkış
 * mesajı, iki sunucunun adı arasında bir boşlukla ayrılmış şekilde oluşturulur.
 * İlk ad, hala bağlı olan sunucunun adıdır ve ikinci ad, bağlantısı kesilen
 * sunucunun adıdır. Başka bir nedenle, bir istemci bağlantısı QUIT komutu
 * göndermeden kapatılırsa (örneğin, istemci ölür ve soket üzerinde EOF
 * oluşursa), sunucu, oluşmasına neden olan olayın doğasını yansıtan bir
 * mesajla çıkış mesajını doldurmak zorundadır.
 *
 * Komut: QUIT
 * Parametreler: [<Quit Message>]
 * Sayısal Yanıtlar:
 * - RPL_QUIT: Kullanıcı başarıyla çıkış yaptığında.
 *
 * Örnekler:
 * - QUIT : Kullanıcı, varsayılan mesajla çıkış yapar.
 * - QUIT :Client quit unexpectedly: Kullanıcı, "Client quit unexpectedly" mesajıyla çıkış yapar.
 */


void Server::Quit(std::vector<std::string>& params, Client& cli)
{
    if (params[0] == "")
        println("Client " + cli._nick + " quit", RED);
    else
        println("Client " + cli._nick + " quit (" + params[0] + ")", RED);
    cliIt baseIt = _clients.begin();
    for (baseIt = _clients.begin(); baseIt != _clients.end(); ++baseIt) {
        if (baseIt->_nick == cli._nick)
            break;
    }
    int flag;
    chanIt itChan = _channels.begin();
    while (itChan != _channels.end())
    {
        flag = 0;
        for (cliIt itClient = itChan->_channelClients.begin(); itClient != itChan->_channelClients.end(); ++itClient) {
            if (itClient->_nick != baseIt->_nick)
                continue;
            itChan->_channelClients.erase(itClient);
            if (itChan->_channelClients.size() > 0)
            {
                itChan->_opNick = itChan->_channelClients[0]._nick;
                showRightGui(cli, *itChan);
            }
            else if (itChan->_channelClients.size() == 0)
            {
                println("Channel " + itChan->_name + " is deleted", RED);
                _channels.erase(itChan);
                if (_channels.size() == 0)
                {
                    _channels.clear();
                    Server::writeMessage(cli._clientFd, RPL_QUIT(cli._nick, params[0].c_str()));
                    kickClient(baseIt);
                    return;
                }
                itChan = _channels.begin();
                flag = 1;
            }
            break;
        }
        if (flag != 1)
            ++itChan;
    }
    Server::writeMessage(cli._clientFd, RPL_QUIT(cli._nick, params[0].c_str()));
    kickClient(baseIt);
}