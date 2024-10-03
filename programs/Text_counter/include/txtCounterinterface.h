#ifndef TXTCOUNTERINTERFACE_H
#define TXTCOUNTERINTERFACE_H

#include "exeptions.h"
#include "users.h"
#include "stringManagment.h"
#include "utils.h"

#include <ncurses.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <filesystem>

#define INPUT 100 // Cantidad de letras permitidas en el input

class Interface {
    private:
        WINDOW* input_win;
        WINDOW* header_win;
        WINDOW* output_win;
        WINDOW* menu_win;

        std::string fileType;
        std::string pathToProcess;
        std::string pathToOutput;

        std::vector<void(Interface::*)()> options;
        std::vector<std::string> optionsStrings;

        Interface();
        // Eliminamos el constructor de copia
        Interface(const Interface&) = delete;
        // Eliminamos el operador de asignacion
        Interface& operator=(const Interface&) = delete;

        void setOptions();

    public:
        // Metodo para obtener una instancia unica
        static Interface& getInstance();
        // Metodos de ventana y opciones
        void interfaceInputOutput();
        void showMessageOutput(std::string message);
        void setFileProcessPath();
        void setFileOutputPath();
        void setFileType();
        void process();
        void updateMenu();

        void interfaceExit();
};

#endif // TXTCOUNTERINTERFACE_H
