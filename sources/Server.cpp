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
	int client_fd, status, i, wait;

	for (i = 1; i < MAX_CLIENTS; i++)
	{
		_pollfds[i].fd = -1;
		_pollfds[i].events = POLLIN;
	}

	while (true)
	{
		if (_clientCount == 0)
			wait = 5000;
		else
			wait = -1;
		status = poll(_pollfds, MAX_CLIENTS, wait);
		if (status == 0)
			throw runtime_error("Timeout");
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
					close(client_fd);
					throw runtime_error("Server is full");
				}
				if (_pollfds[i].fd == -1)
				{
					_pollfds[i].fd = client_fd;
					println("Client connected.\n fd: " + convertString(i), GREEN);
					_clientCount++;
					break;
				}
			}
		}
	}
}