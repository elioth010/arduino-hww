/*  
 *  eHealth sensor platform for Arduino and Raspberry from Cooking-hacks.
 *  
 *  Copyright (C) Libelium Comunicaciones Distribuidas S.L. 
 *  http://www.libelium.com 
 *  
 *  This program is free software: you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation, either version 3 of the License, or 
 *  (at your option) any later version. 
 *  a
 *  This program is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License 
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *  
 *  Version:           2.0
 *  Design:            David Gascón 
 *  Implementation:    Luis Martin & Ahmad Saad
 */


#include "PinChangeInt.h"
#include "eHealth.h"
#include <Wire.h>

int cont = 0;
boolean readBloodData = false;

int pulse = 0;
int oxygen = 0;
int airFlow = 0;
float temperature = 0.0;
int bloodPressureSis = 0;
int bloodPressureDia = 0;

int readingCount = 0;

void setup() {
  eHealth.initPulsioximeter();
  //eHealth.readBloodPressureSensor();
  //Attach the inttruptions for using the pulsioximeter.
  PCintPort::attachInterrupt(6, readPulsioximeter, RISING);
  Wire.begin();
  Serial.begin(9600);
  readingCount = 0;
}  

void loop() { 
  getCurrentAirFlow();
  if(readingCount >= 60){
    pulse = getCurrentPulse();
    oxygen = getCurrentOxygen();
    temperature = getCurrentTemperature();
    readBloodPressureData();
    Serial.println("Data send");
    sendData();
    airFlow = 0;
    readingCount = 0;
  }
  readingCount = readingCount + 1;
  //sendData();
  delay(1000);
  
  /*if(!readBloodData){
    readBloodPressureData();
  }*/
}
//Include always this code when using the pulsioximeter sensor
//=========================================================================
void readPulsioximeter(){
  cont ++;
  if (cont == 50) { //Get only of one 50 measures to reduce the latency
    eHealth.readPulsioximeter();
    cont = 0;
  }
}

void readBloodPressureData(){
  uint8_t numberOfData = eHealth.getBloodPressureLength();
  //Serial.print(F("Number of measures : "));
  //Serial.println(numberOfData, DEC);
  delay(100);
  for (int i = 0; i<numberOfData; i++) {
    bloodPressureSis =  30+eHealth.bloodPressureDataVector[i].systolic;
    bloodPressureDia =  eHealth.bloodPressureDataVector[i].diastolic;
  }
  readBloodData = true;
}

int getCurrentPulse(){
  return eHealth.getBPM();
}

int getCurrentOxygen(){
  return eHealth.getOxygenSaturation();
}

int getCurrentTemperature(){
  return eHealth.getTemperature();
}

int getCurrentAirFlow(){
  if(eHealth.getAirFlow()> 40){
    airFlow++;
  }
    
}

void sendData(){
  char outBuf[10];
  String data = String(pulse) + "," + String(oxygen);
  dtostrf(temperature, 2 , 1 ,outBuf);
  data += "|" + String(outBuf) +"|";
  data += String(airFlow) +"|";
  data += String(bloodPressureDia) + ",";
  data += String(bloodPressureDia);

  Wire.beginTransmission(8);
  Wire.write(string2char(data));
  Wire.endTransmission();
  Wire.flush();
}  

char* string2char(String command){ 
  if(command.length()!=0){ 
    char *p = const_cast<char*>(command.c_str()); 
    return p; 
  } 
}

