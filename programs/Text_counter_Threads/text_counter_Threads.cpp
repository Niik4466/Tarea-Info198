#include "utils.h"
#include <thread>
#include <queue>
#include <vector>
#include <mutex>
#include <filesystem>

// Crea el queue de tareas a realizar por los threads
std::queue<std::string> createPathQueue(std::filesystem::path filesPath, std::string extension){
  std::queue<std::string> pathQueue;
  
  // Verifico si el archivo existe
  if (!std::filesystem::exists(filesPath) || !std::filesystem::is_directory(filesPath)){
    printf("El archivo dado no es un directorio/n");
    exit(EXIT_FAILURE);
  }

  // Itero por sobre los archivos
  for (const std::filesystem::directory_entry& file : std::filesystem::directory_iterator(filesPath)){
    // Verificamos que sea un archivo regular y que su extension sea la entregada desde argumento
    if (file.is_regular_file() && file.path().extension() == extension) {
      // Si se cumple la condicion, se encola el path
      pathQueue.push(file.path().string());
    }
  }

  return pathQueue;
}

// Define el trabajo que realizara cada thread (Aqui se encuentra el sacar datos del queue y el uso de mutex)
void threadWork(int id, std::queue<std::string>& pathQueue, std::mutex& pathQueueMutex,  std::string childrenPath, std::string outputPath, std::string idPath, std::string extension, std::string stopWordsPath){
  while(!pathQueue.empty()){
    std::string filePath;
    //Bloqueamos la cola mientras obtenemos el dato
    {
      std::lock_guard<std::mutex> lock(pathQueueMutex);
      // Si la cola no esta vacia obtenemos un elemento
      if (!pathQueue.empty()){
        filePath = pathQueue.front();
        pathQueue.pop();
      }
    }
    // Procesamos el elemento extraido
    textCount(childrenPath, filePath, outputPath, idPath, extension, stopWordsPath);
  }
}

// Trabajo principal del programa, recibe los argumentos necesarios para ser el conteo y le relega el trabajo a un script en bash
void textCount(std::string childrenPath, std::string filePath, std::string outputPath, std::string idPath, std::string extension, std::string stopWordsPath){
  std::string call = childrenPath + " " + filePath + " " + outputPath + " " + idPath + " " + extension + " " + stopWordsPath;
  int exit_value = system(call.c_str());
  if (exit_value != 0){
    printf("El conteo de $s no se pudo llevar a cabo debido a un error\n", filePath);
  }
}

int main(int argc, char** argv){
  // Seteamos las variables que vienen desde argumento
  std::filesystem::path thisExecutablePath = argv[0];
  std::string thisPath = thisExecutablePath.parent_path().string();
  std::string childrenPath = thisPath + "/children.sh";
  std::string createIDPath = thisPath + "/createID.sh";
  std::string filesPath = argv[1];
  std::string outputPath = argv[2];
  std::string idPath = argv[3];
  std::string extension = argv[4];
  std::string stopWordsPath = argv[5];
  int cantThreads = atoi(argv[6]);
  
  // Creamos las ID's antes de realizar el procesamiento paralelo
  std::string call = createIDPath + " " + extension + " " + filesPath + " " + idPath;

  // Creamos nuestra cola de archivos paths a procesar.
  std::queue<std::string> pathQueue = createPathQueue(filesPath, extension);
  // Creamos el mutex para el acceso a la cola
  std::mutex pathQueueMutex;

  // Creamos los threads
  std::vector<std::thread> threads;
  
  for (int i = 0; i < cantThreads; i++)
    threads.emplace_back(threadWork, i, std::ref(pathQueue), std::ref(pathQueueMutex), childrenPath, outputPath, idPath, extension, stopWordsPath);

  for (std::thread& thread : threads)
    if (thread.joinable()) thread.join();

  return 0;
}
