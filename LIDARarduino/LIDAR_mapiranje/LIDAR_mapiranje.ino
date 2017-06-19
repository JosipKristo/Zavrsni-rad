/***********************************************************************************************************
/* Arduino Uno LIDAR Lite Sweep Scan
/* By: Josip Krišto
/* Date: 12.7.2016
/* 
/* Primjer baziran na primjeru "Sweep" iz biblioteke Servo i  Pulsed Light 3D primjerima.
/* Servo motori su spojeni na pinove 9 i 10 i rotiraju se od 0 do 180 stupnjeva kontinuirano odredenim korakom.
/* Lidar Lite v2 je spojen na VCC (+5 V DC), GND, SDA and SCL.
/* 
/* Ovaj primjer koristi bliblioteku Servo i Wire za I2C povezivanje.
/*
/**********************************************************************************************************/

#include <Wire.h>
#include <Servo.h>

#define    LIDARLite_ADDRESS   0x62          // Default I2C Address of LIDAR-Lite.
#define    RegisterMeasure     0x00          // Register to write to initiate ranging.
#define    MeasureValue        0x04          // Value to initiate ranging.
#define    RegisterHighLowB    0x8f          // Register to get both High and Low bytes in 1 call.


Servo myXservo;  // Servo objekt za kontroliranje X servo motora
Servo myYservo;  // Servo objekt za kontroliranje Y servo motora

int posX = 0;  // Varijable za pohranu vrijednosti pozicije X i Y serva
int posY = 0;
int distance = 0;    // Izmjerena udaljenost

void setup()
{
  // Serial output
  Serial.begin(57600);
  Serial.println("< START >");
  
  myXservo.attach(10);  // spajanje X serva na pin 10 
  myYservo.attach(9); // spajanje Y serva na pin 9
  
  // LIDAR control
  Wire.begin();  // Otvaranje i spajanje na irc bus kao master

  delay(100);   // cekanje da budemo sigurni da je sve spojeno kako treba prije slanja i primanja podataka

}

// Mjerenej udaljenosti  LIDAR-om
int lidarGetRange(void)
{
  int val = -1;
  
  Wire.beginTransmission((int)LIDARLite_ADDRESS); // transmit to LIDAR-Lite
  Wire.write((int)RegisterMeasure); // sets register pointer to  (0x00)  
  Wire.write((int)MeasureValue); // sets register pointer to  (0x00)  
  Wire.endTransmission(); // stop transmitting

  delay(20); // cekanje 20 ms za slanje

  Wire.beginTransmission((int)LIDARLite_ADDRESS); // transmit to LIDAR-Lite
  Wire.write((int)RegisterHighLowB); // sets register pointer to (0x8f)
  Wire.endTransmission(); // stop transmitting

  delay(20); // cekanje 20 ms za slanje
  
  Wire.requestFrom((int)LIDARLite_ADDRESS, 2); // request 2 bytes from LIDAR-Lite

  if(2 <= Wire.available()) // Ako su 2 bajta primljena
  {
    val = Wire.read(); // primi viši bajt (prepiši preko prošlog čitanja)
    val = val << 8; // shift-aj viši bajt da bude viši 8 bitova
    val |= Wire.read(); // primi niži bajt kao nižih 8 bitova
  }
  
  return val;
}

void serialPrintRange(int posX,int posY, int distance)
{
    Serial.print(posX);
    Serial.print(",");
    Serial.print(posY);
    Serial.print(",");
    Serial.println(distance);
}

void loop()
{
  while (millis() > 30000) {
  for(posX = 60; posX <= 100; posX += 1)    // zakretanje X osi
  {
    for(posY = 100; posY <= 180; posY += 1) {    // zakretanje Y osi
      myXservo.write(posX);
      myYservo.write(posY);
      distance = lidarGetRange();
      serialPrintRange(posX, posY, distance);
      

      }
      for(posY = 180; posY >= 100; posY -= 1) {
        myXservo.write(posX);
        myYservo.write(posY);
        distance = lidarGetRange();
        serialPrintRange(posX,posY, distance);
        
        
      }
      delay(20);
  }

      for(posX = 100; posX >= 60; posX -= 1) {

        for(posY = 100; posY <= 180; posY += 1) {
          myXservo.write(posX);
          myYservo.write(posY);
          distance = lidarGetRange();
          serialPrintRange(posX, posY, distance);
          
        }
        for(posY = 180; posY >= 100; posY -= 1) {
          myXservo.write(posX);
          myYservo.write(posY);
          distance = lidarGetRange();
          serialPrintRange(posX, posY, distance);
          
        }
        delay(20);
      }
  }
    // myXservo.write(80);
   //  myYservo.write(140);
  //  delay(25);
}

