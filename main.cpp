#include "libraries/Server.hpp"

bool is_digit(string str)
{
    int len = str.length();
    for (int i = 0; i < len; i++)
        if (!isdigit(str[i]))
            return false;
    return true;
}

int main(int ac, char** av) {

    if(ac == 3)
    { 
        try
        {
            if (!is_digit(av[1]) || !is_digit(av[2]))
                throw runtime_error("Port and password must be digits");
            Server *server = new Server(atoi(av[1]), av[2]);
            server->start();
        }
        catch(const std::exception& e)
        {
            printlnErr(e.what(), RED);
        }
        
    }
    else
    {
        println("Usage: ./server <port> <password>", RED); 
        return 1;
    }
    return 0;
}