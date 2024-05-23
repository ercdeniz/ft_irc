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

void mapper(const string &channelStr, const string &keyStr, multimap<string, string> &channelKeyMap)
{
	vector<string> channels = Server::splitString(channelStr, ',');
	vector<string> keys = Server::splitString(keyStr, ',');

	for (size_t i = 0; i < channels.size(); ++i)
	{
		string channel = channels[i];
		string key = "";
		if (i < keys.size())
			key = keys[i];
		else
			key = "";
		channelKeyMap.insert(pair<string, string>(channel, key));
	}
}

bool hasChannel(const string &channelName, const vector<Channel> &channels, int *index)
{
	for (size_t i = 0; i < channels.size(); ++i)
		if (!channels[i].getChannelName().compare(channelName))
		{
			*index = i;
			return true;
		}
	return false;
}

bool hasInclude(const string channelName, vector<string> channels)
{
	for (size_t i = 0; i < channels.size(); ++i)
		if (!channels[i].compare(channelName))
			return true;
	return false;
}

void Server::JOIN(int fdIndex, vector<string> args)
{
	if (args.size() < 2 || args.size() > 3)
	{
		printFd(_pollfds[fdIndex].fd, "Not enough arguments", RED);
		return;
	}

	string channelStr = args[1];
	string keyStr = args.size() == 3 ? args[2] : "";

	multimap<string, string> channelsMap;
	mapper(channelStr, keyStr, channelsMap);

	vector<string> errorMessages;
	for (map<string, string>::iterator it = channelsMap.begin(); it != channelsMap.end();  ++it)
	{
		const string &channelName = it->first;
		if (channelName.length() > 200 || channelName.length() == 0)
			errorMessages.push_back("Channel name must be at most 200 characters");
		if (channelName.find('#') == string::npos && channelName.find('&') == string::npos)
			errorMessages.push_back("Channel name must start with '#' or '&'");
		if (channelName.find(' ') != string::npos || channelName.find(',') != string::npos || channelName.find('\a') != string::npos)
			errorMessages.push_back("Channel name cannot contain ' ', ',' or '\\a'");
		int index = -1;
		bool existsInClient = hasInclude(trim(channelName, "&#"), clients[fdIndex - 1]->clientChannels);
		if (_channels.size() != 0 && hasChannel(channelName, _channels, &index)) // channel exists
		{
			if (_channels[index].getLimit() != -1 && _channels[index]._channelClients.size() >= size_t(_channels[index].getLimit()))
				errorMessages.push_back("Channel is full");
			if (_channels[index].getChannelPassword().compare(it->second))
				errorMessages.push_back("Wrong password");
			if (_channels[index]._channelClients[fdIndex - 1] == clients[fdIndex - 1] && existsInClient)
				errorMessages.push_back("Already joined");
			else
			{
				clients[fdIndex - 1]->clientChannels.push_back(trim(channelName, "&#"));
				_channels[index]._channelClients.push_back(clients[fdIndex - 1]);
				printFd(_pollfds[fdIndex].fd, RPL_JOIN(clients[fdIndex - 1]->getNickname(), channelName));
				cout << RPL_JOIN(clients[fdIndex - 1]->getNickname(), channelName) << endl;
				return;
			}
		}
		cout << "size: " << errorMessages.size() << endl;
		if (errorMessages.size() == 0) // channel does not exist
		{
			_channels.push_back(Channel(channelName, it->second));
			_channels[_channels.size() - 1]._channelClients.push_back(clients[fdIndex - 1]);
			clients[fdIndex - 1]->clientChannels.push_back(trim(channelName, "&#"));
			printFd(_pollfds[fdIndex].fd, RPL_JOIN(clients[fdIndex - 1]->getNickname(), channelName));
			printFd(_pollfds[fdIndex].fd, RPL_NAMREPLY(clients[fdIndex - 1]->getNickname(), channelName, clients[fdIndex - 1]->getNickname()));
			printFd(_pollfds[fdIndex].fd, RPL_ENDOFNAMES(clients[fdIndex - 1]->getNickname(), channelName));
			cout << RPL_JOIN(clients[fdIndex - 1]->getNickname(), channelName) << endl;
		}
		else
		{
			printFd(_pollfds[fdIndex].fd, "Error(s) occurred while joining channel: " + channelName, RED);
			for (size_t i = 0; i < errorMessages.size(); ++i)
				printFd(_pollfds[fdIndex].fd, RPL_JOIN(clients[fdIndex - 1]->getNickname(), channelName));
			errorMessages.clear();
		}
	}
}