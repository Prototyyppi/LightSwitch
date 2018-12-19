#ifndef LightSwitch_header
#define LightSwitch_header
//#include <ESP8266WiFi.h>
#include "Arduino.h"

/* Declare names for pin numbers */
enum buttons {
  BUTTON_GPIO = 2,
  GREEN_GPIO,
  YELLOW_GPIO,
  RED_GPIO,
};
//const int BUZZER_GPIO = 4; Unused
#define ALL_CLEAR 101


class LightSwitch
{
  public:
    LightSwitch();
    int toggle_light(int COMMAND);
    int get_response(int TIMEOUT);
    int button_pressed(int GPIO);
  private:
    int _send_query(String data);
    /* Wi-Fi Settings */
    String API_KEY = "XXXXXXXX";
//    WiFiClient client;
};


/* Declare some failure codes */
enum return_types{
  EOK = 0,
  EFAIL,
  ETIMEOUT,
  UNCLEAR,
};

// Library: esp8266 by ESP8266 Community
// Wi-Fi Settings
//ssid = "XXX"; // your wireless network name (SSID)
//password = "XXXXXXXX"; // your Wi-Fi network password
//API_KEY = "XXXXXXXX";





#endif //LightSwitch_header
