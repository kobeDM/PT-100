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


// Use software SPI: CS, DI, DO, CLK

Adafruit_MAX31865 thermo2 = Adafruit_MAX31865(13, 12, 11, 10);
Adafruit_MAX31865 thermo = Adafruit_MAX31865(7 ,6, 5, 4);
//Adafruit_MAX31865 thermo2 = Adafruit_MAX31865(10, 11, 12, 13);
//Adafruit_MAX31865 thermo = Adafruit_MAX31865(5 ,4, 3, 2);
 
// The value of the Rref resistor. Use 430.0 for PT100 and 4300.0 for PT1000
#define RREF      430.0
// The 'nominal' 0-degrees-C resistance of the sensor
// 100.0 for PT100, 1000.0 for PT1000
#define RNOMINAL  100.0

void setup() {
  Serial.begin(115200);
//  Serial.println("Adafruit MAX31865 PT100 Sensor Test!");

//  thermo.begin(MAX31865_3WIRE);  // set to 2WIRE or 4WIRE as necessary
  thermo.begin(MAX31865_4WIRE);  // set to 2WIRE or 4WIRE as necessary
  thermo2.begin(MAX31865_4WIRE);  // set to 2WIRE or 4WIRE as necessary
}


void loop() {
  uint16_t rtd = thermo.readRTD();

//  Serial.print("RTD value: "); Serial.println(rtd);
  float ratio = rtd;
  ratio /= 32768;
 // Serial.print("Ratio = "); Serial.println(ratio,8);
 // Serial.print("Resistance = "); Serial.println(RREF*ratio,8);
  //Serial.print("Temperature = "); Serial.println(thermo.temperature(RNOMINAL, RREF));

  uint16_t rtd2 = thermo2.readRTD();
//  Serial.print("RTD2 value: "); Serial.println(rtd2);
  float ratio2 = rtd2;
  ratio2 /= 32768;
 // Serial.print("Ratio2 = "); Serial.println(ratio2,8);
//  Serial.print("Resistance2 = "); Serial.println(RREF*ratio2,8);
 // Serial.print("Temperature2 = "); Serial.println(thermo2.temperature(RNOMINAL, RREF));


  // Check and print any faults
  uint8_t fault = thermo.readFault();
  uint8_t fault2 = thermo2.readFault();

  if (fault) {
      Serial.print("-1\t-300\t");
    thermo.clearFault();
    }
  
  else{ 
    Serial.print(RREF*ratio,2);Serial.print("\t");
    Serial.print(thermo.temperature(RNOMINAL, RREF));Serial.print("\t");
  }

  
  if (fault2) {
      Serial.print("-1\t-300");
    thermo2.clearFault();
    }
    
    else{
    Serial.print(RREF*ratio2,2);Serial.print("\t");
  Serial.print(thermo2.temperature(RNOMINAL, RREF));
    }
 Serial.println();
  delay(1000);
}
    

 
