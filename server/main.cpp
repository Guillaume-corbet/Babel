/*
** EPITECH PROJECT, 2020
** cpp_babel_2020
** File description:
** main
*/

#include "Server.hpp"

int main(int argc, char **argv)
{
    if (argc != 2 || std::atoi(argv[1]) <= 0) {
        std::cout << "USAGE : ./babel port" << std::endl;
        std::cout << "port = number of the port" << std::endl;
        return (84);
    }
    
    try
	{
		boost::asio::io_context io_context;
		
		Server *server = new Server(io_context, std::atoi(argv[1]));
		io_context.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
    return (0);
}