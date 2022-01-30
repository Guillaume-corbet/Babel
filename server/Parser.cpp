/*
** EPITECH PROJECT, 2020
** cpp_babel_2020
** File description:
** Parser
*/

#include "Parser.hpp"

Parser::Parser(Db &db):_myDb(db)
{
    _command["CREATEACCOUNT"] = &Parser::createAccount;
    _command["LOGIN"] = &Parser::login;
    _command["LOGOUT"] = &Parser::logout;
    _command["ADDFRIEND"] = &Parser::addFriend;
    _command["DELETEFRIEND"] = &Parser::deleteFriend;
    _command["ACCEPTFRIENDREQUEST"] = &Parser::acceptFriendRequest;
    _command["REFUSEFRIENDREQUEST"] = &Parser::refuseFriendRequest;
    _command["STARTCALL"] = &Parser::startCall;
    _command["ENDCALL"] = &Parser::endCall;
    _command["GETCALLREQUEST"] = &Parser::getCallRequest;
    _command["ACCEPTCALL"] = &Parser::acceptCall;
    _command["REFUSECALL"] = &Parser::refuseCall;
    _command["GETFRIEND"] = &Parser::getFriend;
    _command["GETFRIENDCONNECTED"] = &Parser::getFriendConnected;
    _command["GETFRIENDREQUEST"] = &Parser::getFriendRequest;
    _command["ISREADYTOCALL"] = &Parser::readytocall;
    _command["EXIT"] = &Parser::exit;
}

Parser::~Parser()
{
    this->_myDb.~Db();
}

std::string Parser::getArgument(std::string input, int i) const
{
    std::string response;
    size_t j = 0;

    while (i > 1) {
        while (input.size() > j && input.at(j) != ' ') {
            j = j + 1;
        }
        j = j + 1;
        i = i - 1;
    }
    while (input.size() > j && input.at(j) != ' ') {
        response.push_back(input.at(j));
        j = j + 1;
    }
    return (response);
}

std::string Parser::parse(std::string input)
{
    std::string argument = getArgument(input, 1);
    std::string toSend;

    auto it = this->_command.find(argument);
    if (it != this->_command.end()) {
        toSend = (this->*it->second)(input);
    } else {
        return ("ERROR\n");
    }
    return (toSend);
}

std::string Parser::createAccount(std::string input)
{
    std::cout << "CREATE ACCOUNT" << std::endl;
    return (this->_myDb.createUser(this->getArgument(input, 2), this->getArgument(input, 3), _ip));
}

std::string Parser::login(std::string input)
{
    std::cout << "LOGIN" << std::endl;
    return (this->_myDb.login(this->getArgument(input, 2), this->getArgument(input, 3), _ip));
}

std::string Parser::logout(std::string input)
{
    std::cout << "LOGOUT" << std::endl;
    return (this->_myDb.logout(this->getArgument(input, 2)));
}

std::string Parser::addFriend(std::string input)
{
    std::cout << "ADD FRIEND" << std::endl;
    return (this->_myDb.createRequestContact(this->getArgument(input, 2), this->getArgument(input, 3)));
}

std::string Parser::deleteFriend(std::string input)
{
    std::cout << "DELETE FRIEND" << std::endl;
    return (this->_myDb.deleteContact(this->getArgument(input, 2), this->getArgument(input, 3)));
}

std::string Parser::acceptFriendRequest(std::string input)
{
    std::cout << "ACCEPTE FRIEND REQUEST" << std::endl;
    return (this->_myDb.createContact(this->getArgument(input, 2), this->getArgument(input, 3)));
}

std::string Parser::refuseFriendRequest(std::string input)
{
    std::cout << "REFUSE FRIEND REQUEST" << std::endl;
    return (this->_myDb.deleteRequestContact(this->getArgument(input, 2), this->getArgument(input, 3)));
}

std::string Parser::getFriend(std::string input)
{
    std::cout << "GET FRIEND" << std::endl;
    return (this->_myDb.getAllFriend(this->getArgument(input, 2)));
}

std::string Parser::getFriendConnected(std::string input)
{
    std::cout << "GET FRIEND CONNECTED" << std::endl;
    return (this->_myDb.getFriendConnected(this->getArgument(input, 2)));
}

std::string Parser::getFriendRequest(std::string input)
{
    std::cout << "GET FRIEND REQUEST" << std::endl;
    return (this->_myDb.getFriendRequest(this->getArgument(input, 2)));
}

std::string Parser::getUsers(std::string input)
{
    std::cout << "GET USERS" << std::endl;
    return (this->_myDb.getUsers(this->getArgument(input, 2)));
}

std::string Parser::getUser(std::string input)
{
    std::cout << "GET USER" << std::endl;
    return ("User\n");
}

std::string Parser::startCall(std::string input)
{
    std::cout << "START CALL" << std::endl;
    //return (this->_myDb.startCall(this->getArgument(input, 2), this->getArgument(input, 3), this->getArgument(input, 4)));
    return ("start call\n");
}

std::string Parser::endCall(std::string input)
{
    std::cout << "END CALL" << std::endl;
    return ("stop call\n");
}

std::string Parser::getCallRequest(std::string input)
{
    std::cout << "GET CALL" << std::endl;

    //return (_myDb.getCall(getArgument(input, 2), this->getArgument(input, 3), this->getArgument(input, 4)));
    return ("GET CALL\n");
}

std::string Parser::acceptCall(std::string input)
{
    std::cout << "CALL ACCEPTED" << std::endl;

    //return (_myDb.acceptCall(getArgument(input, 2)));
    return ("ACCEPT CALL\n");
}

std::string Parser::refuseCall(std::string input)
{
    std::cout << "CALL REFUSED" << std::endl;

    //return (_myDb.refuseCall(getArgument(input, 2)));
    return ("REFUSED CALL\n");
}

std::string Parser::readytocall(std::string input)
{
    std::cout << "READY TO CALL" << std::endl;
    return ("ready to call\n");
}

std::string Parser::exit(std::string input)
{
    std::cout << "EXIT" << std::endl;
    if (this->getArgument(input, 2).length() > 0)
        this->_myDb.logout(this->getArgument(input, 2));
    return ("Exit\n");
}

/*std::string Parser::acceptCall(std::string input)
{
    std::cout << "ACCEPT CALL" << std::endl;
    return (this->_myDb.acceptCall(this->getArgument(input, 2), this->getArgument(input, 3)));
}

std::string Parser::refuseCall(std::string input)
{
    std::cout << "REFUSE CALL" << std::endl;
    return (this->_myDb.refuseCall(this->getArgument(input, 2), this->getArgument(input, 3)));
}*/

void Parser::setIp(std::string ip)
{
    this->_ip = ip;
}