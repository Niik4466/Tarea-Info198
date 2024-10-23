#include <iostream>
#include <future> // para el uso de std::async
#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>    // Para leer instrucciones.txt
#include <filesystem> // Para verificar que archivos existan rapidamente

void distributorCall(std::string distributor_path, std::string message, int id_core, std::string path_results, std::string path_core) {
  
  message = "(" + std::to_string(id_core) + ":" + message + ")";
  std::string call = distributor_path + " \"" + message + "\" " + path_results + " " + path_core;
  system(call.c_str());
}

int main(int argc, char** argv) {
  if (argc != 6) {
    printf("No se dieron los argumentos suficientes.\nEl programa debe ejecutarse como ./planificador <instructions_path> <distributor_path> <results_path> <core_path> <cant_cores>\n");
    return 1;
  }

  // Recibimos las variables desde argumentos
  std::string instructions_path = argv[1];
  std::string distributor_path = argv[2];
  std::string results_path = argv[3];
  std::string core_path = argv[4];
  int cant_cores = atoi(argv[5]);

  // Creamos el vector de path para la cantidad de cores a ejecutar
  std::vector<std::string> cores_file_paths(cant_cores);
  for (int i = 0; i < cant_cores; i++) {
    cores_file_paths.at(i) = core_path + "/." + std::to_string(i);
  }

  // Borramos el archivo anterior que almacenaba resultados (si existía)
  system(("rm " + results_path + " 2> /dev/null").c_str());

  // Borramos los archivos temporales

  // Abrimos el archivo de instrucciones
  std::ifstream instructions_file(instructions_path);
  if (!instructions_file.is_open()) {
    printf("No fue posible abrir el archivo de instrucciones");
    return 1;
  }

  // String donde se almacenan las lineas
  std::string line;
  // Vector de futuros. Almacenan la informacion de un thread creado por std::async
  std::vector<std::future<void>> futures(cant_cores);

  // Agregamos las primeras
  for (int i = 0; i < cant_cores && !instructions_file.eof(); i++) {
    std::getline(instructions_file, line);
    futures.at(i) = std::async(std::launch::async, distributorCall, distributor_path, line, i, results_path, core_path);
  }

  // Mientras el archivo no se nos haya acabado
  while (!instructions_file.eof()) {
    // Ejecutaremos instrucciones para cada core
    for (int i = 0; i < cant_cores && !instructions_file.eof(); i++) {
      if (!std::filesystem::exists(cores_file_paths.at(i)) && futures.at(i).wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
        // Obtenemos una linea del archivo.
        std::getline(instructions_file, line);
        // Ejecutamos una instrucción de manera asíncrona.
        futures.at(i) = std::async(std::launch::async, distributorCall, distributor_path, line, i, results_path, core_path);
      }
    }
  }

  // Esperamos a que todas las tareas se completen
  for (auto& future : futures) {
    future.get();
  }

  printf("%s\n", results_path.c_str());

  // Programa ejecutado con éxito!!
  return 0;
}
