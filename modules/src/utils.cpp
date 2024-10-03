#include "utils.h"


// Carga y setea las variables de entorno definidas en .env
void setEnv(){
    std::ifstream envFile(".env");

    if (!envFile.is_open()){
        printf("No se encontraron variables de entorno\n");
        return;
    }

    size_t pos;
    std::string line;
    std::string variableName;
    std::string variableValue;

    while(std::getline(envFile, line)){
        if (line.empty() || line.at(0) == '#') continue; //ignoramos las lineas vacias y comentarios
    
        pos = line.find("="); // Encontramos el caracter '='
        if (pos == std::string::npos) continue; // Si no encuentra el caracter continua a la siguiente linea

        variableName = line.substr(0, pos);
        variableValue = line.substr(pos+1);
        
        // Seteamos la variable de entorno
        setenv(variableName.c_str(), variableValue.c_str(), 1);
    }
}

// Ejecuta un proceso hijo y guarda lo que imprima este en el buffer, retornandolo
std::string executeWithBuffer(std::string call){
    

    char * buffer = new char[256];
    std::string result;
    FILE* externalProgram = popen(call.c_str(), "r");

    if (externalProgram == nullptr){
        return "Programa no encontrado";
    }

    while(fgets(buffer, 256, externalProgram)){
        result += buffer;
    }

    delete[] buffer;

    pclose(externalProgram);

    return result;
}

// Lee un archivo y lo almacena en un string
std::string readFile(std::string path){
    std::ifstream file(path);
    std::string result;

    if (!file.is_open())
        return result;

    std::string line;

    while (std::getline(file, line)){
        result += line;
        result += "\n";
    }

    return result;
}