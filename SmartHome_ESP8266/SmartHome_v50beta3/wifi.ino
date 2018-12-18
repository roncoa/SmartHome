#include <ESP8266WiFi.h>

void setup_wifi() {
  /* Possible values for WiFi.status()
    WL_NO_SHIELD                 = 255   // for compatibility with WiFi Shield library
    WL_IDLE_STATUS               = 0
    WL_NO_SSID_AVAIL             = 1
    WL_SCAN_COMPLETED            = 2
    WL_CONNECTED                 = 3
    WL_CONNECT_FAILED            = 4
    WL_CONNECTION_LOST           = 5
    WL_DISCONNECTED              = 6
  */
  int i = 0;
  delay(50);
  // Debug_MSG_LN("** Scan Networks **");
  // byte numSsid = WiFi.scanNetworks();
  // Debug_MSG("SSID List:");
  // Debug_MSG_LN((String) numSsid);
  Debug_MSG("Connecting to ");
  Debug_MSG((String) ssid);
  Debug_MSG(" ");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while ((WiFi.status() != WL_CONNECTED) and i < MAX_RET_WIFI) {
    ++i;
    Debug_MSG(".");
    digitalWrite(Status_LED_GPIO, 0 ^ Flag_inversione_Status_LED_GPIO);
    delay(250);
    digitalWrite(Status_LED_GPIO, 1 ^ Flag_inversione_Status_LED_GPIO);
    delay(250);
  }
  if (WiFi.status() != WL_CONNECTED) {
    Debug_MSG_LN("  NOT connected!");
  }
  else {
    Debug_MSG("  connected! (");
    Debug_MSG("RSSI:");
    Debug_MSG((String) WiFi.RSSI());
    Debug_MSG_LN(")");
    Debug_MSG("Local  IP   : ");
    Debug_MSG_LN(WiFi.localIP().toString());
    Debug_MSG("MAC address : ");
    WiFi.macAddress(mac);
    Debug_MSG_LN(macToStr(mac));
  }
  Debug_MSG("Synchronizing DATE/TIME: ");
  Debug_MSG_LN(getTime());
}

void loop_wifi() {
  /*Possible values for client.state()
    MQTT_CONNECTION_TIMEOUT      = -4
    MQTT_CONNECTION_LOST         = -3
    MQTT_CONNECT_FAILED          = -2
    MQTT_DISCONNECTED            = -1
    MQTT_CONNECTED               =  0
    MQTT_CONNECT_BAD_PROTOCOL    =  1
    MQTT_CONNECT_BAD_CLIENT_ID   =  2
    MQTT_CONNECT_UNAVAILABLE     =  3
    MQTT_CONNECT_BAD_CREDENTIALS =  4
    MQTT_CONNECT_UNAUTHORIZED    =  5
  */
  if (TIMER_REFRESH_CONNESSIONE.Wait(TEMPO_REFRESH_CONNESSIONE)) {
    Debug_MSG_LN("Check connections:");
    if (WiFi.status() != WL_CONNECTED) {
      Debug_MSG("WIFI ERRORE DI RETE: ");
      switch (WiFi.status()) {
        case 255:
          Debug_MSG_LN("WL_NO_SHIELD");
          break;
        case 0:
          Debug_MSG_LN("WL_IDLE_STATUS");
          break;
        case 1:
          Debug_MSG_LN("WL_NO_SSID_AVAIL");
          break;
        case 2:
          Debug_MSG_LN("WL_SCAN_COMPLETED");
          break;
        case 3:
          Debug_MSG_LN("WL_CONNECTED");
          break;
        case 4:
          Debug_MSG_LN("WL_CONNECT_FAILED");
          break;
        case 5:
          Debug_MSG_LN("WL_CONNECTION_LOST");
          break;
        case 6:
          Debug_MSG_LN("WL_DISCONNECTED");
          break;
        default:
          Debug_MSG_LN("WL_UNKNOWN_ERROR");
          break;
      }
      //Debug_MSG_LN("RIAVVIO L'ESP");
      delay(250);
      WiFi.disconnect(true);
      delay(250);
      //ESP.restart();
      setup_wifi();
    }
    else {
      Debug_MSG("WIFI OK! - ");
      Debug_MSG_LN(WiFi.localIP().toString());
    }
    if (!client.connected()) {
      Debug_MSG("MQTT ERRORE DI RETE: ");
      switch (client.state()) {
        case -4:
          Debug_MSG_LN("MQTT_CONNECTION_TIMEOUT");
          break;
        case -3:
          Debug_MSG_LN("MQTT_CONNECTION_LOST");
          break;
        case -2:
          Debug_MSG_LN("MQTT_CONNECT_FAILED");
          break;
        case -1:
          Debug_MSG_LN("MQTT_DISCONNECTED");
          break;
        case 0:
          Debug_MSG_LN("MQTT_CONNECTED");
          break;
        case 1:
          Debug_MSG_LN("MQTT_CONNECT_BAD_PROTOCOL");
          break;
        case 2:
          Debug_MSG_LN("MQTT_CONNECT_BAD_CLIENT_ID");
          break;
        case 3:
          Debug_MSG_LN("MQTT_CONNECT_UNAVAILABLE");
          break;
        case 4:
          Debug_MSG_LN("MQTT_CONNECT_BAD_CREDENTIALS");
          break;
        case 5:
          Debug_MSG_LN("MQTT_CONNECT_UNAUTHORIZED");
          break;
        default:
          Debug_MSG_LN("MQTT_UNKNOWN_ERROR");
          break;
      }
      setup_MQTT();
    }
    else {
      Debug_MSG_LN("MQTT OK!");
      Send_Online();
    }
    Debug_MSG("FreeHeap:");
    Debug_MSG_LN((String) ESP.getFreeHeap());
  }
}
