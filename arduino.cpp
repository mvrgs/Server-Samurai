#include <libserial/SerialPort.h>
#include <libserial/SerialStream.h>


LibSerial::SerialStream arduino;
    arduino.Open("/dev/ttyACM0"); 
    arduino.SetBaudRate(LibSerial::BaudRate::BAUD_9600);

    if (arduino.IsOpen()) {
        std::cout << "Conexión establecida con Arduino." << std::endl;


       
    } else {
        std::cerr << "No se pudo abrir el puerto serial." << std::endl;
    }