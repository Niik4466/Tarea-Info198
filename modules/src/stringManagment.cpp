#include "stringManagment.h"

// Transforma un string de la forma Int:Int:Int:Int a un vector<Int>
std::vector<int> stringToVector(std::string list, std::string sep){
    std::regex separator(sep);

    std::vector<int> number_list;

    std::sregex_token_iterator begin(list.begin(), list.end(), separator, -1);
    std::sregex_token_iterator end;

    for (std::sregex_token_iterator iter = begin; iter != end; ++iter){
        number_list.push_back(stoi(*iter));
    }

    return number_list;
}


// Retorna True si el string chain es demasiado grande para ser un entero
bool isTooBigInt(std::string chain){
    try{
        stoi(chain);
        return false;
    }
    catch(std::out_of_range& e){
        return true;
    }
}

// Retorna True si el string chain es demasiado grande para ser un flotante
bool isTooBigFloat(std::string chain){
    try{
        stof(chain);
        return false;
    }
    catch(std::out_of_range& e){
        return true;
    }
}

// Transforma un string a un int
int stringToInt(std::string chain){
    try{
        return stoi(chain);
    }
    catch(std::out_of_range& e){
        printf("El numero ingresado es demasiado grande!!\n");
        exit(EXIT_FAILURE);
    }
}

// Transforma un string a un flotante
float stringToFloat(std::string chain){
    try{
        return stof(chain);
    }
    catch (std::out_of_range& e){
        printf("El numero ingresado es demasiado grande!!\n");
        exit(EXIT_FAILURE);
    }
}

// Retorna True si un string dado es flotante, false si no
bool isFloat(std::string chain){
    std::regex float_regex(R"(^-?\d+\.\d+$)");
    return std::regex_match(chain, float_regex);
}

// Retorna True si el string ingresado es un Int, false si no
bool isInt(std::string chain){
    std::regex int_regex(R"(^-?\d+$)");
    return std::regex_match(chain, int_regex);
}

// Retorna True si el String ingresado es palindrome, false si no.
bool isPalindrome(std::string chain){
    int izq = 0;
    int der = chain.size()-1;

    char char_izq;
    char char_der;

    for (; izq != static_cast<int>(chain.size()/2); ++izq, --der){
        char_izq = chain.at(izq);
        char_der = chain.at(der);

        if (isalpha(char_izq) && isupper(char_izq))
            char_izq = tolower(char_izq);

        if (isalpha(char_der) && isupper(char_der))
            char_der = tolower(char_der);

        if (char_izq != char_der)
            return false;
    }

    return true;
}

// Cuenta la cantidad de voclaes en una palabra y lo retorna
int countVocals(std::string chain){
    int count = 0;
    std::string vocals = "aeiouAEIOU";
    
    for (const char character : chain){
        if (vocals.find(character) != std::string::npos)
            ++count;
    }

    return count;
}

// Cuenta la cantidad de caracteres alfabeticos que tiene un string, retorna la cuenta.
int countAlph(std::string chain){
    int count = 0;
    for (char character : chain){
        if (std::isalpha(character))
            ++count;
    }

    return count;
}