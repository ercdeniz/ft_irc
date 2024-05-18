#include "libraries/Server.hpp"
#include <signal.h>

Server *server = NULL;

void exitServ()
{
	for (size_t i = 0; i < server->clients.size(); i++)
		delete server->clients[i];
	delete server;
	server = NULL;
}

void signalHandler(int signum)
{
	println("\nSignal received: " + convertString(strsignal(signum)), RED);
	for (int i = 0; i < MAX_CLIENTS; i++)
		server->sendMessage(i);
	exitServ();
	exit(signum);
}

void signalCatcher()
{
	struct sigaction sa;
	sa.sa_handler = signalHandler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	for (int i = 1; i < NSIG; ++i)
		if (i != SIGKILL && i != SIGSTOP && i != SIGWINCH)
			if (signal(i, signalHandler) == SIG_ERR)
				printlnErr("Signal error: " + convertString(strsignal(i)), RED);
}

int main(int ac, char **av)
{
	if (ac == 3)
	{
		try
		{
			signalCatcher();
			Server::argControl(av);
			server = new Server(atoi(av[1]), av[2]);
			server->start();
		}
		catch (const std::exception &e)
		{
			printlnErr(string(strerror(errno)) + ": " + string(e.what()), RED);
		}
		if (server)
			exitServ();
	}
	else
	{
		println("Usage: ./server <port> <password>", RED);
		return 1;
	}
	return 0;
}