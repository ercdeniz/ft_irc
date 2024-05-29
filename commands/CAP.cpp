#include "../includes/Server.hpp"

/**
 * @brief CAP Komutu
 * 
 * CAP komutu, istemci ve sunucu arasında CAPABILITY komutu ile belirtilen yeteneklerin
 * kullanılabilirliğini belirlemek için kullanılır.
 * 
 * Komut: CAP
 * Parametreler: <subcommand>
 * Sayısal Yanıtlar: Yok
 * Örnekler:
 * 
 * - CAP LS : Sunucunun desteklediği yetenekleri listeler.
 * 
 */
void Server::Cap(std::vector<std::string>& params, Client& cli)
{
    if (params[0] == "LS")
        cli._isCap = HEX;
    else
        cli._isCap = NC;
}
