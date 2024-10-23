# Variables
CXX = g++
CXXFLAGS = -Wall -std=c++17
CORE_SRC = core/core.cpp
PLANIFICADOR_SRC = planificador/planificador.cpp
CORE_TARGET = core/core
PLANIFICADOR_TARGET = planificador/planificador

# Objetivos
all: $(CORE_TARGET) $(PLANIFICADOR_TARGET)

$(CORE_TARGET): $(CORE_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(PLANIFICADOR_TARGET): $(PLANIFICADOR_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(CORE_TARGET) $(PLANIFICADOR_TARGET)

.PHONY: all clean
