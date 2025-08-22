# Makefile para Laboratorio 1 - Sistemas Operativos
# Organizador de Archivos Siberia
# Estudiantes: Cornejo & Lobos

#Compilador y banderas
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = LAB1_Cornejo_Lobos
SRC = $(TARGET).cpp

#Regla principal
all: $(TARGET)

$(TARGET): $(SRC)
	@echo "[...] Compilando $(SRC)..."
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) -lstdc++fs
	@echo "[✓] Compilacion exitosa. Ejecutable: $(TARGET)"

#Ejecutar el programa
run: $(TARGET)
	@echo "Ejecutando organizador de archivos Siberia..."
	@if [ -d "Input" ]; then \
		./$(TARGET) Input/; \
	else \
		echo "[✖] Error: No existe la carpeta 'Input'"; \
		echo "[i] Crea la carpeta Input/ y coloca los archivos del ZIP ahi"; \
		echo "[i] O ejecuta 'make prepare' para crear la estructura"; \
	fi

#Limpiar archivos generados
clean:
	@echo "Limpiando archivos generados..."
	rm -f $(TARGET)
	@echo "Ejecutable eliminado."

#Crear estructura de directorios
prepare:
	@echo "Preparando estructura del proyecto..."
	mkdir -p Input/
	@echo "Carpeta Input/ creada."
	@echo "Ahora coloca aqui los archivos extraidos del ZIP"

#Mostrar ayuda
help:
	@echo "MAKEFILE - LABORATORIO SISTEMAS OPERATIVOS"
	@echo "=========================================="
	@echo "COMANDOS DISPONIBLES:"
	@echo "  make          - Compila el programa"
	@echo "  make run      - Ejecuta el programa con Input/"
	@echo "  make clean    - Elimina el ejecutable"
	@echo "  make prepare  - Crea carpeta Input/"
	@echo "  make help     - Muestra esta ayuda"

.PHONY: all run clean prepare help