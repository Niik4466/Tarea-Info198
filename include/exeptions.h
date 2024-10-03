#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>

//Funciones
void verifyUserIndex(const int &userIndex);
void verifyPassword(bool result);
void verifyString(bool isEmpty);
void verifyVector(std::string list, std::string sep);
void verifyNumType(bool resultado);
bool verifyNonCero(int num);
bool verifyNonCero(float num);

#endif // EXCEPTIONS_H