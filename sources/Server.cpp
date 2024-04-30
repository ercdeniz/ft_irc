#include "../libraries/Server.hpp"

Server::Server(int port, string password)
{
	int socket_fd;
	int opt = 1;

	_port = port;
	_password = password;
	_client_count = 0;

	if ((socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
		throw runtime_error("Error creating socket");

	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		throw runtime_error("Error setsockopt");

	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_port = htons(_port);
	_serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (::bind(socket_fd, (sockaddr *)&_serverAddr, sizeof(_serverAddr)) == -1)
		throw runtime_error("Error binding socket");

	if (listen(socket_fd, MAX_CLIENTS) == -1)
		throw runtime_error("Error listening on socket");

	cout << "Server started on port " << _port << endl;

	_pollfds[0].fd = socket_fd;
	_pollfds[0].events = POLLIN;
}

int Server::get_client_count()
{
	return _client_count;
}

void Server::start()
{
	int client_fd;
	int wait;
	int i;
	for (i = 1; i < MAX_CLIENTS; i++)
	{
		_pollfds[i].fd = -1;
		_pollfds[i].events = POLLIN;
	}

	while (true)
	{
		if (_client_count == 0)
			wait = 5000;
		else
			wait = -1;

		poll(_pollfds, MAX_CLIENTS, wait);
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
					_client_count++;
					break;
				}
			}
		}
	}
}