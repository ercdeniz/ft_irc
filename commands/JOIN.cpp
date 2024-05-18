#include "../libraries/Server.hpp"

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
 * @param args	JOIN komutunun argümanları (kanal adı ve gerekirse anahtar)
 */

void Server::JOIN(int fdIndex, vector<string> args)
{
	if (args.size() < 2)
	{
		printFd(_pollfds[fdIndex].fd, "Not enough arguments", RED);
		return;
	}

	string channelName = args[1];
	string key = args.size() > 2 ? args[2] : "";

	if (channelName.find(',') == string::npos)
	{
		// Tek kanal
		println("Tek kanal. Kanal adı: " + channelName + ", Anahtar: " + key);
		
	}
	else
	{
		// Birden fazla kanal
		vector<string> channels = splitString(channelName, ',');
		for (size_t i = 0; i < channels.size(); i++)
		{
			if (channels[i].find('&') == string::npos && channels[i].find('#') == string::npos)
			{
				printFd(_pollfds[fdIndex].fd, "Invalid channel name!\nUsage: &channel or #channel", RED);
				return;
			}
			string name = trim(channels[i], "&#");
			if (name.find(' ') != string::npos || name.find(',') != string::npos || name.find('\a') != string::npos)
			{
				printFd(_pollfds[fdIndex].fd, "Invalid channel name!\nChannel name cannot contain space, comma or control G", RED);
				return;
			}

		}
		println("Birden fazla kanal. Kanal adları: " + channelName + ", Anahtarlar: " + key);
	}
}