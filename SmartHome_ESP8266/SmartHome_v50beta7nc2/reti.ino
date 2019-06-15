AsyncMqttClient mqttClient;

Ticker mqttReconnectTimer, wifiReconnectTimer, rebootTimer, onlineTimer;

WiFiEventHandler wifiConnectHandler, wifiDisconnectHandler;

#include <time.h>                      // time() ctime()
#include <sys/time.h>                   // struct timeval
#include <coredecls.h>                  // settimeofday_cb()
#include <simpleDSTadjust.h>

#define TZ              1       // (utc+) TZ in hours
#define DST_MN          60      // use 60min for summer time in some countries

struct dstRule CEST = {"CEST", Last, Sun, Mar, 2, ((TZ * 60) + DST_MN) * 60}; //Central European Summer Time +120 min.
struct dstRule CET = {"CET", Last, Sun, Oct, 3, (TZ * 60) * 60};   //Central European Standard Time +60 min.
simpleDSTadjust dstAdjusted(CEST, CET);

boolean firsttime_MQTT = true;
bool cbtime_set = false;
timeval tv;
time_t now;

String MQTT_ACK     = "ack";
String MQTT_RESET   = "reset";
String MQTT_REBOOT  = "reboot";
String MQTT_INFO    = "info";
String MQTT_STATO   = "stato";
String Will_offline = "offline";
String Will_online  = "online";
String WillTopicStr = WillTopic;
IPAddress S_IP, S_SUBNET, S_GATEWAY, S_DNSIP;

void connectToWifi() {
  Debug_MSG_LN("Connecting to Wi-Fi");
  if ((!S_IP.fromString(IP) or !S_SUBNET.fromString(SUBNET) or !S_GATEWAY.fromString(GATEWAY) or !S_DNSIP.fromString(DNSIP)) and Static_IP ) {
    Static_IP = false;
    Debug_MSG_LN(" Static IP value error!");
  }
  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.setOutputPower(0);
  WiFi.setAutoReconnect(true);
  WiFi.mode(WIFI_STA);
  if (Static_IP) {
    Debug_MSG_LN(" Setup static IP");
    Debug_MSG_LN("  IP      : " + IP);
    Debug_MSG_LN("  GATEWAY : " + GATEWAY);
    Debug_MSG_LN("  SUBNET  : " + SUBNET);
    Debug_MSG_LN("  DNSIP   : " + DNSIP);
    WiFi.config(S_IP, S_GATEWAY, S_SUBNET, S_DNSIP);
  }
  if (!Static_IP) {
    Debug_MSG_LN(" Setup DHCP");
  }
  WiFi.begin(ssid, password);
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Debug_MSG_LN("WIFI  connected!");
  Debug_MSG_LN(" SSID       : " + (String) WiFi.SSID());
  Debug_MSG_LN(" RSSI       : " + (String) WiFi.RSSI());
  Debug_MSG_LN(" Local IP   : " + WiFi.localIP().toString());
  WiFi.macAddress(mac);
  Debug_MSG_LN(" MAC address: " + macToStr(mac));
  // Debug_MSG_LN("Synchronizing DATE/TIME: " + getTime());
#ifdef ArduinoIDE_OTA
  setup_OTA();
#endif
#ifdef WEB_SERVER
  setup_web();
#endif
#ifdef TELNET_DEBUG
  setup_telnet();
#endif
  String clientName = "SmartHomeClient-";
  clientName += macToStr(mac);
  //mqttClient.setClientId((char*)clientName.c_str());
  //Debug_MSG_LN("MQTT Client: " + clientName);
  if (WillTopicStr == "" ) WillTopicStr = macToStr(mac);
  mqttClient.setWill((char*) WillTopicStr.c_str(), 1, false, (char*) Will_offline.c_str());
  Debug_MSG_LN("MQTT WillTopic: " + WillTopicStr);
  connectToMqtt();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  Debug_MSG("Disconnected from WiFi: ");
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
  mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  wifiReconnectTimer.once(2, connectToWifi);
}

void connectToMqtt() {
  Debug_MSG_LN("Connecting to MQTT");
  mqttClient.connect();
}

void onMqttConnect(bool sessionPresent) {
  Debug_MSG_LN("MQTT connected!");
  rebootTimer.detach();
  onlineTimer.attach(60, Send_HA_Online);
  Send_Alive();
  Send_HA_Online();
  Send_ACK();
  SubscribeTopic();
  /* uint16_t packetIdSub = mqttClient.subscribe("test / lol", 2);
     Serial.print("Subscribing at QoS 2, packetId: ");
     Serial.println(packetIdSub);
     mqttClient.publish("test / lol", 0, true, "test 1");
     Serial.println("Publishing at QoS 0");
     uint16_t packetIdPub1 = mqttClient.publish("test / lol", 1, true, "test 2");
     Serial.print("Publishing at QoS 1, packetId: ");
     Serial.println(packetIdPub1);
     uint16_t packetIdPub2 = mqttClient.publish("test / lol", 2, true, "test 3");
     Serial.print("Publishing at QoS 2, packetId: ");
     Serial.println(packetIdPub2);
  */
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Debug_MSG("Disconnected from MQTT: ");
  switch ((int)reason) {
    case 0:
      Debug_MSG_LN("TCP_DISCONNECTED");
      break;
    case 1:
      Debug_MSG_LN("MQTT_UNACCEPTABLE_PROTOCOL_VERSION");
      break;
    case 2:
      Debug_MSG_LN("MQTT_IDENTIFIER_REJECTED");
      break;
    case 3:
      Debug_MSG_LN("MQTT_SERVER_UNAVAILABLE");
      break;
    case 4:
      Debug_MSG_LN("MQTT_MALFORMED_CREDENTIALS");
      break;
    case 5:
      Debug_MSG_LN("MQTT_NOT_AUTHORIZED");
      break;
    case 6:
      Debug_MSG_LN("ESP8266_NOT_ENOUGH_SPACE");
      break;
    case 7:
      Debug_MSG_LN("TLS_BAD_FINGERPRINT");
      break;
    default:
      Debug_MSG_LN("MQTT_UNKNOWN_ERROR");
      break;
  }
  onlineTimer.detach();
  if (!rebootTimer.active() && (Tempo_Reboot > 0)) rebootTimer.once(Tempo_Reboot, cmd_reboot);
  if (WiFi.isConnected()) {
    mqttReconnectTimer.once(2, connectToMqtt);
  }
  else {
    wifiReconnectTimer.once(2, connectToWifi);      //verificare se funziona
  }
}

void onMqttMessage(char* topic, char* message, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  char new_message[len + 1];
  new_message[len] = '\0';
  strncpy(new_message, message, len);
  message = new_message;
  /*  Debug_MSG_LN("* Publish received.");
    Debug_MSG_LN("*  topic: " + (String)topic);
    Debug_MSG_LN("*  message: " + (String)message);
    Debug_MSG_LN("*  qos: " + (String)properties.qos);
    Debug_MSG_LN("*  dup: " + (String)properties.dup);
    Debug_MSG_LN("*  retain: " + (String)properties.retain);
    Debug_MSG_LN("*  len: " + (String)len);
    Debug_MSG_LN("*  index: " + (String)index);
    Debug_MSG_LN("*  total: " + (String)total);
  */
  if (String(topic) != ACK_Topic) {
    Debug_MSG_LN("MQTT message arrived [" + (String)topic + "] " + (String)message);
    Debug_MSG_LN(" index:" + (String)index + " len:" + (String)len + " total:" + (String)total);
  }
#if Tapparella_nodi>0
  callback_Tapparella(topic, message);
#endif
#if Interruttore_nodi>0
  callback_Interruttore(topic, message);
#endif
#if Impulso_nodi>0
  callback_Impulso(topic, message);
#endif
#if Termostato_nodi>0
  callback_Termostato(topic, message);
#endif
#if Temporizzatore_nodi>0
  callback_Temporizzatore(topic, message);
#endif
#if Sensore_nodi>0
  callback_Sensore(topic, message);
#endif
#if Display_nodi>0
  callback_Display(topic, message);
#endif
#if Power_nodi>0
  callback_Power(topic, message);
#endif
  if (String(topic) == ACK_Topic) {                   // messaggio eseguito da tutti i nodi nella rete
    if ((String)message == MQTT_ACK) {                // ack
#ifdef HomeAssistant_discovery
      HA_discovery_crea_nodi();
#endif
      Send_ACK();
    }
    if ((String)message == MQTT_STATO) {               // stato
      Send_Stato_ALL();
    }

    if ((String)message == MQTT_INFO) {               // info
      String payload;
      payload = " {\"MAC_address\":\"";
      payload += macToStr(mac);
      payload += "\",\"Versione\":\"";
      payload += "SmartHome_V";
      payload += Versione;
      payload += "\",\"IP\":\"";
      payload += WiFi.localIP().toString();
      payload += "\",\"Hostname_OTA\":\"";
      String tmp = "SmartHome_V";
      tmp += Versione;
      tmp += "_";
      tmp += macToStr(mac);
      if ((String)Hostname_OTA != "") tmp = Hostname_OTA;
      payload += tmp;
      payload += "\",\"WILLTopic\":\"";
      payload += WillTopicStr;
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
      payload += "\",\"Vcc\":\"";
      payload += Vcc();
      payload += "\"}";
      MQTTPublish(ACK_Topic, payload);
    }
  }
  if (String(topic) != ACK_Topic) {                   // messaggio eseguito dal solo nodo intessato
    if ((String)message == MQTT_RESET) {
      EEPROM_clear_reset();
    }
    if ((String)message == MQTT_REBOOT) {
      Debug_MSG_LN("RIAVVIO L'ESP (da mqtt)");
      cmd_reboot();
    }
  }
}

void cmd_reboot() {
  Debug_MSG_LN("REBOOTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT!");
  unblockingDelay(500);
  ESP.restart();
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  /*Debug_MSG_LN("* Subscribe acknowledged.");
    Debug_MSG_LN("*  packetId: " + (String)packetId);
    Debug_MSG_LN("*  qos: " + (String)qos);*/
}

void onMqttUnsubscribe(uint16_t packetId) {
  /*Debug_MSG_LN("* Unsubscribe acknowledged.");
    Debug_MSG_LN("*  packetId: " + (String)packetId);*/
}
void onMqttPublish(uint16_t packetId) {
  /*Debug_MSG_LN("* Publish acknowledged.");
    Debug_MSG_LN("*  packetId: " + (String)packetId);*/
}

void MQTTPublish(String topic, String payload) {
  mqttClient.publish((char*) topic.c_str(), 1, false, (char*) payload.c_str());
  // uint16_t packetIdPub = mqttClient.publish("test / lol", 2, false, "test 3");
}
void MQTTSubscribe(String topic) {
  mqttClient.subscribe((char*) topic.c_str(), 1);
}

void setup_reti() {
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setCleanSession(true);
  mqttClient.setMaxTopicLength(1024);
  mqttClient.setKeepAlive(5);
  mqttClient.setCredentials(mqtt_user, mqtt_password);
  mqttClient.setServer(mqtt_server, mqtt_port);
  settimeofday_cb(time_is_set);
  configTime(0, 0, "pool.ntp.org", "time.nist.gov", "us.pool.ntp.org");
  if (Tempo_Reboot > 0) rebootTimer.once(Tempo_Reboot, cmd_reboot);
  connectToWifi();
}


String get_time() {
  now = dstAdjusted.time(nullptr);
  char buffer[80];
  strftime (buffer, 80, "%X", localtime(&now));
  if (cbtime_set == true) {
    return ((String)buffer);
  }
  else {
    return ("Time not sync.");
  }
}
String get_date() {
  now = dstAdjusted.time(nullptr);
  char buffer[80];
  strftime (buffer, 80, "%d/%m/%Y", localtime(&now));
  if (cbtime_set == true) {
    return ((String)buffer);
  }
  else {
    return ("Date not sync.");
  }
}

void time_is_set(void) {
  gettimeofday(&tv, nullptr);
  cbtime_set = true;
  Debug_MSG_LN("Sincronizzazione data/ora:");
  Debug_MSG_LN(" " + get_date() + " " + get_time());
  /*
    %a Abbreviated weekday name
    %A Full weekday name
    %b Abbreviated month name
    %B Full month name
    %c Date and time representation for your locale
    %d Day of month as a decimal number (01-31)
    %H Hour in 24-hour format (00-23)
    %I Hour in 12-hour format (01-12)
    %j Day of year as decimal number (001-366)
    %m Month as decimal number (01-12)
    %M Minute as decimal number (00-59)
    %p Current locale's A.M./P.M. indicator for 12-hour clock
    %S Second as decimal number (00-59)
    %U Week of year as decimal number,  Sunday as first day of week (00-51)
    %w Weekday as decimal number (0-6; Sunday is 0)
    %W Week of year as decimal number, Monday as first day of week (00-51)
    %x Date representation for current locale
    %X Time representation for current locale
    %y Year without century, as decimal number (00-99)
    %Y Year with century, as decimal number
    %z %Z Time-zone name or abbreviation, (no characters if time zone is unknown)
    %% Percent sign
    You can include text literals (such as spaces and colons) to make a neater display or for padding between adjoining columns.
    You can suppress the display of leading zeroes  by using the "#" character  (%#d, %#H, %#I, %#j, %#m, %#M, %#S, %#U, %#w, %#W, %#y, %#Y)
  */
}

void Send_Alive() {
  String payload = macToStr(mac);
  payload += " ";
  payload += WiFi.localIP().toString();
  payload += " v";
  payload += Versione;
  payload += " ALIVE!";
  start_millis = millis();
  MQTTPublish(ACK_Topic, payload);
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
  Send_HA_Online;
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
#if Power_nodi>0
  ACK_Power();
#endif
}


void Send_Stato_ALL() {
#if Tapparella_nodi>0
  for (int i = 0; i < Tapparella_nodi; i++) Stato_TAP (i);
#endif
#if Termostato_nodi>0
  for (int i = 0; i < Termostato_nodi; i++) Stato_TER (i);
#endif
#if Interruttore_nodi>0
  for (int i = 0; i < Interruttore_nodi; i++) Stato_INT (i);
#endif
#if Impulso_nodi>0
  for (int i = 0; i < Impulso_nodi; i++) Stato_IMP (i);
#endif
#if Temporizzatore_nodi>0
  for (int i = 0; i < Temporizzatore_nodi; i++) Stato_TEM (i);
#endif
#if Sensore_nodi>0
  for (int i = 0; i < Sensore_nodi; i++) Stato_SEN (i);
#endif
#if Display_nodi>0
  for (int i = 0; i < Display_nodi; i++) Stato_DIS (i);
#endif
#if Power_nodi>0
  for (int i = 0; i < Power_nodi; i++) Stato_POW (i);
#endif
}

void Send_HA_Online() {
  MQTTPublish(WillTopicStr, Will_online);
  Debug_MSG_LN(" " + WillTopicStr + " -> " + Will_online);
}

void SubscribeTopic() {
  MQTTSubscribe(ACK_Topic);
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
#if Power_nodi>0
  Subscribe_Power();
#endif
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
#if Power_nodi>0
  HA_discovery_Power();
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
    MQTTPublish(HAtopic, HApayload);
    Debug_MSG(".");
  }
  for (int i = 0; i < 9; i++) {
    HAtopic = "homeassistant/switch/";
    HAtopic += macToStr(mac);
    HAtopic += "/";
    HAtopic += "INT";
    HAtopic += i;
    HAtopic += "/config";
    MQTTPublish(HAtopic, HApayload);
    Debug_MSG(".");
    HAtopic = "homeassistant/switch/";
    HAtopic += macToStr(mac);
    HAtopic += "/";
    HAtopic += "IMP";
    HAtopic += i;
    HAtopic += "/config";
    MQTTPublish(HAtopic, HApayload);
    Debug_MSG(".");
    HAtopic = "homeassistant/switch/";
    HAtopic += macToStr(mac);
    HAtopic += "/";
    HAtopic += "TEM";
    HAtopic += i;
    HAtopic += "/config";
    MQTTPublish(HAtopic, HApayload);
    Debug_MSG(".");
  }
  Debug_MSG_LN(" OK!");
}
