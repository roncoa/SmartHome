#include <ESP8266WiFi.h>
#include <TPush.h>

int          retry_connessioni_wifi = 0;
int          retry_connessioni_mqtt = 0;

TPush TIMER_REFRESH_CONNESSIONE;

WiFiEventHandler gotIpEventHandler, disconnectedEventHandler;

void setup_wifi() {
  gotIpEventHandler = WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP & event)
  {
    Debug_MSG_LN("WIFI  connected!");
    Debug_MSG_LN(" SSID       : " + (String) WiFi.SSID());
    Debug_MSG_LN(" RSSI       : " + (String) WiFi.RSSI());
    Debug_MSG_LN(" Local IP   : " + WiFi.localIP().toString());
    WiFi.macAddress(mac);
    Debug_MSG_LN(" MAC address: " + macToStr(mac));
    Debug_MSG_LN("Synchronizing DATE/TIME: " + getTime());
#ifdef ArduinoIDE_OTA
    setup_OTA();
#endif
    if (reconnect()) {
      lastReconnectAttempt = 0;
    }
  });
  disconnectedEventHandler = WiFi.onStationModeDisconnected([](const WiFiEventStationModeDisconnected & event)
  {
    Debug_MSG_LN("Wifi error:" + (String)WiFi.status());
  });
  Debug_MSG_LN("Setup Wifi");
  WiFi.setAutoReconnect(true);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.setOutputPower(0);
  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
}

void loop_wifi() {
  /*
  Possible values for WiFi.status()
      WL_NO_SHIELD                 = 255   // for compatibility with WiFi Shield library
      WL_IDLE_STATUS               = 0
      WL_NO_SSID_AVAIL             = 1
      WL_SCAN_COMPLETED            = 2
      WL_CONNECTED                 = 3
      WL_CONNECT_FAILED            = 4
      WL_CONNECTION_LOST           = 5
      WL_DISCONNECTED              = 6
  Possible values for client.state()
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
  if (TIMER_REFRESH_CONNESSIONE.Wait(TEMPO_REFRESH_CONNESSIONI)) {
    Debug_MSG_LN("Check connections:");
    if (WiFi.localIP().toString() == "0.0.0.0" or WiFi.localIP().toString() == "(IP unset)") {
      retry_connessioni_wifi++;
      Debug_MSG(" WIFI ERRORE DI RETE (" + (String)retry_connessioni_wifi + "/" + MAX_RETRY_CONNESSIONI + ") : ");
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
      WiFi.disconnect();
      setup_wifi();
    }
    else {
      retry_connessioni_wifi = 0;
      Debug_MSG_LN(" WIFI OK! - " + WiFi.localIP().toString());
    }
    if (!client.connected()) {
      retry_connessioni_mqtt++;
      Debug_MSG(" MQTT ERRORE DI RETE (" + (String)retry_connessioni_mqtt + "/" + MAX_RETRY_CONNESSIONI + ") : ");
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
    }
    else {
      retry_connessioni_mqtt = 0;
      Debug_MSG_LN(" MQTT OK! - " + (String)mqtt_server + ":" + (String)mqtt_port);
      Send_HA_Online();
    }
    if ((retry_connessioni_wifi >= MAX_RETRY_CONNESSIONI or retry_connessioni_mqtt >= MAX_RETRY_CONNESSIONI) and MAX_RETRY_CONNESSIONI != 0) {
      Debug_MSG_LN("RIAVVIO L'ESP (da MAX_RETRY_CONNESSIONI)");
      WiFi.disconnect();
      delay(500);
      ESP.restart();
    }
    Debug_MSG_LN(" FreeHeap:" + (String) ESP.getFreeHeap());
  }
#ifdef ArduinoIDE_OTA
  loop_OTA();
#endif
}
