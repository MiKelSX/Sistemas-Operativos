//Bibliotecas necesarias y usadas
#include <iostream>     //Para cout, cerr, endl
#include <filesystem>   //Para namespace fs
#include <fstream>      //Para ifstream en LeerConteArchivo()
#include <string>       //Para manipulación de strings
#include <map>         //Para el map 'contadores'
#include <algorithm>    //Usada para transform() en minúsculas

namespace fs = std::filesystem; //Alias para usar 'fs' en lugar de 'std::filesystem'
using namespace std; //Permite usar funciones estándar sin 'std::'

//======================== VARIABLES GLOBALES ========================
//Diccionario que guarda contadores para cada tipo de archivo
//Cada vez que se encuentra un archivo, se incrementa el contador correspondiente
map<string, int> contadores = {
    {"pista",1},{"driver",1},{"program",1},{"dll",1},{"basura",1}};

//Estructura para almacenar estadísticas del procesamiento
struct Estadisticas {
    int total_Arch=0, txt_Arch=0, pistas=0, drivers=0, programs=0, dlls=0, basura=0, carpetas_escans=0;
} estadis;

//======================== CREACIÓN DE CARPETAS ========================
/*Función createOutputFolders: Crea la estructura de carpetas necesaria en el directorio de salida
outputPath: Ruta donde se crearán las carpetas*/
void createOutputFolders(const string &outputPath) {
    try {
        //Crea todas las carpetas necesarias para organizar los archivos
        fs::create_directories(outputPath + "/System/Drivers");
        fs::create_directories(outputPath + "/System/Programs");
        fs::create_directories(outputPath + "/System/Dynamic Link-Library");
        fs::create_directories(outputPath + "/Pistas");
        fs::create_directories(outputPath + "/Basura");
        cout << "[✓] Estructura de carpetas creada en: " << outputPath << "/" << endl;
    } catch (const exception& e) {
        //Si hay un error al crear las carpetas, muestra el error y termina el programa
        cerr << "[✖] Error creando carpetas: " << e.what() << endl;
        exit(1); //Termina el programa si no se pueden crear las carpetas
    }
}

//======================== LECTURA DE ARCHIVOS ========================
/*Función LeerConteArchivo: Lee el contenido de un archivo y lo convierte a minúsculas
 Retorno: String con el contenido del archivo en minúsculas */
string LeerConteArchivo(const string &filepath) { //filepath: Ruta del archivo a leer
    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "[!]  No se pudo abrir: " << filepath << endl;
        return ""; //Retorna cadena vacía si no se pudo abrir el archivo
    }
    string line, content;
    while (getline(file, line)) {
        //Convierte cada línea a minúsculas
        transform(line.begin(), line.end(), line.begin(), ::tolower);
        content += line + "\n";
    }
    return content;
}

//======================== CLASIFICACIÓN ========================
//Función ClassificarArchivo: Determina la categoría de un archivo según su contenido
string ClassificarArchivo(const string &filepath) {
    string content = LeerConteArchivo(filepath); //filepath: Ruta del archivo a clasificar
    if (content.empty()) return "basura";

    //Pistas: deben tener "tipo: log" y "distancia:"
    //Reglas de clasificación según el contenido del archivo
    //Retorno String con la categoría del archivo
    if (content.find("tipo: log") != string::npos && content.find("distancia:") != string::npos) return "pista";
    if (content.find("extension: .exe") != string::npos) return "program";
    if (content.find("extension: .sys") != string::npos) return "driver";
    if (content.find("extension: .dll") != string::npos) return "dll";
    return "basura";
}

//======================== MOVER Y RENOMBRAR ========================
/*Función MoverYRenombrar: Mueve y renombra un archivo según su categoría
Parámetros:
 -filepath: Ruta original del archivo
 -category: Categoría asignada al archivo
 -outputPath: Ruta base donde se moverá el archivo
 -Retorno bool: true si se movió correctamente, false si hubo error */
bool MoverYRenombrar(const string &filepath, const string &category, const string &outputPath) {
    string extension, NewNombre, destPath;
    //Determinar nombre y extensión final según categoría
    //FLUJO: Si, Sino categoria esta "___", crea nombre ___X.txt y lo moverá a carpeta ___/
    if (category == "pista") { NewNombre="pista"+to_string(contadores[category]++)+".txt"; destPath=outputPath+"/Pistas/"+NewNombre; estadis.pistas++; }
    else if (category == "driver") { NewNombre="driver"+to_string(contadores[category]++)+".sys"; destPath=outputPath+"/System/Drivers/"+NewNombre; estadis.drivers++; }
    else if (category == "program") { NewNombre="program"+to_string(contadores[category]++)+".exe"; destPath=outputPath+"/System/Programs/"+NewNombre; estadis.programs++; }
    else if (category == "dll") { NewNombre="dll"+to_string(contadores[category]++)+".dll"; destPath=outputPath+"/System/Dynamic Link-Library/"+NewNombre; estadis.dlls++; }
    else { NewNombre="basura"+to_string(contadores[category]++)+".txt"; destPath=outputPath+"/Basura/"+NewNombre; estadis.basura++; }

    try {
        fs::rename(filepath, destPath); //mover archivo
        cout << "📄 " << fs::path(filepath).filename().string() << " -> " << category << " (" << NewNombre << ") [✓] MOVIDO" << endl;
        return true; //Retorna true si se movió correctamente
    } catch (const exception& e) {
        cerr << "[✖] Error moviendo " << filepath << ": " << e.what() << endl;
        return false; //Retorna false si hubo un error al mover el archivo
    }
}

//======================== PROCESAMIENTO PRINCIPAL ========================
/*Función ProcesarDirect procesa todos los archivos en un directorio y sus subdirectorios
-inputPath: Ruta del directorio a procesar
-outputPath: Ruta donde se organizarán los archivos */
void ProcesarDirect(const string &inputPath, const string &outputPath) {
    cout << "[...] Escaneando directorio: " << inputPath << endl;
    cout << "=================================================" << endl;

    try {
        for (const auto &entrada : fs::recursive_directory_iterator(inputPath)) {
            estadis.total_Arch++;
            //Si es un archivo, clasifícalo y muévelo
            if (entrada.is_directory()) { estadis.carpetas_escans++; cout << "📁 Carpeta: " << entrada.path().filename().string() << endl; }
            // Sino es un archivo regular, verifica su extensión y clasifícalo
            else if (entrada.is_regular_file()) {
                string extension = entrada.path().extension().string();
                transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
                // Si es un archivo .txt, clasifícalo y muévelo
                if (extension==".txt") { estadis.txt_Arch++; MoverYRenombrar(entrada.path().string(), ClassificarArchivo(entrada.path().string()), outputPath); }
                else { cout << "📄 Ignorado (no .txt): " << entrada.path().filename().string() << endl; }
            }
        }
    } catch (const exception& e) { cerr << "[✖] Error procesando directorio: " << e.what() << endl; } // Captura errores al procesar el directorio
}

//======================== ESTADÍSTICAS FINALES ========================
void MostrarEstadisticas() { //Muestra un resumen de todas las operaciones realizadas
    cout << endl;
    cout << "===============================================" << endl;
    cout << "              RESUMEN FINAL - SIBERIA" << endl;
    cout << "===============================================" << endl << endl;

    cout << "[~] ESTADÍSTICAS GENERALES:" << endl;
    cout << "   Total archivos encontrados: " << estadis.total_Arch << endl;
    cout << "   Carpetas escaneadas: " << estadis.carpetas_escans << endl;
    cout << "   Archivos .txt procesados: " << estadis.txt_Arch << endl;
    cout << "   [✓] Archivos MOVIDOS exitosamente de Input/ a Output/" << endl << endl;

    cout << "[~] CLASIFICACIÓN Y MOVIMIENTO:" << endl;
    cout << "   [#] Pistas (logs): " << estadis.pistas << " archivos movidos" << endl;
    cout << "   [#] Programas (.exe): " << estadis.programs << " archivos movidos" << endl;
    cout << "   [#] Drivers (.sys): " << estadis.drivers << " archivos movidos" << endl;
    cout << "   [#] DLLs (.dll): " << estadis.dlls << " archivos movidos" << endl;
    cout << "   [#]  Basura: " << estadis.basura << " archivos movidos" << endl << endl;

    cout << "[~] ARCHIVOS ORGANIZADOS EN:" << endl;
    cout << "   Output/System/Drivers/ -> " << estadis.drivers << " archivos" << endl;
    cout << "   Output/System/Programs/ -> " << estadis.programs << " archivos" << endl;
    cout << "   Output/System/Dynamic Link-Library/ -> " << estadis.dlls << " archivos" << endl;
    cout << "   Output/Pistas/ -> " << estadis.pistas << " archivos" << endl;
    cout << "   Output/Basura/ -> " << estadis.basura << " archivos" << endl << endl;

    cout << "[~] ESTADO DE INPUT/:" << endl;
    try {
        int remaining=0; 
        for (const auto &entrada : fs::recursive_directory_iterator("Input")) // Recorre todos los archivos en Input/
            // Si es un archivo regular y tiene extensión .txt, cuenta como restante
            if (entrada.is_regular_file() && entrada.path().extension()==".txt") remaining++;
        // Si no quedan archivos .txt, muestra mensaje de éxito
        if (remaining==0) cout << "   [✓] Input/ vacío de archivos .txt (todos fueron movidos)" << endl;
        else cout << "   [!]  Input/ aún contiene " << remaining << " archivos .txt sin procesar" << endl;
    } catch (...) { cout << "   [!]  No se pudo verificar estado de Input/" << endl; }

    if (estadis.txt_Arch>0) {
        cout << endl << "   [✓] PROCESO COMPLETADO - Todos los archivos han sido MOVIDOS y organizados!" << endl;
        cout << "   [i] Input/ ahora está vacío de archivos .txt" << endl;
        cout << "   [i] Todos los archivos están organizados en Output/" << endl;
    } else {
        cout << "   [!]  No se encontraron archivos .txt para procesar." << endl;
        cout << "   [i] Verifica que la carpeta Input/ contenga archivos .txt" << endl;
    }
}

//======================== VALIDACIÓN DE INPUT ========================
void validateInput(const string &inputPath) { //Verifica que el directorio de entrada exista y contenga archivos .txt
    if (!fs::exists(inputPath)) { cerr << "[✖] Error: El directorio '"<<inputPath<<"' no existe."<<endl; cout<<"[i] Asegúrate de que hayas extraído los archivos en la carpeta Input/"<<endl; exit(1); }
    if (!fs::is_directory(inputPath)) { cerr << "[✖] Error: '"<<inputPath<<"' no es un directorio."<<endl; exit(1); }

    bool tieneTxtxt=false;
    for (const auto &entrada : fs::recursive_directory_iterator(inputPath)) 
        if (entrada.is_regular_file() && entrada.path().extension()==".txt") { tieneTxtxt=true; break; }
    if (!tieneTxtxt) { cout<<"[!]  Advertencia: No se encontraron archivos .txt en "<<inputPath<<endl; cout<<"[i] Verifica que hayas extraído correctamente los archivos del ZIP"<<endl; }
}

//======================== MAIN (FUNCIÓN PRINCIPAL) ========================
int main(int argc, char* argv[]) {
    cout << "===================================================" << endl;
    cout << " _        _    ____     _           ____    ___   " << endl;
    cout << "| |      / \\  | __ )   / |         / ___|  / _ \\ " << endl;
    cout << "| |     / _ \\ |  _ \\   | |  _____  \\___   | | | | " << endl;
    cout << "| |___ / ___ \\| |_) |  | | |_____|  ___)  | |_| | " << endl;
    cout << "|_____/_/   \\_\\____/   |_|         |____/  \\___/  " << endl;
    cout << "                                                      " << endl;
    cout << "    ORGANIZADOR DE ARCHIVOS SIBERIA" << endl;
    cout << "    Estudiantes: Cornejo & Lobos" << endl;
    cout << "===============================================" << endl << endl;

    if (argc<2) {
        cerr << "[✖] Error: Uso incorrecto." << endl << endl;
        cout << "[*] USO CORRECTO:" << endl;
        cout << "   " << argv[0] << " Input/" << endl << endl;
        cout << "[i] INFORMACIÓN:" << endl;
        cout << "   - El programa busca archivos .txt en Input/ y subcarpetas" << endl;
        cout << "   - Clasifica según contenido y los organiza en Output/" << endl;
        cout << "   - Pistas: 'tipo: log' + 'distancia:'" << endl;
        cout << "   - Sistemas: 'extension: .exe/.sys/.dll'" << endl;
        return 1;
    }

    string inputPath=argv[1], outputPath="Output";
    cout<<"██▓▒­░ Análisis de archivos ░▒▓██"<<endl;
    cout<<"📂 Directorio de entrada: "<<fs::absolute(inputPath)<<endl;
    cout<<"📂 Directorio de salida: "<<fs::absolute(outputPath)<<endl<<endl;

    validateInput(inputPath); //Validar Input
    createOutputFolders(outputPath); //Crear estructura Output
    cout<<endl;
    ProcesarDirect(inputPath,outputPath); //Procesar archivos
    MostrarEstadisticas(); //Mostrar resumen

    return 0;
}