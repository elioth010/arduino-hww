/*
   Web client sketch for IDE v1.0.1 and w5100/w5200
   Uses POST method.
   Posted November 2012 by SurferTim
*/

#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>

byte mac[] = {  
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

//Change to your server domain
char serverName[] = "52.36.27.188";

// change to your server's port
int serverPort = 80;

// change to the page on that server
char pageName[] = "/salud/bitacora/guardar";

EthernetClient client;
int totalCount = 0; 
// insure params is big enough to hold your variables
char params[1000];


// set this to the number of milliseconds delay
// this is 30 seconds
#define delayMillis 30000UL

unsigned long thisMillis = 0;
unsigned long lastMillis = 0;

IPAddress ip(10,0,0,100);
IPAddress gateway(10,0,0, 1);
IPAddress subnet(255, 255, 255, 0);

void setup() {
  
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  
  Serial.begin(9600);

  // disable SD SPI
  pinMode(4,OUTPUT);
  digitalWrite(4,HIGH);

  Serial.print(F("Starting ethernet..."));
  Ethernet.begin(mac, ip, gateway, gateway, subnet);
  Serial.println(Ethernet.localIP());

  delay(2000);
  Serial.println(F("Ready"));
}

void loop(){
  delay(100);
}


byte postPage(char* domainBuffer,int thisPort,char* page,char* thisData){
  int inChar;
  char outBuf[64];

  Serial.print(F("connecting..."));

  if(client.connect(domainBuffer,thisPort) == 1)
  {
    Serial.println(F("connected"));

    // send the header
    sprintf(outBuf,"POST %s HTTP/1.1",page);
    client.println(outBuf);
    sprintf(outBuf,"Host: %s",domainBuffer);
    client.println(outBuf);
    client.println(F("Connection: close\r\nContent-Type: application/x-www-form-urlencoded"));
    sprintf(outBuf,"Content-Length: %u\r\n",strlen(thisData));
    client.println(outBuf);

    // send the body (variables)
    client.print(thisData);
  } 
  else
  {
    Serial.println(F("failed"));
    return 0;
  }

  int connectLoop = 0;

  while(client.connected())
  {
    while(client.available())
    {
      inChar = client.read();
      Serial.write(inChar);
      connectLoop = 0;
    }

    delay(1);
    connectLoop++;
    if(connectLoop > 10000)
    {
      Serial.println();
      Serial.println(F("Timeout"));
      client.stop();
    }
  }

  Serial.println();
  Serial.println(F("disconnecting."));
  client.stop();
  return 1;
}

void receiveEvent(int param0) {
  char c[1000]; 
  int i= 0; 
  while (0 < Wire.available()) { // loop through all but the last
    c[i] = Wire.read(); // receive byte as a character
    i++;
  }
  /*{\"sensor\":1, \"lecturas\" : [64, 98]},";
    postInfo += "{\"sensor\":2, \"lecturas\" : [43]},";
    postInfo += "{\"sensor\":3, \"lecturas\" : [34]},";
    postInfo += "{\"sensor\":4, \"lecturas\" : [120,80,65]}*/
  String data = String(c);
  Serial.println(data);
  Wire.flush();
  //sendPost(data);
}

void sendPost(String data){
  // If using a static IP, comment out the next line
  // Ethernet.maintain();

  thisMillis = millis();
  if(thisMillis - lastMillis > delayMillis)
  {
    lastMillis = thisMillis;
    Serial.println("Set the post info");
    // params must be url encoded.
    IPAddress address= Ethernet.localIP();
    String postInfo="info={\"data\": \""+data;
    postInfo += "\", \"ip\" : \"";
    postInfo += String(address[0]) + "." + String(address[1]) + "." + String(address[2]) + "." + String(address[3]);
    postInfo += "\"}";
    //sprintf(params,"info=%c",postInfo);
    postInfo.toCharArray(params,1000);
    Serial.println(serverName);
    Serial.println(serverPort);
    Serial.println(pageName);
    //Serial.println(params);
    Serial.println(params);    
    if(!postPage(serverName,serverPort,pageName,params)) Serial.print(F("Fail "));
    else Serial.print(F("Pass "));
    totalCount++;
    Serial.println(totalCount,DEC);
  }      
}
