#ifndef USERS_H
#define USERS_H

#include <string>
#include <vector>

// Struct
struct user {
    std::string user_name;
    std::string user_password;
    std::string user_role;
};

// Funciones
std::vector<user> getUsersJson();
int getUserNameIndex(std::string userName, std::vector<user> &users);
bool logIn(std::string password, std::vector<user> &users, const int index);

#endif // USERS_H
