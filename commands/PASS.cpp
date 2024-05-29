#include "../includes/Server.hpp"

/**
 * @brief Bağlantı Parolası Belirleme İşlevi
 *
 * PASS komutu, bir 'bağlantı parolası' belirlemek için kullanılır. Parola,
 * bağlantı kaydedilmeye çalışılmadan önce belirlenmeli ve belirlenmelidir.
 * Şu anda, istemcilerin NICK/USER kombinasyonunu göndermeden önce bir PASS
 * komutu göndermeleri gerekmektedir ve sunucuların *herhangi bir SERVER
 * komutundan önce bir PASS komutu göndermeleri gerekmektedir.
 *
 * Komut: PASS
 * Parametreler: <password>
 * Sayısal Yanıtlar:
 * - RPL_PASS: Parola başarıyla belirlendiğinde.
 * - ERR_ALREADYREGISTRED: İstemci zaten kayıtlıysa.
 * - ERR_NEEDMOREPARAMS: Yetersiz parametre sağlandığında.
 * - ERR_PASSWDMISMATCH: Parola eşleşmediğinde.
 *
 * Örnekler:
 * - PASS mypassword: Bağlantı parolasını "mypassword" olarak belirler.
 */

void Server::Pass(std::vector<std::string>& params, Client& cli)
{
    if (params.size() != 1)
        Server::writeMessage(cli._clientFd, ERR_NEEDMOREPARAMS(params[0], _password));
    else if (cli._passChecked == 1)
        Server::writeMessage(cli._clientFd, ERR_ALREADYREGISTRED);
    else if (params[0] == _password)
    {
        cli._passChecked = 1;
        Server::writeMessage(cli._clientFd, RPL_PASS);
    }
    else
        Server::writeMessage(cli._clientFd, ERR_PASSWDMISMATCH);
    if(cli._bot == 0 && !cli._nick.empty())
        Bot(cli, 0);   
}
