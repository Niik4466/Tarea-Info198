#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <sstream>

void executeTextCounterThreads(const std::string &textCounterThreadsPath, const std::string &archivosDir, const std::string &outputDir, const std::string &idPath, const std::string &extension, const std::string &stopWordsPath, int threadCount, std::ofstream &csvFile) {
    // Construir el comando
    std::string command = textCounterThreadsPath + "/text_counter_threads " 
                          + archivosDir + " " + outputDir + " " 
                          + idPath + " " + extension + " " 
                          + stopWordsPath + " " + std::to_string(threadCount)
                          + " > /dev/null 2>&1"; 
    
    // Medir el tiempo de ejecución
    auto start = std::chrono::high_resolution_clock::now();
    int result = std::system(command.c_str());
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // Escribir los datos en el archivo CSV
    csvFile << threadCount << "," << elapsed.count() << "," << result << std::endl;
}

int main(int argc, char *argv[]) {
    // Verificación de número de argumentos
    if (argc != 12) {
        std::cerr << "Uso: " << argv[0] << " <textCounterThreadsPath> <archivosDir> <outputDir> <idPath> <extension> <stopWordsPath> <repetitions> <datosPath> <analizerPath> <graphPath> <threadCount>" << std::endl;
        return EXIT_FAILURE;
    }

    // Obtener los argumentos
    std::string textCounterThreadsPath = argv[1];
    std::string archivosDir = argv[2];
    std::string outputDir = argv[3];
    std::string idPath = argv[4];
    std::string extension = argv[5];
    std::string stopWordsPath = argv[6];
    std::string repetitions = argv[7];
    std::string datosPath = argv[8];
    std::string analyzerPath = argv[9];
    std::string graphPath = argv[10];
    std::string threadCount = argv[11];

    // Crear un vector con los valores de threads
    std::vector<int> hilosArray;
    size_t pos = 0;
    while ((pos = threadCount.find(',')) != std::string::npos) {
        hilosArray.push_back(std::stoi(threadCount.substr(0, pos)));
        threadCount.erase(0, pos + 1);
    }
    hilosArray.push_back(std::stoi(threadCount)); // Añadir el último número

    // Obtener la fecha y hora actuales
    std::time_t now = std::time(nullptr);
    char buf[80];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", std::localtime(&now));
    std::string timestamp(buf);

    // Crear el nombre del archivo CSV con la fecha y hora
    std::string csvFileName = datosPath + "/execution_times_" + timestamp + ".csv";

    // Abrir el archivo CSV para escribir los resultados
    std::ofstream csvFile(csvFileName);
    if (!csvFile.is_open()) {
        std::cerr << "No se pudo abrir el archivo CSV para escribir." << std::endl;
        return 1;
    }

    // Escribir la cabecera del archivo CSV
    csvFile << "Threads,Time,ExitCode" << std::endl;
    int repeticiones = std::stoi(repetitions);
    // Ejecutar el proceso el número de veces que repeticiones lo indique para cada valor en array_threads
    for (int threads : hilosArray) {
        for (int i = 0; i < repeticiones; ++i) {
            executeTextCounterThreads(textCounterThreadsPath, archivosDir, outputDir, idPath, extension, stopWordsPath, threads, csvFile);
        }
    }

    // Cerrar el archivo CSV
    csvFile.close();

    // Llamar al script Python para generar el gráfico
    std::ostringstream oss;
    oss << "python3 " << analyzerPath << "/Analisador.py " << csvFileName << " " << graphPath;
    std::string command = oss.str();
    int result = std::system(command.c_str());
    if (result != 0) {
        std::cerr << "Error al ejecutar el script Python." << std::endl;
        return 1;
    }
    return EXIT_SUCCESS;
}

