#include "../libraries/Server.hpp"

void Server::JOIN(int fdIndex, vector<string> args)
{
	if (args.size() < 2 || args.size() > 3)
	{
		printFd(_pollfds[fdIndex].fd, "Usage: JOIN <channel> [password]", RED);
		return;
	}
    if(args[1][0] != '#')
    {
        printFd(_pollfds[fdIndex].fd, "Channel name start with #", RED);
        return;
    }
	string channelName = trim(args[1], "\n#");
	if (channelName.empty())
		printFd(_pollfds[fdIndex].fd, "Channel name can't be empty", RED);
	if (args.size() == 2)
		Channel channel(args[1]);
	if (args.size() == 3)
    {
        if(args[2].find(' ') != string::npos)
        {
            printFd(_pollfds[fdIndex].fd, "Channel name can't contain spaces", RED);
            return;
        }
		Channel channel(args[1], args[2]);
    }
}