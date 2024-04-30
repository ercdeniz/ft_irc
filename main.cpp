#include "libraries/Server.hpp"

Server *server = NULL;

bool is_digit(string str)
{
    for (string::size_type i = 0; i < str.length(); i++)
    {
        if (!isdigit(str[i]))
            return false;
    }
    return true;
}

int main(int ac, char** av) {

    if(ac == 3)
    { 
        try
        {
            if (!is_digit(av[1]) || !is_digit(av[2]))
                throw runtime_error("Port and password must be digits");
            server = new Server(atoi(av[1]), av[2]);
            cout <<"asdasd "<< server->get_client_count();
            server->start();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
    }
    else
    {
        cout << "Usage: " << av[0] << " <port> <password>" << endl;
        return 1;
    }
    return 0;
}