# arduino-hww
Arduino Source code for eHealth Shield monitor 

HWW es una solucion para pacientes con problemas de salud que tienen que ser monitoreados de manera remota.

Consiste en un Arduino UNO y un Arduino Mega 2560 un shield eHealth V2 y un Shield Ethernet Arduinio.

eHealth V2 Link : https://www.cooking-hacks.com/ehealth-sensor-shield-biometric-medical-arduino-raspberry-pi
Arduino Mega 2560: https://www.arduino.cc/en/Main/ArduinoBoardMega2560
Arduino Uno: https://www.arduino.cc/en/Main/ArduinoBoardUno
Arduino Ethernet Shield: https://www.arduino.cc/en/Main/ArduinoEthernetShield

El programa de monitor.ino tiene un arduino uno que conecta el shield eHealth V2 y a el se le conectan los sensores disponibles,
el programa ethernet.ino tiene la parte de slave writer y recibe los datos del arduino uno hacia el arduino mega y enviarlos a traves del 
Shield Ethernet.
