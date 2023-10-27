#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <stack>
#include <list>
#include <utility>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <random>
#include <algorithm>  // Para usar std::max_element



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
struct MensajeServidor {
    int codigoRespuesta;
    int fila;
    int columna;
    int fila_back;
    int columna_back;
    int dinero;
    Samurai samurai1;
    Samurai samurai2;
};
using namespace std;

const int INF = 1e9;
int dx[] = {-1, 0, 1, 0};
int dy[] = {0, 1, 0, -1};

bool isValid(int x, int y, int n, int m, const vector<vector<int>>& grid, const vector<vector<bool>>& visited) {
    return x >= 0 && x < n && y >= 0 && y < m && !visited[x][y] && grid[x][y] != 1 && grid[x][y] != 2 && grid[x][y] != 3;
}

bool backtracking(vector<vector<int>>& grid, pair<int, int> current, pair<int, int> end, vector<vector<bool>>& visited, vector<pair<int, int>>& path) {
    int n = grid.size();
    int m = grid[0].size();

    if (current == end) {
        return true;
    }

    visited[current.first][current.second] = true;

    for (int i = 0; i < 4; ++i) {
        int newX = current.first + dx[i];
        int newY = current.second + dy[i];

        if (isValid(newX, newY, n, m, grid, visited)) {
            path.push_back({newX, newY});
            if (backtracking(grid, {newX, newY}, end, visited, path)) {
                return true;
            }
            path.pop_back();
        }
    }

    visited[current.first][current.second] = false;
    return false;
}

vector<pair<int, int>> findPath(vector<vector<int>>& grid, pair<int, int> start, pair<int, int> end) {
    int n = grid.size();
    int m = grid[0].size();

    vector<vector<bool>> visited(n, vector<bool>(m, false));
    vector<pair<int, int>> path;
    path.push_back(start);

    if (backtracking(grid, start, end, visited, path)) {
        return path;
    } else {
        return {}; // No se encontró ruta, devuelve una lista vacía
    }
}



void printGrid(const vector<vector<int>>& grid) {
    for (const auto& row : grid) {
        for (int cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }
}



struct Node {
    int x, y, cost, heuristic;

    bool operator<(const Node& other) const {
        return cost + heuristic > other.cost + other.heuristic;
    }
};
bool isValid(int x, int y, int n, int m) {
    return x >= 0 && x < n && y >= 0 && y < m;
}



vector<pair<int, int>> findPathh(const vector<vector<pair<int, int>>>& parent, pair<int, int> start, pair<int, int> end) {
    stack<pair<int, int>> path;
    vector<pair<int, int>> pathList;
    pair<int, int> current = end;

    while (current != start) {
        path.push(current);
        current = parent[current.first][current.second];
    }
    path.push(start);

    while (!path.empty()) {
        pathList.push_back(path.top());
        path.pop();
    }

    return pathList;
}

void printPathh(const vector<vector<pair<int, int>>>& parent, pair<int, int> start, pair<int, int> end) {
    stack<pair<int, int>> path;
    pair<int, int> current = end;
    while (current != start) {
        path.push(current);
        current = parent[current.first][current.second];
    }
    path.push(start);

    cout << "La ruta tomada desde el punto de inicio al punto final es:" << endl;
    while (!path.empty()) {
        cout << "(" << path.top().second << ", " << path.top().first << ")";
        path.pop();
        if (!path.empty()) {
            cout << " -> ";
        }
    }
    cout << endl;
}

vector<pair<int, int>> astar(vector<vector<int>>& grid, pair<int, int> start, pair<int, int> end) {
    int n = grid.size();
    int m = grid[0].size();

    vector<vector<int>> cost(n, vector<int>(m, INF));
    vector<vector<pair<int, int>>> parent(n, vector<pair<int, int>>(m, {-1, -1}));
    vector<vector<bool>> visited(n, vector<bool>(m, false));

    priority_queue<Node> pq;
    pq.push({start.first, start.second, 0, abs(start.first - end.first) + abs(start.second - end.second)});
    cost[start.first][start.second] = 0;

    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();

        if (current.x == end.first && current.y == end.second) {
            return findPathh(parent, start, end);
        }

        if (visited[current.x][current.y]) {
            continue;
        }

        visited[current.x][current.y] = true;

        for (int i = 0; i < 4; ++i) {
            int newX = current.x + dx[i];
            int newY = current.y + dy[i];

            if (isValid(newX, newY, n, m) && !visited[newX][newY] && grid[newX][newY] != 1 && grid[newX][newY] != 2 && grid[newX][newY] != 3) {
                int newCost = current.cost + 1;
                if (newCost < cost[newX][newY]) {
                    cost[newX][newY] = newCost;
                    pq.push({newX, newY, newCost, abs(newX - end.first) + abs(newY - end.second)});
                    parent[newX][newY] = {current.x, current.y};
                }
            }
        }
    }

    return {}; // No se encontró ruta, devuelve una lista vacía
}
void actualizarMatriz(std::vector<std::vector<int>>& grid_obs) {
    int rows = grid_obs.size();
    int cols = grid_obs[0].size();

    // Recorrer la matriz original
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid_obs[i][j] == 1) {
                // Si hay un 1, poner 4 en las celdas a una distancia de 1 (si están dentro del rango) y no son 1 ni 2
                for (int x = i - 1; x <= i + 1; ++x) {
                    for (int y = j - 1; y <= j + 1; ++y) {
                        if (x >= 0 && x < rows && y >= 0 && y < cols && grid_obs[x][y] != 1 && grid_obs[x][y] != 2 && grid_obs[x][y] != 3) {
                            grid_obs[x][y] += 4;
                        }
                    }
                }
            } else if (grid_obs[i][j] == 2) {
                // Si hay un 2, poner 5 en un cuadrado de 2x2 alrededor (si están dentro del rango) y no son 1 ni 2
                for (int x = i - 2; x <= i + 2; ++x) {
                    for (int y = j - 2; y <= j + 2; ++y) {
                        if (x >= 0 && x < rows && y >= 0 && y < cols && grid_obs[x][y] != 1 && grid_obs[x][y] != 2 && grid_obs[x][y] != 3) {
                            grid_obs[x][y] += 5;
                        }
                    }
                }
            }else if (grid_obs[i][j] == 3) {
                // Si hay un 2, poner 5 en un cuadrado de 2x2 alrededor (si están dentro del rango) y no son 1 ni 2
                for (int x = i - 2; x <= i + 2; ++x) {
                    for (int y = j - 2; y <= j + 2; ++y) {
                        if (x >= 0 && x < rows && y >= 0 && y < cols && grid_obs[x][y] != 1 && grid_obs[x][y] != 2 && grid_obs[x][y] != 3) {
                            grid_obs[x][y] += 6;
                        }
                    }
                }
            }
        }
    }
}
void limpiarObstaculos(std::vector<std::vector<int>>& grid_obs) {
    int rows = grid_obs.size();
    int cols = grid_obs[0].size();

    // Recorrer la matriz y eliminar los números que no sean 1, 2 o 3
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid_obs[i][j] != 1 && grid_obs[i][j] != 2 && grid_obs[i][j] != 3) {
                grid_obs[i][j] = 0;
            }
        }
    }
}




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
    hijo.probabilidadSupervivencia = (padre1.probabilidadSupervivencia + padre2.probabilidadSupervivencia) / 1.7;
    hijo.generacionesEsperadas = (padre1.generacionesEsperadas + padre2.generacionesEsperadas) / 2;
    hijo.inteligenciaEmocional = (padre1.inteligenciaEmocional + padre2.inteligenciaEmocional) / 1.5;
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
    // Crear un socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error al crear el socket." << std::endl;
        return -1;
    }

    // Configurar la dirección del servidor
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345); // Puerto del servidor
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Vincular el socket al puerto y la dirección
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error al vincular el socket." << std::endl;
        return -1;
    }

    // Escuchar conexiones entrantes
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error al escuchar las conexiones." << std::endl;
        return -1;
    }

    std::cout << "Servidor esperando conexiones..." << std::endl;

    // Aceptar conexiones entrantes
    int clientSocket = accept(serverSocket, NULL, NULL);
    if (clientSocket == -1) {
        std::cerr << "Error al aceptar la conexión." << std::endl;
        return -1;
    }

    std::cout << "Cliente conectado." << std::endl;
    // Inicializar el grid como 10x10 con celdas vacías (0)
    vector<vector<int>> grid = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    vector<vector<int>> grid_back= {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    vector<vector<int>> grid_obs = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };


    pair<int, int> start = {0, 0}; // Punto de inicio
    pair<int, int> end = {9, 9};   // Punto final

    cout << "Matriz con obstáculos:" << endl;
    printGrid(grid);
   
    vector<pair<int, int>> result = astar(grid, start, end);
    vector<pair<int, int>> result_back = findPath(grid_back, start, end);
    pair<int,int> currentPos=start;
    pair<int,int> currentPos_back=start;
    int obs;



    // Inicializar dos poblaciones de samurais
    std::vector<Samurai> poblacion1 = inicializarPoblacion(20);
    std::vector<Samurai> poblacion2 = inicializarPoblacion(20);

    // Encontrar y mostrar el mejor samurái de cada población
    Samurai mejorSamuraiPoblacion1 = encontrarMejorSamurai(poblacion1);
    Samurai mejorSamuraiPoblacion2 = encontrarMejorSamurai(poblacion2);

;


    std::cout << "Mejor samurái de la población 1:\n";
    imprimirPoblacion({mejorSamuraiPoblacion1});
    int dinero = 10;
    int iteraciones = 0;
    
    while (currentPos != end || currentPos_back != end ){
        dinero++;
        iteraciones++;
        arduino<<iteraciones;
        if (mejorSamuraiPoblacion1.resistencia<=0){
            reemplazarPoblacion(poblacion1);
            mejorSamuraiPoblacion1 = encontrarMejorSamurai(poblacion1);
            currentPos=start;
        }
        if (mejorSamuraiPoblacion2.resistencia<=0){
            reemplazarPoblacion(poblacion2);
            mejorSamuraiPoblacion2 = encontrarMejorSamurai(poblacion1);
            currentPos_back=start;
        }
        vector<pair<int, int>> result = astar(grid, currentPos, end);
        vector<pair<int, int>> result_back = findPath(grid_back, currentPos_back, end);
        // Obtener el primer elemento
        if (!result.empty()) {
            std::pair<int, int> primerElemento = result.front();
            result.erase(result.begin());
            currentPos=result.front();
            
            int fila = primerElemento.first;
            int columna = primerElemento.second;

            std::pair<int, int> primerElemento_back = result_back.front();
            result_back.erase(result_back.begin());
            currentPos_back=result_back.front();
            


            int fila_back = primerElemento_back.first;
            int columna_back = primerElemento_back.second;

            if (grid_obs[fila_back][columna_back]!=0){
                mejorSamuraiPoblacion2.resistencia-=grid_obs[fila_back][columna_back];
                arduino.put("a");
                std::cout << "hola: " << mejorSamuraiPoblacion2.resistencia  << std::endl;
            }
            if (grid_obs[fila][columna]!=0){
                mejorSamuraiPoblacion1.resistencia-=grid_obs[fila][columna];
                arduino.put("a");
                std::cout << "hola: " << mejorSamuraiPoblacion1.resistencia  << std::endl;
            }
            // Enviar fila y columna al cliente
            MensajeServidor mensajeServidor;
            mensajeServidor.codigoRespuesta = 200; // Código de respuesta (puedes definir tus propios códigos)
            mensajeServidor.fila = currentPos.first; // Usar el valor de fila que recibiste del cliente
            mensajeServidor.columna = currentPos.second; // Usar el valor de columna que recibiste del cliente
            mensajeServidor.fila_back = currentPos_back.first; // Usar el valor de fila que recibiste del cliente
            mensajeServidor.columna_back = currentPos_back.second;
            mensajeServidor.dinero = dinero;
            mensajeServidor.samurai1=mejorSamuraiPoblacion1;
            mensajeServidor.samurai2=mejorSamuraiPoblacion2;
            // Enviar el mensaje al cliente
            send(clientSocket, &mensajeServidor, sizeof(mensajeServidor), 0);

           

            // Eliminar todos los 2 del grid
            for (int i = 0; i < 10; ++i) {
                for (int j = 0; j < 10; ++j) {
                    if (grid[i][j] == 8) {
                        grid[i][j] = 0; // 0 representa una celda vacía en el grid, ajusta esto según tu lógica
                    }
                }
            }
            for (int i = 0; i < 10; ++i) {
                for (int j = 0; j < 10; ++j) {
                    if (grid_back[i][j] == 8) {
                        grid_back[i][j] = 0; // 0 representa una celda vacía en el grid, ajusta esto según tu lógica
                    }
                }
            }
            grid[fila][columna]=8;
            grid_back[fila_back][columna_back]=8;
            // Eliminar el primer elemento
            result.erase(result.begin());
            result_back.erase(result_back.begin());
            printGrid(grid_back);
        // Recibir los valores de filaUsuario y columnaUsuario del cliente



            MensajeServidor mensajeCliente;
            recv(clientSocket, &mensajeCliente, sizeof(mensajeCliente), 0);

            // Procesar la respuesta del servidor
            int codigoRespuesta = mensajeCliente.codigoRespuesta;
            int filaUsuario = mensajeCliente.fila;
            int columnaUsuario = mensajeCliente.columna;

        // Realizar acciones según el código de respuesta del servidor
            if (codigoRespuesta == 71) {
                obs = 1;
                dinero-=1;
                }
            else if (codigoRespuesta == 72){
                obs = 2;
                dinero-=2;
            }
            else if (codigoRespuesta == 73){
                obs = 3;
                dinero-=4;
            }else if (codigoRespuesta == 0){
                obs = 0;
                
            }


      
                        

            std::cout << "Cliente ha ingresado fila: " << filaUsuario << ", columna: " << columnaUsuario << std::endl;


            // Verificar si las coordenadas son válidas y colocar el número 1 en el grid
            if (filaUsuario >= 0 && filaUsuario < 10 && columnaUsuario >= 0 && columnaUsuario < 10) {

                grid[filaUsuario][columnaUsuario] = obs;
                grid_back[filaUsuario][columnaUsuario] = obs;
                grid_obs[filaUsuario][columnaUsuario] =obs;
                limpiarObstaculos(grid_obs);
                actualizarMatriz(grid_obs);
                printGrid(grid_obs);
            } else {
                std::cout << "Coordenadas inválidas. No se puede colocar el número 1 en esa posición." << std::endl;
            }

            }
            std::cout << std::endl;

            
        } 

    




    
    // Cerrar los sockets
    close(clientSocket);
    close(serverSocket);
    return 0;
}
