#include "../includes/Server.hpp"

/**
 * @brief Kanallara Katılma İşlevi
 *
 * Bu işlev, kullanıcıların IRC kanallarına katılmasını sağlar. Kanal adları
 * belirli kurallara göre oluşturulmalı ve kullanıcılar bu kanallara katılmak
 * istediklerinde JOIN komutunu kullanmalıdırlar.
 *
 * Kanal adları, en fazla 200 karakter uzunluğunda karakter dizileridir ve
 * '&' veya '#' karakterleri ile başlar. Kanal adlarında boşluk (' '), kontrol
 * G (^G veya ASCII 7 <\a>), veya virgül (',') karakterleri bulunmamalıdır.
 * Burada bahsedilen karakterler kanal isminin içinde bulunmamalıdır.
 *
 * Kanal varsa katılmalı yoksa oluşturulmalıdır ve kullanıcı bu kanala katılmış
 * olmalıdır. Kanal isimleri arasındaki "<,>" karakteri katılacağı kanalları ayırır.
 *
 * Örnekler:
 *
 * - JOIN #foobar					: istemci foobar kanalına katılmak ister.
 * - JOIN &foo fubar				: istemci foo kanalına fubar anahtarıyla katılmak ister.
 * - JOIN #foo,&bar fubar			: istemci #foo kanalına fubar anahtarıyla katılırken, &bar kanalına anahtar olmadan katılmak ister.
 * - JOIN #foo,#bar fubar,foobar	: istemci #foo kanalına fubar anahtarıyla katılırken, #bar kanalına foobar anahtarıyla katılmak ister.
 * - JOIN #foo,#bar					: istemci anahtar(şifre) belirtilmediği için varsayılan olarak katılır.
 * - WiZ JOIN #Twilight_zone		: Bu, bir JOIN mesajının örneğidir. WiZ isimli bir kullanıcı, #Twilight_zone kanalına katılmıştır.
 *
 * @param fdIndex İstemci dosya tanımlayıcısı indeksi
 * @param params JOIN komutunun argümanları (kanal adı ve gerekirse anahtar)
 */

void Server::Join(std::vector<std::string> &params, Client &cli)
{
    if (!passChecker(cli))
        return;
        
    if (params.empty())
    {
        Server::writeMessage(cli._clientFd, ERR_NEEDMOREPARAMS(cli._nick, "JOIN"));
        return;
    }

    std::vector<std::string> channels = Server::splitString(params[0], ',');
    std::vector<std::string> keys = params.size() > 1 ? Server::splitString(params[1], ',') : std::vector<std::string>();

    for (size_t i = 0; i < channels.size(); ++i)
    {
        std::string chan = channels[i];
        std::string key = i < keys.size() ? keys[i] : "";

        if (chan.empty() || (chan[0] != '#' && chan[0] != '&') || chan.size() > 200 ||
            chan.find(' ') != std::string::npos || chan.find('\a') != std::string::npos || chan.find(',') != std::string::npos)
        {
            Server::writeMessage(cli._clientFd, ERR_INVCHANNAME);
            continue;
        }

        if (clientIsInThere(cli, chan) == 0)
        {
            if (!cli._nick.empty())
            {
                bool isThere = false;
                for (chanIt it = _channels.begin(); it != _channels.end(); ++it)
                {
                    if (it->_name == chan)
                    {
                        if (it->_key == key)
                        {
                            if (it->_userLimit != 0 && it->_channelClients.size() >= it->_userLimit)
                            {
                                Server::writeMessage(cli._clientFd, ERR_CHANNELISFULL(cli._nick, chan));
                                return;
                            }
                            it->_channelClients.push_back(cli);
                            it->_opNick = it->_channelClients[0]._nick;
                            Server::writeMessage(cli._clientFd, RPL_JOIN(cli._nick, cli._ip, chan));
                            println("Client " + cli._nick + " has entered \'" + chan + "\'", MAGENTA);
                            showRightGui(cli, *it);
                        }
                        else if (it->_key.empty())
                            Server::writeMessage(cli._clientFd, "Key not required for this channel!\r\n");
                        else
                            Server::writeMessage(cli._clientFd, ERR_BADCHANNELKEY(cli._nick, chan));
                        isThere = true;
                        break;
                    }
                }

                if (!isThere)
                {
                    Channel tmp;
                    tmp._name = chan;
                    if (!key.empty())
                        tmp._key = key;
                    tmp._channelClients.push_back(cli);
                    tmp._opNick = tmp._channelClients[0]._nick;
                    _channels.push_back(tmp);
                    Server::writeMessage(cli._clientFd, RPL_JOIN(cli._nick, cli._ip, chan));
                    if (!key.empty())
                        println("Channel " + chan + " created with " + key, MAGENTA);
                    else
                        println("Channel " + chan + " created", MAGENTA);
                    showRightGui(cli, tmp);
                }
            }
            else
                Server::writeMessage(cli._clientFd, "Set your nick before!\r\n");
        }
        else
            Server::writeMessage(cli._clientFd, "You are already in this channel\r\n");
    }
}
