#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <random>
#include <algorithm>  // Para usar std::max_element
#include <libserial/SerialPort.h>
#include <libserial/SerialStream.h>

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
int Resistencia(int edad,int probabilidadSupervivencia,int inteligenciaEmocional, int Condicion, int troncoSuperior, int troncoInferior){
    // Ponderaciones para cada característica (ajusta estos valores según la importancia de cada característica)
    const double pesoProbabilidad = 0.45;
    const double pesoInteligencia = 0.2;
    const double pesoCondicion = 0.3;
    const double pesoTroncoSuperior = 0.35;
    const double pesoTroncoInferior = 0.25;

    // Ajusta mu y sigma según tus necesidades para la función de Gauss
    double mu = 40.0; // Edad media donde la resistencia es máxima
    double sigma = 10.0; // Desviación estándar, controla la forma de la curva

    // Calcula la contribución de la edad usando la función de Gauss
    double contribucionEdad = exp(-pow(edad - mu, 2) / (2 * pow(sigma, 2)));

    int resistencia = static_cast<int>((contribucionEdad * 50) + (pesoProbabilidad * probabilidadSupervivencia) +
                         (pesoInteligencia * inteligenciaEmocional) + (pesoCondicion * Condicion) +
                         (pesoTroncoSuperior * troncoSuperior) + (pesoTroncoInferior * troncoInferior)); 
    return resistencia;


}
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
        samurai.resistencia = Resistencia(samurai.edad,samurai.probabilidadSupervivencia,samurai.inteligenciaEmocional,samurai.condicionFisica,samurai.fuerzaTroncoSuperior,samurai.fuerzaTroncoInferior);

        poblacion.push_back(samurai);
    }

    return poblacion;
}

// Función para encontrar el mejor samurái de una población basado en todos los atributos
Samurai encontrarMejorSamurai(const std::vector<Samurai>& poblacion) {
    Samurai mejorSamurai = *std::max_element(poblacion.begin(), poblacion.end(),
        [](const Samurai& a, const Samurai& b) {
            return a.resistencia < b.resistencia;
        });
    return mejorSamurai;
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
// Función para seleccionar a dos padres diferentes de la población
std::pair<Samurai, Samurai> seleccionarPadres(const std::vector<Samurai>& poblacion) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, poblacion.size() - 1);

    Samurai padre1 = poblacion[dist(gen)];
    Samurai padre2 = poblacion[dist(gen)];

    while (padre2.ID == padre1.ID) {
        padre2 = poblacion[dist(gen)];  // Asegura que padre2 sea diferente de padre1
    }

    return std::make_pair(padre1, padre2);
}
// Función para realizar el cruce (crossover) entre dos padres y producir un hijo
Samurai cruzar(const Samurai& padre1, const Samurai& padre2,int i) {
    Samurai hijo;
    // Realiza el cruce utilizando ciertos atributos de los padres (ajusta según tus necesidades)
    hijo.ID = i;
    hijo.edad = (padre1.edad + padre2.edad) / 2;  // Cruce de edad: promedio de los padres
    hijo.probabilidadSupervivencia = (padre1.probabilidadSupervivencia + padre2.probabilidadSupervivencia) / 1.9;
    hijo.generacionesEsperadas = (padre1.generacionesEsperadas + padre2.generacionesEsperadas) / 2;
    hijo.inteligenciaEmocional = (padre1.inteligenciaEmocional + padre2.inteligenciaEmocional) / 1.7;
    hijo.condicionFisica = (padre1.condicionFisica + padre2.condicionFisica) / 1.6;
    hijo.fuerzaTroncoSuperior = (padre1.fuerzaTroncoSuperior + padre2.fuerzaTroncoSuperior ) / 1.6;
    hijo.fuerzaTroncoInferior = (padre1.fuerzaTroncoInferior + padre2.fuerzaTroncoInferior) / 1.6;
    hijo.resistencia= Resistencia(hijo.edad,hijo.probabilidadSupervivencia,hijo.inteligenciaEmocional,hijo.condicionFisica,hijo.fuerzaTroncoSuperior,hijo.fuerzaTroncoInferior);

    // Aplica mutación con cierta probabilidad (ajusta según tus necesidades)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> mutationChance(0.0, 1.0);
    if (mutationChance(gen) < 0.1) {  // Probabilidad de mutación del 10%
        hijo.edad = std::uniform_int_distribution<int>(30, 60)(gen);  // Cambia la edad aleatoriamente
        hijo.probabilidadSupervivencia = std::uniform_int_distribution<int>(60, 100)(gen);  // Cambia la probabilidad de supervivencia aleatoriamente
        hijo.generacionesEsperadas = std::uniform_int_distribution<int>(1, 3)(gen);  // Cambia generaciones esperadas aleatoriamente
        hijo.inteligenciaEmocional = std::uniform_int_distribution<int>(1, 11)(gen);  // Cambia inteligencia emocional aleatoriamente
        hijo.condicionFisica = std::uniform_int_distribution<int>(1, 12)(gen);  // Cambia condición física aleatoriamente
        hijo.fuerzaTroncoSuperior = std::uniform_int_distribution<int>(4, 10)(gen);  // Cambia fuerza tronco superior aleatoriamente
        hijo.fuerzaTroncoInferior = std::uniform_int_distribution<int>(3, 10)(gen);  // Cambia fuerza tronco inferior aleatoriamente
        hijo.resistencia= Resistencia(hijo.edad,hijo.probabilidadSupervivencia,hijo.inteligenciaEmocional,hijo.condicionFisica,hijo.fuerzaTroncoSuperior,hijo.fuerzaTroncoInferior);

    }

    return hijo;
}

void reemplazarPoblacion(std::vector<Samurai>& poblacion) {
    std::vector<Samurai> nuevaPoblacion;

    for (int i = 0; i < poblacion.size(); ++i) {
        // Selecciona dos padres diferentes para cada cruce
        std::pair<Samurai, Samurai> padres = seleccionarPadres(poblacion);

        // Realiza el cruce entre los padres y produce un hijo
        Samurai hijo = cruzar(padres.first, padres.second,i);

        // Agrega al hijo a la nueva población
        nuevaPoblacion.push_back(hijo);
    }

    // Reemplaza la población actual con la nueva generación
    poblacion = nuevaPoblacion;
}

double calcularMediaResistencia(const std::vector<Samurai>& poblacion) {
    double sumaResistencias = 0.0;
    int cantidadSamurais = poblacion.size();

    // Suma las resistencias de todos los samuráis en la población
    for (const Samurai& samurai : poblacion) {
        sumaResistencias += samurai.resistencia;
    }

    // Calcula la media dividiendo la suma total de resistencias por el número de samuráis
    double mediaResistencia = cantidadSamurais > 0 ? sumaResistencias / cantidadSamurais : 0.0;
    
    return mediaResistencia;
}


int main() {
    LibSerial::SerialStream arduino;
    arduino.Open("/dev/ttyACM0"); 
    arduino.SetBaudRate(LibSerial::BaudRate::BAUD_9600);
    
    if (arduino.IsOpen()) {
        std::cout << "Conexión establecida con Arduino." << std::endl;

        
        // Enviar un número al Arduino
        int numero = 15;
        arduino << numero;
        usleep(10000); // Esperar 1 segundo
        // Enviar 'a' al Arduino
        arduino.put('a');
        usleep(10000); // Esperar 1 segundo (1000000 microsegundos)



        std::cout << "Datos enviados correctamente." << std::endl;
    } else {
        std::cerr << "No se pudo abrir el puerto serial." << std::endl;
    }

    arduino.Close();


    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 5);

    // Inicializar dos poblaciones de samurais
    std::vector<Samurai> poblacion1 = inicializarPoblacion(15);
    std::vector<Samurai> poblacion2 = inicializarPoblacion(17);

    // Imprimir las poblaciones
    std::cout << "Población 1:\n";
    imprimirPoblacion(poblacion1);
    std::cout << "aaaa" << calcularMediaResistencia(poblacion1) << "\n";
    std::cout << "Población 2:\n";
    imprimirPoblacion(poblacion2);

    // Encontrar y mostrar el mejor samurái de cada población
    Samurai mejorSamuraiPoblacion1 = encontrarMejorSamurai(poblacion1);
    Samurai mejorSamuraiPoblacion2 = encontrarMejorSamurai(poblacion2);


    std::cout << "Mejor samurái de la población 1:\n";
    imprimirPoblacion({mejorSamuraiPoblacion1});
    

    std::cout << "Mejor samurái de la población 2:\n";
    imprimirPoblacion({mejorSamuraiPoblacion2});

    std::cout << "Población 1 gen 2:\n";
    reemplazarPoblacion(poblacion1);
    imprimirPoblacion(poblacion1);
    std::cout << "aaaa" << calcularMediaResistencia(poblacion1) << "\n";
        std::cout << "Población 1 gen 3:\n";
    reemplazarPoblacion(poblacion1);
    imprimirPoblacion(poblacion1);
    std::cout << "aaaa" << calcularMediaResistencia(poblacion1) << "\n";
        std::cout << "Población 1 gen 4:\n";
    reemplazarPoblacion(poblacion1);
    imprimirPoblacion(poblacion1);
    std::cout << "aaaa" << calcularMediaResistencia(poblacion1) << "\n";
        std::cout << "Población 1 gen 5:\n";
    reemplazarPoblacion(poblacion1);
    imprimirPoblacion(poblacion1);
    std::cout << "aaaa" << calcularMediaResistencia(poblacion1) << "\n";


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