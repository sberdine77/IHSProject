#include <SerialStream.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>

using namespace LibSerial;

char* serialRecievePrint(SerialStream* serial_port){
    char next_byte;
    char* ID = new char[8];    

    for(int i = 0; i<8;i++){
        serial_port->get(next_byte);
        if(next_byte == '.'){
            i=-1;          
        }
        else{
            ID[i] = next_byte;
        }
    }

    return ID;
}

int serialRecieve(SerialStream* serial_port)
{
    using namespace std;
    using namespace LibSerial;

    char c;
    serial_port->Open("/dev/ttyUSB1");
    if (!serial_port->good()) {
        std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
                  << "Error: Could not open serial port."
                  << std::endl;
        exit(1);
    } 

     serial_port->SetBaudRate( SerialStreamBuf::BAUD_9600 ) ;
     if ( ! serial_port->good() )
     {
         std::cerr << "Error: Could not set the baud rate." <<  std::endl ;
         exit(1) ;
     }

     serial_port->SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
     if ( ! serial_port->good() )
     {
         std::cerr << "Error: Could not set the character size." <<  
std::endl ;
         exit(1) ;
     }

     serial_port->SetParity( SerialStreamBuf::PARITY_NONE ) ;
     if ( ! serial_port->good() )
     {
         std::cerr << "Error: Could not disable the parity." <<  
std::endl ;
         exit(1) ;
     }

     serial_port->SetNumOfStopBits( 1 ) ;
     if ( ! serial_port->good() )
     {
         std::cerr << "Error: Could not set the number of stop bits."
                   << std::endl ;
         exit(1) ;
     }

     serial_port->SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE ) ;
     if ( ! serial_port->good() )
     {
         std::cerr << "Error: Could not use hardware flow control."
                   << std::endl ;
         exit(1) ;
     }

    //char out_buf[] = "check";
    //serial_port->write(out_buf, 5);  //<-- FIRST COMMAND
}

int main(int argc, char** argv){
    using namespace std;
    using namespace LibSerial;
    int x;
    char* ID;
    SerialStream *serial_rfid = new SerialStream;

    serialRecieve(serial_rfid); 
    while(1){
        ID = serialRecievePrint(serial_rfid);
        for(int i = 0; i<8;i++){
            std::cout << ID[i];
        }
        std::cout << std::endl;

    }

    return 0;
}

