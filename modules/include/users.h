#ifndef USERS_H
#define USERS_H

#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <sstream>

// Clase
class User {
    private:
        std::string user_name;
        std::string user_password;
        std::string user_role;
    public:
        User();
        User(std::string userName, std::string userPassword, std::string userRole);
        std::string toString();
        std::string getUserName();
        std::string getUserRole();
        std::string getUserPassword();
        bool confirmPassword(std::string password);
};

// Funciones
bool logIn(std::string password, std::vector<User> &users, const int index);
std::vector<User> getUsers();
int getUserNameIndex(std::string userName, std::vector<User> &users);
bool writeUserstxt(std::vector<User> users);

#endif // USERS_H
