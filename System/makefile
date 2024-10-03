CXX = g++
## Se utiliza -lncurses para compilar con ncurses.
CXXFLAGS = -Wall -lncurses
MODULES_DIR = ../modules/src
INCLUDE_DIR = ../modules/include
LOCAL_INCLUDE_DIR = include
OBJ_DIR = out

# Archivos fuente
SRCS = src/main.cpp src/interface.cpp $(MODULES_DIR)/exeptions.cpp $(MODULES_DIR)/stringManagment.cpp $(MODULES_DIR)/users.cpp $(MODULES_DIR)/utils.cpp
OBJS = $(OBJ_DIR)/main.o $(OBJ_DIR)/interface.o $(OBJ_DIR)/exeptions.o $(OBJ_DIR)/stringManagment.o $(OBJ_DIR)/users.o $(OBJ_DIR)/utils.o

# Nombre del ejecutable
EXEC = ../system

# Compilar todo
all: $(EXEC)

# Regla para crear el ejecutable
$(EXEC): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(CXXFLAGS)

# Compilar los archivos fuente en objetos
$(OBJ_DIR)/%.o: src/%.cpp
	$(CXX) -c $< -o $@ -I$(INCLUDE_DIR) -I$(LOCAL_INCLUDE_DIR)

$(OBJ_DIR)/%.o: $(MODULES_DIR)/%.cpp
	$(CXX) -c $< -o $@ -I$(INCLUDE_DIR)

# Limpiar los archivos objeto
clean:
	rm -f $(OBJS) $(EXEC)
