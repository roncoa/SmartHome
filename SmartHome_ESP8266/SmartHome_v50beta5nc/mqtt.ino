#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define TEMPO_RETRY_MQTT                5000

boolean firsttime_MQTT = true;
unsigned long lastReconnectAttempt = 0;
unsigned long previousMillis_MQTT_loop = 0;
unsigned long noww = 0;

String MQTT_ACK     = "ack";
String MQTT_RESET   = "reset";
String MQTT_REBOOT  = "reboot";
String MQTT_INFO    = "info";
String Will_offline = "offline";
String Will_online  = "online";
String WillTopicStr = WillTopic;

WiFiClient espClient;
PubSubClient client(espClient);

boolean reconnect() {
  if (macToStr(mac) == "000000000000") {
    return (false);
  }
  String clientName = "SmartHomeClient-";
  clientName += macToStr(mac);
  //clientName += "-";
  //clientName += millis();
  client.disconnect();
  if (WillTopicStr == "" ) WillTopicStr = macToStr(mac);
  if (client.connect(string2char(clientName), mqtt_user, mqtt_password,   string2char(WillTopicStr), 1, false,  string2char(Will_offline))) {
    Debug_MSG_LN("MQTT connected!");
    delay(100);
    Send_Alive();
    Send_ACK();
    Send_HA_Online();
    SubscribeTopic();
  }
  else {
    Debug_MSG_LN("MQTT error:" + (String)client.state());
  }
  return client.connected();
}

void setup_MQTT() {
  delay(1000);
  Debug_MSG_LN("Setup MQTT");
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop_MQTT() {
  if (WiFi.localIP().toString() == "0.0.0.0" or WiFi.localIP().toString() == "(IP unset)") {
    return;
  }
  noww = millis();
  if (!client.connected()) {
    if (noww - lastReconnectAttempt > TEMPO_RETRY_MQTT) {
      Debug_MSG_LN("MQTT error:" + (String)client.state());
      lastReconnectAttempt = noww;
      // Attempt to reconnect
      if (reconnect()) {
        lastReconnectAttempt = 0;
      }
    }
  }
  else {
    if (noww - previousMillis_MQTT_loop >= 100)
    {
      previousMillis_MQTT_loop = noww;
      client.loop();
    }
  }
}

void Send_Alive() {
  String payload = macToStr(mac);
  payload += " ";
  payload += WiFi.localIP().toString();
  payload += " v";
  payload += Versione;
  payload += " start at ";
  payload += getTime();
  start_millis = millis();
  client.publish(ACK_Topic, (char*) payload.c_str());
  delay(100);
  Debug_MSG_LN(payload);
#ifdef HomeAssistant_discovery
  if (firsttime_MQTT == true) {
    firsttime_MQTT = false;
    HA_discovery_cancella_nodi();
    HA_discovery_crea_nodi();
  }
#endif
}

void Send_ACK() {
#if Tapparella_nodi>0
  ACK_Tapparella();
#endif
#if Termostato_nodi>0
  ACK_Termostato();
#endif
#if Interruttore_nodi>0
  ACK_Interruttore();
#endif
#if Impulso_nodi>0
  ACK_Impulso();
#endif
#if Temporizzatore_nodi>0
  ACK_Temporizzatore();
#endif
#if Sensore_nodi>0
  ACK_Sensore();
#endif
#if Display_nodi>0
  ACK_Display();
#endif
}

void Send_HA_Online() {
  client.publish((char*) WillTopicStr.c_str(), (char*) Will_online.c_str());
  delay(100);
  Debug_MSG_LN(" " + WillTopicStr + " -> " + Will_online);
}

void HA_discovery_crea_nodi() {
#if Tapparella_nodi>0
  HA_discovery_Tapparella();
#endif
#if Termostato_nodi>0
  HA_discovery_Termostato();
#endif
#if Interruttore_nodi>0
  HA_discovery_Interruttore();
#endif
#if Impulso_nodi>0
  HA_discovery_Impulso();
#endif
#if Temporizzatore_nodi>0
  HA_discovery_Temporizzatore();
#endif
#if Sensore_nodi>0
  HA_discovery_Sensore();
#endif
#if Display_nodi>0
  HA_discovery_Display();
#endif
}

void HA_discovery_cancella_nodi() {
  String HAtopic;
  String HApayload;
  HApayload = "";
  Debug_MSG("Rimuovo nodi Home assistant discovery ");
  for (int i = 0; i < 4; i++) {
    HAtopic = "homeassistant/cover/";
    HAtopic += macToStr(mac);
    HAtopic += "/";
    HAtopic += "TAP";
    HAtopic += i;
    HAtopic += "/config";
    client.publish((char*) HAtopic.c_str(), (char*) HApayload.c_str());
    delay(100);
    Debug_MSG(".");
  }
  for (int i = 0; i < 9; i++) {
    HAtopic = "homeassistant/switch/";
    HAtopic += macToStr(mac);
    HAtopic += "/";
    HAtopic += "INT";
    HAtopic += i;
    HAtopic += "/config";
    client.publish((char*) HAtopic.c_str(), (char*) HApayload.c_str());
    delay(100);
    Debug_MSG(".");
    HAtopic = "homeassistant/switch/";
    HAtopic += macToStr(mac);
    HAtopic += "/";
    HAtopic += "IMP";
    HAtopic += i;
    HAtopic += "/config";
    client.publish((char*) HAtopic.c_str(), (char*) HApayload.c_str());
    delay(100);
    Debug_MSG(".");
    HAtopic = "homeassistant/switch/";
    HAtopic += macToStr(mac);
    HAtopic += "/";
    HAtopic += "TEM";
    HAtopic += i;
    HAtopic += "/config";
    client.publish((char*) HAtopic.c_str(), (char*) HApayload.c_str());
    delay(100);
    Debug_MSG(".");
  }
  Debug_MSG_LN(" OK!");
}

void SubscribeTopic() {
  client.subscribe(ACK_Topic);
  delay(250);
  Debug_MSG_LN("MQTT subscribe " + String(ACK_Topic));
#if Tapparella_nodi>0
  Subscribe_Tapparella();
#endif
#if Interruttore_nodi>0
  Subscribe_Interruttore();
#endif
#if Impulso_nodi>0
  Subscribe_Impulso();
#endif
#if Termostato_nodi>0
  Subscribe_Termostato();
#endif
#if Temporizzatore_nodi>0
  Subscribe_Temporizzatore();
#endif
#if Sensore_nodi>0
  Subscribe_Sensore();
#endif
#if Display_nodi>0
  Subscribe_Display();
#endif
}

void callback(char* topic, byte * message, unsigned int length) {
  String payload;
  String message_String;
  message_String = "";
  for (int ii = 0; ii < length; ii++) {
    message_String += (char)message[ii];
  }
  Debug_MSG("Message arrived [");
  Debug_MSG((String) topic);
  Debug_MSG("] ");
  Debug_MSG_LN(message_String);
#if Tapparella_nodi>0
  callback_Tapparella(topic, message, length);
#endif
#if Interruttore_nodi>0
  callback_Interruttore(topic, message, length);
#endif
#if Impulso_nodi>0
  callback_Impulso(topic, message, length);
#endif
#if Termostato_nodi>0
  callback_Termostato(topic, message, length);
#endif
#if Temporizzatore_nodi>0
  callback_Temporizzatore(topic, message, length);
#endif
#if Sensore_nodi>0
  callback_Sensore(topic, message, length);
#endif
#if Display_nodi>0
  callback_Display(topic, message, length);
#endif
  if (String(topic) == ACK_Topic) {                   // messaggio eseguito da tutti i nodi nella rete
    if (message_String == MQTT_ACK) {
#ifdef HomeAssistant_discovery
      HA_discovery_crea_nodi();
#endif
      Send_ACK();
      delay(100);
    }
    if (message_String == MQTT_INFO) {
      String payload;
      payload = "{\"MAC_address\":\"";
      payload += macToStr(mac);
      payload += "\",\"IP\":\"";
      payload += WiFi.localIP().toString();
      payload += "\",\"Time_alive\":\"";
      unsigned long nowMillis = millis();
      unsigned long seconds = (nowMillis - start_millis) / 1000;
      int days = seconds / 86400;
      seconds %= 86400;
      byte hours = seconds / 3600;
      seconds %= 3600;
      byte minutes = seconds / 60;
      seconds %= 60;
      payload += ((String) days + "d:" + (String) hours + "h:" + (String) minutes + "m:" + (String) seconds  + "s");
      payload += "\",\"FreeHeap\":\"";
      payload += (String) ESP.getFreeHeap();
      payload += "\",\"Hostname_OTA\":\"";
      String tmp = "SmartHome_V";
      tmp += Versione;
      tmp += "_";
      tmp += macToStr(mac);
      if ((String)Hostname_OTA != "") tmp = Hostname_OTA;
      payload += tmp;
      payload += "\",\"WILLTopic\":\"";
      payload += WillTopicStr;
      payload += "\",\"Vcc\":\"";
      payload += Vcc();
      payload += "\"}";
      client.publish(ACK_Topic, (char*) payload.c_str());
      delay(250);
    }
  }
  if (String(topic) != ACK_Topic) {                   // messaggio eseguito dal solo nodo intessato
    if (message_String == MQTT_RESET) {
      EEPROM_clear_reset();
    }
    if (message_String == MQTT_REBOOT) {
      Debug_MSG_LN("RIAVVIO L'ESP (da mqtt)");
      delay(500);
      ESP.restart();
    }
  }
}
