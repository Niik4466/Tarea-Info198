#include "exeptions.h"
#include "users.h"
#include "stringManagment.h"
#include "utils.h"
#include "interface.h"

int main (int argc, char** argv){

    setEnv(); //seteamos las variables de entorno    

    std::string phrase;
    std::vector<int> list;
    std::string num;
    int userIndex = -1;
    std::vector<User> users = getUsers();
    
    {
    
    int option;
    std::string password;
    std::string userName;

    while ((option = getopt(argc, argv, "u:p:t:n:v:")) != -1){
        switch (option){
            case 'u':
                userName = optarg ? optarg : "";
                verifyUserFormat(userName);
                userIndex = getUserNameIndex(userName, users);
                verifyUserIndex(userIndex);
                break;
            case 'p':
                password = optarg ? optarg : "";
                verifyPassFormat(password);
                break;
            case 't':
                //printf("Frase Ingresada\n");
                phrase = optarg ? optarg : "";
                break;
            case 'v':
                //printf("Vector Ingresado\n");
                verifyVector(optarg, getenv("SEP"));
                list = stringToVector(optarg, getenv("SEP"));
                break;
            case 'n':
                //printf("Numero Ingresado\n");
                verifyNumType(isInt(optarg) || isFloat(optarg));
                num = optarg;
                break;
        }
    }

    if (userName.empty()){
        printf("No se ingreso nombre de Usuario");
        exit(EXIT_FAILURE);
    }
    if (password.empty()){
        printf("No se ingreso una clave");
        exit(EXIT_FAILURE);
    }

    verifyPassword(logIn(password, users, userIndex));
    printf("Bienvenido %s\n", userName.c_str());

    }

    User user = users.at(userIndex);

    printf("Inciando...\n");

    if (user.getUserRole() == "Admin"){
        Interface& interface = Interface::getInstance(phrase, list, num, user, users);
        interface.interfaceInputOutput();
    }
    else{
        Interface& interface = Interface::getInstance(phrase, list, num, user);
        interface.interfaceInputOutput();
    }

    return EXIT_SUCCESS;
}