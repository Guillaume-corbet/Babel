/*
** EPITECH PROJECT, 2020
** cpp_babel_2020
** File description:
** Server
*/

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include "Connexion.hpp"
#include "Db.hpp"

class Server {
    public:
        Server(boost::asio::io_service& io_service, int port);
        ~Server();
    protected:
    private:
        void startAccept();
        void handleAccept(Connexion::pointer new_connection, const boost::system::error_code& error);
        boost::asio::ip::tcp::acceptor _acceptor;
        boost::asio::io_context &_io_context;
        Db _mydb;
};

#endif /* !SERVER_HPP_ */