#include "LightSwitch.h"
#include <SoftwareSerial.h>

LightSwitch* lightswitch = new LightSwitch();

int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;

#define RX 9
#define TX 10
SoftwareSerial esp8266(RX,TX); 

void setup() {
  /* Debugging */
  Serial.begin(9600);
  /* Led pins are output pins */
  pinMode(BUTTON_GPIO, INPUT);
  pinMode(GREEN_GPIO, OUTPUT);
  pinMode(YELLOW_GPIO, OUTPUT);
  pinMode(RED_GPIO, OUTPUT);

  String ssid = "XXX";
  String passwd = "XXXXXXXX";
  esp8266.begin(115200);
  /* Check connection */
  sendCommand("AT",5,"OK");
  /* Disable AP mode */
  sendCommand("AT+CWMODE=1",5,"OK");
  /* Connect */
  sendCommand("AT+CWJAP=\""+ ssid +"\",\""+ passwd +"\"",20,"OK");
}

void loop() {
  int response = EFAIL;
  /* 1. Poll button presses */
  while(lightswitch->button_pressed(BUTTON_GPIO))
    delay(50);
  /* 2. Button pressed, get response */
  lightswitch->toggle_light(YELLOW_GPIO);
  response = get_response(5000);
  /* 3. Toggle light accordingly */
  if (response != ETIMEOUT)
    lightswitch->toggle_light(response);
  else
    lightswitch->toggle_light(RED_GPIO);
  delay(5000);
  /* 4. Reset everything */
  lightswitch->toggle_light(ALL_CLEAR);
}

void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }

    /* client.println("POST /update HTTP/1.1");
    client.println("Host: api.thingspeak.com");
    client.println("User-Agent: ARDUINO/1.337");
    client.println("Connection: close");
    client.println("X-THINGSPEAKAPIKEY: " + API_KEY);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Content-Length: " + String(data.length()));
    client.println("");
    client.print(data);*/
