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

string Server::trim(const string& str, const string& whitespaces = " \t\n\r\f\v\"\'") {
	if (str.empty())
		return str;
	size_t firstNonSpace = str.find_first_not_of(whitespaces);
	size_t lastNonSpace = str.find_last_not_of(whitespaces);
	if (firstNonSpace == string::npos || lastNonSpace == string::npos)
		return "";
	return str.substr(firstNonSpace, lastNonSpace - firstNonSpace + 1);
}

vector<string> Server::splitString(const string &str, char delimiter)
{
	vector<string> tokens;
	string token;
	int count = 0;
	bool inQuotes = false;
	bool validQuotes = true;

	for (size_t i = 0; i <= str.length(); i++)
	{
		if (str[i] == '"')
		{
			inQuotes = !inQuotes;
			count++;
			validQuotes = false;
			if (count % 2 == 0)
				validQuotes = true;
		}
		else if ((str[i] == delimiter || i == str.length()) && !inQuotes && validQuotes)
		{
			tokens.push_back(token);
			token.clear();
		}
		else
			token += str[i];
	}
	if (!validQuotes)
		throw invalid_argument("Invalid number of quotes");
	return tokens;
}

string Server::toUpper(const string &str)
{
	string result = str;
	transform(result.begin(), result.end(), result.begin(), ::toupper);
	return result;
}