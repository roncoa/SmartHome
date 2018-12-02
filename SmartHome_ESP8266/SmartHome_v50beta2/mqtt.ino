#include <ESP8266WiFi.h>
#include <PubSubClient.h>

String MQTT_ACK     = "ack";
String MQTT_RESET   = "reset";
String MQTT_REBOOT  = "reboot";
String MQTT_IP      = "ip";
String MQTT_MAC     = "mac";
String Will_offline = "offline";
String Will_online  = "online";
String WillTopicStr = WillTopic;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_MQTT() {
  //WiFi.macAddress(mac);
  if (macToStr(mac) == "000000000000") {
    Debug_MSG("Attempting MQTT connection... ");
    Debug_MSG_LN("error: mac=null");
    return;
  }

  int i = 0;
  String clientName = "SmartHomeClient-";
  clientName += macToStr(mac);
  //clientName += "-";
  //clientName += millis();
  Debug_MSG("MQTT Client : ");
  Debug_MSG_LN(clientName);
  if (WillTopicStr == "" ) WillTopicStr = macToStr(mac);
  while (!client.connected() and i < MAX_RET_MQTT) {
    ++i;
    Debug_MSG("Attempting MQTT connection... ");
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
    if (client.connect(string2char(clientName), mqtt_user, mqtt_password,   string2char(WillTopicStr), 1, false,  string2char(Will_offline))) {
      Debug_MSG_LN("MQTT connected!");
      delay(100);
      Send_Alive();
      Send_ACK();
      Send_Online();
      SubscribeTopic();
    }
    else {
      Debug_MSG("MQTT failed, rc = ");
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
  }
}

void loop_MQTT() {
  client.loop();
}

void Send_Alive() {
  String payload = macToStr(mac);
  payload += " ";
  payload += WiFi.localIP().toString();
  payload += " v";
  payload += Versione;
  payload += " start at ";
  payload += getTime();
  client.publish(ACK_Topic, (char*) payload.c_str());     // Pubblica su ACK_Topic -> MAC + IP + version + " start at " + time
  delay(100);
  Debug_MSG_LN(payload);
  Alive_Tapparella();
  Alive_Termostato();
  Alive_Interruttore();
  Alive_Impulso();
  Alive_Temporizzatore();
  Alive_Sensore();
  //Alive_NuovoNodo();
}

void Send_ACK() {
  ACK_Tapparella();
  ACK_Termostato();
  ACK_Interruttore();
  ACK_Impulso();
  ACK_Temporizzatore();
  ACK_Sensore();
  //ACK_NuovoNodo();
}

void Send_Online() {
  client.publish((char*) macToStr(mac).c_str(), (char*) Will_online.c_str());
  delay(100);
  Debug_MSG(macToStr(mac));
  Debug_MSG(" ");
  Debug_MSG_LN(Will_online);
}

void SubscribeTopic() {
  client.subscribe(ACK_Topic);                            // Sottoscrivi ACK_Topic
  delay(250);
  Debug_MSG_LN("MQTT subscribe " + String(ACK_Topic));
  Subscribe_Tapparella();
  Subscribe_Interruttore();
  Subscribe_Impulso();
  Subscribe_Termostato();
  Subscribe_Temporizzatore();
  Subscribe_Sensore();
  //Subscribe_NuovoNodo();
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

  callback_Tapparella(topic, message, length);
  callback_Interruttore(topic, message, length);
  callback_Impulso(topic, message, length);
  callback_Termostato(topic, message, length);
  callback_Temporizzatore(topic, message, length);
  callback_Sensore(topic, message, length);
  //callback_NuovoNodo(topic, message, length);

  if (String(topic) == ACK_Topic) {                                                       // se arriva il comando sul topic "ACK"
    if (message_String == MQTT_ACK) {                                                     // Topic "ack" = "ack"
      Send_ACK();
      delay(100);
    }
  }
  if (String(topic) != ACK_Topic) {
    if (message_String == MQTT_RESET) {                                                   // messaggio = "reset"
      EEPROM_clear_reset();
    }
    if (message_String == MQTT_REBOOT) {                                                  // messaggio = "reboot"
      delay(500);
      ESP.restart();
    }
    if ((message_String == MQTT_IP) or (message_String == MQTT_MAC)) {                                                      // messaggio = "ip" o "mac"
      String payload;
      payload = "{\"MAC_address\":\"";
      payload += macToStr(mac);
      payload += "\",\"IP\":\"";
      payload += WiFi.localIP().toString();
      payload += "\"}";
      client.publish(ACK_Topic, (char*) payload.c_str());
      delay(250);
    }
  }
}
