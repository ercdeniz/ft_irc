#include "../includes/Server.hpp"

/**
 * @brief Kullanıcı Bilgilerini Belirtme İşlevi
 *
 * USER mesajı, bir bağlantının başlangıcında, yeni bir kullanıcının kullanıcı adını,
 * ana bilgisayar adını, sunucu adını ve gerçek adını belirtmek için kullanılır.
 * Ayrıca, sunucular arası iletişimde IRC'ye yeni gelen bir kullanıcıyı belirtmek
 * için de kullanılır, çünkü bir istemciden NICK ve USER mesajları alındıktan sonra
 * kullanıcı kaydedilir.
 * 
 * Sunucular arası USER mesajı, istemcinin NICK adı ile öncelenmelidir. Ana bilgisayar
 * adı ve sunucu adı, doğrudan bağlı bir istemciden USER komutu geldiğinde (güvenlik
 * nedenleriyle) IRC sunucusu tarafından normalde yok sayılır, ancak sunucu arası
 * iletişimde kullanılır. Bu, bir yeni kullanıcının ağın geri kalanına tanıtıldığında
 * eşlik eden USER gönderilmeden önce, bir NICK'in her zaman uzak bir sunucuya gönderilmesi
 * gerektiği anlamına gelir.
 * 
 * Gerçek ad parametresinin, son parametre olması gerektiği unutulmamalıdır, çünkü
 * içerisinde boşluk karakterleri bulunabilir ve bunun bir önek olarak tanınması için
 * bir iki nokta üst üste (':') ile öneklenmelidir.
 * 
 * Yalnızca USER mesajına dayanarak bir istemcinin kullanıcı adı hakkında yalan söylemesi
 * kolay olduğundan, bir "Kimlik Sunucusu"nun kullanılması önerilir. Kullanıcının
 * bağlandığı ana bilgisayarın bu tür bir sunucusu etkinleştirilmişse, kullanıcı adı,
 * "Kimlik Sunucusu"ndan gelen yanıtta belirtildiği gibi ayarlanır.
 *
 * Komut: USER
 * Parametreler: <username> <hostname> <servername> :<realname>
 * Sayısal Yanıtlar:
 * - ERR_ALREADYREGISTRED: İstemci zaten kayıtlıysa.
 * - ERR_NEEDMOREPARAMS: Yetersiz parametre sağlandığında.
 *
 * Örnekler:
 * - USER guest 0 0 :Guest User: Yeni bir kullanıcının bilgilerini belirtir.
 */


void Server::User(std::vector<std::string>& params, Client& cli)
{
    if (cli._isCap == NC && !passChecker(cli))
        return;
    if (params.size() < 4)
        Server::writeMessage(cli._clientFd, ERR_NEEDMOREPARAMS(cli._nick, params[0]));
    else if (!cli._user.empty())
        Server::writeMessage(cli._clientFd, ERR_ALREADYREGISTRED);
    else {
        cli._user = params[0];
        cli._host = params[1];
        cli._ip = params[2];
        if (params[3][0] != ':')
            cli._realName = params[3];
        else
        {
            size_t i = 3;
            while (i < params.size()) {
                cli._realName += params[i];
                if (i != params.size() - 1)
                    cli._realName += " ";
                ++i;
            }
            cli._realName.erase(0, 1);
        }
    }
}
