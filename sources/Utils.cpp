#include "../libraries/Server.hpp"

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
	bool inQuotes = false;
	stringstream ss(str);

	while (getline(ss, token, delimiter))
	{
		if (token[0] == '"' && token.back() == '"')
			tokens.push_back(token.substr(1, token.size() - 2));
		else if (inQuotes)
			tokens.back() += delimiter + token;
		else
		{
			tokens.push_back(token);
			inQuotes = token[0] == '"';
		}
	}
	return tokens;
}

string Server::toUpper(const string &str)
{
	string result = str;
	transform(result.begin(), result.end(), result.begin(), ::toupper);
	return result;
}