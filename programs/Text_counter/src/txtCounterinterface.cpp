#include "txtCounterinterface.h"


// Constructor unico de la interfaz
Interface::Interface(){
    // Iniciamos ncurses

    initscr();      // Inicializar ncurses
    cbreak();       // Desactiva el buffer de linea

    // Seteamos las variables height y width usadas para dar tamanio a las ventanas
    fileType.clear();
    pathToProcess.clear();
    pathToOutput.clear();

    int height, width;
    getmaxyx(stdscr, height, width);

    {
    std::string heightEnv = getenv("HEIGHT") ? getenv("HEIGHT") : " ";
    std::string widthEnv = getenv("WIDTH") ? getenv("WIDTH") : " ";

    if (isInt(heightEnv))
        height = stringToInt(heightEnv);
    if (isInt(widthEnv))
        width = stringToInt(widthEnv);
    }

    // auxiliar para imprimir mensajes
    char message[100];

    // Creamos la ventanita del header
    header_win = newwin(height/4, width, 0, 0);

    sprintf(message, "Programa contador de palabras");
    mvwprintw(header_win, 1, 1, message);
    mvwprintw(header_win, 2, 1, "###############################");
    sprintf(message, "pid: %i", getpid());
    mvwprintw(header_win, 3, 1, message);    
    wrefresh(header_win);

    curs_set(1); // Muestra el puntero del teclado en la cajita de comandos

    // Crear ventanas para el menú y para la entrada de comandos
    menu_win = newwin(height / 4, width, height * 2/4, 0);

    // Creamos las opciones del menu
    setOptions();

    // Despliega las opciones en el menu
    updateMenu();

    wrefresh (menu_win); // refrescar el menu

    // Creamos la ventanita de output de comandos
    output_win = newwin(height/4, width, height * 1/4, 0);
    wrefresh(output_win);

    // Creamos la ventana para el input de comandos
    input_win = newwin(height/4, width, height * 3/4, 0);
    showMessageOutput("Seleccione la opcion: ");
    wrefresh(input_win);
}


Interface& Interface::getInstance(){
    static Interface interface;

    return interface;
}

// Setea todas las opciones de nuestro programa
void Interface::setOptions(){
    char message[100];
    int i = 0;

    // Generamos nuestro vector de opciones
    this->options.push_back(&Interface::interfaceExit);
    sprintf(message, "(%i) Salir", i);
    this->optionsStrings.push_back(message);
    i++;
    
    this->options.push_back(&Interface::setFileType);
    sprintf(message, "(%i) extension de archivos a procesar(ej: txt): ", i);
    this->optionsStrings.push_back(message);
    i++;

    this->options.push_back(&Interface::setFileProcessPath);
    sprintf(message, "(%i) path de carpeta a procesar (eje: /home/lvc/in):", i);
    this->optionsStrings.push_back(message);
    i++;

    this->options.push_back(&Interface::setFileOutputPath);
    sprintf(message, "(%i) path de carpeta que contendra la respuesta del proceso (eje: /home/lvc/out): ", i);
    this->optionsStrings.push_back(message);
    i++;

    this->options.push_back(&Interface::process);
    sprintf(message, "(%i) procesar", i);
    this->optionsStrings.push_back(message);
}

// Funcion principal del programa, se encarga de manejar los comandos que ingresa el usuario y los output de estos
void Interface::interfaceInputOutput(){
    char buffer[10];
    int option = -1;
    while (true) {
        // Mostrar el prompt
        wclear(input_win);  // Limpiar la ventana de comandos
        box(input_win, 0, 0);  // Dibujar el borde de la ventana
        wrefresh(input_win);  // Refrescar la ventana de comandos para mostrar el prompt
        mvwprintw(input_win, 1, 1, " ");
        wgetnstr(input_win, buffer, 3);

        if (isInt(buffer)){
            option = stringToInt(buffer);
            if (option < static_cast<int>(options.size()) && option >= 0){
                (this->*options.at(option))();
                continue;
            }
        }

        showMessageOutput("Comando no reconocido\n seleccione la opcion: ");
    }
}

// Sale de la interfaz y del programa
void Interface::interfaceExit(){
    endwin();
    printf("Programa terminado\n");
    exit(EXIT_SUCCESS);
}

// Pide el tipo de archivo que va a procesar
void Interface::setFileType(){
    char buffer[INPUT];
    showMessageOutput("Ingrese la extension de archivos a procesar");
    
    {
        wclear(input_win);  // Limpiar la ventana de comandos
        box(input_win, 0, 0);  // Dibujar el borde de la ventana
        mvwprintw(input_win, 1, 1, " ");
        wrefresh(input_win);  // Refrescar la ventana de comandos para mostrar el prompt

        wgetnstr(input_win, buffer, INPUT);
        
        fileType = buffer;
    }

    showMessageOutput("Extension registrada con exito!\n ingrese la opcion:");

    updateMenu();

}

// Refresca el menu y aniade opciones nuevas
void Interface::updateMenu(){

    char message[256];
    wclear(menu_win);
    
    sprintf(message, "(%i) extension de archivos a procesar(ej: txt): %s", 1, fileType.c_str());
    optionsStrings[1] = message;
    sprintf(message, "(%i) path de carpeta a procesar (eje: /home/lvc/in): %s", 2, pathToProcess.c_str());
    optionsStrings[2] = message;
    sprintf(message, "(%i) path de carpeta que contendra la respuesta del proceso (eje: /home/lvc/out): %s", 3, pathToOutput.c_str());
    optionsStrings[3] = message;
    
    for (int i = 0; i < static_cast<int>(options.size()); i++){
        mvwprintw(menu_win, i+2, 1, optionsStrings.at(i).c_str());
    }

    wrefresh(menu_win);
}

// Pide la informacion de la ruta donde estan los archivos a leer
void Interface::setFileProcessPath(){
    char buffer[INPUT];
    showMessageOutput("Ingrese la ruta de archivos a procesar");
    
    {
        wclear(input_win);  // Limpiar la ventana de comandos
        box(input_win, 0, 0);  // Dibujar el borde de la ventana
        mvwprintw(input_win, 1, 1, " ");
        wrefresh(input_win);  // Refrescar la ventana de comandos para mostrar el prompt

        wgetnstr(input_win, buffer, INPUT);
        
        pathToProcess = buffer;
    }

    if (!verifyPath(pathToProcess)){
        showMessageOutput("La ruta especificada no existe...\n seleccione la opcion:");
        pathToProcess.clear();
        return;
    }

    showMessageOutput("Ruta guardada con exito!\n seleccione la opcion:");

    updateMenu();
}

// Pide la informacion de la ruta donde se guardaran los archivos que contienen las cuentas
void Interface::setFileOutputPath(){
    char buffer[INPUT];
    showMessageOutput("Ingrese la ruta de archivos a procesar");
    {
        wclear(input_win);  // Limpiar la ventana de comandos
        box(input_win, 0, 0);  // Dibujar el borde de la ventana
        mvwprintw(input_win, 1, 1, " ");
        wrefresh(input_win);  // Refrescar la ventana de comandos para mostrar el prompt

        wgetnstr(input_win, buffer, INPUT);
        
        pathToOutput = buffer;
    }

    if (!verifyPath(pathToOutput)){
        showMessageOutput("La ruta especificada no existe...\n seleccione la opcion:");
        pathToOutput.clear();
        return;
    }

    showMessageOutput("Ruta guardada con exito!\n seleccione la opcion:");

    updateMenu();
}

// Ejecuta el proceso script.sh, necesita datos de tipo de archivo, ruta de los archivos, ruta donde se contienen las respuetas.
void Interface::process(){
    if(fileType.empty()){
        showMessageOutput("La extension de archivos no esta especificada\n seleccione la opcion:");
        return;
    }

    if (pathToProcess.empty()){
        showMessageOutput("La ruta a la carpeta no esta especificada\n seleccione la opcion:");
        return;
    }

    if (pathToOutput.empty()){
        showMessageOutput("La ruta a la carpeta de salida no esta especificada\n seleccione la opcion:");
        return;
    }

    // Obtengo las variables de entorno
    std::string txtCounterPath = getenv("TXTCOUNTER_PATH") ? getenv("TXTCOUNTER_PATH") : "";
    std::string tempPath = getenv("TEMP_PATH") ? getenv("TEMP_PATH") : "";

    if (txtCounterPath.empty()){
        endwin();
        printf("Ruta a la carpeta Text_Counter no encontrada");
        exit(EXIT_FAILURE);
    }

    std::string call = txtCounterPath + "/src/script.sh " + fileType + " " + pathToProcess + " " + pathToOutput;

    showMessageOutput("Procesando...");

    std::string result = executeWithBuffer(call);

    showMessageOutput("Listo!");

    endwin();

    printf("%s", result.c_str());

    // Creo archivo con el output

    result = "\"" + result + "\"";

    // Si esta TEMP_PATH en nuestra variable de entorno entonces guarda el archivo
    if (!tempPath.empty()){
        call = "echo -n " + result + " > " + tempPath + "/output.txt";
        system(call.c_str());
    }

    exit(EXIT_SUCCESS);
}

// Muestra un mensaje en la ventana de output
void Interface::showMessageOutput(std::string message){
    wclear(output_win); // Limpia la pantalla
    mvwprintw(output_win, 3, 0, message.c_str()); // Imprime el mensaje
    wrefresh(output_win); // Refrezca para que se muestre el mensaje
}