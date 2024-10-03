#ifndef UTILS_H
#define UTILS_H

#include <map>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <ncurses.h>
#include <cstring>
#include <fstream>

// Declaracion de funciones
void setEnv();
std::string executeWithBuffer(std::string call);
std::string readFile(std::string path);
#endif // UTILS_H