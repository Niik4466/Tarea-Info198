#include "exeptions.h"
#include "users.h"
#include "stringManagment.h"
#include "utils.h"
#include "interface.h"
#include <cstdlib>

int main (int argc, char** argv){
    setEnv(); //seteamos las variables de entorno    

    // Eliminamos los puertos 2020 y 2021 si estan abiertos
    //system("fuser -k '2020/tcp' && fuser -k '2021/tcp'");

    // Iniciar motor.sh
    std::string invertedIndexPath = getenv("INVERTED_INDEX") ? getenv("INVERTED_INDEX") : "";
    std::string topk = getenv("TOPK") ? getenv("TOPK") : "";
    std::string motor_path = getenv("MOTOR_DE_BUSQUEDA_PATH") ? getenv("MOTOR_DE_BUSQUEDA_PATH") : "";
    if (invertedIndexPath.empty()) {
        printf("No se encontró la ruta del índice invertido\n");
        exit(EXIT_FAILURE);
    }

    std::string motorCommand = motor_path + "/motor.sh \"" + invertedIndexPath +"/.txt.INDEX" + "\" " + topk + " & > logsMotor.txt";
    system(motorCommand.c_str());

    sleep(1);

    // Iniciar cache
    std::string memorySize = getenv("MEMORY_SIZE") ? getenv("MEMORY_SIZE") : "";
    std::string cache_path = getenv("CACHE_PATH") ? getenv("CACHE_PATH") : "";
    if (memorySize.empty()) {
        printf("No se encontró la variable de entorno MEMORY_SIZE\n");
        exit(EXIT_FAILURE);
    }

    std::string cacheCommand = cache_path + "/cache " + memorySize + " & > logsCache.txt";
    system(cacheCommand.c_str());

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