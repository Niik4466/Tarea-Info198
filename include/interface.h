#ifndef INTERFACE_H
#define INTERFACE_H

#include "exeptions.h"
#include "users.h"
#include "stringManagment.h"

#include <ncurses.h>
#include <cstring>
#include <string>
#include <cstdlib>
#include <vector>
#include <vector>
#include <string>
#include <cstdlib>

// Funciones
void interfaceInputOutput(int height, int width, std::string num, std::string phrase, std::vector<int> list);
int interfaceMenu(std::string num, std::string phrase, std::vector<int> list);
void option1(std::string phrase, WINDOW* output_win);
void option2(std::string phrase, WINDOW* output_win);
void option3(std::string phrase, WINDOW* output_win);
void option4(std::vector<int> list, WINDOW* output_win);
void option5(std::string str_num, WINDOW* output_win);
void showMessage(WINDOW* win, std::string message);

#endif // INTERFACE_H
