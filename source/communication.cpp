#include <SerialStream.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string.h>

using namespace LibSerial;

struct Person{
    char* Nome;
    char* tag;
    char password;
    int level;
}pessoas[7];

/*class Pessoa{
    public:
        char* Nome;
        char* tag;
        char password;
        int level;
    public:
        Pessoa(char* Nome, char* tag, char password, int level){
            this->Nome = Nome;
            this->tag = tag;
            this->password = password;
            this->level = level;
        }
};*/

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

    for(int i=0; i<7;i++){
        pessoas[i].password = 49+i;
    }

    pessoas[0].Nome = (char*)("Lucas\0");
    pessoas[0].tag = (char*)("372F64F0\0");
    pessoas[0].level = 3;

    pessoas[1].Nome = (char*)("Thyago\0");
    pessoas[1].tag = (char*)("A70B64F0\0");
    pessoas[1].level = 3;

    pessoas[2].Nome = (char*)("Mateus\0");
    pessoas[2].tag = (char*)("E71EB654\0");
    pessoas[2].level = 1;

    pessoas[3].Nome = (char*)("Savio\0");
    pessoas[3].tag = (char*)("876EA2FD\0");
    pessoas[3].level = 3;

    pessoas[4].Nome = (char*)("Carriço\0");
    pessoas[4].tag = (char*)("278F63F0\0");
    pessoas[4].level = 3;

    //Pessoa *pessoa1 = new Pessoa((char*)("Carriço\0"), (char*)("278F63F0\0"), '1', 1);

    int x;
    int i;

    char* ID;
    SerialStream *serial_rfid = new SerialStream;

    serialRecieve(serial_rfid); 
    while(1){
        ID = serialRecievePrint(serial_rfid);
        for(i= 0; i<5;i++){ 
            if(strcmp(ID, pessoas[i].tag) == 0)
            {
                std::cout << pessoas[i].Nome << std::endl;
                break;
            }
        }
        if(i==5)
        {
            std::cout << "deu merda";
        }
        std::cout << std::endl;
        

    }

    return 0;
}

