//#include "opencv2/core/core.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
#include "iostream"
#include <SerialStream.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string.h>
#include <fcntl.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>



using namespace LibSerial;
//using namespace cv;
//using namespace std

typedef struct Pio {
	int pio;
	int dado;
} pio;

struct Person {
	char* Nome;
	char* tag;
	char password;
	int level;
	ALLEGRO_BITMAP* image;
	//char* foto;
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

	ID[8] = '\0';

	return ID;
}


int* strTo7Seg(char* string) {
	//int array1[4];
	//int array2[4];
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

void serialRecieve(SerialStream* serial_port)
{
	using namespace std;
	using namespace LibSerial;

	//char c;
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
	//Mat Imagem;

	unsigned char hexdigit[] = { 0x3F, 0x06, 0x5B, 0x4F,
		0x66, 0x6D, 0x7D, 0x07,
		0x7F, 0x6F, 0x77, 0x7C,
		0x39, 0x5E, 0x79, 0x71 };

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_BITMAP *negado = NULL;

	if(!al_init()) {
      al_show_native_message_box(display, "Error", "Error", "Failed to initialize allegro!", 
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
      return 0;
    }

    if(!al_init_image_addon()) {
      al_show_native_message_box(display, "Error", "Error", "Failed to initialize al_init_image_addon!", 
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
      return 0;
    }

    display = al_create_display(800,600);

    if(!display) {
      al_show_native_message_box(display, "Error", "Error", "Failed to initialize display!", 
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
      return 0;
    }	

	negado = al_load_bitmap("not_pass.jpg");

   if(!negado) {
      al_show_native_message_box(display, "Error", "Error", "Failed to load image!", 
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
      al_destroy_display(display);
      return 0;
   }

   //al_draw_bitmap(negado,0,0,0);

   al_flip_display();
	//cout << "abriusaporra!" << std::endl;

	pio j, k;
	j.pio = 5;

	int dev = open("/dev/de2i150_altera", O_RDWR);

	for (int i = 0; i<7; i++) {
		pessoas[i].password = 49 + i;
	}

	pessoas[0].Nome = (char*)("Lucas\0");
	pessoas[0].tag = (char*)("372F64F0\0");
	pessoas[0].level = 3;
	pessoas[0].image = al_load_bitmap("lucas.jpg");
	//pessoas[0].foto = (char*)("lama2.jpeg");
	if(!pessoas[0].image) {
      al_show_native_message_box(display, "Error", "Error", "Failed to load image!", 
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
      al_destroy_display(display);
      return 0;
   }

	pessoas[1].Nome = (char*)("Thyago\0");
	pessoas[1].tag = (char*)("A70B64F0\0");
	pessoas[1].level = 3;
	pessoas[1].image = al_load_bitmap("thyago.jpg");
	//pessoas[1].foto = (char*)("thyago.jpg");
	if(!pessoas[1].image) {
      al_show_native_message_box(display, "Error", "Error", "Failed to load image!", 
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
      al_destroy_display(display);
      return 0;
   }

	pessoas[2].Nome = (char*)("Mateus\0");
	pessoas[2].tag = (char*)("E71EB654\0");
	pessoas[2].level = 1;
	pessoas[2].image = al_load_bitmap("mateus.jpg");
	//pessoas[2].foto = (char*)("mateus.jpg");
	if(!pessoas[2].image) {
      al_show_native_message_box(display, "Error", "Error", "Failed to load image!", 
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
      al_destroy_display(display);
      return 0;
   }

	pessoas[3].Nome = (char*)("Sávio\0");
	pessoas[3].tag = (char*)("876EA2FD\0");
	pessoas[3].level = 3;
	pessoas[3].image = al_load_bitmap("savio.jpg");
	//pessoas[3].foto = (char*)("savio.jpg");
	if(!pessoas[3].image) {
      al_show_native_message_box(display, "Error", "Error", "Failed to load image!", 
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
      al_destroy_display(display);
      return 0;
   }

	pessoas[4].Nome = (char*)("Carriço\0");
	pessoas[4].tag = (char*)("278F63F0\0");
	pessoas[4].level = 3;
	pessoas[4].image = al_load_bitmap("carrico.jpg");
	//pessoas[4].foto = (char*)("Daniel.jpeg");
	if(!pessoas[4].image) {
      al_show_native_message_box(display, "Error", "Error", "Failed to load image!", 
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
      al_destroy_display(display);
      return 0;
   }

	//Pessoa *pessoa1 = new Pessoa((char*)("Carriço\0"), (char*)("278F63F0\0"), '1', 1);

	//int x;
	int i;
	int* var;
	char* ID;
	SerialStream *serial_rfid = new SerialStream;

	serialRecieve(serial_rfid);
	while (1) {
		ID = NULL;
		ID = serialRecievePrint(serial_rfid);
		var = strTo7Seg(ID);

		print7Seg(k, j, hexdigit, dev, 1, var[0]);
		print7Seg(k, j, hexdigit, dev, 2, var[1]);

		read(dev, &j, 4);

		std::cout << j.dado << std::endl;

		for (i = 0; i<5; i++) {
			//std::cout << "i= " << i << std::endl;
			//std::cout << "tag= " << pessoas[i].tag << std::endl;
			//std::cout << "id= " << ID << std::endl;

			if (strcmp(ID, pessoas[i].tag) == 0)
			{
				
				std::cout << pessoas[i].Nome << std::endl;
				
				
				if (pessoas[i].level >= j.dado)
				{
					std::cout << "Acesso permitido" << std::endl;
					al_clear_to_color(al_map_rgb(0,0,0));
					al_draw_bitmap(pessoas[i].image,0,0,0);
					al_flip_display();
					k.pio = 3;
					k.dado = 255;
					write(dev, &k, 4);

					k.pio = 4;
					k.dado = 0;
					write(dev, &k, 4);

					//Imagem = imread(pessoas[i].foto, CV_LOAD_IMAGE_COLOR);
					//if (!Imagem.data)
					//{
						//cout << "Could not open or find the image" << std::endl;
						//return -1;
					//}
					//namedWindow("Foto", CV_WINDOW_AUTOSIZE);
					//imshow("Foto", Imagem);

				}
				else
				{
					al_clear_to_color(al_map_rgb(0,0,0));
					std::cout << "Acesso negado" << std::endl;
   					al_draw_bitmap(negado,0,0,0);
   					al_flip_display();

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
			al_clear_to_color(al_map_rgb(0,0,0));
			std::cout << "Acesso negado" << std::endl;
			al_draw_bitmap(negado,0,0,0);
   			al_flip_display();

			k.pio = 3;
			k.dado = 0;
			write(dev, &k, 4);

			k.pio = 4;
			k.dado = 262143;
			write(dev, &k, 4);
		}

	}

	/*al_destroy_display(display);
	for( int z=0; z<5 ; z++){
		al_destroy_bitmap(pessoas[z].image);
	}	
	al_destroy_bitmap(negado);*/
	return 0;
}
