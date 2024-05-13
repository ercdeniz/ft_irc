#include "../libraries/Server.hpp"

void Server::socketGenerate()
{
	int socket_fd;
	int opt = 1;

	if ((socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
		throw runtime_error("Error creating socket");

	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		throw runtime_error("Error setsockopt");

	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_port = htons(_port);
	_serverAddr.sin_addr.s_addr = INADDR_ANY;

	if (::bind(socket_fd, (sockaddr *)&_serverAddr, sizeof(_serverAddr)) == -1)
		throw runtime_error("Error binding socket");

	if (listen(socket_fd, MAX_CLIENTS) == -1)
		throw runtime_error("Error listening on socket");

	println("Server started on port: " + convertString(_port), GREEN);

	_pollfds[0].fd = socket_fd;
	_pollfds[0].events = POLLIN;
}

Server::Server(int port, string password)
{
	_port = port;
	_password = password;
	socketGenerate();
	_clientCount = 0;
}

void Server::start()
{
	int client_fd, status, i, wait, bytes;
	char buf[BUFFER + 1];

	for (i = 1; i < MAX_CLIENTS; i++)
	{
		_pollfds[i].fd = -1;
		_pollfds[i].events = POLLIN;
	}

	while (true)
	{
		if (_clientCount == 0)
			wait = 15000;
		else
			wait = -1;
		status = poll(_pollfds, MAX_CLIENTS, wait);
		if (status == 0)
		{
			delete this;
			throw runtime_error("Timeout");
		}
		if (status == -1)
			throw runtime_error("Error polling");
		if (_pollfds[0].revents)
		{
			if ((client_fd = accept(_pollfds[0].fd, NULL, NULL)) == -1)
				throw runtime_error("Error accepting client");

			for (i = 1; i < MAX_CLIENTS; i++)
			{
				if (i == MAX_CLIENTS)
				{
					printFd(client_fd, "Server is full");
					close(client_fd);
					throw runtime_error("Server is full");
				}
				if (_pollfds[i].fd == -1)
				{
					Client client = Client();
					_pollfds[i].fd = client_fd;
					println("Client connected!\n-> fd: " + convertString(client_fd), GREEN);
					printFd(client_fd, "Welcome to the S.E.N. IRC Server", MAGENTA);
					clients.push_back(client);
					_clientCount++;
					break;
				}
			}
		}
		for (i = 1; i <= MAX_CLIENTS; i++)
		{
			if (_pollfds[i].revents)
			{
				bytes = recv(_pollfds[i].fd, buf, BUFFER, 0);
				if (bytes <= 0)
					QUIT(i);
				else
				{
					buf[bytes] = '\0';
					handleCommand(i, buf);
				}
			}
		}
	}
}

void Server::handleCommand(int i, char *buf)
{
	if (!buf)
		return;
	size_t len;
	string command = toUpper(string(buf));

	if (command.find("\r") != string::npos)
		clients[i - 1].setIsNc(false);

	if (!(clients[i - 1].getIsNc()))
		command.erase(remove(command.begin(), command.end(), '\r'), command.end());

	vector<string> args = splitString(command, ' ');

	if (args.size() == 1)
		len = args[0].length() - 1;
	else
		len = args[0].length();

	if (!args[0].compare(0, len, "PASS"))
		PASS(i, args);
	else if (!args[0].compare(0, len, "QUIT") || !args[0].compare(0, len, "EXIT"))
		QUIT(i);
	else if (clients[i - 1].getHasPass())
	{
		if (!(args[0].compare(0, len, "USER")))
			USER(i, args);
		else if (!(args[0].compare(0, len, "NICK")))
			NICK(i, args);
		else if (!clients[i - 1].getNickname().empty() && !clients[i - 1].getUsername().empty())
			println(clients[i - 1].getNickname() + ": " + convertString(buf), RED);
		else
			printFd(_pollfds[i].fd, "Enter nickname and username first", RED);
	}
	else
		printFd(_pollfds[i].fd, "Enter password first", RED);
}