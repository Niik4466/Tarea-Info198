#include "interface.h"

// Constructor unico de la interfaz
Interface::Interface(std::string &phrase, std::vector<int> &list, std::string &num, User user, std::vector<User> &users){
    
    printf("Iniciando Ventanas...\n");
    // Seteamos las variables principales
    this->phrase = phrase;
    this->list = list;
    this->num = num;
    this->user = user;
    this->users = users;
    // Iniciamos ncurses

    initscr();      // Inicializar ncurses
    cbreak();       // Desactiva el buffer de linea

    // Seteamos las variables height y width usadas para dar tamanio a las ventanas
    
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
    char message[50];

    // Creamos la ventanita del header
    header_win = newwin(height/10, width, 0, 0);

    sprintf(message, "SISTEMA De jueguete\t PID = %i", getpid());
    mvwprintw(header_win, 1, 1, message);

    sprintf(message, "Nombre de usuario: %s", user.getUserName().c_str());
    mvwprintw(header_win, 2, 1, message);
    sprintf(message, "Rol: %s", user.getUserRole().c_str());
    mvwprintw(header_win, 3, 1, message);

    wrefresh(header_win);

    // Creamos la ventanita de output de comandos

    curs_set(1); // Muestra el puntero del teclado en la cajita de comandos

    // Crear ventanas para el menú y para la entrada de comandos
    menu_win = newwin(2 * height / 10, width, height * 7/10, 0);

    wrefresh (menu_win); // refrescar el menu

    // Creamos la ventanita de output de comandos
    output_win = newwin(height *6/10, width, height /10, 0);
    box(output_win, ' ', '#');
    mvwprintw(output_win, 1, 1, "Respuesta de la ejecucion:");
    wrefresh(output_win);

    // Creamos la ventana para el input de comandos
    input_win = newwin(height/10, width, height *9/10, 0);
    mvwprintw(output_win, 1, 1, "Ingrese opcion: ");
    wrefresh(input_win);
}

Interface& Interface::getInstance(std::string &phrase, std::vector<int> &list, std::string &num, User user){
    std::vector<User> users;
    static Interface interface(phrase, list, num, user, users);

    // Seteamos opciones
    interface.setOptions(user);

    return interface;
}

Interface& Interface::getInstance(std::string &phrase, std::vector<int> &list, std::string &num, User user, std::vector<User> users){
    static Interface interface(phrase, list, num, user, users);

    //Seteamos opciones
    interface.setOptions(user, users);

    return interface;
}

// Setea todas las opciones del rol "Usuario Generico"
void Interface::setOptions(User user){
    char message[50];
    int i = 0;
    
    // Generamos nuestro vector de opciones
    this->options.push_back(&Interface::interfaceExit);
    sprintf(message, "%i. Salir", i);
    this->optionsStrings.push_back(message);
    i++;

    this->options.push_back(&Interface::usrPalindromes);
    sprintf(message, "%i. Deteccion de palindromos", i);
    this->optionsStrings.push_back(message);
    i++;

    this->options.push_back(&Interface::usrVocaleCount);
    sprintf(message, "%i. Contar vocales", i);
    this->optionsStrings.push_back(message);
    i++;

    this->options.push_back(&Interface::usrLettersCount);
    sprintf(message, "%i. Cantidad de letras de un texto", i);
    this->optionsStrings.push_back(message);
    i++;

    this->options.push_back(&Interface::usrVectorSumProm);
    sprintf(message, "%i. Promedio y sumatoria de un vector", i);
    this->optionsStrings.push_back(message);
    i++;

    this->options.push_back(&Interface::usrFunctionCalc);
    sprintf(message, "%i. Calcular f(x)=5x*x+1/x", i);
    this->optionsStrings.push_back(message);
    i++;

    this->options.push_back(&Interface::wordsTxtCount);
    sprintf(message, "%i. Contar palabras", i);
    this->optionsStrings.push_back(message);
    i++;

    interfaceMenu();
}

// Setea todas las opciones del rol "Admin"
void Interface::setOptions(User user, std::vector<User> users){
    char message[50];
    int i = 0;

    // Generamos nuestro vector de opciones

    this->options.push_back(&Interface::interfaceExit);
    sprintf(message, "%i. Salir", i);
    this->optionsStrings.push_back(message);
    i++;

    this->options.push_back(&Interface::insertUsers);
    sprintf(message, "%i. Ingresar nuevo Usuario", i);
    this->optionsStrings.push_back(message);
    i++;

    this->options.push_back(&Interface::listUsers);
    sprintf(message, "%i. Listar usuarios", i);
    this->optionsStrings.push_back(message);
    i++;

    this->options.push_back(&Interface::deleteUsers);
    sprintf(message, "%i. Eliminar usuario", i);
    this->optionsStrings.push_back(message);
    i++;

    this->options.push_back(&Interface::usrPalindromes);
    sprintf(message, "%i. Deteccion de palindromos", i);
    this->optionsStrings.push_back(message);
    i++;

    this->options.push_back(&Interface::usrVocaleCount);
    sprintf(message, "%i. Contar vocales", i);
    this->optionsStrings.push_back(message);
    i++;

    this->options.push_back(&Interface::usrLettersCount);
    sprintf(message, "%i. Cantidad de letras de un texto", i);
    this->optionsStrings.push_back(message);
    i++;

    this->options.push_back(&Interface::usrVectorSumProm);
    sprintf(message, "%i. Promedio y sumatoria de un vector", i);
    this->optionsStrings.push_back(message);
    i++;

    this->options.push_back(&Interface::usrFunctionCalc);
    sprintf(message, "%i. Calcular f(x)=5x*x+1/x", i);
    this->optionsStrings.push_back(message);
    i++;

    this->options.push_back(&Interface::wordsTxtCount);
    sprintf(message, "%i. Contar palabras", i);
    this->optionsStrings.push_back(message);
    i++;

    this->options.push_back(&Interface::parallelCountWithThreads);
    sprintf(message, "%i. Conteo paralelo con threads", i);
    this->optionsStrings.push_back(message);
    i++;

    this->options.push_back(&Interface::createInvertedIndex);
    sprintf(message, "%i. Crear indice invertido", i);
    this->optionsStrings.push_back(message);
    i++;

    interfaceMenu();
}

// Setea las opciones de menu
void Interface::interfaceMenu(){
    // Imprimir el menú en la primera mitad de la pantalla
    wclear(menu_win);
    mvwprintw(menu_win, 1, 1, "Opciones:");

    // Despliega las opciones en el menu
    for (int i = 0; i < static_cast<int>(options.size()); i++){
        mvwprintw(menu_win, i+2, 1, optionsStrings.at(i).c_str());
    }
    wrefresh(menu_win);
}

// Funcion principal del programa, se encarga de manejar los comandos que ingresa el usuario y los output de estos
void Interface::interfaceInputOutput(){
    char buffer[3];
    int option = -1;
    while (true) {
        // Mostrar el prompt
        wclear(input_win);  // Limpiar la ventana de comandos
        box(input_win, 0, 0);  // Dibujar el borde de la ventana
        mvwprintw(input_win, 1, 1, "Ingrese opcion: ");
        wrefresh(input_win);  // Refrescar la ventana de comandos para mostrar el prompt

        wgetnstr(input_win, buffer, 3);

        if (isInt(buffer)){
            option = stringToInt(buffer);
            if (option < static_cast<int>(options.size()) && option >= 0){
                (this->*options.at(option))();
                continue;
            }
        }

        showMessageOutput("Comando no reconocido");
    }

    endwin();
}

// Maneja la opcion 1 -- deteccion de palindromos --
void Interface::usrPalindromes(){
    if (phrase.empty()){
        showMessageOutput("No se ha ingresado palabra!");
        return;
    }

    char message[256]; 

    if (!isAlpha(phrase)){
        sprintf(message, "Solo se pueden detectar palindromes son frases sin numeros!!");
        showMessageOutput(message);
        return;
    }

    if (isPalindrome(phrase))
        sprintf(message, "La frase %s es palindrome\n", phrase.c_str());
    else 
        sprintf(message, "La frase %s no es palindrome\n", phrase.c_str());

    showMessageOutput(message);
}

// Maneja la opcion 2 -- contar vocales --
void Interface::usrVocaleCount(){
    if (phrase.empty()){
        showMessageOutput("No se ha ingresado palabra!");
        return;
    }

    char message[80];

    sprintf(message, "la cantidad de vocales en la frase %s es: %i", phrase.c_str(), countVocals(phrase));

    showMessageOutput(message);
}

// Maneja la opcion 3 -- cantidad de letras en un texto --
void Interface::usrLettersCount(){
    if (phrase.empty()){
        showMessageOutput("No se ha ingresado palabra!");
        return;
    }
    char message[80];

    sprintf(message, "la cantidad de letras del texto %s es: %i", phrase.c_str(), countAlph(phrase));

    showMessageOutput(message);
}

// Maneja la opcion 4 -- promedio y sumatoria de un vector --
void Interface::usrVectorSumProm(){
    if (list.empty()){
        showMessageOutput("No se ha ingresado vector!");
        return;
    }

    // Crea las variables iniciales
    int sum = 0;
    float prom;
    
    // Suma cada elemento
    for (const int &i : list){
        sum += i;
    }
    // Con la suma de cada elemento sacamos el promedio dividiendolo por la cantidad
    prom = sum/list.size();

    char message[256];

    sprintf(message,"La sumatoria del vector es: %i\n  El promedio de los elementos del vector es: %.3f\n", sum, prom);

    showMessageOutput(message);

}

// Maneja la opcion 5 -- calcular f(x)=5x*x+1/x --
void Interface::usrFunctionCalc(){
    if (num.empty()){
        showMessageOutput("No se ha ingresado numero!");
        return;
    }
    char message[256];

    if(isInt(num)){
        if (isTooBigInt(num)){
            showMessageOutput("El numero ingresado es demasiado grande!!");
            return;
        }

        int numNumeric = stringToInt(num);
        if (!verifyNonCero(numNumeric)){
            showMessageOutput("No se puede dividir por 0!!");
            return;
        }

        sprintf(message,"El resultado de f(%d) es: %.3f\n", numNumeric, static_cast<float>(5*numNumeric*numNumeric+1/numNumeric));

        showMessageOutput(message);

    }
    else{
        if (isTooBigFloat(num)){
            showMessageOutput("El numero ingresado es demasiado grande!!");
            return;
        }

        float numNumeric = stringToFloat(num);

        if (!verifyNonCero(numNumeric)){
            showMessageOutput("No se puede dividir por 0!!");
            return;
        }

        sprintf(message,"El resultado de f(%f) es: %.3f\n", static_cast<float>(5*numNumeric*numNumeric+1/numNumeric));

        showMessageOutput(message);
    }
}

// Imprime la palabra `message` en la pantalla `win` tomando el puntero en (0,2) 
void Interface::showMessageOutput(std::string message){
    wclear(output_win); // Limpia la pantalla
    box(output_win, ' ', '#');
    mvwprintw(output_win, 1, 1, "Respuesta de la ejecucion:");
    mvwprintw(output_win, 3, 0, message.c_str()); // Imprime el mensaje
    wrefresh(output_win); // Refrezca para que se muestre el mensaje
}

// Sale del programa
void Interface::interfaceExit(){
    endwin();
    exit(EXIT_SUCCESS);
}

// Agregar nuevos usuarios (solo es posible agregar usuarios genericos)
void Interface::insertUsers(){
    std::string userName;
    std::string userPassword;

    char *buffer = new char[50];
    while (userName.empty()){
        wclear(menu_win); // Limpiamos la ventana de menu
        mvwprintw(menu_win, 1, 1, "0. Salir");
        mvwprintw(menu_win, 2, 1, "Ingrese nombre de Usuario");
        wrefresh(menu_win);
        wclear(input_win);  // Limpiar la ventana de comandos
        box(input_win, 0, 0);  // Dibujar el borde de la ventana
        mvwprintw(input_win, 1, 1, "");
        wrefresh(input_win);  // Refrescar la ventana de comandos para mostrar el prompt

        wgetnstr(input_win, buffer, 50);

        if (buffer[0] == '0'){
            interfaceMenu();
            return;
        }
        
        if (!verifyUserFormat(buffer)){
            showMessageOutput("Formato de nombre incorrecto!");
            continue;
        }

        if (getUserNameIndex(buffer, users) != -1){
            showMessageOutput("El nombre ingresado ya existe");
            continue;
        }

        userName = buffer;
    }

    buffer = new char[50];
    while(userPassword.empty()){
        wclear(menu_win); // Limpiamos la ventana de menu
        mvwprintw(menu_win, 1, 1, "0. Salir");
        mvwprintw(menu_win, 2, 1, "Ingrese la contrasenia del usuario");
        wrefresh(menu_win);
        wclear(input_win);  // Limpiar la ventana de comandos
        box(input_win, 0, 0);  // Dibujar el borde de la ventana
        mvwprintw(input_win, 1, 1, "");
        wrefresh(input_win);  // Refrescar la ventana de comandos para mostrar el prompt

        wgetnstr(input_win, buffer, 50);

        if (buffer[0] == '0'){
            interfaceMenu();
            return;
        }
        
        if (!verifyPassFormat(buffer)){
            showMessageOutput("Formato de contrasenia incorrecto!");
            continue;
        }

        userPassword = buffer;
    }

    users.push_back(User(userName, userPassword, "Usuario Generico"));

    writeUserstxt(users);

    showMessageOutput("Usuario generado con exito!");

    interfaceMenu();
}

// Listar los usuarios
void Interface::listUsers(){
    std::string stringUsers = "";
    for (User &item : users){
        stringUsers += item.toString() + "\n";
    }

    showMessageOutput(stringUsers.c_str());
}

// Eliminar un usuario
void Interface::deleteUsers(){
    char buffer[50];
    int userIndex = -1;
    while (userIndex == -1){
        wclear(menu_win); // Limpiamos la ventana de menu
        mvwprintw(menu_win, 1, 1, "0. Salir");
        mvwprintw(menu_win, 2, 1, "Ingrese nombre de Usuario");
        wrefresh(menu_win);
        wclear(input_win);  // Limpiar la ventana de comandos
        box(input_win, 0, 0);  // Dibujar el borde de la ventana
        mvwprintw(input_win, 1, 1, "");
        wrefresh(input_win);  // Refrescar la ventana de comandos para mostrar el prompt

        wgetnstr(input_win, buffer, 50);

        if (buffer[0] == '0'){
            interfaceMenu();
            return;
        }

        if (!verifyUserFormat(buffer)){
            showMessageOutput("Formato de nombre incorrecto!");
            continue;
        }

        userIndex = getUserNameIndex(buffer, users);

        if (userIndex == -1){
            showMessageOutput("Usuario no encontrado");
            continue;
        }

        if (users.at(userIndex).getUserRole() == "Admin"){
            showMessageOutput("No se puede borrar, el usuario es administrador");
            userIndex = -1;
            continue;
        }

    }

    users.erase(users.begin() + userIndex);

    writeUserstxt(users);

    showMessageOutput("Usuario Eliminado.");

    interfaceMenu();
}

// Ejecuta el programa externo wordsTxtCount como proceso hijo
void Interface::wordsTxtCount(){
    // Elimino el enlace de comunicacion previo para evitar errores
    std::string tempPath = getenv("TEMP_PATH") ? getenv("TEMP_PATH") : "";

    if (tempPath.empty()){
        showMessageOutput("No fue posible establecer la comunicacion con el proceso hijo");
        return;
    }

    std::string call = "rm " + tempPath + "/output.txt";
    system(call.c_str());

    std::string result;

    showMessageOutput("Ejecutando programa...");

    std::string txtCountPath = getenv("TXTCOUNTER_PATH") ? getenv("TXTCOUNTER_PATH") : "";

    if(txtCountPath.empty()){
        showMessageOutput("No se encontro el programa");
        return;
    }

    call = txtCountPath + "/textCounter";

    system(call.c_str());

    tempPath += "/output.txt";

    result = readFile(tempPath);

    !result.empty() ? showMessageOutput(result) : showMessageOutput ("");
}

// Ejecuta el Script de conteo de palabras en paralelo
void Interface::parallelCountWithThreads() {
     // Construye la llamada al script
    std::string countThreadPath = getenv("PARALLEL_COUNT_PATH") ? getenv("PARALLEL_COUNT_PATH") : "";
    if(countThreadPath.empty()){
        showMessageOutput("No se encontro el programa");
        return;
    }

    std::string extension = getenv("EXTENSION") ? getenv("EXTENSION") : "";
    std::string archivosDir = getenv("ARCHIVOS_DIR") ? getenv("ARCHIVOS_DIR") : "";
    std::string salidaDir = getenv("OUTPUT_DIR") ? getenv("OUTPUT_DIR") : "";
    std::string cantidadThreads = getenv("CANTIDAD_THREAD") ? getenv("CANTIDAD_THREAD") : "";
    std::string mapaArchivo = getenv("MAPA_ARCHIVO") ? getenv("MAPA_ARCHIVO") : "";
    std::string stopWords = getenv("STOP_WORD") ? getenv("STOP_WORD") : "";

    if (extension.empty() || archivosDir.empty() || salidaDir.empty() || cantidadThreads.empty() || mapaArchivo.empty() || stopWords.empty()) {
        showMessageOutput("No se encontraron las variables de entorno necesarias");
        return;
    }
    // Verifica que las rutas de las variables existan
    if (!std::filesystem::exists(countThreadPath)) {
        showMessageOutput("El path de conteo paralelo no existe.");
        return;
    }

    if (!std::filesystem::exists(archivosDir)) {
        showMessageOutput("El path de archivos no existe.");
        return;
    }

    if (!std::filesystem::exists(salidaDir)) {
        showMessageOutput("El path de salida no existe.");
        return;
    }

    if (!std::filesystem::exists(mapaArchivo)) {
        showMessageOutput("El path del mapa de archivo no existe.");
        return;
    }

    if (!std::filesystem::exists(stopWords)) {
        showMessageOutput("El path de stop words no existe.");
        return;
    }

    // Construye la llamada al script
    std::string call = countThreadPath + "/controlFather.sh " + extension + " " + archivosDir + " " + salidaDir + " " + cantidadThreads + " " + mapaArchivo + " " + stopWords + " " + countThreadPath + "/children.sh" + " 2> \"logs.txt\" ";
    showMessageOutput("Ejecutando programa...");
    std::string result = executeWithBuffer(call);

    // Maneja la salida del script
    showMessageOutput(result);
}

//Ejecuta el Script para crear el indice invertido
void Interface::createInvertedIndex(){
    // Construye la llamada al script
    std::string command = "./invertedIndex.sh";

    // Ejecuta el script
    int result = system(command.c_str());

    // Maneja la salida del script
    if (result == 0) {
        std::cout << "Script ejecutado exitosamente." << std::endl;
    } else {
        std::cerr << "Error al ejecutar el script." << std::endl;
    }
}