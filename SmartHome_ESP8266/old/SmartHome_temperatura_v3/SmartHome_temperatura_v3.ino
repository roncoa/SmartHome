/*
  SmartHome temperatura V 3.0

  Comandi da inviare al topic "Temperatura_Topic":
  man           -> Imposta il termostato in "manuale"
  auto          -> Imposta il termostato in "automatico"
  t=XX o T=XX   -> Imposta il termostato alla temperatura XX
  1on           -> comando ON 1
  1off          -> comando OFF 1
  2on           -> comando ON 2
  2off          -> comando OFF 2
  stato         -> restituisce sul topic ACK lo stato dei relè e per quanto tempo la tapparella può restare in azione (in sec.)
  stato2        -> restituisce sul topic ACK2 la temperatura e lo stato dei relè
  read          -> legge la temperatura
  reset         -> pulisce la memoria EEPROM e resetta l'ESC
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <TPush.h>
#include <EEPROM.h>
#include "DHT.h"
#include "SSD1306.h"

// Arduino OTA
//#include <ESP8266WiFi.h>
//#include <ESP8266mDNS.h>
//#include <WiFiUdp.h>
#include <ArduinoOTA.h>

// MQTT server
const char* ssid = "WIFI_SSID" ;                  // WIFI SSID
const char* password = "WIFI_PASSWORD";           // WIFI password
const char* mqtt_server = "MQTT_SERVER";          // MQTT server
const char* mqtt_user = "MQTT_USER";              // MQTT user
const char* mqtt_password = "MQTT_PASSWORD";      // MQTT password
const int   mqtt_port = MQTT_PORT;                // MQTT port
//const char* mqtt_server = "192.168.1.200";      // MQTT server raspberry
//const int   mqtt_port = 1883;                   // MQTT port raspberry

// DEBUG
#define DEBUG                                     // Commentare questa riga per disabilitare il SERIAL DEBUG

// HARDWARE
//#define ESP01                                    // Commentare l'hardware non corrente
#define NODEMCU                                   // Commentare l'hardware non corrente
//#define ELECTRODRAGON                            // Commentare l'hardware non corrente

// Tipo nodo
#define TIPO_NODO                   "TEM"         // "TAP"->tapparella "TEM"->temperatura "INT"->interruttore

// Sensore temperatura
#define DHTTYPE                     DHT22         // DHT 22  (AM2302)

// MQTT topic
#define ACK_Topic                   "ack"
#define ACK2_Topic                  "ack2"
#define Temperatura_Topic           "temperatura/xxxxx"

// TEMPI
#define MAX_RET_WIFI                20            // Indica per quante volte ritenta di connettersi al WIFI
#define MAX_RET_MQTT                2             // Indica per quante volte ritenta di connettersi al server MQTT
#define TEMPO_REFRESH_CONNESSIONE   60000         // Indica il timeout di refresh della connessione (60000=1 min.)
#define TEMPO_RELE                  200           // Indica il tempo tra una commutazione RELE e la successiva
#define TEMPO_TERMOSTATO            5000          // Indica il tempo di refresh del termostato in AUTO

// GPIO
#if defined(ESP01)  //  GPIO spenti -> 4 5 12 13 14 15
#define Flag_inversione_RELE        1             // Inversione del segnale di uscita RELE       (0=normale - 1=invertito)  
#define Flag_inversione_Status_LED  1             // Inversione del segnale di uscita Status_LED (0=normale - 1=invertito)
#define Status_LED                  4             // spento
#define RELE1_temperatura           0             // GPIO0
#define RELE2_temperatura           5             // spento
#define DHTPIN                      2             // GPIO2
#define I2C_DISPLAY_ADDRESS         0x3c          // Display I2C address
#define SDA_PIN                     1             // GPIO1 Display SDA
#define SDC_PIN                     3             // GPIO3 Display SDC
#endif
#if defined(NODEMCU)             // OK
#define Flag_inversione_RELE        1             // Inversione del segnale di uscita RELE       (0=normale - 1=invertito)  
#define Flag_inversione_Status_LED  0             // Inversione del segnale di uscita Status_LED (0=normale - 1=invertito)
#define Status_LED                  BUILTIN_LED   //BUILTIN_LED -> GPIO16 (nodemcu) , GPIO1 (ESP01 TX)
#define RELE1_temperatura           14            // GPIO14 (D5)
#define RELE2_temperatura           12            // GPIO12 (D6)
#define DHTPIN                      2             // GPIO2 (D4)
#define I2C_DISPLAY_ADDRESS         0x3c          // Display I2C address
#define SDA_PIN                     4             // GPIO4 (D2) Display SDA
#define SDC_PIN                     5             // GPIO5 (D1) Display SDC
#endif
#if defined(ELECTRODRAGON)       // OK
#define Flag_inversione_RELE        0             // Inversione del segnale di uscita RELE       (0=normale - 1=invertito)  
#define Flag_inversione_Status_LED  1             // Inversione del segnale di uscita Status_LED (0=normale - 1=invertito)
#define Status_LED                  BUILTIN_LED   //BUILTIN_LED -> GPIO16 (nodemcu) , GPIO1 (ESP01 TX)
#define RELE1_temperatura           13            // GPIO13
#define RELE2_temperatura           12            // GPIO12
#define DHTPIN                      14            // GPIO14
#define I2C_DISPLAY_ADDRESS         0x3c          // Display I2C address
#define SDA_PIN                     4             // GPIO4 Display SDA
#define SDC_PIN                     5             // GPIO5 Display SDC
#endif

// VARIABILI
uint8_t     mac[6];
int         lamp = 0;
float       t;                              // Temperature Celsius
float       h;                              // Humidity
float       f;                              // Temperature Fahrenheit
float       hi;                             // Heat index in Celsius (isFahreheit = false)
float       soglia = 0.25;                  // +/- 0.25
float       termostato = 18.00;
boolean     AUTO = false;

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE, 15);
TPush TIMER1, TIMER2, TIMER, T_LAMPEGGIO;
SSD1306 display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN);

void setup() {
#if defined(DEBUG)
  Serial.begin(115200);
#endif
  Serial.println();
  Serial.println(" ***** SmartHome temperatura v3 *****");
  Serial.println("Status_LED      = GPIO" + String(Status_LED));
  Serial.println("RELE1           = GPIO" + String(RELE1_temperatura));
  Serial.println("RELE2           = GPIO" + String(RELE2_temperatura));
  Serial.println("DTH22           = GPIO" + String(DHTPIN));
  Serial.println("SSD1306 ADDRESS = 0x"   + String(I2C_DISPLAY_ADDRESS, HEX));
  Serial.println("SDA             = GPIO" + String(SDA_PIN));
  Serial.println("SDC             = GPIO" + String(SDC_PIN));

  // Inizializza EEPROM
  EEPROM.begin(512);
  delay(10);

  // Initialize the Status_LED
  pinMode(Status_LED, OUTPUT);
  digitalWrite(Status_LED, 1 ^ Flag_inversione_Status_LED);

  // Initialize GPIO
  pinMode(RELE1_temperatura, OUTPUT);
  digitalWrite(RELE1_temperatura, 0 ^ Flag_inversione_RELE);
  pinMode(RELE2_temperatura, OUTPUT);
  digitalWrite(RELE2_temperatura, 0 ^ Flag_inversione_RELE);

  // Initialize the display
  display.init();
  display.flipScreenVertically();
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, "Smart home");
  display.drawString(0, 25, Temperatura_Topic);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 45, "Firmware v3");
  display.display();
  delay(2500);

  // Inizializza connessioni
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  setup_wifi();
  beginOTA(Temperatura_Topic);
  reconnect();

  dht.begin();

  EEPROM_read();

}

void setup_wifi() {
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, "SETUP WIFI");
  display.drawString(0, 25, "Connecting to:");
  display.drawString(0, 45, ssid);
  display.display();

  int i = 0;
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  Serial.print(" ");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while ((WiFi.status() != WL_CONNECTED) and i < MAX_RET_WIFI) {
    ++i;
    Serial.print(".");
    digitalWrite(Status_LED, 0 ^ Flag_inversione_Status_LED);
    delay(250);
    digitalWrite(Status_LED, 1 ^ Flag_inversione_Status_LED);
    delay(250);
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("  NOT connected!");
  }
  else {
    Serial.println("  connected!");
  }
  Serial.println();
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  //Serial.print("SoftAP IP: ");
  //Serial.println(WiFi.softAPIP());
  Serial.print("MAC address : ");
  WiFi.macAddress(mac);
  Serial.println(macToStr(mac));
  digitalWrite(Status_LED, 1 ^ Flag_inversione_Status_LED);
}


void reconnect() {
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, "SETUP MQTT");
  display.drawString(0, 25, "ESP8266Client-");
  display.drawString(0, 45, macToStr(mac));
  display.display();

  int i = 0;
  String clientName = "ESP8266Client-";
  clientName += macToStr(mac);
  clientName += "-";
  clientName += millis();
  while (!client.connected() and i < MAX_RET_MQTT) {
    ++i;
    Serial.print("MQTT Client: ");
    Serial.println(clientName);
    Serial.print("Attempting MQTT connection... ");
    if (client.connect(string2char(clientName), mqtt_user, mqtt_password)) {
      Serial.println("connected");
      String payload = macToStr(mac);
      payload += " start at ";
      payload += getTime();
      client.publish(ACK_Topic, (char*) payload.c_str());  // Once connected, publish an announcement...
      delay(50);
      Send_ACK();
      client.subscribe(ACK_Topic); // ... and resubscribe
      client.subscribe(Temperatura_Topic); // ... and resubscribe
    }
    else {
      Serial.print("failed, rc = ");
      Serial.println(client.state());
    }
  }
}

// starts OTA server
void beginOTA(const char* hostname) {
  ArduinoOTA.setHostname(hostname);
  //ArduinoOTA.setPassword("admin");
  ArduinoOTA.onStart([]() {
    Serial.println("Start OTA ");
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 25, "UPGRADING");
    display.drawString(0, 45, "FIRMWARE");
    display.display();
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd OTA");
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 25, "DONE!");
    display.display();
    delay(1000);
    display.clear();
    Serial.println("Restart ESP");
    ESP.restart();
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    char buf[10];
    sprintf(buf, "Progress: %u%%\r", (progress / (total / 100)));
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 0, buf);
    display.drawString(0, 25, "UPGRADING");
    display.drawString(0, 45, "FIRMWARE");
    display.display();
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("OTA is up");
}

void Send_ACK() {
  String payload = macToStr(mac);
  payload += " alive! ";
  payload += TIPO_NODO;
  payload += " ";
  payload += Temperatura_Topic;
  client.publish(ACK_Topic, (char*) payload.c_str());     // Pubblica su ACK_Topic -> MAC + " alive! " TIPO_NODO + " " + Temperatura_Topic
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
  }
  Serial.println();
  //digitalWrite(Status_LED, 0 ^ Flag_inversione_Status_LED);
  String payload;
  if (String(topic) == Temperatura_Topic) {                                                                             // se arriva il comando sul topic "temperatura"
    if ((char)message[0] == 'r' & (char)message[1] == 'e' & (char)message[2] == 'a' & (char)message[3] == 'd' ) {       // Topic "temperatura" = "read"
      delay(50);
      sendTemperature();                                                                                                // sendTemperature()
      delay(50);
    }
    if ((char)message[0] == '1' & (char)message[1] == 'o' & (char)message[2] == 'n' ) {                                 // Topic "temperatura" = "1on"
      digitalWrite(RELE1_temperatura, 1 ^ Flag_inversione_RELE);                                                        // Accendo il RELE1
      delay(TEMPO_RELE);                                                                                                // Aspetto
    }
    if ((char)message[0] == '1' & (char)message[1] == 'o' & (char)message[2] == 'f' & (char)message[3] == 'f' ) {       // Topic "temperatura" = "1off"
      digitalWrite(RELE1_temperatura, 0 ^ Flag_inversione_RELE);                                                        // Spengo il RELE1
      delay(TEMPO_RELE);                                                                                                // Aspetto
    }
    if ((char)message[0] == '2' & (char)message[1] == 'o' & (char)message[2] == 'n' ) {                                 // Topic "temperatura" = "2on"
      digitalWrite(RELE2_temperatura, 1 ^ Flag_inversione_RELE);                                                        // Accendo il RELE2
      delay(TEMPO_RELE);                                                                                                // Aspetto
    }
    if ((char)message[0] == '2' & (char)message[1] == 'o' & (char)message[2] == 'f' & (char)message[3] == 'f' ) {       // Topic "temperatura" = "2off"
      digitalWrite(RELE2_temperatura, 0 ^ Flag_inversione_RELE);                                                        // Spengo il RELE2
      delay(TEMPO_RELE);                                                                                                // Aspetto
    }
    if (((char)message[0] == 'T' or (char)message[0] == 't') &
        (char)message[1] == '=' ) {                                                                                     // Topic "temperatura" = "T=" o "t="
      String stringtmp = "";
      for (int i = 2; i < length; i++)  {
        stringtmp = stringtmp + (char)message[i];
      }
      Serial.println(stringtmp);
      termostato = stringtmp.toFloat();
      Serial.println(termostato);
      EEPROM_write();
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] != '2' ) {                           // Topic "Temperatura" = "stato"
      payload = macToStr(mac);
      payload += " RSSI=";
      payload += WiFi.RSSI();
      payload += "dB ";
      payload += " T=";
      char tmp[4];                            // string buffer
      String stringtmp = "";                  //data on buff is copied to this string
      dtostrf(termostato, 4, 1, tmp);
      for (int i = 0; i < sizeof(tmp); i++)  {
        stringtmp += tmp[i];
      }
      payload += stringtmp;
      if (AUTO == true) payload += " AUTO";
      else payload += " MAN";
      payload += " RELE1=";
      if (digitalRead(RELE1_temperatura) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE1_temperatura) == false ^ Flag_inversione_RELE) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE2_temperatura) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE2_temperatura) == false ^ Flag_inversione_RELE) payload += "OFF";
      client.publish(ACK_Topic, (char*) payload.c_str());
      delay(100);
    }

    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] == '2' ) {                           // Topic "tapparella" = "stato2"
      delay(50);
      sendTemperature();                                                                                                // sendTemperature()
      delay(50);
      payload = macToStr(mac);
      payload += " T=";
      char tmp[4];                            // string buffer
      String stringtmp = "";                  //data on buff is copied to this string
      dtostrf(termostato, 4, 1, tmp);
      for (int i = 0; i < sizeof(tmp); i++)  {
        stringtmp += tmp[i];
      }
      payload += stringtmp;
      if (AUTO == true) payload += " AUTO";
      else payload += " MAN";
      payload += " RELE1=";
      if (digitalRead(RELE1_temperatura) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE1_temperatura) == false ^ Flag_inversione_RELE) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE2_temperatura) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE2_temperatura) == false ^ Flag_inversione_RELE) payload += "OFF";
      client.publish(ACK_Topic, (char*) payload.c_str());
      delay(100);
    }
    if ((char)message[0] == 'a' & (char)message[1] == 'u' & (char)message[2] == 't' & (char)message[3] == 'o' ) {       // Topic "temperatura" = "auto"
      AUTO = true;
      EEPROM_write();
    }
    if ((char)message[0] == 'm' & (char)message[1] == 'a' & (char)message[2] == 'n') {                                  // Topic "temperatura" = "man"
      AUTO = false;
      EEPROM_write();
    }
    if ((char)message[0] == 'r' & (char)message[1] == 'e' & (char)message[2] == 's' &
        (char)message[3] == 'e' & (char)message[4] == 't' ) {                                                           // Topic "temperatura" = "reset"
      EEPROM_clear_reset();
    }
  }

  if (String(topic) == ACK_Topic) {                                                                                   // se arriva il comando sul topic "ACK"
    if ((char)message[0] == 'a' & (char)message[1] == 'c' & (char)message[2] == 'k' ) {                               // Topic "temperatura" = "ack"
      Send_ACK();
      delay(100);
    }
  }
  //digitalWrite(Status_LED, 1 ^ Flag_inversione_Status_LED);
}

void sendTemperature() {
  h = dht.readHumidity();
  delay(50);
  t = dht.readTemperature(false);                           // Read temperature as Celsius (the default)
  delay(50);
  f = dht.readTemperature(true);                            // Read temperature as Fahrenheit (isFahrenheit = true)

  if (isnan(h) || isnan(t) || isnan(f)) {                   // Check if any reads failed and exit early (to try again).
    String payload = macToStr(mac);
    payload += " Failed to read from DHT sensor!";
    Serial.println("Failed to read from DHT sensor!");
    client.publish(ACK_Topic, (char*) payload.c_str());
    return;
  }
  hi = dht.computeHeatIndex(t, h, false);                   // Compute heat index in Celsius (isFahreheit = false) float hi
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" % ");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hi);
  Serial.println(" *C");
  String payload = macToStr(mac);
  payload += " T:";
  payload += t;
  payload += "°C H:";
  payload += h;
  payload += "% HI:";
  payload += hi;
  payload += "°C";
  client.publish(ACK_Topic, (char*) payload.c_str());
}

void loop() {
  ArduinoOTA.handle();

  if (T_LAMPEGGIO.Wait(250)) {
    lamp = lamp + 1;
    if (lamp > 6) lamp = 0;
    if (lamp > 1 and !client.connected()) lamp = 0;
    if (lamp == 0) digitalWrite(Status_LED, 1 ^ Flag_inversione_Status_LED);
    if (lamp == 1) digitalWrite(Status_LED, 0 ^ Flag_inversione_Status_LED);
    if (lamp == 2) digitalWrite(Status_LED, 1 ^ Flag_inversione_Status_LED);
    if (lamp == 3) digitalWrite(Status_LED, 0 ^ Flag_inversione_Status_LED);
    if (lamp == 4) digitalWrite(Status_LED, 0 ^ Flag_inversione_Status_LED);
    if (lamp == 5) digitalWrite(Status_LED, 0 ^ Flag_inversione_Status_LED);
    if (lamp == 6) digitalWrite(Status_LED, 0 ^ Flag_inversione_Status_LED);

  }



  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  if (digitalRead(RELE1_temperatura) == true ^ Flag_inversione_RELE) {
    display.drawString(128, 0, "ON");
  }
  if (digitalRead(RELE1_temperatura) == false ^ Flag_inversione_RELE) {
    display.drawString(128, 0, "OFF");
  }
  if (client.connected()) {
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 0, String(termostato));

    display.setTextAlignment(TEXT_ALIGN_CENTER);
    if (AUTO) {
      display.drawString(68, 0, "AUTO");
    }
    else {
      display.drawString(68, 0,  "MAN");
    }
  }
  else {
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 5, "No connection!");
  }
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 25, "Temp:");
  display.drawString(0, 45, String(t) + " °C");
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(128, 25, "Umidità:");
  display.drawString(128, 45, String(h) + " %");
  display.display();

  if (TIMER.Wait(TEMPO_REFRESH_CONNESSIONE)) {
    Serial.println("Timer -> check connections");
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("ERRORE DI RETE: WiFi.status() != WL_CONNECTED");
      Serial.println("RIAVVIO L'ESP");
      ESP.restart();
      //setup_wifi();
    }
    if (!client.connected()) {
      Serial.println("ERRORE DI RETE: !client.connected()");
      reconnect();
    }
  }

  if (TIMER2.Wait(TEMPO_TERMOSTATO)) {
    h = dht.readHumidity();
    delay(50);
    t = dht.readTemperature(false);                                                   // Read temperature as Celsius (the default)
    if (isnan(t)) {                                                                         // Check if any reads failed and exit early (to try again).
      //      String payload = macToStr(mac);
      //      payload += " Failed to read from DHT sensor!";
      Serial.println("Failed to read from DHT sensor!");
      //      client.publish(ACK_Topic, (char*) payload.c_str());
      return;
    }
    if (AUTO) {
      if (t < (termostato - soglia)) {
        digitalWrite(RELE1_temperatura, 1 ^ Flag_inversione_RELE);                                                        // Accendo il RELE1
        delay(TEMPO_RELE);                                                                                                // Aspetto
      }
      if (t > (termostato + soglia)) {
        digitalWrite(RELE1_temperatura, 0 ^ Flag_inversione_RELE);                                                        // Spengo il RELE1
        delay(TEMPO_RELE);                                                                                                // Aspetto
      }
    }
  }
  client.loop();
}


String macToStr(const uint8_t* mac)
{
  String result;
  for (int i = 0; i < 6; ++i) {
    if (mac[i] < char(16)) result += "0";
    result += String(mac[i], 16);
    //    if (i < 5)
    //      result += ':';
  }
  return result;
}

char* string2char(String command) {
  if (command.length() != 0) {
    char *p = const_cast<char*>(command.c_str());
    return p;
  }
}

String getTime() {
  WiFiClient client;
  while (!!!client.connect("google.com", 80)) {
    Serial.println("connection failed, retrying...");
  }
  client.print("HEAD / HTTP/1.1\r\n\r\n");
  while (!!!client.available()) {
    yield();
  }
  while (client.available()) {
    if (client.read() == '\n') {
      if (client.read() == 'D') {
        if (client.read() == 'a') {
          if (client.read() == 't') {
            if (client.read() == 'e') {
              if (client.read() == ':') {
                client.read();
                String theDate = client.readStringUntil('\r');
                client.stop();
                return theDate;
              }
            }
          }
        }
      }
    }
  }
}

void EEPROM_clear_reset() {
  EEPROM.begin(512);
  // write a 0 to all 512 bytes of the EEPROM
  Serial.print("EEPROM clear:");
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 0);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("EEPROM CLEARED!");
  EEPROM.end();
  ESP.restart();
}

void EEPROM_read() {
  int address = 10;
  int tmp;
  Serial.print("Reading EEPROM MAN/AUTO: ");
  tmp = eepromReadInt(address);
  if (tmp == 1) {
    AUTO = true;
    Serial.println("AUTO");
  }
  if (tmp == 0) {
    AUTO = false;
    Serial.println("MAN");
  }
  if ((tmp != 1 and tmp != 0) or isnan(tmp)) {
    AUTO = false;
    Serial.println("MAN");
  }
  address = 12;
  Serial.print("Reading EEPROM termostato: ");
  termostato = eepromReadFloat(address);
  if (termostato<2 or termostato>30 or isnan(termostato)) termostato = 18;
  Serial.println(termostato);
}

void EEPROM_write() {
  int address = 10;
  Serial.print("Writing MAN/AUTO:");
  if (AUTO == true) {
    int tmp = 1;
    eepromWriteInt(address, tmp);
    Serial.println("AUTO");
  }
  if (AUTO == false) {
    int tmp = 0;
    eepromWriteInt(address, tmp);
    Serial.println("MAN");
  }
  address = 12;
  Serial.print("Writing TERMOSTATO:");
  Serial.println(termostato);
  eepromWriteFloat(address, termostato);
  EEPROM.commit();
}

int eepromReadInt(int address) {
  int value = 0x0000;
  value = value | (EEPROM.read(address) << 8);
  value = value | EEPROM.read(address + 1);
  return value;
}
void eepromWriteInt(int address, int value) {
  EEPROM.write(address, (value >> 8) & 0xFF );
  EEPROM.write(address + 1, value & 0xFF);
}
float eepromReadFloat(int address) {
  union u_tag {
    byte b[4];
    float fval;
  } u;
  u.b[0] = EEPROM.read(address);
  u.b[1] = EEPROM.read(address + 1);
  u.b[2] = EEPROM.read(address + 2);
  u.b[3] = EEPROM.read(address + 3);
  return u.fval;
}
void eepromWriteFloat(int address, float value) {
  union u_tag {
    byte b[4];
    float fval;
  } u;
  u.fval = value;
  EEPROM.write(address  , u.b[0]);
  EEPROM.write(address + 1, u.b[1]);
  EEPROM.write(address + 2, u.b[2]);
  EEPROM.write(address + 3, u.b[3]);
}
