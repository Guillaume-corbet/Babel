/*
** EPITECH PROJECT, 2020
** cpp_babel_2020
** File description:
** Db
*/

#include "Db.hpp"

Db::Db()
{
    this->_db = NULL;
}

Db::~Db()
{
    if (this->_db != NULL)
        sqlite3_close(_db);
}

bool Db::init()
{
    int rc;
    rc = sqlite3_open_v2("babel.db", &_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if(rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(_db) << std::endl;
        return (false);
    }
    std::cout << "Opened database successfully" << std::endl;
    if (createTableUsers() == false || createTableContacts() == false || createTableRequestContacts() == false) {
        return (false);
    }
    return (true);
}

bool Db::createTableContacts()
{
    int rc;
    std::string sql;
    char *zErrMsg = 0;

    sql = "CREATE TABLE IF NOT EXISTS Contacts(Id INT NOT NULL, Id_contact INT NOT NULL);";
    rc = sqlite3_exec(_db, sql.c_str(), callbackDb, 0, &zErrMsg);
   
    if( rc != SQLITE_OK ){
        std::cerr << "SQL error: " << std::string(zErrMsg) << std::endl;
        sqlite3_free(zErrMsg);
        return (false);
    }
    std::cout << "Table created successfully" << std::endl;
    return (true);
}

bool Db::createTableUsers()
{
    int rc;
    std::string sql;
    char *zErrMsg = 0;

    sql = "CREATE TABLE IF NOT EXISTS Users(Id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, Username TEXT NOT NULL, Password TEXT NOT NULL, Status INTEGER NOT NULL, LastIp TEXT, Port TEXT, UserCall TEXT);";
    rc = sqlite3_exec(_db, sql.c_str(), callbackDb, 0, &zErrMsg);
    if( rc != SQLITE_OK ) {
        std::cerr << "SQL error: " << std::string(zErrMsg) << std::endl;
        sqlite3_free(zErrMsg);
        return (false);
    }
    std::cout << "Table created successfully" << std::endl;
    return (true);
}

bool Db::createTableRequestContacts()
{
    int rc;
    std::string sql;
    char *zErrMsg = 0;

    sql = "CREATE TABLE IF NOT EXISTS RequestContacts(Id INT NOT NULL, Id_contact INT NOT NULL);";
    rc = sqlite3_exec(_db, sql.c_str(), callbackDb, 0, &zErrMsg);
   
    if( rc != SQLITE_OK ){
        std::cerr << "SQL error: " << std::string(zErrMsg) << std::endl;
        sqlite3_free(zErrMsg);
        return (false);
    }
    std::cout << "Table created successfully" << std::endl;
    return (true);
}

bool Db::check_if_already_login(std::string username)
{
    int rc;
    sqlite3_stmt *stmt;
    std::string query_sql;

    query_sql = "SELECT Username, Status FROM Users";
    rc = sqlite3_prepare_v2(_db, query_sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        return (false);
    }
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        sqlite3_finalize(stmt);
        return (false);
    }
    if (rc == SQLITE_DONE) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        sqlite3_finalize(stmt);
        return (false);
    }

    while (rc != SQLITE_DONE)
    {
        if (username == std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))))
            if (std::string("0") == std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))))
                return (true);
            else
                return (false);
        rc = sqlite3_step(stmt);
    }
    return (false);
}

std::string Db::login(std::string username, std::string password, std::string ip)
{
    int rc;
    std::string sql;
    char *zErrMsg = 0;
    std::string db_username;
    std::string db_password;
    std::string return_getUser;
    size_t pos;

    if (check_if_already_login(username) == false)
        return (LOGIN_FAIL);

    return_getUser = getUser(username);
    pos = return_getUser.find(";");
    return_getUser.erase(0, pos + 1);
    pos = return_getUser.find(";");
    db_username = return_getUser.substr(0, pos);
    return_getUser.erase(0, pos + 1);
    pos = return_getUser.find(";");
    db_password = return_getUser.substr(0, pos);

    if (db_username != username || db_password != password) {
        return (LOGIN_FAIL);
    }

    sql = "UPDATE Users SET Status = 1, LastIP = '" + ip + "' WHERE Username = '" + username + "' AND Password = '" + password + "';";
    rc = sqlite3_exec(_db, sql.c_str(), callbackDb, 0, &zErrMsg);
   
    if( rc != SQLITE_OK ){
        std::cerr << "SQL error: " << std::string(zErrMsg) << std::endl;
        sqlite3_free(zErrMsg);
        return (LOGIN_SQL_ERROR);
    }
    std::cout << "User updated successfully" << std::endl;
    return (LOGIN_SUCCES);
}

std::string Db::logout(std::string username)
{
    int rc;
    std::string sql;
    char *zErrMsg = 0;
    std::string db_username;
    std::string return_getUser = getUser(username);
    size_t pos = return_getUser.find(";");

    return_getUser.erase(0, pos + 1);
    pos = return_getUser.find(";");
    db_username = return_getUser.substr(0, pos);

    if (db_username != username) {
        return (LOGOUT_FAIL);
    }

    sql = "UPDATE Users SET Status = 0 WHERE Username = '" + username + "';";
    rc = sqlite3_exec(_db, sql.c_str(), callbackDb, 0, &zErrMsg);
   
    if( rc != SQLITE_OK ){
        std::cerr << "SQL error: " << std::string(zErrMsg) << std::endl;
        sqlite3_free(zErrMsg);
        return (LOGOUT_SQL_ERROR);
    }
    std::cout << "User updated successfully" << std::endl;
    return (LOGOUT_SUCCES);
}

bool Db::all_logout()
{
    int rc;
    sqlite3_stmt *stmt;
    std::string query_sql;
    std::vector<std::string> all_username;
    char *zErrMsg = 0;

    if (check_if_empty_table("Users") == true) {
        std::cerr << "Table Users is empty" << std::endl;
        return (false);
    }

    query_sql = "UPDATE Users SET Status = 0, UserCall = '', Port = '';";
    rc = sqlite3_exec(_db, query_sql.c_str(), callbackDb, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        std::cerr << "SQL error: " << std::string(zErrMsg) << std::endl;
        sqlite3_free(zErrMsg);
        return (false);
    }
    std::cout << "All Users are logout and reset" << std::endl;
    return (true);
}

bool Db::acceptCall(std::string username) // quand une user accept un call, quand il a etais ciblée par un request call
{
    int rc;
    std::string sql;
    sqlite3_stmt *stmt;
    char *zErrMsg = 0;
    bool found = false;
    std::string port;
    std::string usercall;

    sql = "SELECT Username, Port, UserCall FROM Users";
    rc = sqlite3_prepare_v2(_db, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        return (false);
    }
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        sqlite3_finalize(stmt);
        return (false);
    }
    if (rc == SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return (false);
    }

    while (rc != SQLITE_DONE) {
        if (username == std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)))) {
            found = true;
            port = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            usercall = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        }
        rc = sqlite3_step(stmt);
    }
    
    if (found == true) {
        sql = "UPDATE Users SET Status = 2, UserCall = '" + usercall + "', Port = '" + port + "' WHERE Username = '" + username + "';";
        rc = sqlite3_exec(_db, sql.c_str(), callbackDb, 0, &zErrMsg);
        if( rc != SQLITE_OK ){
            std::cerr << "SQL error: " << std::string(zErrMsg) << std::endl;
            sqlite3_free(zErrMsg);
            return (false);
        }
    } else {
        return (false);
    }
    return (true);
}

bool Db::in_call(std::string username)
{
    int rc;
    std::string sql;
    char *zErrMsg = 0;

    // std::cout << "DEBUG in_call: argument (username) = " << username << std::endl;

    sql = "UPDATE Users SET Status = 2 WHERE Username = '" + username + "';";
    rc = sqlite3_exec(_db, sql.c_str(), callbackDb, 0, &zErrMsg);
   
    if( rc != SQLITE_OK ){
        std::cerr << "SQL error: " << std::string(zErrMsg) << std::endl;
        sqlite3_free(zErrMsg);
        return (false);
    }
    std::cout << "User updated successfully" << std::endl;
    return (true);
}

bool Db::not_in_call(std::string username) // endcall
{
    int rc;
    std::string sql;
    char *zErrMsg = 0;

    // std::cout << "DEBUG not_in_call: argument (username) = " << username << std::endl;

    sql = "UPDATE Users SET Status = 1, Port = '', UserCall = '' WHERE Username = '" + username + "';";
    rc = sqlite3_exec(_db, sql.c_str(), callbackDb, 0, &zErrMsg);
   
    if( rc != SQLITE_OK ){
        std::cerr << "SQL error: " << std::string(zErrMsg) << std::endl;
        sqlite3_free(zErrMsg);
        return (false);
    }
    std::cout << "User updated successfully" << std::endl;
    return (true);
}

std::string Db::getCallRequest()
{
    int rc;
    sqlite3_stmt *stmt;
    std::string query_sql;
    std::string return_value;

    query_sql = "SELECT Username, Status, Port, UserCall, LastIp FROM Users";
    rc = sqlite3_prepare_v2(_db, query_sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        return (GET_CALL_REQUEST_SQL_ERROR);
    }
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        sqlite3_finalize(stmt);
        return (GET_CALL_REQUEST_SQL_ERROR);
    }
    if (rc == SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return (GET_CALL_REQUEST_SQL_ERROR);
    }

    return_value.append(GET_CALL_REQUEST);

    while (rc != SQLITE_DONE) {
        if ("3" == std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)))) {
            return_value.append(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))) + " " +
                                std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))) + " " +
                                std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))) + ";");
        }
        rc = sqlite3_step(stmt);
    }
    if (return_value.compare(GET_CALL_REQUEST) == 0)
        return(GET_CALL_REQUEST_NO_CALL_INCOMING);
    return_value.pop_back();
    return_value.append("\n");
    return (return_value);
}

bool Db::requestCall(std::string username, std::string port, std::string usercall)
{
    int rc;
    std::string sql;
    char *zErrMsg = 0;

    // std::cout << "DEBUG not_in_call: argument (username) = " << username << std::endl;

    sql = "UPDATE Users SET Status = 3, UserCall = '" + usercall + "', Port = '" + port + "' WHERE Username = '" + username + "';";
    rc = sqlite3_exec(_db, sql.c_str(), callbackDb, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        std::cerr << "SQL error: " << std::string(zErrMsg) << std::endl;
        sqlite3_free(zErrMsg);
        return (false);
    }

    std::cout << "User updated successfully" << std::endl;
    return (true);
}

bool Db::verifDbUser(std::string username)
{
    int rc;
    sqlite3_stmt *stmt;
    std::string query_sql;

    if (check_if_empty_table("Users") == true) {
        std::cerr << "Table Users is empty" << std::endl;
        return (false);
    }

    query_sql = "SELECT Username FROM Users";
    rc = sqlite3_prepare_v2(_db, query_sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        return (false);
    }
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        sqlite3_finalize(stmt);
        return (false);
    }
    if (rc == SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return (false);
    }

    while (rc != SQLITE_DONE) {
        if (username == std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))))
            return (true);
        rc = sqlite3_step(stmt);
    }
    return (false);
}

bool Db::verifDbContacts(std::string Id, std::string Id_contact)
{
    int rc;
    sqlite3_stmt *stmt;
    std::string query_sql;

    if (check_if_empty_table("Contacts") == true) {
        std::cerr << "Table Contacts is empty" << std::endl;
        return (false);
    }

    query_sql = "SELECT Id, Id_contact FROM Contacts";
    rc = sqlite3_prepare_v2(_db, query_sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        return (false);
    }
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        sqlite3_finalize(stmt);
        return (false);
    }
    if (rc == SQLITE_DONE) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        sqlite3_finalize(stmt);
        return (false);
    }

    while (rc != SQLITE_DONE) {
        if (Id == std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))) &&
            Id_contact == std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))))
            return (true);
        rc = sqlite3_step(stmt);
    }
    return (false);
}

bool Db::verifDbRequestContacts(std::string Id, std::string Id_contact)
{
    int rc;
    sqlite3_stmt *stmt;
    std::string query_sql;

    if (check_if_empty_table("RequestContacts") == true) {
        std::cerr << "Table RequestContacts is empty" << std::endl;
        return (false);
    }

    query_sql = "SELECT Id, Id_contact FROM RequestContacts";
    rc = sqlite3_prepare_v2(_db, query_sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        return (false);
    }
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        sqlite3_finalize(stmt);
        return (false);
    }
    if (rc == SQLITE_DONE) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        sqlite3_finalize(stmt);
        return (false);
    }

    while (rc != SQLITE_DONE) {
        if (Id == std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))) && 
            Id_contact == std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))))
            return (true);
        rc = sqlite3_step(stmt);
    }
    return (false);
}

std::string Db::getStatus(std::string username)
{
    int rc;
    sqlite3_stmt *stmt;
    std::string query_sql;
    std::string return_value;

    if (check_if_empty_table("Users") == true) {
        std::cerr << "Table Users is empty" << std::endl;
        return (USER_TABLE_EMPTY);
    }

    query_sql = "SELECT Username, Status FROM Users";
    rc = sqlite3_prepare_v2(_db, query_sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        return (GET_STATUS_SQL_ERROR);
    }
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        sqlite3_finalize(stmt);
        return (GET_STATUS_SQL_ERROR);
    }
    if (rc == SQLITE_DONE) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        sqlite3_finalize(stmt);
        return (GET_STATUS_SQL_ERROR);
    }

    return_value.append(GET_STATUS);

    while (rc != SQLITE_DONE) {
        if (username == std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)))) {
            return_value.append(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))));
        }
        rc = sqlite3_step(stmt);
    }
    return_value.append("\n");
    return (return_value);
}

std::string Db::getFriendRequest(std::string username)
{
    int rc;
    sqlite3_stmt *stmt;
    std::string query_sql;
    std::vector<std::string> all_id_friends;
    std::string return_value;
    std::string Id = getUser(username).substr(0, getUser(username).find(std::string(";")));

    if (check_if_empty_table("RequestContacts") == true) {
        std::cerr << "Table RequestContacts is empty" << std::endl;
        return (REQUESTCONTACTS_TABLE_EMPTY);
    }

    query_sql = "SELECT Id, Id_contact FROM RequestContacts";
    rc = sqlite3_prepare_v2(_db, query_sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        return (GET_FRIEND_REQUEST_SQL_ERROR);
    }
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        sqlite3_finalize(stmt);
        return (GET_FRIEND_REQUEST_SQL_ERROR);
    }
    if (rc == SQLITE_DONE) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        sqlite3_finalize(stmt);
        return (GET_FRIEND_REQUEST_SQL_ERROR);
    }

    while (rc != SQLITE_DONE) {
        // if (Id == std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)))) {
        //     all_id_friends.push_back(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))));
        // }
        if (Id == std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)))) {
            all_id_friends.push_back(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))));
        }
        rc = sqlite3_step(stmt);
    }

    return_value.append(GET_FRIEND_REQUEST_SUCCES);

    for (std::vector<std::string>::iterator it = all_id_friends.begin(); it != all_id_friends.end(); it++)
        return_value.append(getUsername_by_id(*it) + ";");
    return_value.pop_back();
    return_value.append("\n");
    return (return_value);
}

std::string Db::getFriendConnected(std::string username)
{
    int rc;
    sqlite3_stmt *stmt;
    std::string query_sql;
    std::vector<std::string> all_id_friends;
    std::string return_value;
    std::string Id = getUser(username).substr(0, getUser(username).find(std::string(";")));

    if (check_if_empty_table("Contacts") == true) {
        std::cerr << "Table Contacts is empty" << std::endl;
        return (GET_ALL_FRIEND_NO_CONTACT);
    }

    query_sql = "SELECT Id, Id_contact FROM Contacts";
    rc = sqlite3_prepare_v2(_db, query_sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        return (GET_FRIEND_CONNECTED_SQL_ERROR);
    }
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        sqlite3_finalize(stmt);
        return (GET_FRIEND_CONNECTED_SQL_ERROR);
    }
    if (rc == SQLITE_DONE) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        sqlite3_finalize(stmt);
        return (GET_FRIEND_CONNECTED_SQL_ERROR);
    }

    while (rc != SQLITE_DONE) {
        if (Id == std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)))) {
            all_id_friends.push_back(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))));
        } if (Id == std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)))) {
            all_id_friends.push_back(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))));
        }
        rc = sqlite3_step(stmt);
    }

    return_value.append(GET_FRIEND_CONNECTED_SUCCES);

    for (std::vector<std::string>::iterator it = all_id_friends.begin(); it != all_id_friends.end(); it++)
        return_value.append(getUsername_by_id_if_connected(*it) + ";");
    return_value.pop_back();
    return_value.append("\n");
    return (return_value);
}

std::string Db::getAllFriend(std::string username)
{
    int rc;
    sqlite3_stmt *stmt;
    std::string query_sql;
    std::vector<std::string> all_id_friends;
    std::string return_value;
    std::string Id = getUser(username).substr(0, getUser(username).find(std::string(";")));

    if (check_if_empty_table("Contacts") == true) {
        std::cerr << "Table Contacts is empty" << std::endl;
        return (GET_ALL_FRIEND_NO_CONTACT);
    }

    query_sql = "SELECT Id, Id_contact FROM Contacts";
    rc = sqlite3_prepare_v2(_db, query_sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        return (GET_ALL_FRIEND_SQL_ERROR);
    }
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        sqlite3_finalize(stmt);
        return (GET_ALL_FRIEND_SQL_ERROR);
    }
    if (rc == SQLITE_DONE) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        sqlite3_finalize(stmt);
        return (GET_ALL_FRIEND_SQL_ERROR);
    }

    while (rc != SQLITE_DONE) {
        if (Id == std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)))) {
            all_id_friends.push_back(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))));
        } if (Id == std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)))) {
            all_id_friends.push_back(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))));
        }
        rc = sqlite3_step(stmt);
    }

    return_value.append(GET_ALL_FRIEND_SUCCES);

    for (std::vector<std::string>::iterator it = all_id_friends.begin(); it != all_id_friends.end(); it++)
        return_value.append(getUsername_and_status_by_id(*it) + ";");
    return_value.pop_back();
    return_value.append(" \n");
    return (return_value);
}

std::string Db::createUser(std::string username, std::string password, std::string ip)
{
    int rc;
    std::string sql;
    char *zErrMsg = 0;

    if (verifDbUser(username) == true) {
        std::cerr << "User already exist" << std::endl;
        return (CREATE_USER_USER_ALREADY_EXIST);
    }

    sql = "INSERT INTO Users ('Username', 'Password', 'Status', 'LastIp') VALUES ('" + username + "', '" + password + "', 1, '" + ip + "');";
    rc = sqlite3_exec(_db, sql.c_str(), callbackDb, 0, &zErrMsg);
   
    if( rc != SQLITE_OK ){
        std::cerr << "SQL error: " << std::string(zErrMsg) << std::endl;
        sqlite3_free(zErrMsg);
        return (CREATE_USER_SQL_ERROR);
    }
    std::cout << "User created successfully" << std::endl;
    return (CREATE_USER_SUCCES);
}

std::string Db::createContact(std::string username, std::string username_friends)
{
    int rc;
    std::string sql;
    char *zErrMsg = 0;
    std::string Id = getUser(username).substr(0, getUser(username).find(std::string(";")));
    std::string Id_contact = getUser(username_friends).substr(0, getUser(username_friends).find(std::string(";")));

    if (verifDbContacts(Id, Id_contact) == true || verifDbContacts(Id_contact, Id) == true) {
        std::cerr << "Contact already exist" << std::endl;
        return (CREATE_CONTACT_ALREADY_EXIST);
    }
    if (verifDbRequestContacts(Id, Id_contact) == true)
        deleteRequestContact_by_id(Id, Id_contact);
    else if (verifDbRequestContacts(Id_contact, Id) == true)
        deleteRequestContact_by_id(Id_contact, Id);
    else {
        std::cerr << "Not RequestContact in db" << std::endl;
        return (CREATE_CONTACT_NO_REQUEST_CONTACT);
    }
    
    sql = "INSERT INTO Contacts ('Id', 'Id_contact') VALUES ('" + Id + "', '" + Id_contact + "');";
    rc = sqlite3_exec(_db, sql.c_str(), callbackDb, 0, &zErrMsg);
   
    if( rc != SQLITE_OK ){
        std::cerr << "SQL error: " << std::string(zErrMsg) << std::endl;
        sqlite3_free(zErrMsg);
        return (CREATE_CONTACT_SQL_ERROR);
    }
    std::cout << "Contact created successfully" << std::endl;
    return (CREATE_CONTACT_SUCCES);
}

std::string Db::deleteRequestContact(std::string username, std::string username_friends)
{
    int rc;
    std::string sql;
    char *zErrMsg = 0;
    std::string Id = getUser(username).substr(0, getUser(username).find(std::string(";")));
    std::string Id_contact = getUser(username_friends).substr(0, getUser(username_friends).find(std::string(";")));

    if (std::isdigit(std::atoi(Id.c_str())) == true || std::isdigit(std::atoi(Id_contact.c_str())) == true) {
        std::cerr << "User not exist" << std::endl;
        return (DELETE_REQUEST_CONTACT_USER_NOT_EXIST);
    }

    if (verifDbRequestContacts(Id, Id_contact) == true)
        sql = "DELETE FROM RequestContacts WHERE Id=" + Id + " AND Id_contact=" + Id_contact + ";";
    else if (verifDbRequestContacts(Id_contact, Id) == true)
        sql = "DELETE FROM RequestContacts WHERE Id=" + Id_contact + " AND Id_contact=" + Id + ";";
    else {
        std::cerr << "Not RequestContact in db" << std::endl;
        return (DELETE_REQUEST_CONTACT_NO_REQUEST_CONTACT);
    }

    rc = sqlite3_exec(_db, sql.c_str(), callbackDb, 0, &zErrMsg);
   
    if( rc != SQLITE_OK ){
        std::cerr << "SQL error: " << std::string(zErrMsg) << std::endl;
        sqlite3_free(zErrMsg);
        return (DELETE_REQUEST_CONTACT_SQL_ERROR);
    }
    std::cout << "RequestContact deleted successfully" << std::endl;
    return (DELETE_REQUEST_CONTACT_SUCCES);
}


bool Db::deleteRequestContact_by_id(std::string Id, std::string Id_contact)
{
    int rc;
    std::string sql;
    char *zErrMsg = 0;

    if (verifDbRequestContacts(Id, Id_contact) == true)
        sql = "DELETE FROM RequestContacts WHERE Id=" + Id + " AND Id_contact=" + Id_contact + ";";
    else if (verifDbRequestContacts(Id_contact, Id) == true)
        sql = "DELETE FROM RequestContacts WHERE Id=" + Id_contact + " AND Id_contact=" + Id + ";";
    else {
        std::cerr << "Not RequestContact in db" << std::endl;
        return (false);
    }

    rc = sqlite3_exec(_db, sql.c_str(), callbackDb, 0, &zErrMsg);
   
    if( rc != SQLITE_OK ){
        std::cerr << "SQL error: " << std::string(zErrMsg) << std::endl;
        sqlite3_free(zErrMsg);
        return (false);
    }
    std::cout << "RequestContact deleted successfully" << std::endl;
    return (true);
}

std::string Db::createRequestContact(std::string username, std::string username_friends)
{
    int rc;
    std::string sql;
    char *zErrMsg = 0;
    std::string Id = getUser(username).substr(0, getUser(username).find(std::string(";")));
    std::string Id_contact = getUser(username_friends).substr(0, getUser(username_friends).find(std::string(";")));

    if (username == username_friends) {
        std::cerr << "Contact already exist" << std::endl;
        return (CREATE_REQUEST_CONTACT_CONTACT_ALREADY_EXIST);
    }

    if (Id.size() == 0 || Id_contact.size() == 0 ||
        std::isdigit(std::atoi(Id.c_str())) == true || std::isdigit(std::atoi(Id_contact.c_str())) == true) {
        std::cerr << "User not exist" << std::endl;
        return (CREATE_REQUEST_CONTACT_USER_NOT_EXIST);
    }
    
    if (verifDbRequestContacts(Id, Id_contact) == true || verifDbRequestContacts(Id_contact, Id) == true) {
        std::cerr << "RequestContacts already exist" << std::endl;
        return (CREATE_REQUEST_CONTACT_ALREADY_EXIST);
    }

    if (verifDbContacts(Id, Id_contact) == true || verifDbContacts(Id_contact, Id) == true) {
        std::cerr << "Contacts already exist" << std::endl;
        return (CREATE_REQUEST_CONTACT_CONTACT_ALREADY_EXIST);
    }

    sql = "INSERT INTO RequestContacts ('Id', 'Id_contact') VALUES ('" + Id + "', '" + Id_contact + "');";
    rc = sqlite3_exec(_db, sql.c_str(), callbackDb, 0, &zErrMsg);
   
    if( rc != SQLITE_OK ){
        std::cerr << "SQL error: " << std::string(zErrMsg) << std::endl;
        sqlite3_free(zErrMsg);
        return (CREATE_REQUEST_CONTACT_SQL_ERROR);
    }
    std::cout << "RequestContacts created successfully" << std::endl;
    return (CREATE_REQUEST_CONTACT_SUCCES);
}

std::string Db::deleteContact(std::string username, std::string username_friends)
{
    int rc;
    std::string return_value;
    std::string sql;
    char *zErrMsg = 0;
    std::string Id = getUser(username).substr(0, getUser(username).find(std::string(";")));
    std::string Id_contact = getUser(username_friends).substr(0, getUser(username_friends).find(std::string(";")));

    if (verifDbContacts(Id_contact, Id) == true) {
        std::string tmp = Id;
        Id = Id_contact;
        Id_contact = tmp;
        if (verifDbContacts(Id_contact, Id) == true)
            return (DELETE_CONTACT_NO_CONTACT);
    }

    sql = "DELETE FROM Contacts WHERE Id=" + Id + " AND Id_contact=" + Id_contact + ";";
    rc = sqlite3_exec(_db, sql.c_str(), callbackDb, 0, &zErrMsg);
   
    if( rc != SQLITE_OK ){
        std::cerr << "SQL error: " << std::string(zErrMsg) << std::endl;
        sqlite3_free(zErrMsg);
        return (DELETE_CONTACT_SQL_ERROR);
    }
    std::cout << "Contact deleted successfully" << std::endl;
    return (DELETE_CONTACT_SUCCES);
}

std::string Db::getAllUsers()
{
    int rc;
    sqlite3_stmt *stmt;
    std::string query_sql;
    std::string return_value;

    if (check_if_empty_table("Users") == true) {
        std::cerr << "Table Users is empty" << std::endl;
        return (return_value);
    }

    query_sql = "SELECT Id, Username, Password, Status FROM Users";
    rc = sqlite3_prepare_v2(_db, query_sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        return (return_value);
    }
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        sqlite3_finalize(stmt);
        return (return_value);
    }
    if (rc == SQLITE_DONE) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        sqlite3_finalize(stmt);
        return (return_value);
    }

    while (rc != SQLITE_DONE) {
        return_value.append(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))) + ";" +
                            std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))) + ";" +
                            std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))) + ";" +
                            std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))));
        rc = sqlite3_step(stmt);
    }
    return (return_value);
}

std::string Db::getUsers(std::string username)
{
    int rc;
    sqlite3_stmt *stmt;
    std::string query_sql;
    std::string return_value;

    if (check_if_empty_table("Users") == true) {
        std::cerr << "Table Users is empty" << std::endl;
        return (USER_TABLE_EMPTY);
    }

    query_sql = "SELECT Username FROM Users";
    rc = sqlite3_prepare_v2(_db, query_sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        return (GET_USERS_SQL_ERROR);
    }
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        sqlite3_finalize(stmt);
        return (GET_USERS_SQL_ERROR);
    }
    if (rc == SQLITE_DONE) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        sqlite3_finalize(stmt);
        return (GET_USERS_SQL_ERROR);
    }

    return_value.append(GET_USERS_SUCCES);

    while (rc != SQLITE_DONE) {
        if (username.compare(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)))) <= 0)
            return_value.append(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))) + ";");
        // if (std::strcmp(username.c_str(), std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))).c_str()) <= 0) { // Bah déjà faudrait du coup faire un strncmp et même tu fais juste avec des substr tu peux très bien le faire fonctionner comme ca c plus propre et c mieux que dutiliser des char *
        //     return_value.append(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))) + ";");
        // }
        rc = sqlite3_step(stmt);
    }
    return_value.pop_back();
    return_value.append("\n");
    return (return_value);
}

std::string Db::getUser(std::string username)
{
    int rc;
    sqlite3_stmt *stmt;
    std::string query_sql;
    std::string return_value;

    if (check_if_empty_table("Users") == true) {
        std::cerr << "Table Users is empty" << std::endl;
        return (return_value);
    }

    query_sql = "SELECT Id, Username, Password, Status FROM Users";
    rc = sqlite3_prepare_v2(_db, query_sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        return (return_value);
    }
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        sqlite3_finalize(stmt);
        return (return_value);
    }
    if (rc == SQLITE_DONE) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        sqlite3_finalize(stmt);
        return (return_value);
    }

    while (rc != SQLITE_DONE) {
        if (username == std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)))) {
            return_value.append(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))) + ";" +
                                std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))) + ";" +
                                std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))) + ";" +
                                std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))));
        }
        rc = sqlite3_step(stmt);
    }
    return (return_value);
}

bool Db::check_if_empty_table(std::string table)
{
    int rc;
    sqlite3_stmt *stmt;
    std::string query_sql;

    query_sql = "SELECT count(*) FROM " + table;
    rc = sqlite3_prepare_v2(_db, query_sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        return (false);
    }
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        sqlite3_finalize(stmt);
        return (false);
    }
    if (rc == SQLITE_DONE) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        sqlite3_finalize(stmt);
        return (false);
    }

    while (rc != SQLITE_DONE)
    {
        if ("0" == std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))))
            return (true);
        rc = sqlite3_step(stmt);
    }
    return (false);
}

std::string Db::getUsername_by_id_if_connected(std::string Id)
{
    int rc;
    sqlite3_stmt *stmt;
    std::string query_sql;
    std::string return_value;

    if (check_if_empty_table("Users") == true) {
        std::cerr << "Table Users is empty" << std::endl;
        return (return_value);
    }

    query_sql = "SELECT Id, Username, Password, Status FROM Users";
    rc = sqlite3_prepare_v2(_db, query_sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        return (return_value);
    }
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        sqlite3_finalize(stmt);
        return (return_value);
    }
    if (rc == SQLITE_DONE) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        sqlite3_finalize(stmt);
        return (return_value);
    }

    while (rc != SQLITE_DONE) {
        if (Id == std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))) && "1" == std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)))) {
            return_value.append(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))));
        }
        rc = sqlite3_step(stmt);
    }
    return (return_value);
}

std::string Db::getUsername_and_status_by_id(std::string Id)
{
    int rc;
    sqlite3_stmt *stmt;
    std::string query_sql;
    std::string return_value;

    if (check_if_empty_table("Users") == true) {
        std::cerr << "Table Users is empty" << std::endl;
        return (return_value);
    }

    query_sql = "SELECT Id, Username, Status FROM Users";
    rc = sqlite3_prepare_v2(_db, query_sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        return (return_value);
    }
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        sqlite3_finalize(stmt);
        return (return_value);
    }
    if (rc == SQLITE_DONE) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        sqlite3_finalize(stmt);
        return (return_value);
    }

    while (rc != SQLITE_DONE) {
        if (Id == std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)))) {
            return_value.append(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))) + ";" +
                                std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))));
        }
        rc = sqlite3_step(stmt);
    }
    return (return_value);
}

std::string Db::getUsername_by_id(std::string Id)
{
    int rc;
    sqlite3_stmt *stmt;
    std::string query_sql;
    std::string return_value;

    if (check_if_empty_table("Users") == true) {
        std::cerr << "Table Users is empty" << std::endl;
        return (return_value);
    }

    query_sql = "SELECT Id, Username FROM Users";
    rc = sqlite3_prepare_v2(_db, query_sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        return (return_value);
    }
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        sqlite3_finalize(stmt);
        return (return_value);
    }
    if (rc == SQLITE_DONE) {
        std::cerr << "SQL error: " << std::string(sqlite3_errmsg(_db)) << std::endl;
        sqlite3_finalize(stmt);
        return (return_value);
    }

    while (rc != SQLITE_DONE) {
        if (Id == std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)))) {
            return_value.append(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))));
        }
        rc = sqlite3_step(stmt);
    }
    return (return_value);
}

void Db::seeUsersTable()
{
    int rc;
    std::string sql;
    char *zErrMsg = 0;
    sql = "SELECT * FROM Users";
    rc = sqlite3_exec(_db, sql.c_str(), callbackDb, 0, &zErrMsg);
   
    if( rc != SQLITE_OK ){
        std::cerr << "SQL error: " << std::string(zErrMsg) << std::endl;
        sqlite3_free(zErrMsg);
        return;
    }
    return;
}

void Db::seeRequestContactsTable()
{
    int rc;
    std::string sql;
    char *zErrMsg = 0;
    sql = "SELECT * FROM RequestContacts";
    rc = sqlite3_exec(_db, sql.c_str(), callbackDb, 0, &zErrMsg);
   
    if( rc != SQLITE_OK ){
        std::cerr << "SQL error: " << std::string(zErrMsg) << std::endl;
        sqlite3_free(zErrMsg);
        return;
    }
    return;
}

void Db::seeContactsTable()
{
    int rc;
    std::string sql;
    char *zErrMsg = 0;
    sql = "SELECT * FROM Contacts";
    rc = sqlite3_exec(_db, sql.c_str(), callbackDb, 0, &zErrMsg);
   
    if( rc != SQLITE_OK ){
        std::cerr << "SQL error: " << std::string(zErrMsg) << std::endl;
        sqlite3_free(zErrMsg);
        return;
    }
    return;
}

static int callbackDb(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i;
    for(i = 0; i<argc; i++) {
        std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL");
    }
    std::cout << std::endl;
    return 0;
}