#include <iostream>
#include <cstdlib>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <string>
#include <unordered_map>
#include <queue>

#define IP "127.0.0.1"          // Localhost
#define PORT_SEARCH_ENGINE 2021 // Puerto en el que escucha el proceso "motor.sh"
#define PORT_SEARCH 2020        // Puerto a abrir para que el proceso "busqueda" escuche las respuestas
#define VERBOSE 0               // Para imprimir mensajes de debug

// Busca un resultado almacenado en el hashmap llamado `cache`, retorna "" si no fue encontrada la respuesta
std::string getAnswerFromHash(const std::string& message, const std::unordered_map<std::string, std::string>& cache);

// Agrega un nuevo resultado al hashmap. Si la cantidad de elementos almacenada en hashmap es mayor a `limit`, entonces elimina el primer elemento almacenado y lo guarda en esa poscicion.
void insertNewResultToHash(const std::string &message, const std::string &answer, std::queue<std::string> &answers, std::unordered_map<std::string, std::string> &cache, const short int &limit);

int main(int argc, char** argv) {
    if (argc != 2){
        if (VERBOSE)
            fputs_unlocked("No se ingresan argumentos suficientes", stderr);
        return 1;
    }

// ---------------------- CREAR EL SERVIDOR     -------------------

    // Crear un socket del servidor
    int cacheServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (cacheServerSocket == -1) {
        if (VERBOSE)
            fputs_unlocked("No fue posible crear el socket.\n", stderr);
        return -1;
    }

    // Configurar la dirección del servidor
    struct sockaddr_in cacheServerAdress;
    cacheServerAdress.sin_family = AF_INET;
    cacheServerAdress.sin_port = htons(PORT_SEARCH);   // Puerto del proceso actual
    cacheServerAdress.sin_addr.s_addr = inet_addr(IP); // IP localhost

    // Vincular el socket a la dirección y el puerto
    if (bind(cacheServerSocket, (struct sockaddr*)&cacheServerAdress, sizeof(cacheServerAdress)) == -1) {
        if (VERBOSE)
            fputs_unlocked("No fue posible vincular el socket al puerto.\n", stderr);
        close(cacheServerSocket);
        return -1;
    }

    // Escuchar por conexiones entrantes
    if (listen(cacheServerSocket, 5) == -1) {
        if (VERBOSE)
            fputs_unlocked("Error al escuchar por conexiones entrantes.\n", stderr);
        close(cacheServerSocket);
        return -1;
    }

    // Aceptar una conexión entrante
    struct sockaddr_in BusquedaAdress;
    socklen_t BusquedaAdressLength = sizeof(BusquedaAdress);
    int busquedaSocket = accept(cacheServerSocket, (struct sockaddr*)&BusquedaAdress, &BusquedaAdressLength);
    if (busquedaSocket == -1) {
        if (VERBOSE)
            fputs_unlocked("Error al aceptar la conexión.\n", stderr);
        close(cacheServerSocket);
        return -1;
    }

// --------------------------------------------------------------------
// ---------------------- CONECTAR AL MOTOR.SH ------------------------

    // Creamos un socket para escuchar al motor de busqueda
    int engineServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (engineServerSocket == -1){
        if (VERBOSE)
            fputs_unlocked("No fue posible crear un socket para escuchar a engine.\n", stderr);
        return -1;
    }

    // Configuramos la direccion del motor_busqueda para conectarnos
    struct sockaddr_in engineServerAdress;
    engineServerAdress.sin_family = AF_INET;
    engineServerAdress.sin_port = htons(PORT_SEARCH_ENGINE);  // Puerto que está escuchando el coproc
    engineServerAdress.sin_addr.s_addr = inet_addr(IP);       // Dirección IP del servidor

    // Conectar al motor de busqueda
    if (connect(engineServerSocket, (struct sockaddr*)&engineServerAdress, sizeof(engineServerAdress)) == -1) {
        if (VERBOSE)
            fputs_unlocked("No fue posible conectar con el motor de busqueda.\n", stderr);
        close(cacheServerSocket);
        close(engineServerSocket);
        return -1;
    }

// --------------------------------------------------------------------------
// ------------------------------ PROCESO -----------------------------------

    // Seteamos nuestras variables
    char buffer[1024];   // Buffer para obtener los elementos desde busqueda o motor de busqueda
    std::string message; // variable donde se guardan los mensajes recibidos desde el proceso de busqueda
    std::string answer;  // variable donde se guardan las respuestas desde el motor de busqueda

    std::queue<std::string> answers;                    // Cola de respuestas para manejar las eliminaciones en el hashmap
    std::unordered_map<std::string, std::string> cache; // Hashmap que funciona como cache
    short int limit = atoi(argv[1]);                    // limite de elementos a agregar.

    // ARREGLAR SALIR AHORA
    while(message != "SALIR AHORA\n"){

        // Recibir mensaje desde cacheSocket
        int bytesRead = recv(busquedaSocket, buffer, sizeof(buffer), 0);
        if (bytesRead == -1 && VERBOSE) {
            fputs_unlocked("Error al recibir datos del cliente.\n", stderr);
        } 
        else {
            if (VERBOSE){
                buffer[bytesRead] = '\0';
                printf("Mensaje del cliente: %s\n", buffer); 
            }
        }

        message = buffer;

        answer = getAnswerFromHash(message, cache);
        if (answer.size() == 0){ // Si el mensaje no existia en cache lo tenemos que buscar
            if (VERBOSE)
                printf("No existe la palabra %s en cache\n", message.c_str());
            // Enviar datos al engineSocket
            if (send(engineServerSocket, (message + "\n").c_str(), strlen(message.c_str()), 0) == -1) {
                if (VERBOSE)
                    printf("No fue posible enviar el mensaje");
                close(engineServerSocket);
                return -1;
            }
            
            // Recibir datos desde engineSocket
            ssize_t bytesReceived = recv(engineServerSocket, buffer, sizeof(buffer) - 1, 0); // -1 para reservar espacio para el terminador nulo
            if (VERBOSE){
                if (bytesReceived < 0) 
                    std::cout << "Error al recibir datos del servidor" << std::endl;
                
                else {
                    buffer[bytesReceived] = '\0'; // Asegurar que el buffer esté terminado en nulo
                    std::cout << "Respuesta del servidor: " << buffer << std::endl; // Imprimir la respuesta
                }
            }
            answer = buffer; // Almacenamos la respuesta
            insertNewResultToHash(message, answer, answers, cache, limit);
        }

        // Enviar la respuesta a cacheServerSocket
        if (send(busquedaSocket, (answer + "\n").c_str(), strlen(answer.c_str()), 0) == -1 && VERBOSE) {
            fputs_unlocked("No fue posible enviar la respuesta", stderr);
        }
    }
    // Cerrar el socket del cliente
    close(cacheServerSocket);
    close(engineServerSocket);
    close(busquedaSocket);

    return 0;
}

std::string getAnswerFromHash(const std::string& message, const std::unordered_map<std::string, std::string>& cache){
    auto it = cache.find(message);
    if (it != cache.end()) 
        return it->second;

    return "";
}

void insertNewResultToHash(const std::string &message, const std::string &answer, std::queue<std::string> &answers, std::unordered_map<std::string, std::string> &cache, const short int &limit){
    if (answers.size() <= limit){   // Si el tamano de la cola es menor al limite, no es necesario borrar nada.
        answers.push(message);      // Agregamos el mensaje a la cola
        cache[message] = answer;    // Creamos una nueva clave-valor (message-answer)
        return;
    }

    std::string older = answers.front(); // Obtenemos la respuesta mas antigua
    cache.erase(older);                  // Borramos la clave del hashmap
    cache[message] = answer;             // Creamos una nueva clave-valor (message-answer)
    answers.pop();                       // Eliminamos el mensaje mas antigua
    answers.push(message);               // Agregamos el mensaje mas nuevo
}