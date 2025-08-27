# Laboratorio 1 - Sistemas Operativos
## Organizador de Archivos Siberia

### Integrantes:
- Miguel Cornejo, Rut: 21372437-1
- Diego Lobos, Rut: 21799105-6

### Descripción:
Programa en C++ que clasifica y organiza archivos encontrados en una expedición arqueológica en Siberia según tres categorías: Sistema, Pistas y Basura, cumpliendo con los requisitos especificados en el PDF del laboratorio.

### Bibliotecas Utilizadas:
- **iostream**: Para operaciones de entrada/salida y mensajes en consola
- **filesystem**: Para operaciones con archivos y directorios (recorrido, creación, movimiento)
- **fstream**: Para lectura y manipulación de archivos de texto
- **string**: Para manipulación de cadenas de texto
- **map**: Para almacenar y gestionar los contadores de cada categoría de archivos
- **algorithm**: Para transformación de texto a minúsculas (búsqueda insensible a mayúsculas)

### Especificación de Algoritmos y Desarrollo:

#### 1. Algoritmo de Clasificación por Extensión
- **Entrada**: Nombre de archivo con extensión
- **Proceso**: 
  - Extraer extensión del archivo
  - Convertir a minúsculas para comparación insensible a mayúsculas
  - Clasificar según extensión:
    - .sys → Drivers
    - .exe → Programs
    - .dll → DLLs
    - otros → Basura
- **Complejidad**: O(1) por archivo

#### 2. Algoritmo de Clasificación por Contenido (para archivos .txt)
- **Entrada**: Contenido completo del archivo
- **Proceso**:
  - Leer contenido línea por línea
  - Convertir cada línea a minúsculas
  - Buscar patrones específicos:
    - "tipo: log" Y "distancia:" → Pistas
    - "extension: .exe" → Programs
    - "extension: .sys" → Drivers
    - "extension: .dll" → DLLs
  - Si no encuentra patrones → Basura
- **Complejidad**: O(n) donde n es el número de líneas

#### 3. Algoritmo de Recorrido Recursivo de Directorios
- **Entrada**: Ruta del directorio raíz (Input/)
- **Proceso**:
  - Usar recursive_directory_iterator para recorrer todos los archivos y subdirectorios
  - Para cada entrada:
    - Si es directorio: incrementar contador de carpetas
    - Si es archivo: aplicar algoritmos de clasificación
- **Complejidad**: O(n) donde n es el número total de archivos y directorios

#### 4. Algoritmo de Renombrado y Movimiento
- **Entrada**: Archivo clasificado y categoría asignada
- **Proceso**:
  - Generar nuevo nombre basado en categoría y contador
  - Construir ruta de destino según estructura requerida
  - Mover archivo usando rename() del filesystem
- **Complejidad**: O(1) por archivo

#### Desarrollo Realizado:
- Implementado en C++17 utilizando la biblioteca de sistema de archivos
- Procesamiento recursivo de directorios anidados
- Sistema de contadores para renombrado automático
- Validación de entrada y manejo de errores
- Generación de estadísticas de procesamiento
- Creación automática de la estructura de carpetas de salida

### Estructura de Salida:
```
Output/
├── System/
│   ├── Drivers/                 # Archivos .sys
│   ├── Programs/                # Archivos .exe  
│   └── Dynamic Link-Library/    # Archivos .dll
├── Pistas/                      # Archivos de texto con información de log
└── Basura/                      # Archivos no clasificables
```
### Supuestos Utilizados:
1. Los archivos de sistema se identifican principalmente por su extensión (.sys, .exe, .dll)
2. Las pistas deben contener AMBOS patrones: "tipo: log" y "distancia:" como se muestra en el ejemplo del PDF
3. Solo se procesan archivos .txt para la clasificación por contenido
4. Los archivos se mueven (no copian) desde Input/ a Output/
5. Los nombres originales se pierden y se renombran con formato descriptivo
6. El programa debe ejecutarse en Linux con permisos adecuados
7. El directorio Input debe existir y contener los archivos a procesar

### Instrucciones de Compilación:
```
make
```
### Instrucciones de Ejecución:
```
make run
```
o
```
./LAB1_Cornejo_Lobos Input/
```
### Flujo de Trabajo Completo:
1. Compilar el programa: `make`
2. Preparar la estructura: `make prepare` (crea carpeta Input)
3. Extraer los archivos del ZIP proporcionado en la carpeta Input/
4. Ejecutar: `make run`
5. Ver resultados en la carpeta Output/

### Comandos del Makefile:
- `make` - Compila el programa
- `make run` - Ejecuta el programa con Input/
- `make clean` - Elimina el ejecutable
- `make prepare` - Crea carpeta Input/
- `make help` - Muestra ayuda de comandos

### Estructura del Proyecto:
```
Lab_1/
├── Input/                 # Archivos originales (proporcionados)
├── Output/                # Resultados generados (creado automáticamente)
├── LAB1_Cornejo_Lobos.cpp # Código fuente principal
├── Makefile              # Archivo de compilación
└── README.md             # Este archivo
```
### Requisitos del Sistema:
- Compilador C++17 (g++ 7 o superior)
- Sistema operativo Linux
- Biblioteca estándar de C++ con soporte para filesystem

### Consideraciones Importantes:
- ADVERTENCIA: Los archivos se mueven (no copian) de Input/ a Output/
- ADVERTENCIA: El directorio Input/ quedará vacío después de la ejecución, pero se mantienen carpetas anidadas
- Independiente si esta la carpeta o no se puede crear con `make prepare`, lo mismo con Output si no está al ejecutar el programa de crean los resultados de la carpeta
- Los archivos se organizan exactamente como se especifica en el PDF
- El programa genera estadísticas detalladas del procesamiento
- El código sigue todas las reglas y especificaciones del laboratorio
