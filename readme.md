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
         - El programa llamado es textCounter, el cual exige al usuario ingresar la extension de los archivos a leer, ruta donde se encuentra su carpeta de archivos y ruta en donde se guardaran los archivos.
         - Si no hay archivos entonces no hace nada, si un texto esta vacio o no contiene palabras. Entonces no guarda el conteo de palabras e indica que hay 0 palabras.

2) De administrador:
  
    1. Ingresar nuevo usuario:
        - Crea un nuevo usuario con los datos: nombre, contrasenia y rol de usuario.
      
    2. Listar usuarios:
        - Muestra el nombre de los usuarios disponibles.

    3. Eliminar Usuarios:
        - Elimina un usuario teniendo el nombre del usuario, solo se pueden eliminar usuarios genericos.
