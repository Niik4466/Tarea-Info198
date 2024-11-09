#include "buscador.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unordered_map>

#define IP "127.0.0.1"
#define PORT_SEARCH 2020

// Función para generar un mapa de ID a nombre de archivo
std::unordered_map<int, std::string> generateIDMap(const std::string& idFilePath) {
    std::unordered_map<int, std::string> idMap;
    std::ifstream file(idFilePath);

    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << idFilePath << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name;
        int id;
        
        // Lee el texto hasta el ; y luego el número
        if (std::getline(ss, name, ';') && ss >> id) {
            // Guarda el texto como valor y el número como clave
            idMap[id] = name;
        }
    }

    file.close();
    return idMap;
}

int checkPortOpen(const char* ip, int port) {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1) {
            return 0;
        }

        struct sockaddr_in server;
        server.sin_family = AF_INET;
        server.sin_port = htons(port);
        server.sin_addr.s_addr = inet_addr(ip);

        int result = connect(sock, (struct sockaddr*)&server, sizeof(server));
        close(sock);

        return result == 0 ? 1 : 0;
}
// Implementación de los métodos de la clase Interface

void Interface::loadIDFile() {
    const char* env_p = getenv("MAPA_ARCHIVO");
    std::string idPath = env_p ? std::string(env_p) : "";
    if(idPath.empty()) {
        showMessageOutput("Error: No se encontró la ruta del archivo ID.txt");
        exit(EXIT_FAILURE);
    }

    file_names = generateIDMap(idPath);
}

Interface::Interface() {
    initscr();
    cbreak();

    int height, width;
    getmaxyx(stdscr, height, width);

    // Configurar ventanas
    header_win = newwin(height/6, width, 0, 0);
    output_win = newwin(height/6, width, height/6, 0);
    response_win = newwin(height/2, width, height/3, 0); 
    input_win = newwin(height/6, width, height*5/6, 0);

    // Cargar archivo ID.txt
    loadIDFile();

    // Crear el Socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        showMessageOutput("Error creando socket");
        endwin();
        exit(EXIT_FAILURE);

    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT_SEARCH);
    server_addr.sin_addr.s_addr = inet_addr(IP);
    memset(&(server_addr.sin_zero), '\0', 8); // Zero the rest of the struct
    // Conectar al Cache
    if (connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        showMessageOutput("Error conectando al cache");
        close(socket_fd);
        endwin();
        exit(EXIT_FAILURE);
    }

    // Inicializar ventanas
    box(response_win, 0, 0);
    mvwprintw(header_win, 1, 1, "Programa Buscador");
    mvwprintw(header_win, 2, 1, "###############################");
    mvwprintw(header_win, 3, 1, "pid: %d", getpid());
    mvwprintw(header_win, 4, 1, "Ingrese 'SALIR AHORA' para terminar");

    wrefresh(header_win);
    wrefresh(response_win);
    wrefresh(output_win);
    wrefresh(input_win);
}

void Interface::interfaceInputOutput() {
    char buffer[1024];
    char response[1024];
    std::string message;
    while (true) {
        wclear(input_win);
        box(input_win, 0, 0);
        mvwprintw(input_win, 1, 1, "Ingrese frase: ");
        wrefresh(input_win);
        
        wgetnstr(input_win, buffer, sizeof(buffer));
        std::string input(buffer);
        if (input == "SALIR AHORA") {
            interfaceExit();
            break;
        }
        
        // Enviar consulta al cache
        if (send(socket_fd, (input + "\n").c_str(), input.length() + 1, 0) == -1) {
            showMessageOutput("Error enviando mensaje");
            continue;
        }

        // Recibir respuesta
        ssize_t bytes_received = recv(socket_fd, response, sizeof(response)-1, 0);
        if (bytes_received > 0) {
            response[bytes_received] = '\0';
            message = response;
            showResponse(response);
        } else {
            showMessageOutput("No se recibió respuesta del cache");
        }
    }
}

// Añade manejo de excepciones al parsear IDs en showResponse
void Interface::showResponse(const std::string& response) {
    wclear(response_win);
    box(response_win, 0, 0);
    mvwprintw(response_win, 1, 1, "Resultados:");

    // Separar la consulta de los resultados
    size_t pos = response.find(';');
    if (pos == std::string::npos) {
        mvwprintw(response_win, 2, 1, "Formato de respuesta inválido");
        wrefresh(response_win);
        return;
    }

    std::string query = response.substr(0, pos);
    std::string results = response.substr(pos + 1);

    // Construir el nuevo mensaje
    std::string formattedMessage = query;

    // Procesar cada par (IDx,score)
    size_t start = 0;
    while ((start = results.find("(ID", start)) != std::string::npos) {
        size_t idStart = start + 3;
        size_t idEnd = results.find(",", idStart);
        size_t scoreEnd = results.find(")", idStart);
        
        if (idEnd == std::string::npos || scoreEnd == std::string::npos) break;

        // Extraer ID y puntaje
        int id = std::stoi(results.substr(idStart, idEnd - idStart));
        std::string score = results.substr(idEnd + 1, scoreEnd - idEnd - 1);

        // Buscar nombre del archivo en el hashmap
        auto it = file_names.find(id);
        if (it != file_names.end()) {
            formattedMessage += " (" + it->second + ",Puntaje: " + score + ")";
        }

        start = scoreEnd + 1;
    }

    mvwprintw(response_win, 2, 1, "Mensaje recibido: %s", formattedMessage.c_str());
    wrefresh(response_win);
}

void Interface::interfaceExit() {
    close(socket_fd);
    endwin();
    printf("Programa terminado\n");
    exit(EXIT_SUCCESS);
}

void Interface::showMessageOutput(const std::string& message) {
    wclear(output_win);
    box(output_win, 0, 0);
    mvwprintw(output_win, 1, 1, message.c_str());
    wrefresh(output_win);
}

int main() {
    //if (checkPortOpen(IP, PORT_SEARCH)) {
    //     return 1;
    // }
    
    Interface interface;
    interface.interfaceInputOutput();

    return 0;
}