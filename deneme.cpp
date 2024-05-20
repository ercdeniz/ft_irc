#include <iostream>
#include <vector>
#include <string>
int ft_wordcount(const std::string &s, char c)
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

int ft_wordlen(const std::string &s, char c, size_t start)
{
	int len = 0;
	while (start < s.length() && s[start] != c)
	{
		len++;
		start++;
	}
	return len;
}

std::vector<std::string> ft_split(const std::string &str, char delimiter)
{
	std::vector<std::string> result;
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

int main() {
    std::string s = "join #asd,";
    char delimiter = ',';

    std::vector<std::string> result = ft_split(s, delimiter);

    for (const std::string &str : result) {
        std::cout << str << std::endl;
    }

    return 0;
}