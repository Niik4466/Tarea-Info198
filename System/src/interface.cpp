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
    this->parallelCountExecuted = false;    // Inicializa la variable de control de ejecución de conteo paralelo
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
    menu_win = newwin(3 * height / 10, width, height * 6/10, 0);

    wrefresh (menu_win); // refrescar el menu

    // Creamos la ventanita de output de comandos
    output_win = newwin(height *5/10, width, height /10, 0);
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

    this->options.push_back(&Interface::planificador);
    sprintf(message, "%i. PLANIFICADOR", i);
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

    this->options.push_back(&Interface::performanceAnalysis);
    sprintf(message, "%i. Analisis de performance", i);
    this->optionsStrings.push_back(message);
    i++;

    this->options.push_back(&Interface::planificador);
    sprintf(message, "%i. PLANIFICADOR", i);
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

// Ejecuta el programa Text_counter_threads en paralelo
void Interface::parallelCountWithThreads() {
    std::string result;

    showMessageOutput("Ejecutando programa...");

    std::string parallelCountPath = getenv("PARALLEL_COUNT_PATH") ? getenv("PARALLEL_COUNT_PATH") : "";

    if(parallelCountPath.empty()){
        showMessageOutput("No se encontro el programa");
        return;
    }

    std::string extension = getenv("EXTENSION") ? getenv("EXTENSION") : "";
    std::string archivosDir = getenv("ARCHIVOS_DIR") ? getenv("ARCHIVOS_DIR") : "";
    std::string cantidadThread = getenv("CANTIDAD_THREAD") ? getenv("CANTIDAD_THREAD") : "";
    std::string mapaArchivo = getenv("MAPA_ARCHIVO") ? getenv("MAPA_ARCHIVO") : "";
    std::string stopWord = getenv("STOP_WORD") ? getenv("STOP_WORD") : "";
    std::string outputDir = getenv("OUTPUT_DIR") ? getenv("OUTPUT_DIR") : "";

    if (extension.empty() || archivosDir.empty() || cantidadThread.empty() || mapaArchivo.empty() || stopWord.empty() || outputDir.empty()) {
        showMessageOutput("No se encontraron las variables de entorno necesarias");
        return;
    }

    std::string call = parallelCountPath + "/text_counter_threads " + archivosDir + " " + outputDir + " " + mapaArchivo + " " + extension + " " + stopWord + " " + cantidadThread;

    result = executeWithBuffer(call);

    if (!result.empty()) {
        showMessageOutput(result);
        parallelCountExecuted = true;  // Marca que la función se ha ejecutado
    } else {
        showMessageOutput("El programa no se ejecuto con exito.");
    }
}

//Ejecuta el Script para crear el indice invertido
void Interface::createInvertedIndex() {

    // Verifica si la función parallelCountWithThreads ya se ha ejecutado
    if (!parallelCountExecuted) {   // Si no se ha ejecutado, muestra un mensaje de error
        showMessageOutput("Debe ejecutar primero la opcion de conteo paralelo con threads.");
        return;
    }

    // Construye la llamada al script
    std::string invertedIndexPath = getenv("INVERTED_INDEX_PATH") ? getenv("INVERTED_INDEX_PATH") : "";
    if(invertedIndexPath.empty()){
        showMessageOutput("No se encontro el programa");
        return;
    }

    std::string extension = getenv("EXTENSION") ? getenv("EXTENSION") : "";
    std::string archivosDir = getenv("OUTPUT_DIR") ? getenv("OUTPUT_DIR") : "";
    std::string salidaDir = getenv("INVERTED_INDEX") ? getenv("INVERTED_INDEX") : "";

    if (extension.empty() || archivosDir.empty() || salidaDir.empty()) {
        showMessageOutput("No se encontraron las variables de entorno necesarias");
        return;
    }

    // Verifica que las rutas de las variables existan
    if (!std::filesystem::exists(invertedIndexPath)) {
        showMessageOutput("El path del script de índice invertido no existe.");
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

    // Construye la llamada al script
    std::string call = invertedIndexPath + "/invertedIndex.sh " + extension + " " + archivosDir + " " + salidaDir + " 2> \"logs.txt\" ";
    showMessageOutput("Ejecutando programa...");
    std::string result = executeWithBuffer(call);

    // Maneja la salida del script
    if (result.empty()) {
        showMessageOutput("El script no se ejecuto con exito.");
    } else {
        showMessageOutput("El script se ejecuto con exito y el archivo de salida quedo ubicado en: ." + result);
    }
}

// Ejecuta el programa de analisis de performance
void Interface::performanceAnalysis() {
// Obtener las variables de entorno necesarias
    std::string arrayThreadsStr = getenv("ARRAY_THREADS") ? getenv("ARRAY_THREADS") : "";
    std::string repetitions = getenv("REPETICIONES") ? getenv("REPETICIONES") : "";
    std::string datosPath = getenv("DATOS_PATH") ? getenv("DATOS_PATH") : "";
    std::string textCounterThreadsPath = getenv("PARALLEL_COUNT_PATH") ? getenv("PARALLEL_COUNT_PATH") : "";
    std::string archivosDir = getenv("ARCHIVOS_DIR") ? getenv("ARCHIVOS_DIR") : "";
    std::string outputDir = getenv("OUTPUT_DIR") ? getenv("OUTPUT_DIR") : "";
    std::string idPath = getenv("MAPA_ARCHIVO") ? getenv("MAPA_ARCHIVO") : "";
    std::string extension = getenv("EXTENSION") ? getenv("EXTENSION") : "";
    std::string stopWordsPath = getenv("STOP_WORD") ? getenv("STOP_WORD") : "";
    std::string ejecutadorPath = getenv("EJECUTADOR_PATH") ? getenv("EJECUTADOR_PATH") : "";
    std::string analizerPath = getenv("ANALISADOR_PATH") ? getenv("ANALISADOR_PATH") : "";
    std::string graphPath = getenv("GRAFICO_PATH") ? getenv("GRAFICO_PATH") : "";

    if (arrayThreadsStr.empty() || repetitions.empty() || datosPath.empty() || textCounterThreadsPath.empty() || archivosDir.empty() || outputDir.empty() || idPath.empty() || extension.empty() || stopWordsPath.empty() || ejecutadorPath.empty()) {
        showMessageOutput("No se encontraron las variables de entorno necesarias");
        return;
    }

     // Construir el comando usando ostringstream
    std::ostringstream call;
    call << ejecutadorPath << "/ejecutador " 
         << textCounterThreadsPath << " "   //1
         << archivosDir << " "              //2
         << outputDir << " "                //3
         << idPath << " "                   //4
         << extension << " "                //5
         << stopWordsPath << " "            //6
         << repetitions << " "              //7
         << datosPath << " "                //8
         << analizerPath << " "             //9
         << graphPath << " "                //10
         << arrayThreadsStr;                //11

    showMessageOutput("Ejecutando programa...");
    
    std::string output = executeWithBuffer(call.str());    
    // Verificar el resultado de la ejecución
    if (output.find("Programa no encontrado") == std::string::npos) {
        showMessageOutput("El programa se ejecuto con exito los resultado quedaron en: " + datosPath);
    } else {
        std::ostringstream errorMessage;
        errorMessage << "El programa no se ejecuto con exito. Salida: " << output;
        showMessageOutput(errorMessage.str());
    }
}

// Realiza uno o varios calculos con cores logicos en paralelo.
void Interface::planificador() {
    // Leemos las variables de entorno
    std::string instructionsPath = getenv("INSTRUCTIONS_PATH");
    std::string resultPath = getenv("RESULT_PATH");
    std::string cantCores = getenv("CANT_CORES");
    std::string distributorPath = getenv("DISTRIBUTOR_PATH");
    std::string planificatorPath = getenv("PLANIFICATOR_PATH");
    std::string corePath = getenv("CORE_PATH");

    // Validamos la existencia de las variables de entorno
    if (instructionsPath.empty() && resultPath.empty() && cantCores.empty() && distributorPath.empty() && planificatorPath.empty() && corePath.empty()){
        showMessageOutput("No se encontraron las variables de entorno necesarias");
        return;
    }

    // Validamos la existencia de los paths necesarios
    if (!std::filesystem::exists(instructionsPath) && !std::filesystem::exists(corePath) && !std::filesystem::exists(planificatorPath) && !std::filesystem::exists(distributorPath) && !std::filesystem::exists(corePath + "/core")){
        showMessageOutput("No fue posible ejecutar. Rutas incorrectas");
        return;
    }


    char buffer[64];
    wclear(output_win);  // Limpiar la ventana de salida
    box(output_win, ' ', '#');  // Dibujar el borde de la ventana
    mvwprintw(output_win, 1, 1, "Ejecutando....");
    wrefresh(output_win);  // Refrescar la ventana de salida para mostrar el prompt

    wclear(input_win);  // Limpiar la ventana de comandos
    box(input_win, 0, 0);  // Dibujar el borde de la ventana
    mvwprintw(input_win, 1, 1, " ");
    wrefresh(input_win);  // Refrescar la ventana de comandos para mostrar el prompt

    // Realizamos la llamada la sistema
    std::string call = planificatorPath + " " + instructionsPath + " " + distributorPath + " " + resultPath + " " + corePath + " " + cantCores;
    std::string result = executeWithBuffer(call);
    showMessageOutput("Listo!\nEl archivo con los resultados esta en: " + result + "\nIngrese cualquier cosa para continuar");
    wgetnstr(input_win, buffer, 64);
    showMessageOutput("");

}