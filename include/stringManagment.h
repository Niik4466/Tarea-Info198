#ifndef STRINGMANAGMENT_H
#define STRINGMANAGMENT_H

#include <string>
#include <vector>
#include <regex>

// Declaracion de funciones
std::vector<int> stringToVector(std::string list, std::string sep);
int stringToInt(std::string chain);
bool isFloat(std::string chain);
bool isInt(std::string chain);
float stringToFloat(std::string chain);
bool isPalindrome(std::string chain);
int countVocals(std::string chain);
int countAlph(std::string chain);
bool isTooBigFloat(std::string chain);
bool isTooBigInt(std::string chain);

#endif // STRINGMANAGMENT_H