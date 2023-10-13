#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

// Estructura de un punto en el tablero
struct Point {
    int x;
    int y;
};

// Función ficticia para pruebas
std::vector<Point> AStarAlgorithm(const std::string& data) {
    
    std::vector<Point> path;
    path.push_back({0, 0});
    path.push_back({1, 1});
    path.push_back({2, 2});
    // ...

    return path;
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);  // Utiliza el mismo puerto que el cliente
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 5);

    while (true) {
        int clientSocket = accept(serverSocket, NULL, NULL);

        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));

        // Recibir datos del cliente
        recv(clientSocket, buffer, sizeof(buffer), 0);

        // Convierte los datos en una cadena
        std::string data(buffer);

        // Imprimir los datos en la consola del servidor
        std::cout << "Datos recibidos del cliente: " << data << std::endl;

        // Lógica del servidor: ejecuta el algoritmo A* y obtén la ruta
        std::vector<Point> path = AStarAlgorithm(data);

        // Envía la ruta al cliente
        send(clientSocket, path.data(), path.size() * sizeof(Point), 0);

        close(clientSocket);
    }

    return 0;
}