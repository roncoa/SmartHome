Ticker rebootTimer, onlineTimer;

WiFiEventHandler wifiConnectHandler, wifiDisconnectHandler;

#include <time.h>                      // time() ctime()
#include <sys/time.h>                   // struct timeval
#include <coredecls.h>                  // settimeofday_cb()
#include <simpleDSTadjust.h>
#include "AsyncPing.h"

#define TZ              1       // (utc+) TZ in hours
#define DST_MN          60      // use 60min for summer time in some countries

struct dstRule CEST = {"CEST", Last, Sun, Mar, 2, ((TZ * 60) + DST_MN) * 60}; //Central European Summer Time +120 min.
struct dstRule CET = {"CET", Last, Sun, Oct, 3, (TZ * 60) * 60};   //Central European Standard Time +60 min.
simpleDSTadjust dstAdjusted(CEST, CET);

WiFiClient espClient;
PubSubClient MQTTclient(espClient);
AsyncPing pingMQTT;

long lastReconnectAttempt = 0;
boolean firsttime_MQTT = true;
bool cbtime_set = false;
bool ping_MQTT_OK = false;
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

void setup_reti() {
  Debug_MSG_LN("Setup to Wi-Fi");
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
  pingMQTT.on(true, [](const AsyncPingResponse & response) {
    if (response.answer) ping_MQTT_OK = true;
    else ping_MQTT_OK = false;
    return true; //do not stop
  });
  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.setOutputPower(0);
  WiFi.setAutoReconnect(true);
  WiFi.mode(WIFI_STA);
  MQTTclient.setServer(mqtt_server, mqtt_port);
  MQTTclient.setCallback(callback);
  if ((!S_IP.fromString(IP) or !S_SUBNET.fromString(SUBNET) or !S_GATEWAY.fromString(GATEWAY) or !S_DNSIP.fromString(DNSIP)) and Static_IP ) {
    Static_IP = false;
    Debug_MSG_LN(" Static IP value error! use DHCP");
  }
  if (Static_IP) {
    Debug_MSG_LN(" Static IP");
    Debug_MSG_LN("  IP      : " + IP);
    Debug_MSG_LN("  GATEWAY : " + GATEWAY);
    Debug_MSG_LN("  SUBNET  : " + SUBNET);
    Debug_MSG_LN("  DNSIP   : " + DNSIP);
    WiFi.config(S_IP, S_GATEWAY, S_SUBNET, S_DNSIP);
  }
  else Debug_MSG_LN(" DHCP");
  settimeofday_cb(time_is_set);
  configTime(0, 0, "pool.ntp.org", "time.nist.gov", "us.pool.ntp.org");
  WiFi.begin(ssid, password);
  if (Tempo_Reboot > 0) rebootTimer.once(Tempo_Reboot, cmd_reboot);
  lastReconnectAttempt = 0;
}

void loop_reti() {
  if (!MQTTclient.connected() && WiFi.status() == WL_CONNECTED) {
    onlineTimer.detach();
    if (!rebootTimer.active() && (Tempo_Reboot > 0)) rebootTimer.once(Tempo_Reboot, cmd_reboot);
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      pingMQTT.begin(mqtt_server);
      lastReconnectAttempt = now;
      Debug_MSG("Disconnected from MQTT: ");
      if (!ping_MQTT_OK) {
        Debug_MSG_LN("PING ERROR");
        Debug_MSG_LN("Connecting to MQTT");
        return;
      }
      switch (MQTTclient.state()) {
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
      // Attempt to reconnect
      if (reconnect()) {
        lastReconnectAttempt = 0;
      }
    }
  }
  MQTTclient.loop();
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Debug_MSG_LN("WIFI  connected!");
  Debug_MSG_LN(" SSID       : " + (String) WiFi.SSID());
  Debug_MSG_LN(" RSSI       : " + (String) WiFi.RSSI());
  Debug_MSG_LN(" Local IP   : " + WiFi.localIP().toString());
  WiFi.macAddress(mac);
  Debug_MSG_LN(" MAC address: " + macToStr(mac));
  if (WillTopicStr == "" ) WillTopicStr = macToStr(mac);

#ifdef ArduinoIDE_OTA
  String tmp = "SmartHome_V";
  tmp += Versione;
  tmp += "_";
  tmp += macToStr(mac);
  if (Hostname_OTAStr == "") Hostname_OTAStr = tmp;
#endif

  pingMQTT.begin(mqtt_server);
#ifdef WEB_SERVER
  setup_web();
#endif
#ifdef TELNET_DEBUG
  setup_telnet();
#endif
#ifdef ArduinoIDE_OTA
  setup_OTA();
#endif
  reconnect();
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
}

boolean reconnect() {
  Debug_MSG_LN("Connecting to MQTT");
  if (macToStr(mac) == "000000000000") {
    Debug_MSG_LN("macaddress error");
    return (false);
  }
  String clientName = "SmartHomeClient-";
  clientName += macToStr(mac);
  MQTTclient.disconnect();
  if (MQTTclient.connect(string2char(clientName), mqtt_user, mqtt_password,   string2char(WillTopicStr), 1, false,  string2char(Will_offline))) {
    Debug_MSG_LN("MQTT connected!");
    Debug_MSG_LN(" Client: " + clientName);
    rebootTimer.detach();
    onlineTimer.attach(60, Send_HA_Online);
    Send_Alive();
    SubscribeTopic();
    Send_ACK();
  }
  return MQTTclient.connected();
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message_String = "";
  char message[length];
  for (int ii = 0; ii < length; ii++) {
    message[ii] = (char)payload[ii];
    message_String += message[ii];
  }
  message[length] = '\0';
  if (String(topic) != ACK_Topic) {                   // messaggio eseguito dal solo nodo intessato
    Debug_MSG_LN("MQTT message arrived [" + (String)topic + "] " + (String)message_String);
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
#if IR_nodi>0
    callback_IR(topic, message);
#endif
    if ((String)message == MQTT_RESET) {              // reset
      EEPROM_clear_reset();
    }
    if ((String)message == MQTT_REBOOT) {             // reboot
      Debug_MSG_LN("RIAVVIO L'ESP (da mqtt)");
      cmd_reboot();
    }
  }
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
      DynamicJsonDocument JSONdoc(1024);
      JSONdoc["MAC_address"] = macToStr(mac);
      JSONdoc["Versione"] = "SmartHome_V" + Versione;
      JSONdoc["IP"] = WiFi.localIP().toString();
      JSONdoc["Hostname_OTA"] = Hostname_OTAStr;
      JSONdoc["WILLTopic"] = WillTopicStr;
      unsigned long nowMillis = millis();
      unsigned long seconds = (nowMillis - start_millis) / 1000;
      int days = seconds / 86400;
      seconds %= 86400;
      byte hours = seconds / 3600;
      seconds %= 3600;
      byte minutes = seconds / 60;
      seconds %= 60;
      JSONdoc["Time_alive"] = ((String) days + "d:" + (String) hours + "h:" + (String) minutes + "m:" + (String) seconds  + "s");
      JSONdoc["FreeHeap"] = ESP.getFreeHeap();
      JSONdoc["Vcc"] = Vcc();
      String payload = "";
      serializeJson(JSONdoc, payload);
      MQTTPublish(ACK_Topic, payload);
    }
  }
}

void cmd_reboot() {
  Debug_MSG_LN("REBOOTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT!");
  unblockingDelay(500);
  ESP.restart();
}

void MQTTPublish(String topic, String payload) {
  MQTTclient.publish((char*) topic.c_str(), (char*) payload.c_str());
}

void MQTTSubscribe(String topic) {
  MQTTclient.subscribe((char*) topic.c_str(), 1);
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
  Debug_MSG_LN("Sincronizzazione data/ora: " + get_date() + " " + get_time());
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
  DynamicJsonDocument JSONdoc(1024);
  JSONdoc["ALIVE"] = "ALIVE";
  JSONdoc["MacAddress"] = macToStr(mac);
  JSONdoc["WillTopic"] = WillTopicStr;
#ifdef ArduinoIDE_OTA
  JSONdoc["Hostname_OTA"] = Hostname_OTAStr;
#else
  JSONdoc["Hostname_OTA"] = "";
#endif
  JSONdoc["IP"] = WiFi.localIP().toString();
  JSONdoc["Versione"] = Versione;
  JSONdoc["TAP"] = Tapparella_nodi;
  JSONdoc["INT"] = Interruttore_nodi;
  JSONdoc["IMP"] = Impulso_nodi;
  JSONdoc["TER"] = Termostato_nodi;
  JSONdoc["TEM"] = Temporizzatore_nodi;
  JSONdoc["SEN"] = Sensore_nodi;
  JSONdoc["DIS"] = Display_nodi;
  JSONdoc["POW"] = Power_nodi;
  JSONdoc["IR_"] = IR_nodi;
  String payload = "";
  serializeJson(JSONdoc, payload);
  MQTTPublish(ACK_Topic, payload);

  payload = macToStr(mac);
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
#if IR_nodi>0
  ACK_IR();
#endif
  Send_HA_Online();
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
#if IR_nodi>0
  for (int i = 0; i < IR_nodi; i++) Stato_IR (i);
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
#if IR_nodi>0
  Subscribe_IR();
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
#if IR_nodi>0
  HA_discovery_IR();
#endif
}

void HA_discovery_cancella_nodi() {
  String HAtopic;
  String HApayload;
  HApayload = "";
  Debug_MSG("Rimuovo nodi Home assistant discovery ");
  for (int i = 0; i < 5; i++) {
    HAtopic = "homeassistant/cover/";
    HAtopic += macToStr(mac);
    HAtopic += "/";
    HAtopic += "TAP";
    HAtopic += i;
    HAtopic += "/config";
    MQTTPublish(HAtopic, HApayload);
    Debug_MSG(".");
  }
  for (int i = 0; i < 10; i++) {
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
