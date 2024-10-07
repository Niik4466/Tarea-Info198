#ifndef INTERFACE_H
#define INTERFACE_H

#include "exeptions.h"
#include "users.h"
#include "stringManagment.h"
#include "utils.h"

#include <ncurses.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <unistd.h>

class Interface {
    private:
        WINDOW* output_win;
        WINDOW* input_win;
        WINDOW* header_win;
        WINDOW* menu_win;
        std::string phrase;
        std::string num;
        std::vector<int> list;
        std::vector<void(Interface::*)()> options;
        std::vector<std::string> optionsStrings;
        User user;
        std::vector<User> users;


        Interface(std::string &phrase, std::vector<int> &list, std::string &num, User user, std::vector<User> &users);
        // Eliminamos el constructor de copia
        Interface() = delete;
        Interface(const Interface&) = delete;
        // Eliminamos el operador de asignacion
        Interface& operator=(const Interface&) = delete;

        void setOptions(User user, std::vector<User> users);
        void setOptions(User user);

    public:
        // Metodo para obtener una instancia unica
        static Interface& getInstance(std::string &phrase, std::vector<int> &list, std::string &num, User user, std::vector<User> users);
        static Interface& getInstance(std::string &phrase, std::vector<int> &list, std::string &num, User user);
        // Metodos de ventana y opciones
        void interfaceInputOutput();
        void interfaceMenu();
        void showMessageOutput(std::string message);

        void interfaceExit();
        void usrPalindromes();
        void usrVocaleCount();
        void usrLettersCount();
        void usrVectorSumProm();
        void usrFunctionCalc();
        void wordsTxtCount();
        void parallelCountWithThreads();

        void insertUsers();
        void listUsers();
        void deleteUsers();
};

#endif // INTERFACE_H
