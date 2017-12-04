/**
  ******************************************************************************
  * @file    vr_sample_control_led.ino
  * @author  JiapengLi
  * @brief   This file provides a demostration on 
              how to control led by using VoiceRecognitionModule
  ******************************************************************************
  * @note:
        voice control led
  ******************************************************************************
  * @section  HISTORY
    
    2013/06/13    Initial version.
  */
  
#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"

/**        
  Connection
  Arduino    VoiceRecognitionModule
   2   ------->     TX
   3   ------->     RX
*/
VR myVR(2,3);    // 2:RX 3:TX, you can choose your favourite pins.

uint8_t records[7]; // save record
uint8_t buf[64];

int led = 13;

#define user1 (0)
#define user2 (1)
#define user3 (2)
#define user4 (3)
#define user5 (4)
#define user6 (5)
#define user7 (6)

/**
  @brief   Print signature, if the character is invisible, 
           print hexible value instead.
  @param   buf     --> command length
           len     --> number of parameters
*/

/**
  @brief   Print signature, if the character is invisible, 
           print hexible value instead.
  @param   buf  -->  VR module return value when voice is recognized.
             buf[0]  -->  Group mode(FF: None Group, 0x8n: User, 0x0n:System
             buf[1]  -->  number of record which is recognized. 
             buf[2]  -->  Recognizer index(position) value of the recognized record.
             buf[3]  -->  Signature length
             buf[4]~buf[n] --> Signature
*/

void setup()
{
  /** initialize */
  myVR.begin(9600);
  
  Serial.begin(115200);
  //Serial.println("Elechouse Voice Recognition V3 Module\r\nControl LED sample");
  
  pinMode(led, OUTPUT);
    
  if(myVR.clear() == 0){
    //Serial.println("Recognizer cleared.");
  }else{
    //Serial.println("Not find VoiceRecognitionModule.");
    //Serial.println("Please check connection and restart Arduino.");
    while(1);
  }
  
  if(myVR.load((uint8_t)user6) >= 0){
    //Serial.println("onRecord loaded");
  }
  
  if(myVR.load((uint8_t)user7) >= 0){
    //Serial.println("offRecord loaded");
  }
  if(myVR.load((uint8_t)user1) >= 0){
    //Serial.println("user1 loaded");
  }
  if(myVR.load((uint8_t)user2) >= 0){
    //Serial.println("user2 loaded");
  }
  if(myVR.load((uint8_t)user3) >= 0){
    //Serial.println("user3 loaded");
  }
  if(myVR.load((uint8_t)user4) >= 0){
    //Serial.println("user4 loaded");
  }
  if(myVR.load((uint8_t)user5) >= 0){
    //Serial.println("user5 loaded");
  }
}

void loop()
{
  int ret;
  ret = myVR.recognize(buf, 50);
  if(ret>0){
    switch(buf[1]){
      case user1:
       Serial.write("1");
       break;
      case user2:
       Serial.write("2");
       break;
      case user3:
       Serial.write("3");
       break;
      case user4:
       Serial.write("4");
       break;
      case user5:
       Serial.write("5");
       break;
      case user6:
       Serial.write("6");
       break;
      case user7:
       Serial.write("7");
       break;
      default:
        //Serial.println("Record function undefined");
        break;
    }
    /** voice recognized */
  }
}



