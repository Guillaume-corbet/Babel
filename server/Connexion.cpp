/*
** EPITECH PROJECT, 2020
** cpp_babel_2020
** File description:
** Connexion
*/

#include "Connexion.hpp"

Connexion::Connexion(boost::asio::io_context &io_context, Db &db): _socket(io_context)
{
	this->_parser = new Parser(db);
}

Connexion::~Connexion()
{
	this->_parser->~Parser();
}

boost::asio::ip::tcp::socket &Connexion::socket()
{
	return this->_socket;
}
					
void Connexion::start()
{
	this->_send = "Bienvenue sur le serveur!\n";
	this->_parser->setIp(this->_socket.remote_endpoint().address().to_string());
	boost::asio::async_write(this->_socket, boost::asio::buffer(this->_send), boost::bind(&Connexion::handleReceive, shared_from_this(), boost::asio::placeholders::error));
}	
					
void Connexion::handleReceive(const boost::system::error_code &error)
{
	this->_send = "";
	boost::asio::async_read_until(this->_socket, _receive, '\n', boost::bind(&Connexion::handleSend, shared_from_this(), boost::asio::placeholders::error));
}

void Connexion::handleSend(const boost::system::error_code &error)
{
	std::string str(boost::asio::buffers_begin(_receive.data()), boost::asio::buffers_end(_receive.data()));
	for(unsigned int i=0; i < str.size(); i++) {
    	if(str[i] == '\r' || str[i] == '\n') {
        	str.erase(i,1);
			i = i - 1;
    	}
   	}
	std::cout << str << std::endl;
	_receive.consume(_receive.size());
	this->_send = this->_parser->parse(str);
	if (this->_send == "Exit\n")
		this->~Connexion();
	boost::asio::async_write(this->_socket, boost::asio::buffer(this->_send), boost::bind(&Connexion::handleReceive, shared_from_this(), boost::asio::placeholders::error));
}