#include "../libraries/Server.hpp"

bool is_digit(string str)
{
	int len = str.length();
	for (int i = 0; i < len; i++)
		if (!isdigit(str[i]))
			return false;
	return true;
}

void Server::argControl(char **av)
{
	if (!is_digit(av[1]) || !is_digit(av[2]))
	{
		errno = EINVAL;
		throw invalid_argument("Port and password must be digits");
	}
	int port = atoi(av[1]);
	if (port < 1024 || port > 49151)
	{
		errno = EINVAL;
		throw out_of_range("Port must be between 1024 and 49151");
	}
}

void Server::sendMessage(int index)
{
	printFd(_pollfds[index].fd, "Server shutting down...", RED);
}

string Server::trim(const string &str, const string &whitespaces = " \t\n\r\f\v\"\'")
{
	if (str.empty())
		return str;
	size_t firstNonSpace = str.find_first_not_of(whitespaces);
	size_t lastNonSpace = str.find_last_not_of(whitespaces);
	if (firstNonSpace == string::npos || lastNonSpace == string::npos)
		return "";
	return str.substr(firstNonSpace, lastNonSpace - firstNonSpace + 1);
}

int ft_wordcount(const string &s, char c)
{
	unsigned int index = 0;
	size_t i = 0;
	while (i < s.length())
	{
		if (s[i] == c)
			i++;
		else
		{
			while (i < s.length() && s[i] != c)
				i++;
			index++;
		}
	}
	return index;
}

int ft_wordlen(const string &s, char c, size_t start)
{
	int len = 0;
	while (start < s.length() && s[start] != c)
	{
		len++;
		start++;
	}
	return len;
}

vector<string> Server::splitString(const string &str, char delimiter)
{
	vector<string> result;
	size_t j = 0;
	size_t word_count = ft_wordcount(str, delimiter);

	while (result.size() < word_count)
	{
		while (j < str.length() && str[j] == delimiter)
			j++;
		int len = ft_wordlen(str, delimiter, j);
		if (len > 0)
			result.push_back(str.substr(j, len));
		j += len;
	}
	return result;
}

string Server::toUpper(const string &str)
{
	string result = str;
	transform(result.begin(), result.end(), result.begin(), ::toupper);
	return result;
}