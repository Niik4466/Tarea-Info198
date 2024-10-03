#include "exeptions.h"

// Verifica que la id obtenida del usuario sea distinto de -1
void verifyUserIndex(const int &userIndex){
    if (userIndex != -1)
        return;
    printf("Usuario o clave incorrecta\n");
    exit(EXIT_FAILURE);
}

// Verifica si el resultado del logIn sea verdadero.
void verifyPassword(bool result){
    if (result)
        return;
    printf("Usuario o clave incorrecta\n");
    exit(EXIT_FAILURE);
}

// Verifica que la palabra entregada no sea vacia
void verifyString(bool isEmpty){
    if (isEmpty){
        printf("No se ingreso una palabra!\n");
        exit(EXIT_FAILURE);
    }
}

// Verifica que el vector este en el formato correcto. numeros enteros separados por un caracter
void verifyVector(std::string list, std::string sep){
    // Defino mi lenguaje
    // En este caso refiere a todos los numeros posibles y un char repetido las veces necesarias terminando con un solo numero. 
    std::regex language("^\\d+(" + sep + "\\d+)*$");

    if (std::regex_match(list, language))
        return;
    printf("El vector ingresado no coincide con el patron requerido\n");
    exit(EXIT_FAILURE);
}

// Dado un resultado, imprime un mensaje y sale del programa.
void verifyNumType(bool resultado){
    if (resultado)
        return;
    printf("La opcion ingresada debe ser numerica!\n");
    exit(EXIT_FAILURE);
}

// Verifica si el int entregado es 0
bool verifyNonCero(int num){
    if (num != 0)
        return true;
    
    return false;
}

// Verifica si el float entregado es 0
bool verifyNonCero(float num){
    if (num != 0.0)
        return true;
    
    return false;
}

// Verifica que el nombre de usuario este en el formato correcto. (3 caracteres Min. Solo caracteres alfabeticos)
bool verifyUserFormat(std::string userName){
    if (userName.size() < 3){
        return false;
    }

    for (char &character : userName){
        if (!isalpha(character)){
            return false;
        }
    }
    return true;
}

// Verifica que la clave este en el formato correcto. (6 caracteres Min. tanto numericos como alfabeticos)
bool verifyPassFormat(std::string password){
    if (password.size() < 6){
        return false;
    }

    for (char &character : password){
        if (!isalnum(character)){
            return false;
        }
    }
    return true;
}

// Verifica si un string es alfabetico
bool isAlpha(std::string phrase){
    for (char &c : phrase){
        if (isdigit(c))
            return false;
    }
    return true;
}

// Verifica si existe la ruta a la carpeta
bool verifyPath(std::string path){
    return std::filesystem::exists(path);
}