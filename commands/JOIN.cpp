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

void mapper(const string &channelStr, const string &keyStr, multimap<string, string> &channelKeyMap) {
    vector<string> channels = Server::splitString(channelStr, ',');
    vector<string> keys = Server::splitString(keyStr, ',');

    for (size_t i = 0; i < channels.size(); ++i) {
        string channel = channels[i];
        string key = (i < keys.size()) ? keys[i] : "";
		channelKeyMap.insert(pair<string, string>(channel, key));
    }
}

void Server::JOIN(int fdIndex, vector<string> args)
{
	if (args.size() < 2)
	{
		printFd(_pollfds[fdIndex].fd, "Not enough arguments", RED);
		return;
	}

	string channelStr = args[1];
	string keyStr = args.size() > 2 ? args[2] : "";

	multimap<string, string> channelsMap;
	mapper(channelStr, keyStr, channelsMap);


	if (channelsMap.size() == 1)
	{
		// Tek kanal
		for (map<string, string>::iterator it = channelsMap.begin(); it != channelsMap.end(); ++it) 
			cout << "Kanal adı: " << it->first << ", Anahtar: " << it->second << endl;
	}
	else
	{
		// Birden fazla kanal
		vector<string> errorMessages;
		for (map<string, string>::iterator it = channelsMap.begin(); it != channelsMap.end(); ++it) 
		{
			const string &channelName = it->first;
			if (channelName.length() > 200)
				errorMessages.push_back("Channel name must be at most 200 characters");
			if (channelName.find('#') == string::npos && channelName.find('&') == string::npos)
				errorMessages.push_back("Channel name must start with '#' or '&'");
			if (channelName.find(' ') != string::npos || channelName.find(',') != string::npos || channelName.find('\a') != string::npos)
				errorMessages.push_back("Channel name cannot contain ' ', ',' or '\\a'" );
			for (size_t i = 0; i < errorMessages.size(); ++i)
				printFd(_pollfds[fdIndex].fd, errorMessages[i], RED);
		}
		
	}
}