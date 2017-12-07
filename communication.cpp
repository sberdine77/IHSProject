#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "iostream"
#include <SerialStream.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string.h>
#include <fcntl.h>


using namespace LibSerial;
using namespace cv;
using namespace std;

typedef struct Pio {
	int pio;
	int dado;
} pio;

struct Person {
	char* Nome;
	char* tag;
	char password;
	int level;
	char* foto;
}pessoas[7];


char* serialRecievePrint(SerialStream* serial_port) {
	char next_byte;
	char* ID = new char[8];

	for (int i = 0; i<8; i++) {
		serial_port->get(next_byte);
		if (next_byte == '.') {
			i = -1;
		}
		else {
			ID[i] = next_byte;
		}
	}

	return ID;
}


int* strTo7Seg(char* string) {
	int array1[4];
	int array2[4];
	int sum1 = 0, sum2 = 0;
	int div = 4096;
	int aux;
	int aux2;
	int* retorno = new int[2];

	for (int i = 0; i<4; i++) {
		if (string[i]<65) {
			aux = (string[i] - 48)*div;
			sum1 = sum1 + aux;
		}
		else {
			aux = (string[i] - 55)*div;
			sum1 = sum1 + aux;
		}

		if (string[i + 4] < 65) {
			aux2 = (string[i + 4] - 48)*div;
			sum2 = sum2 + aux2;
		}
		else {
			aux2 = (string[i + 4] - 55)*div;
			sum2 = sum2 + aux2;
		}
		div = div / 16;
	}

	retorno[0] = sum1;
	retorno[1] = sum2;

	return retorno;
}

void print7Seg(pio k, pio j, unsigned char* hexdigit, int dev, int i, int val) {
	k.pio = i;
	j.dado = val;
	k.dado = hexdigit[j.dado & 0xF]
		| (hexdigit[(j.dado >> 4) & 0xF] << 8)
		| (hexdigit[(j.dado >> 8) & 0xF] << 16)
		| (hexdigit[(j.dado >> 12) & 0xF] << 24);
	k.dado = ~(k.dado);
	write(dev, &k, 4);
}

int serialRecieve(SerialStream* serial_port)
{
	using namespace std;
	using namespace LibSerial;

	char c;
	serial_port->Open("/dev/ttyUSB0");
	if (!serial_port->good()) {
		std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
			<< "Error: Could not open serial port."
			<< std::endl;
		exit(1);
	}

	serial_port->SetBaudRate(SerialStreamBuf::BAUD_9600);
	if (!serial_port->good())
	{
		std::cerr << "Error: Could not set the baud rate." << std::endl;
		exit(1);
	}

	serial_port->SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
	if (!serial_port->good())
	{
		std::cerr << "Error: Could not set the character size." << std::endl;
		exit(1);
	}

	serial_port->SetParity(SerialStreamBuf::PARITY_NONE);
	if (!serial_port->good())
	{
		std::cerr << "Error: Could not disable the parity." << std::endl;
		exit(1);
	}

	serial_port->SetNumOfStopBits(1);
	if (!serial_port->good())
	{
		std::cerr << "Error: Could not set the number of stop bits."
			<< std::endl;
		exit(1);
	}

	serial_port->SetFlowControl(SerialStreamBuf::FLOW_CONTROL_NONE);
	if (!serial_port->good())
	{
		std::cerr << "Error: Could not use hardware flow control."
			<< std::endl;
		exit(1);
	}

	//char out_buf[] = "check";
	//serial_port->write(out_buf, 5);  //<-- FIRST COMMAND
}

int main(int argc, char** argv) {
	using namespace std;
	using namespace LibSerial;
	Mat Imagem;

	unsigned char hexdigit[] = { 0x3F, 0x06, 0x5B, 0x4F,
		0x66, 0x6D, 0x7D, 0x07,
		0x7F, 0x6F, 0x77, 0x7C,
		0x39, 0x5E, 0x79, 0x71 };

	pio j, k;
	j.pio = 5;

	int dev = open("/dev/de2i150_altera", O_RDWR);

	for (int i = 0; i<7; i++) {
		pessoas[i].password = 49 + i;
	}

	pessoas[0].Nome = (char*)("Lucas\0");
	pessoas[0].tag = (char*)("372F64F0\0");
	pessoas[0].level = 3;
	pessoas[0].foto = (char*)("lama2.jpeg");

	pessoas[1].Nome = (char*)("Thyago\0");
	pessoas[1].tag = (char*)("A70B64F0\0");
	pessoas[1].level = 3;
	pessoas[1].foto = (char*)("thyago.jpg");

	pessoas[2].Nome = (char*)("Mateus\0");
	pessoas[2].tag = (char*)("E71EB654\0");
	pessoas[2].level = 1;
	pessoas[2].foto = (char*)("mateus.jpg");

	pessoas[3].Nome = (char*)("Sávio\0");
	pessoas[3].tag = (char*)("876EA2FD\0");
	pessoas[3].level = 3;
	pessoas[3].foto = (char*)("savio.jpg");

	pessoas[4].Nome = (char*)("Carriço\0");
	pessoas[4].tag = (char*)("278F63F0\0");
	pessoas[4].level = 3;
	pessoas[4].foto = (char*)("Daniel.jpeg");

	//Pessoa *pessoa1 = new Pessoa((char*)("Carriço\0"), (char*)("278F63F0\0"), '1', 1);

	int x;
	int i;
	int* var;
	char* ID;
	SerialStream *serial_rfid = new SerialStream;

	serialRecieve(serial_rfid);
	while (1) {
		ID = serialRecievePrint(serial_rfid);
		var = strTo7Seg(ID);

		print7Seg(k, j, hexdigit, dev, 1, var[0]);
		print7Seg(k, j, hexdigit, dev, 2, var[1]);

		read(dev, &j, 4);

		std::cout << j.dado << std::endl;

		for (i = 0; i<5; i++) {
			if (strcmp(ID, pessoas[i].tag) == 0)
			{
				std::cout << pessoas[i].Nome << std::endl;
				Imagem = imread(pessoas[i].foto, CV_LOAD_IMAGE_COLOR);
				if (!Imagem.data)
				{
					cout << "Could not open or find the image" << std::endl;
					return -1;
				}
				namedWindow("Foto", CV_WINDOW_AUTOSIZE);
				imshow("Foto", Imagem);

				if (pessoas[i].level >= j.dado)
				{
					std::cout << "Acesso permitido" << std::endl;
					k.pio = 3;
					k.dado = 255;
					write(dev, &k, 4);

					k.pio = 4;
					k.dado = 0;
					write(dev, &k, 4);
				}
				else
				{
					std::cout << "Acesso negado" << std::endl;

					k.pio = 3;
					k.dado = 0;
					write(dev, &k, 4);

					k.pio = 4;
					k.dado = 262143;
					write(dev, &k, 4);
				}

				break;
			}
		}
		if (i == 5)
		{
			std::cout << "Acesso negado" << std::endl;

			k.pio = 3;
			k.dado = 0;
			write(dev, &k, 4);

			k.pio = 4;
			k.dado = 262143;
			write(dev, &k, 4);
		}

	}

	return 0;
}
