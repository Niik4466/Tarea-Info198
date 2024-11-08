CXX = g++
CXXFLAGS = -Wall -lncurses
MODULES_DIR = ../../modules/src
INCLUDE_DIR = ../../modules/include
LOCAL_INCLUDE_DIR = include
OBJ_DIR = out

# Archivos fuente
SRCS = src/buscador.cpp
SRCS += $(MODULES_DIR)/exeptions.cpp $(MODULES_DIR)/stringManagment.cpp $(MODULES_DIR)/users.cpp $(MODULES_DIR)/utils.cpp
OBJS = $(OBJ_DIR)/buscador.o $(OBJ_DIR)/exeptions.o $(OBJ_DIR)/stringManagment.o $(OBJ_DIR)/users.o $(OBJ_DIR)/utils.o

# Nombre del ejecutable
EXEC = buscador

# Compilar todo
all: $(EXEC)

# Regla para crear el ejecutable
$(EXEC): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(CXXFLAGS)

# Compilar los archivos fuente en objetos
$(OBJ_DIR)/%.o: src/%.cpp | $(OBJ_DIR)
	$(CXX) -c $< -o $@ -I$(INCLUDE_DIR) -I$(LOCAL_INCLUDE_DIR)

$(OBJ_DIR)/%.o: $(MODULES_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) -c $< -o $@ -I$(INCLUDE_DIR)

# Crear el directorio de objetos si no existe
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Regla de limpieza
clean:
	rm -f $(OBJ_DIR)/*.o $(EXEC)