#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <random>
#include <algorithm>  // Para usar std::max_element

// Estructura de un punto en el tablero
struct Point {
    int x;
    int y;
};

// Estructura de un Samurai
struct Samurai {
    int ID;
    int edad;
    int probabilidadSupervivencia;
    int generacionesEsperadas;
    int inteligenciaEmocional;
    int condicionFisica;
    int fuerzaTroncoSuperior;
    int fuerzaTroncoInferior;
    int resistencia;
};

// Función para inicializar una población de samurais con atributos aleatorios
std::vector<Samurai> inicializarPoblacion(int numSamurais) {
    std::vector<Samurai> poblacion;
    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = 0; i < numSamurais; ++i) {
        Samurai samurai;
        samurai.ID = i;
        samurai.edad = std::uniform_int_distribution<int>(20, 60)(gen);
        samurai.probabilidadSupervivencia = std::uniform_int_distribution<int>(0, 100)(gen);
        samurai.generacionesEsperadas = std::uniform_int_distribution<int>(1, 5)(gen);
        samurai.inteligenciaEmocional = std::uniform_int_distribution<int>(1, 10)(gen);
        samurai.condicionFisica = std::uniform_int_distribution<int>(1, 10)(gen);
        samurai.fuerzaTroncoSuperior = std::uniform_int_distribution<int>(1, 10)(gen);
        samurai.fuerzaTroncoInferior = std::uniform_int_distribution<int>(1, 10)(gen);
        samurai.resistencia = std::uniform_int_distribution<int>(1, 10)(gen);

        poblacion.push_back(samurai);
    }

    return poblacion;
}

// Función para encontrar el mejor samurái de una población basado en todos los atributos
Samurai encontrarMejorSamurai(const std::vector<Samurai>& poblacion) {
    auto mejorSamurai = std::max_element(poblacion.begin(), poblacion.end(),
        [](const Samurai& a, const Samurai& b) {
            // Calcular el puntaje de cada samurái basado en todos los atributos
            int puntajeA = a.probabilidadSupervivencia + a.generacionesEsperadas +
                           a.inteligenciaEmocional + a.condicionFisica +
                           a.fuerzaTroncoSuperior + a.fuerzaTroncoInferior + a.resistencia;

            int puntajeB = b.probabilidadSupervivencia + b.generacionesEsperadas +
                           b.inteligenciaEmocional + b.condicionFisica +
                           b.fuerzaTroncoSuperior + b.fuerzaTroncoInferior + b.resistencia;

            // Diferencia en la edad: Menos edad tiene una ventaja
            int diferenciaEdad = a.edad - b.edad;

            // Si ambos tienen la misma diferencia de edad, selecciona el de mayor puntaje
            if (diferenciaEdad == 0) {
                return puntajeA < puntajeB;
            }

            // De lo contrario, selecciona el que tiene la menor diferencia de edad
            return diferenciaEdad < 0;
        });
    return *mejorSamurai;
}

void imprimirPoblacion(const std::vector<Samurai>& poblacion) {
    for (const Samurai& samurai : poblacion) {
        std::cout << "Samurai ID: " << samurai.ID << "\n";
        std::cout << "Edad: " << samurai.edad << "\n";
        std::cout << "Probabilidad de supervivencia: " << samurai.probabilidadSupervivencia << "\n";
        std::cout << "Generaciones esperadas: " << samurai.generacionesEsperadas << "\n";
        std::cout << "Inteligencia Emocional: " << samurai.inteligenciaEmocional << "\n";
        std::cout << "Condición Física: " << samurai.condicionFisica << "\n";
        std::cout << "Fuerza Tronco Superior: " << samurai.fuerzaTroncoSuperior << "\n";
        std::cout << "Fuerza Tronco Inferior: " << samurai.fuerzaTroncoInferior << "\n";
        std::cout << "Resistencia: " << samurai.resistencia << "\n";
        std::cout << "\n";
    }
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 5);

    // Inicializar dos poblaciones de samurais
    std::vector<Samurai> poblacion1 = inicializarPoblacion(5);
    std::vector<Samurai> poblacion2 = inicializarPoblacion(5);

    // Imprimir las poblaciones
    std::cout << "Población 1:\n";
    imprimirPoblacion(poblacion1);

    std::cout << "Población 2:\n";
    imprimirPoblacion(poblacion2);

    // Encontrar y mostrar el mejor samurái de cada población
    Samurai mejorSamuraiPoblacion1 = encontrarMejorSamurai(poblacion1);
    Samurai mejorSamuraiPoblacion2 = encontrarMejorSamurai(poblacion2);

    std::cout << "Mejor samurái de la población 1:\n";
    imprimirPoblacion({mejorSamuraiPoblacion1});

    std::cout << "Mejor samurái de la población 2:\n";
    imprimirPoblacion({mejorSamuraiPoblacion2});

    while (true) {
        int clientSocket = accept(serverSocket, NULL, NULL);

        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));

        recv(clientSocket, buffer, sizeof(buffer), 0);

        std::string data(buffer);

        std::cout << "Datos recibidos del cliente: " << data << std::endl;

        close(clientSocket);
    }

    return 0;
}