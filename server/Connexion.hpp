/*
** EPITECH PROJECT, 2020
** cpp_babel_2020
** File description:
** Connexion
*/

#ifndef CONNEXION_HPP_
#define CONNEXION_HPP_

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <iostream>
#include <algorithm>
#include "Parser.hpp"

class Connexion : public boost::enable_shared_from_this<Connexion> {
    public:
        typedef boost::shared_ptr<Connexion> pointer;
	    static pointer create(boost::asio::io_context &ios, Db &db)
        {
            return pointer(new Connexion(ios, db));
        }
        ~Connexion();
        boost::asio::ip::tcp::socket &socket();
        void start();
    protected:
    private:
        Connexion(boost::asio::io_context &io_context, Db &db);
        void handleReceive(const boost::system::error_code &error);
        void handleSend(const boost::system::error_code &error);
        boost::asio::ip::tcp::socket _socket;
	    std::string _send;
        boost::asio::streambuf _receive;
        Parser *_parser;
};

#endif /* !CONNEXION_HPP_ */
