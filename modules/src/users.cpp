#include "users.h"

// Constructor de la clase, crea un usuario sin ningun campo
User::User(){
    this->user_name = "";
    this->user_password = "";
    this->user_role = "";
}

// Constructor sobrecargado con el nombre, contrasenia y rol de usuario
User::User(std::string userName, std::string userPassword, std::string userRole){
    this->user_name = userName;
    this->user_password = userPassword;
    this->user_role = userRole;
}

// Confirma la contrasenia ingresada con la contrasenia del usuario
bool User::confirmPassword(std::string password){
    return (password == this->user_password);
}

// Obtener el nombre del usuario
std::string User::getUserName(){
    return this->user_name;
}

// Obtener el rol del usuario
std::string User::getUserRole(){
    return this->user_role;
}

// Obtener la clave del usuario
std::string User::getUserPassword(){
    return this->user_password;
}

// Transformacion del objeto en string
std::string User::toString(){
    return this->user_name;
}

// SobreEscribe el archivo .users con todos los usuarios del vector "users"
bool writeUserstxt(std::vector<User> users){
    std::fstream usersFile(getenv("USER_PATH"), std::ios::out);

    if (!usersFile){
        return false;
    }

    for (int i = 0; i < static_cast<int>(users.size()); ++i){
        usersFile << users.at(i).getUserName() << ";" <<
            users.at(i).getUserPassword() << ";" <<
            users.at(i).getUserRole();

        if (i < static_cast<int>(users.size())-1)
            usersFile << "\n";
    }

    usersFile.close();
    return true;
}

// Obtiene todos los usuarios registrados y devuelve un vector con ellos.
std::vector<User> getUsers(){

    // Abrir el archivo desde USER_PATH
    std::fstream usersFile(getenv("USER_PATH"));

    if (!usersFile.is_open()){
        printf("No se encontro la ruta de usuarios\n");
        exit(EXIT_FAILURE);
    }

    // Trabajamos con el archivo .txt para obtener los usuarios.
    std::string line;
    std::vector<User> users;
    std::string userName, userPassword, userRole;

    while (std::getline(usersFile, line)){
        std::stringstream ss(line);
        std::getline(ss, userName, ';');
        std::getline(ss, userPassword, ';');
        std::getline(ss, userRole, ';');

        users.push_back(User(userName, userPassword, userRole));
    }

    // Retorno el vector resultante.
    return users;
}

/* 
    Recibe el vector con los usuarios y verifica si existe el usuario.
    Si existe retorna su indice en el vector, si no, retorna -1
*/
int getUserNameIndex(std::string userName, std::vector<User> &users){
    int index = 0;

    for(User item : users){
        if (item.getUserName() == userName)
            return index;
        index++;
    }

    return -1;
}

// Funcion que verifica la clave una vez es ingresado el usuario.
bool logIn(std::string password, std::vector<User> &users, const int index){
    if (users.at(index).confirmPassword(password))
        return true;
    return false;
}