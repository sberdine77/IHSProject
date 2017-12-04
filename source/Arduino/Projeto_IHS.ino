#include <LiquidCrystal_I2C.h>
#include <Thread.h>
#include <ThreadController.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);
MFRC522 mfrc522(SS_PIN, RST_PIN);

String string;
String string2;

Thread myThread = Thread();
Thread myThread2 = Thread();
ThreadController controller = ThreadController();

// callback for myThread
void lcdT(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(string);
  lcd.setCursor(0,1);
  lcd.print(string2);
  myThread2.enabled = false;
}

void rfidT(){
    // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Mostra UID na serial
  //Serial.print("UID da tag :");
  String conteudo= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     //Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  conteudo.toUpperCase();
  Serial.print(".");
  for (int i = 0; i < conteudo.length(); i++)
  {
    Serial.write(conteudo[i]);   // Push each char 1 by 1 on each loop pass
  }

  //Serial.write(conteudo);
  string = "ID do cartao:";
  string2 = conteudo;
  
  myThread2.enabled = true;
}

void setup(){
  Serial.begin(9600);  // Inicia a serial
  SPI.begin();    // Inicia  SPI bus
  mfrc522.PCD_Init(); // Inicia MFRC522
  
  lcd.begin (16,2);

  controller.add(&myThread); // Notice the & before the thread, IF it's not instantied as a pointer.
  controller.add(&myThread2);

  myThread.enabled = true;
  myThread2.enabled = true;
  
  string = "Aproxime o seu";
  string2 = "cartao do leitor";
  
	myThread.onRun(rfidT);
  myThread2.onRun(lcdT);
	myThread.setInterval(500);
}

void loop(){
	// checks if thread should run
  controller.run();

	// Other code...
}
