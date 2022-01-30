/*
** EPITECH PROJECT, 2020
** cpp_babel_2020
** File description:
** Db
*/

#ifndef DB_HPP_
#define DB_HPP_

#include <sqlite3.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <cctype>

#define LOGIN_SUCCES "10 LOGIN_SUCCES\n"
#define LOGIN_FAIL "11 LOGIN_FAIL\n"
#define LOGIN_SQL_ERROR "19 LOGIN_SQL_ERROR\n"
#define LOGOUT_SUCCES "20 LOGOUT_SUCCES\n"
#define LOGOUT_FAIL "21 LOGOUT_FAIL\n"
#define LOGOUT_SQL_ERROR "29 LOGOUT_SQL_ERROR\n"
#define CREATE_USER_SUCCES "30 CREATE_USER_SUCCES\n"
#define CREATE_USER_USER_ALREADY_EXIST "31 CREATE_USER_USER_ALREADY_EXIST\n"
#define CREATE_USER_SQL_ERROR "39 CREATE_USER_SQL_ERROR\n"
#define CREATE_CONTACT_SUCCES "40 CREATE_CONTACT_SUCCES\n"
#define CREATE_CONTACT_NO_REQUEST_CONTACT "41 CREATE_CONTACT_NO_REQUEST_CONTACT\n"
#define CREATE_CONTACT_ALREADY_EXIST "42 CREATE_CONTACT_ALREADY_EXIST\n"
#define CREATE_CONTACT_SQL_ERROR "49 CREATE_CONTACT_SQL_ERROR\n"
#define CREATE_REQUEST_CONTACT_SUCCES "50 CREATE_REQUEST_CONTACT_SUCCES\n"
#define CREATE_REQUEST_CONTACT_ALREADY_EXIST "51 CREATE_REQUEST_CONTACT_ALREADY_EXIST\n"
#define CREATE_REQUEST_CONTACT_CONTACT_ALREADY_EXIST "52 CREATE_REQUEST_CONTACT_CONTACT_ALREADY_EXIST\n"
#define CREATE_REQUEST_CONTACT_USER_NOT_EXIST "53 CREATE_REQUEST_CONTACT_USER_NOT_EXIST\n"
#define CREATE_REQUEST_CONTACT_SQL_ERROR "59 CREATE_REQUEST_CONTACT_SQL_ERROR\n"
#define DELETE_REQUEST_CONTACT_SUCCES "60 DELETE_REQUEST_CONTACT_SUCCES\n"
#define DELETE_REQUEST_CONTACT_NO_REQUEST_CONTACT "61 DELETE_REQUEST_CONTACT_NO_REQUEST_CONTACT\n"
#define DELETE_REQUEST_CONTACT_USER_NOT_EXIST "62 DELETE_REQUEST_CONTACT_USER_NOT_EXIST\n"
#define DELETE_REQUEST_CONTACT_SQL_ERROR "69 DELETE_REQUEST_CONTACT_SQL_ERROR\n"
#define DELETE_CONTACT_SUCCES "70 DELETE_CONTACT_SUCCES\n"
#define DELETE_CONTACT_NO_CONTACT "71 DELETE_CONTACT_NO_CONTACT\n"
#define DELETE_CONTACT_SQL_ERROR "79 DELETE_CONTACT_SQL_ERROR\n"
#define GET_FRIEND_CONNECTED_SUCCES "100 "
#define GET_FRIEND_CONNECTED_SQL_ERROR "109 GET_FRIEND_CONNECTED_SQL_ERROR\n"
#define GET_USERS_SUCCES "110 "
#define GET_USERS_SQL_ERROR "119 GET_USER_SQL_ERROR\n"
#define GET_ALL_FRIEND_SUCCES "120 "
#define GET_ALL_FRIEND_NO_CONTACT "120 \n"
#define GET_ALL_FRIEND_SQL_ERROR "129 GET_ALL_FRIEND_SQL_ERROR\n"
#define GET_FRIEND_REQUEST_SUCCES "130 "
#define GET_FRIEND_REQUEST_SQL_ERROR "139 GET_FRIEND_REQUEST_SQL_ERROR\n"
#define GET_STATUS "140 "
#define GET_STATUS_SQL_ERROR "149 GET_STATUS_SQL_ERROR\n"
#define GET_CALL_REQUEST "310 "
#define GET_CALL_REQUEST_NO_CALL_INCOMING "311 GET_CALL_REQUEST_NO_CALL_INCOMING\n"
#define GET_CALL_REQUEST_SQL_ERROR "319 GET_CALL_REQUEST_SQL_ERROR\n"

#define USER_TABLE_EMPTY "201 USER_TABLE_EMPTY\n"
#define CONTACT_TABLE_EMPTY "211 CONTACT_TABLE_EMPTY\n"
#define REQUESTCONTACTS_TABLE_EMPTY "221 REQUESTCONTACTS_TABLE_EMPTY\n"
#define DB_ERROR_IN_OPEN "301 DB_ERROR_IN_OPEN\n"
class Db {
    public:
        Db();
        ~Db();
        bool init();
        bool createTableContacts();
        bool createTableUsers();
        bool createTableRequestContacts();
        bool check_if_already_login(std::string username);
        std::string login(std::string username, std::string password, std::string ip); // 10 LOGIN_SUCCES\n | 11 LOGIN_FAIL\n | 19 LOGIN_SQL_ERROR\n
        std::string logout(std::string username); // 20 LOGOUT_SUCCES\n | 21 LOGOUT_FAIL\n | 29 LOGOUT_SQL_ERROR\n
        bool all_logout();
        bool requestCall(std::string username, std::string port, std::string usercall);
        bool acceptCall(std::string username);
        bool in_call(std::string username);
        bool not_in_call(std::string username);
        std::string createUser(std::string username, std::string password, std::string ip); // 30 CREATE_USER_SUCCES\n | 31 CREATE_USER_USER_ALREADY_EXIST\n | 39 CREATE_USER_SQL_ERROR\n
        std::string createContact(std::string username, std::string username_friends); // 40 CREATE_CONTACT_SUCCES\n | 41 CREATE_CONTACT_FAIL_NO_REQUEST_CONTACT\n | 42 CREATE_CONTACT_ALREADY_EXIST\n | 49 CREATE_CONTACT_SQL_ERROR\n
        std::string createRequestContact(std::string username, std::string username_friends); // 50 CREATE_REQUEST_CONTACT_SUCCES\n | 51 CREATE_REQUEST_CONTACT_ALREADY_EXIST\n | 52 CREATE_REQUEST_CONTACT_CONTACT_ALREADY_EXIST\n | 53 CREATE_REQUEST_CONTACT_USER_NOT_EXIST\n | 59 CREATE_REQUEST_CONTACT_SQL_ERROR\n
        bool deleteRequestContact_by_id(std::string Id, std::string Id_contact); // uniquement utiliser dans db
        std::string deleteRequestContact(std::string username, std::string username_friends); // 60 DELETE_REQUEST_CONTACT_SUCCES\n | 61 DELETE_REQUEST_CONTACT_NO_REQUEST_CONTACT\n | 62 DELETE_REQUEST_CONTACT_USER_NOT_EXIST\n | 69 DELETE_REQUEST_CONTACT_SQL_ERROR\n
        std::string deleteContact(std::string username, std::string username_friends); // 70 DELETE_CONTACT_SUCCES\n | 71 DELETE_CONTACT_NO_CONTACT\n | 79 DELETE_CONTACT_SQL_ERROR\n
        bool verifDbUser(std::string username); // 201 USER_TABLE_EMPTY\n
        bool verifDbContacts(std::string Id, std::string Id_contact); // 211 CONTACT_TABLE_EMPTY\n
        bool verifDbRequestContacts(std::string Id, std::string Id_contact); // 221 REQUESTCONTACT_TABLE_EMPTY\n
        bool check_if_empty_table(std::string table);
        std::string getFriendConnected(std::string username); // 100 (succes) | 109 GET_FRIEND_CONNECTED_SQL_ERROR\n
        std::string getUsers(std::string username); // 110 (succes)
        std::string getAllFriend(std::string username); // 120 (succes) | 129 GET_ALL_FRIEND_SQL_ERROR\n
        std::string getFriendRequest(std::string username); // 130 (succes) | 139 GET_FRIEND_REQUEST_SQL_ERROR\n
        std::string getUser(std::string username);
        std::string getStatus(std::string username); // 140 (succes) | 149 GET_STATUS_SQL_ERROR\n
        std::string getAllUsers();
        std::string getCallRequest();
        std::string getUsername_by_id_if_connected(std::string Id);
        std::string getUsername_and_status_by_id(std::string Id);
        std::string getUsername_by_id(std::string Id);
        void seeUsersTable();
        void seeRequestContactsTable();
        void seeContactsTable();
    protected:
        sqlite3 *_db;
    private:
};

static int callbackDb(void *NotUsed, int argc, char **argv, char **azColName);

#endif /* !DB_HPP_ */
