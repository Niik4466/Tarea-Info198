# Proyecto de Sistemas Operativos
## Descripcion
Ofrece una interfaz de usuario y manejo de cuentas, con funcionalidades básicas de un sistema operativo. El programa genera procesos hijos, ejecutando específicamente el proceso `textCounter` para contar las palabras de un texto.

## Requisitos
* **Sistema Operativo**: Linux, macOS.
* **Compilador**: G++ en su version 14.2.1
* **Librerías**:
  - **ncurses**: Para manejo de interfaces de texto en la terminal.
  - **C++ Standard Library**: Incluye `<iostream>`, `<fstream>`, `<map>`, `<string>`, `<cstdlib>`, `<vector>`, `<regex>`, `<unistd.h>`, y `<cstring>`.


## Instalacion / Compilacion
* **Instalar las dependencias**:
   - En **Linux (Debian/Ubuntu)**:
     ```bash
     sudo apt-get update
     sudo apt-get install libncurses5-dev libncursesw5-dev
     ```
   - En **Linux (Arch Linux)**:
     ```bash
     sudo pacman -Sy ncurses
     ```
   - En **macOS** (con Homebrew):
     ```bash
     brew install ncurses
     ```

* **Compilar el programa**:

   - Se ejecuta como
   ```bash
   make
   ```
## Ejecucion
  * Se compila desde la carpeta raiz como:
    ```bash
    /.prg -u <usuario> -p <clave> -t <frase> -v <vector> -n <numero>
    ```

| Argumento | Descripcion | Formato |
| --------  | ----------- | -------- |
| u | Nombre de usuario  | Min 3 caracteres |
| p | Clave del usuario  | Min 6 caracteres o numeros  |
| t | Frase a trabajar   | Se ingresa como "*Frase*"   |
| v | Vector de numeros  | Se ingresa como "n;n;...;n" |
| n | Numero             | Se ingresa como *Num*  Entero o decimal |

## Funciones

- Las funcionalidades del programa se dividen en 2 secciones

1) De usuario:
  
     1. Deteccion de palindromos
        - Detecta si la frase -t ingresada por el usuario es palindrome
     2. Contar vocales
        - Cuenta las vocales de la frase -t ingresada por el usuario
          
     3. Cantidad de letras de un texto
        - Cuenta la cantidad de letras de un texto -t ingresado por el usuario

     4. Promedio y sumatoria de un vector
        - Calcula el promedio y la sumatoria del vector -v ingresado por el usuario

     5. Calcular f(x)=5x*x+1/x:
        - Calcula la funcion f(x) con el numero -n ingresado por el usuario

     6. Contar palabras:
         - Crea un proceso externo el cual se encarga de contar las palabras de los archivos pertenecientes a una carpeta seleccionada por el usuario.
         - El programa llamado es `textCounter`, el cual exige al usuario ingresar la extension de los archivos a leer, ruta donde se encuentra su carpeta de archivos y ruta en donde se guardaran los archivos.
         - Si no hay archivos entonces no hace nada, si un texto esta vacio o no contiene palabras. Entonces no guarda el conteo de palabras e indica que hay 0 palabras.

2) De administrador:
  
   1. Ingresar nuevo usuario:
      - Crea un nuevo usuario con los datos: nombre, contrasenia y rol de usuario.
     
   2. Listar usuarios:
      - Muestra el nombre de los usuarios disponibles.

   3. Eliminar Usuarios:
      - Elimina un usuario teniendo el nombre del usuario, solo se pueden eliminar usuarios genericos.

   4. Conteo paralelo con threads:
      - Ejecuta el programa `Text_counter_threads` en paralelo para contar palabras en archivos de texto utilizando múltiples threads. Requiere la extensión de los archivos, el directorio de archivos, y la cantidad de threads a utilizar.

   5. Crear indice invertido:
      - Ejecuta el programa `Inverted_Index` para crear un índice invertido de los archivos de texto procesados. Requiere la extensión de los archivos, que previamente se haya ejecutado la opcion "Conteo paralelo con threads", el directorio de archivos, y el directorio de salida para los resultados.
      
 ### Variables de Entorno (.env)

   * El archivo `.env` contiene las siguientes variables de entorno que configuran el comportamiento del programa:

      - **HEIGHT**: Altura de la pantalla (por defecto: `DEFAULT`).
      - **WIDTH**: Ancho de la pantalla (por defecto: `DEFAULT`).
      - **SEP**: Separador de vectores (por defecto: `;`).
      - **USER_PATH**: Ruta al archivo de usuarios.
      - **TXTCOUNTER_PATH**: Ruta al programa `Text_counter`.
      - **TEMP_PATH**: Ruta al directorio temporal.
      - **PARALLEL_COUNT_PATH**: Ruta al programa `Text_counter_Threads`.
      - **INVERTED_INDEX_PATH**: Ruta al programa `Inverted_Index`.
      - **EXTENSION**: Extensión de los archivos a procesar (por defecto: `.txt`).
      - **ARCHIVOS_DIR**: Directorio de archivos a procesar.
      - **CANTIDAD_THREAD**: Cantidad de threads a utilizar (por defecto: `4`).
      - **MAPA_ARCHIVO**: Ruta al archivo de ID de threads.
      - **STOP_WORD**: Ruta al archivo de palabras de parada.
      - **INVERTED_INDEX**: Ruta al índice invertido.
      - **OUTPUT_DIR**: Directorio de salida para los resultados de los threads.
      - **DATOS_PATH**: Ruta al directorio de datos de rendimiento.
      - **GRAFICO_PATH**: Ruta al directorio de gráficos de rendimiento.
      - **EJECUTADOR_PATH**: Ruta al programa `Ejecutador`.
      - **ANALISADOR_PATH**: Ruta al programa `Analisador`.
      - **ARRAY_THREADS**: Array de cantidades de threads para pruebas de rendimiento.
      - **REPETICIONES**: Número de repeticiones para pruebas de rendimiento.
      ### Programas Externos

      El programa utiliza los siguientes programas externos para realizar tareas específicas:

      1. **Text_counter**:
         - **Descripción**: Cuenta las palabras de los archivos de texto en un directorio especificado.
         - **Ruta**: Definida en `TXTCOUNTER_PATH`.
         - **Uso**: Se ejecuta como un proceso hijo para contar palabras en archivos de texto.

      2. **Text_counter_Threads**:
         - **Descripción**: Cuenta las palabras de los archivos de texto en paralelo utilizando múltiples threads.
         - **Ruta**: Definida en `PARALLEL_COUNT_PATH`.
         - **Uso**: Se ejecuta en paralelo para mejorar el rendimiento en el conteo de palabras.
      
      3. **Inverted_Index**:
         - **Descripción**: Crea un índice invertido de los archivos de texto procesados.
         - **Ruta**: Definida en `INVERTED_INDEX_PATH`.
         - **Uso**: Se ejecuta para generar un índice invertido que permite búsquedas rápidas de palabras en los archivos de texto.
