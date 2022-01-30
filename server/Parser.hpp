/*
** EPITECH PROJECT, 2020
** cpp_babel_2020
** File description:
** Parser
*/

#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <iostream>
#include <map>
#include <functional>
#include "Db.hpp"

class Parser {
    public:
        Parser(Db &db);
        ~Parser();
        std::string getArgument(std::string input, int i) const;
        std::string parse(std::string input);
        void setIp(std::string ip);
    protected:
    private:
        std::string createAccount(std::string input);
        std::string login(std::string input);
        std::string logout(std::string input);
        std::string addFriend(std::string input);
        std::string deleteFriend(std::string input);
        std::string acceptFriendRequest(std::string input);
        std::string refuseFriendRequest(std::string input);
        std::string getFriendConnected(std::string input);
        std::string getFriendRequest(std::string input);
        std::string getFriend(std::string input);
        std::string getUsers(std::string input);
        std::string getUser(std::string input);
        std::string startCall(std::string input);
        std::string endCall(std::string input);
        std::string getCallRequest(std::string input);
        std::string readytocall(std::string input);
        std::string acceptCall(std::string input);
        std::string refuseCall(std::string input);
        std::string exit(std::string input);
        Db _myDb;
        std::map<std::string, std::string (Parser::*)(std::string input)> _command;
        std::string _ip;
};

#endif /* !PARSER_HPP_ */
