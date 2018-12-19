#include "LightSwitch.h"
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

LightSwitch* lightswitch = new LightSwitch();

int valSensor = 1;
int key = 0;
int value = 0;

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
  sendCommand("AT","OK");
  /* Disable AP mode */
  sendCommand("AT+CWMODE=1","OK");
  /* Connect */
  sendCommand("AT+CWJAP=\""+ ssid +"\",\""+ passwd +"\"","OK");
}

void loop() {
  String response = "";
  int instruction;
  int timeout = 0;
  /* 1. Poll button presses */
  while(lightswitch->button_pressed(BUTTON_GPIO))
    delay(50);
  /* 2. Button pressed, get response */
  lightswitch->toggle_light(YELLOW_GPIO);
  response = sendData("1", "2", "3");
  while ( (instruction == UNCLEAR) && (timeout < ETIMEOUT)) {
    timeout++;
    response = sendData("1", "2", "3");
    instruction = parse_json(response);
  }
  /* 3. Toggle light accordingly */
  if (instruction != UNCLEAR)
    lightswitch->toggle_light(instruction);
  else
    lightswitch->toggle_light(RED_GPIO);
  delay(5000);
  /* 4. Reset everything */
  lightswitch->toggle_light(ALL_CLEAR);
}

bool sendCommand(String command, char readReplay[]) {
  int command_timeout = 10, command_retry = 0;
  boolean found = false;

  Serial.println("SENDING: " + command);
    esp8266.println(command);//at+cipsend

  /*while(command_retry < command_timeout)
  {
    esp8266.println(command);//at+cipsend
    /*if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
    command_retry++;
  }
  
  if(found == true)
    Serial.println("OK");
  else
    Serial.println("NOK");
  */
  return found;

 }


String sendData(String data, String url, String host) {

  sendCommand("AT+CIPSTART=\"TCP\",\"" + host + "\",80", "OK");
  delay(1000);
    String xfer = "POST " + url + " HTTP/1.1\r\n" +
    "Host: " + host + "\r\n" + 
    "Accept: *" + "/" + "*\r\n" +
    "User-Agent: ARDUINO/1.337\r\n" +
    "Content-Type: application/x-www-form-urlencoded\r\n" +
    "Content-Length: " + String(data.length()) + "\r\n\r\n"
    + data;
    sendCommand("AT+CIPSEND=" + xfer.length(), "NONE");
    delay(500);
    if(esp8266.find(">")) { Serial.println("Sending.."); esp8266.print(xfer);}
    if( esp8266.find("SEND OK")) { Serial.println("Packet sent");}
    String response;
    while (esp8266.available()) {

    String tmpResp = esp8266.readString();
    response = response + tmpResp;
    }
    Serial.println(response);
    esp8266.println("AT+CIPCLOSE");
    return response;

}

int parse_json(String response) {

  char input[200];
  response.toCharArray(input,response.length());

  StaticJsonDocument<200> jsonBuffer;
 // JsonArray& arr = jsonBuffer.parseArray(input);
  deserializeJson(jsonBuffer, input);
  JsonObject object = jsonBuffer.as<JsonObject>();
  //if (object.success())
  //{
    //JsonObject object = doc.as<JsonObject>();
    const char* inst = object["instruction"];
    value = strtoul(inst, NULL, 10);
  //} else
   // return EFAIL;
}
