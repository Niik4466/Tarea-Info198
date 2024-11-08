#include "buscador.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

#define IP "127.0.0.1"
#define PORT_SEARCH 2020

// Implementación de los métodos de la clase Interface

void Interface::loadIDFile() {
    const char* env_p = getenv("MAPA_ARCHIVO");
    std::string idPath = env_p ? std::string(env_p) : "";
    if(idPath.empty()) {
        showMessageOutput("Error: No se encontró la ruta del archivo ID.txt");
        return;
    }

    std::ifstream file(idPath);
    if (!file.is_open()) {
        showMessageOutput("Error: No se pudo abrir el archivo ID.txt");
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name;
        int id;
        if (std::getline(ss, name, ';') && ss >> id) {
            file_names[id] = name;
        }
    }
    file.close();
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

    // Conectar al cache
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        showMessageOutput("Error creando socket");
        endwin();
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT_SEARCH);
    server_addr.sin_addr.s_addr = inet_addr(IP);
    memset(&(server_addr.sin_zero), '\0', 8); // Zero the rest of the struct

    if (connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        showMessageOutput("Error conectando al cache");
        close(socket_fd);
        endwin();
        exit(1);
    }

    // Inicializar ventanas
    box(response_win, 0, 0);
    mvwprintw(header_win, 1, 1, "Programa Buscador");
    mvwprintw(header_win, 2, 1, "###############################");
    mvwprintw(header_win, 3, 1, "pid: %d", getpid());

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
        message = response;
        if (bytes_received > 0) {
            response[bytes_received] = '\0';
            showResponse(response);
            showMessageOutput(message);
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

    std::stringstream ss(response);
    std::string line;
    int y = 2;
    while (std::getline(ss, line)) {
        // Parsear la respuesta y mapear IDs a nombres
        size_t pos = line.find(';');
        if (pos != std::string::npos) {
            try {
                int id = std::stoi(line.substr(0, pos));
                std::string score = line.substr(pos + 1);
                std::string filename = file_names.count(id) ? file_names[id] : "Archivo Desconocido";
                mvwprintw(response_win, y++, 1, "%s: %s", filename.c_str(), score.c_str());
                if (y >= (getmaxy(response_win) - 1)) break; // Evitar overflow
            }
            catch (const std::invalid_argument& e) {
                showMessageOutput("Error: Formato de respuesta inválido.");
                break;
            }
            catch (const std::out_of_range& e) {
                showMessageOutput("Error: ID fuera de rango.");
                break;
            }
        }
        else {
            showMessageOutput("Error: Separador ';' no encontrado.");
            break;
        }
    }
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
    Interface interface;
    interface.interfaceInputOutput();
    return 0;
}