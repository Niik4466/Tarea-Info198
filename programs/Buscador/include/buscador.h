#ifndef BUSCADOR_H
#define BUSCADOR_H

#include <ncurses.h>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

class Interface {
private:
    WINDOW* input_win;
    WINDOW* header_win;
    WINDOW* output_win;
    WINDOW* response_win;  // Nueva ventana para respuestas
    int socket_fd;
    struct sockaddr_in server_addr; // Dirección del servidor
    std::map<int, std::string> file_names; // Mapa para ID -> nombre de archivo

    void loadIDFile();

public:
    Interface();
    void interfaceInputOutput();
    void showResponse(const std::string& response);
    void interfaceExit();
    void showMessageOutput(const std::string& message);
    // Otros métodos según sea necesario
};

#endif // BUSCADOR_H