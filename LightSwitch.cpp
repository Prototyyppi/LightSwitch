#include "LightSwitch.h"
#include "Arduino.h"


LightSwitch::LightSwitch() {
	toggle_light(ALL_CLEAR);
  _send_query("POWERING UP");
}

/* Function for toggling light gpios on arduino board */
int LightSwitch::toggle_light(int command) {
  /* If we want to toggle light gpio or it is a special sommand */
  if ((command > BUTTON_GPIO) && (command <= RED_GPIO))
    digitalWrite(command, HIGH);
  else if (command == ALL_CLEAR) {
      digitalWrite(RED_GPIO, LOW);
      digitalWrite(YELLOW_GPIO, LOW);
      digitalWrite(RED_GPIO, LOW);
   } else
     return EFAIL;

   return EOK;
}

int LightSwitch::button_pressed(int GPIO) {
  return digitalRead(GPIO);
}

int LightSwitch::get_response(int timeout) {
  int i = 0;
  _send_query("NULL");
  for (i = 0; i < timeout; i += 100) {
      //read_query_response();
      delay(100);
    }
}

int LightSwitch::_send_query(String data) {
    client.println("POST /update HTTP/1.1");
    client.println("Host: api.thingspeak.com");
    client.println("User-Agent: ARDUINO/1.337");
    client.println("Connection: close");
    client.println("X-THINGSPEAKAPIKEY: " + API_KEY);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Content-Length: " + String(data.length()));
    client.println("");
    client.print(data);
}
