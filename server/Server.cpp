/*
** EPITECH PROJECT, 2020
** cpp_babel_2020
** File description:
** Server
*/

#include "Server.hpp"

Server::Server(boost::asio::io_context &io_context, int port):_io_context(io_context) , _acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
    this->_mydb = Db();
    _mydb.init();
    this->startAccept();
}

Server::~Server()
{
}

void Server::startAccept()
{
    Connexion::pointer new_connection = Connexion::create(_io_context, _mydb);

    _acceptor.async_accept(new_connection->socket(), boost::bind(&Server::handleAccept, this, new_connection, boost::asio::placeholders::error));
}

void Server::handleAccept(Connexion::pointer new_connection, const boost::system::error_code& error)
{
    if (!error)
    {
		std::cout << "new client" << std::endl;
      	new_connection->start();
    }

    startAccept();
}