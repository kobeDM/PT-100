//KRX94-2050 read by ESP32 

#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_MAX31865.h>

/*************************************************** 
  This is a library for the Adafruit PT100/P1000 RTD Sensor w/MAX31865

  Designed specifically to work with the Adafruit RTD Sensor
  ----> https://www.adafruit.com/products/3328

  This sensor uses SPI to communicate, 4 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

// Use software SPI: CS, SDI, SDO, CLK
//channel selector
int CS1=0;
int CS2=15;
int CS3=32;
int CS4=33;

//others 
int SDI_12=16;
int SDO_12=17;
int CLK_12=5;
int SDI_34=25;
int SDO_34=26;
int CLK_34=27;

//Adafruit_MAX31865 thermo2 = Adafruit_MAX31865(10, 11, 12, 13);
Adafruit_MAX31865 thermo1 = Adafruit_MAX31865(CS1, SDI_12, SDO_12,5);
Adafruit_MAX31865 thermo2= Adafruit_MAX31865(CS2, SDI_12, SDO_12,5);
Adafruit_MAX31865 thermo3= Adafruit_MAX31865(CS3, SDI_34, SDO_34,CLK_34);
Adafruit_MAX31865 thermo4= Adafruit_MAX31865(CS4, SDI_34, SDO_34,CLK_34);
// The value of the Rref resistor. Use 430.0 for PT100 and 4300.0 for PT1000
#define RREF      430.0
// The 'nominal' 0-degrees-C resistance of the sensor
// 100.0 for PT100, 1000.0 for PT1000
#define RNOMINAL  100.0

void setup() {
  Serial.begin(115200);
//  Serial.println("Adafruit MAX31865 PT100 Sensor Test!");

//  thermo.begin(MAX31865_3WIRE);  // set to 2WIRE or 4WIRE as necessary
  thermo1.begin(MAX31865_4WIRE);  // set to 2WIRE or 4WIRE as necessary
  thermo2.begin(MAX31865_4WIRE);  // set to 2WIRE or 4WIRE as necessary
  thermo3.begin(MAX31865_4WIRE);  // set to 2WIRE or 4WIRE as necessary
  thermo4.begin(MAX31865_4WIRE);  // set to 2WIRE or 4WIRE as necessary
}


void loop() {
  uint16_t rtd1 = thermo1.readRTD();
  uint16_t rtd2 = thermo2.readRTD();
  uint16_t rtd3 = thermo3.readRTD();
  uint16_t rtd4 = thermo4.readRTD();

  float ratio1 = rtd1;
  ratio1 /= 32768;
  float ratio2 = rtd2;
  ratio2 /= 32768;
  float ratio3 = rtd3;
  ratio3 /= 32768;
  float ratio4 = rtd4;
  ratio4 /= 32768;

  // Check and print any faults
  uint8_t fault1 = thermo1.readFault();
  uint8_t fault2 = thermo2.readFault();
  uint8_t fault3 = thermo3.readFault();
  uint8_t fault4 = thermo4.readFault();

  if (fault1) {
      Serial.print("-1\t-300\t");
    thermo1.clearFault();
    }
  else{ 
    Serial.print(RREF*ratio1,2);Serial.print("\t");
    Serial.print(thermo1.temperature(RNOMINAL, RREF));Serial.print("\t");
  }

  
  if (fault2) {
      Serial.print("-1\t-300\t");
    thermo2.clearFault();
    }    
    else{
    Serial.print(RREF*ratio2,2);Serial.print("\t");
  Serial.print(thermo2.temperature(RNOMINAL, RREF));Serial.print("\t");
    }

  if (fault3) {
      Serial.print("-1\t-300\t");
    thermo3.clearFault();
    }    
    else{
    Serial.print(RREF*ratio3,2);Serial.print("\t");
  Serial.print(thermo3.temperature(RNOMINAL, RREF));Serial.print("\t");
    }

  if (fault4) {
      Serial.print("-1\t-300");
    thermo4.clearFault();
    }    
    else{
    Serial.print(RREF*ratio4,2);Serial.print("\t");
  Serial.print(thermo4.temperature(RNOMINAL, RREF));
    }


    
 Serial.println();
  delay(1000);
}
    

 
