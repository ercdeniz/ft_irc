#include "../libraries/Server.hpp"

Server::Server(int port, string password)
{
	_port = port;
	_password = password;
	socketGenerate();
	_clientCount = 0;
}

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

void Server::isRevent()
{
	int client_fd;

	if (_pollfds[0].revents)
	{
		if ((client_fd = accept(_pollfds[0].fd, NULL, NULL)) == -1)
			throw runtime_error("Error accepting client");

		for (int i = 1; i <= MAX_CLIENTS; i++)
		{
			if (i == MAX_CLIENTS)
			{
				printFd(client_fd, "Sory, server is full", BLUE);
				println("Server is full\n-> fd: " + convertString(client_fd), BLUE);
				close(client_fd);
				return;
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
}

void Server::receiveData()
{
	int bytes;
	char buf[BUFFER + 1];

	for (int i = 1; i <= MAX_CLIENTS; i++)
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

void Server::start()
{
	int status, wait;

	// Initialize pollfds
	for (int i = 1; i <= MAX_CLIENTS; i++)
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
			errno = ETIMEDOUT;
			throw runtime_error("Timeout");
		}
		if (status == -1)
			throw runtime_error("Error polling");
		isRevent();
		receiveData();
	}
}

void Server::handleCommand(int fdIndex, char *buf)
{
	string bufStr(buf);
	if (bufStr.find("\r") != string::npos)
		clients[fdIndex - 1].setIsNc(false);

	vector<string> args = splitString(trim(buf, "\r\n"), ' ');

	string command = toUpper(args[0]);

	cout << "Command: " << command << endl;

	size_t len = command.length();

	if (!command.compare(0, len, "PASS"))
		PASS(fdIndex, args);
	else if (!command.compare(0, len, "QUIT") || !command.compare(0, len, "EXIT"))
		QUIT(fdIndex);
	else if (clients[fdIndex - 1].getHasPass())
	{
		if (!(command.compare(0, len, "USER")))
			USER(fdIndex, args);
		else if (!(command.compare(0, len, "NICK")))
			NICK(fdIndex, args);
		else if (!(command.compare(0, len, "JOIN")))
			JOIN(fdIndex, args);
		else if (!clients[fdIndex - 1].getNickname().empty() && !clients[fdIndex - 1].getUsername().empty())
			println(clients[fdIndex - 1].getNickname() + ": " + convertString(buf), RED);
		else
			printFd(_pollfds[fdIndex].fd, "Enter nickname and username first", RED);
	}
	else
		printFd(_pollfds[fdIndex].fd, "Enter password first", RED);
}
