#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>
#include <regex>
#include <filesystem>

//Funciones
void verifyUserIndex(const int &userIndex);
void verifyPassword(bool result);
void verifyString(bool isEmpty);
void verifyVector(std::string list, std::string sep);
void verifyNumType(bool resultado);
bool verifyNonCero(int num);
bool verifyNonCero(float num);
bool verifyPassFormat(std::string password);
bool verifyUserFormat(std::string userName);
bool isAlpha(std::string phrase);
bool verifyPath(std::string path);


#endif // EXCEPTIONS_H