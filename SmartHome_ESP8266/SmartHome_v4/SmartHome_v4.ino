/*
  SmartHome v4
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <TPush.h>
#include <EEPROM.h>
#include "DHT.h"
#include "SSD1306.h"

// Arduino OTA
#include <ArduinoOTA.h>
//#include <ESP8266WiFi.h>
//#include <ESP8266mDNS.h>
//#include <WiFiUdp.h>

#include "SmartHome_config.h"

// VARIABILI
uint8_t       mac[6];                                 // Variabile MAC ADDRESS
long          TEMPO_tapparella1_MAX = 10000;          // Variabile tempo MAX tapparella 1 in azione (in millisecondi)
long          TEMPO_tapparella2_MAX = 10000;          // Variabile tempo MAX tapparella 2 in azione (in millisecondi)
long          percentualeTapparella;                  // Variabili percentuale tapparella
boolean       In_movimento_tapparella1 = false;       // Variabile tapparella 1 in movimento
boolean       In_movimento_tapparella2 = false;       // Variabile tapparella 2 in movimento
unsigned long ulTimeTapparella1, ulTimeTapparella2;   // Variabili timers tapparelle
unsigned long ulTimeReleACancello1, ulTimeReleBCancello1, ulTimeReleACancello2, ulTimeReleBCancello2; // Variabili timers cancelli
int           t = 0;                                  // Variabile per intercettare il rilascio del pulsante
int           lamp = 0;                               // Variabile per lampeggio LED
float         t_temperatura1;                         // Variabile Temperature Celsius
float         h_temperatura1;                         // Variabile Humidity
float         f_temperatura1;                         // Variabile Temperature Fahrenheit
float         hi_temperatura1;                        // Variabile Heat index in Celsius (isFahreheit = false)
float         termostato_temperatura1 = 18.00;        // Variabile temperatura
boolean       AUTO_temperatura1 = false;              // Variabile MAN/AUTO
boolean       Rele_A_eccitato_cancello1 = false;      // Variabile rele A cancello 1
boolean       Rele_B_eccitato_cancello1 = false;      // Variabile rele B cancello 1
boolean       Rele_A_eccitato_cancello2 = false;      // Variabile rele A cancello 2
boolean       Rele_B_eccitato_cancello2 = false;      // Variabile rele B cancello 2

WiFiClient espClient;
PubSubClient client(espClient);
TPush BottoneTAP1SU, BottoneTAP1GIU, BottoneTAP2SU, BottoneTAP2GIU, BottoneAINT1, BottoneBINT1, BottoneAINT2, BottoneBINT2, BottoneAINT3, BottoneBINT3, BottoneAINT4, BottoneBINT4, BottoneACAN1, BottoneBCAN1, BottoneACAN2, BottoneBCAN2, TIMER, TIMER_TERMOSTATO1, T_LAMPEGGIO;
DHT dht(DHTPIN_temperatura1, DHTTYPE, 15);
SSD1306 display(I2C_DISPLAY_ADDRESS_temperatura1, SDA_PIN_temperatura1, SDC_PIN_temperatura1);

void setup() {
#if defined(DEBUG)
  Serial.begin(115200);
#endif
  // Inizializza EEPROM
  EEPROM.begin(512);
  delay(100);
  Serial.println();
  Serial.println();
  Serial.println(" ***** SmartHome v4 *****");
  Serial.println();
  // Inizializza GPIO
  Serial.println("Status LED             = GPIO" + String(Status_LED));
  pinMode(Status_LED, OUTPUT);                                  // Inizializza Status_LED
  digitalWrite(Status_LED, 0 ^ Flag_inversione_Status_LED);
  if (NODI_TAPPARELLA > 0) {
    Serial.println("- Tapparella 1 : " + String(Tapparella1_Topic));
    Serial.println("RELE    tapparella 1 SU  = GPIO" + String(RELE_tapparella1_SU));
    Serial.println("RELE    tapparella 1 GIU = GPIO" + String(RELE_tapparella1_GIU));
    Serial.println("BOTTONE tapparella 1 SU  = GPIO" + String(BOTTONE_tapparella1_SU));
    Serial.println("BOTTONE tapparella 1 GIU = GPIO" + String(BOTTONE_tapparella1_GIU));
    pinMode(RELE_tapparella1_SU, OUTPUT);
    digitalWrite(RELE_tapparella1_SU, 0 ^ Flag_inversione_RELE);
    pinMode(RELE_tapparella1_GIU, OUTPUT);
    digitalWrite(RELE_tapparella1_GIU, 0 ^ Flag_inversione_RELE);
#if defined(PullDown)
    BottoneTAP1SU.setUp(BOTTONE_tapparella1_SU, HIGH);
    BottoneTAP1GIU.setUp(BOTTONE_tapparella1_GIU, HIGH);
#endif
#if defined(PullUp)
    BottoneTAP1SU.setUp(BOTTONE_tapparella1_SU, LOW);
    BottoneTAP1GIU.setUp(BOTTONE_tapparella1_GIU, LOW);
#endif
  }
  if (NODI_TAPPARELLA > 1) {
    Serial.println("- Tapparella 2 : " + String(Tapparella2_Topic));
    Serial.println("RELE    tapparella 2 SU  = GPIO" + String(RELE_tapparella2_SU));
    Serial.println("RELE    tapparella 2 GIU = GPIO" + String(RELE_tapparella2_GIU));
    Serial.println("BOTTONE tapparella 2 SU  = GPIO" + String(BOTTONE_tapparella2_SU));
    Serial.println("BOTTONE tapparella 2 GIU = GPIO" + String(BOTTONE_tapparella2_GIU));
    pinMode(RELE_tapparella2_SU, OUTPUT);
    digitalWrite(RELE_tapparella2_SU, 0 ^ Flag_inversione_RELE);
    pinMode(RELE_tapparella2_GIU, OUTPUT);
    digitalWrite(RELE_tapparella2_GIU, 0 ^ Flag_inversione_RELE);
#if defined(PullDown)
    BottoneTAP2SU.setUp(BOTTONE_tapparella2_SU, HIGH);
    BottoneTAP2GIU.setUp(BOTTONE_tapparella2_GIU, HIGH);
#endif
#if defined(PullUp)
    BottoneTAP2SU.setUp(BOTTONE_tapparella2_SU, LOW);
    BottoneTAP2GIU.setUp(BOTTONE_tapparella2_GIU, LOW);
#endif
  }
  if (NODI_INTERRUTTORE > 0) {
    Serial.println("- Interruttore 1 : " + String(Interruttore1_Topic));
    Serial.println("RELE A    interruttore 1 = GPIO" + String(RELE_A_interruttore1));
    Serial.println("RELE B    interruttore 1 = GPIO" + String(RELE_B_interruttore1));
    Serial.println("BOTTONE A interruttore 1 = GPIO" + String(BOTTONE_A_interruttore1));
    Serial.println("BOTTONE B interruttore 1 = GPIO" + String(BOTTONE_B_interruttore1));
    pinMode(RELE_A_interruttore1, OUTPUT);
    digitalWrite(RELE_A_interruttore1, EEPROM_read_releAinterruttore1() ^ Flag_inversione_RELE);
    pinMode(RELE_B_interruttore1, OUTPUT);
    digitalWrite(RELE_B_interruttore1, EEPROM_read_releBinterruttore1() ^ Flag_inversione_RELE);
#if defined(PullDown)
    BottoneAINT1.setUp(BOTTONE_A_interruttore1, HIGH);
    BottoneBINT1.setUp(BOTTONE_B_interruttore1, HIGH);
#endif
#if defined(PullUp)
    BottoneAINT1.setUp(BOTTONE_A_interruttore1, LOW);
    BottoneBINT1.setUp(BOTTONE_B_interruttore1, LOW);
#endif
  }
  if (NODI_INTERRUTTORE > 1) {
    Serial.println("- Interruttore 2 : " + String(Interruttore1_Topic));
    Serial.println("RELE A    interruttore 2 = GPIO" + String(RELE_A_interruttore2));
    Serial.println("RELE B    interruttore 2 = GPIO" + String(RELE_B_interruttore2));
    Serial.println("BOTTONE A interruttore 2 = GPIO" + String(BOTTONE_A_interruttore2));
    Serial.println("BOTTONE B interruttore 2 = GPIO" + String(BOTTONE_B_interruttore2));
    pinMode(RELE_A_interruttore2, OUTPUT);
    digitalWrite(RELE_A_interruttore2, EEPROM_read_releAinterruttore2() ^ Flag_inversione_RELE);
    pinMode(RELE_B_interruttore2, OUTPUT);
    digitalWrite(RELE_B_interruttore2, EEPROM_read_releBinterruttore2() ^ Flag_inversione_RELE);
#if defined(PullDown)
    BottoneAINT2.setUp(BOTTONE_A_interruttore2, HIGH);
    BottoneBINT2.setUp(BOTTONE_B_interruttore2, HIGH);
#endif
#if defined(PullUp)
    BottoneAINT2.setUp(BOTTONE_A_interruttore2, LOW);
    BottoneBINT2.setUp(BOTTONE_B_interruttore2, LOW);
#endif
  }
  if (NODI_INTERRUTTORE > 2) {
    Serial.println("- Interruttore 3 : " + String(Interruttore1_Topic));
    Serial.println("RELE A    interruttore 3 = GPIO" + String(RELE_A_interruttore3));
    Serial.println("RELE B    interruttore 3 = GPIO" + String(RELE_B_interruttore3));
    Serial.println("BOTTONE A interruttore 3 = GPIO" + String(BOTTONE_A_interruttore3));
    Serial.println("BOTTONE B interruttore 3 = GPIO" + String(BOTTONE_B_interruttore3));
    pinMode(RELE_A_interruttore3, OUTPUT);
    digitalWrite(RELE_A_interruttore3, EEPROM_read_releAinterruttore3() ^ Flag_inversione_RELE);
    pinMode(RELE_B_interruttore3, OUTPUT);
    digitalWrite(RELE_B_interruttore3, EEPROM_read_releBinterruttore3() ^ Flag_inversione_RELE);
#if defined(PullDown)
    BottoneAINT3.setUp(BOTTONE_A_interruttore3, HIGH);
    BottoneBINT3.setUp(BOTTONE_B_interruttore3, HIGH);
#endif
#if defined(PullUp)
    BottoneAINT3.setUp(BOTTONE_A_interruttore3, LOW);
    BottoneBINT3.setUp(BOTTONE_B_interruttore3, LOW);
#endif
  }
  if (NODI_INTERRUTTORE > 3) {
    Serial.println("- Interruttore 4 : " + String(Interruttore1_Topic));
    Serial.println("RELE A    interruttore 4 = GPIO" + String(RELE_A_interruttore4));
    Serial.println("RELE B    interruttore 4 = GPIO" + String(RELE_B_interruttore4));
    Serial.println("BOTTONE A interruttore 4 = GPIO" + String(BOTTONE_A_interruttore4));
    Serial.println("BOTTONE B interruttore 4 = GPIO" + String(BOTTONE_B_interruttore4));
    pinMode(RELE_A_interruttore4, OUTPUT);
    digitalWrite(RELE_A_interruttore4, EEPROM_read_releAinterruttore4() ^ Flag_inversione_RELE);
    pinMode(RELE_B_interruttore4, OUTPUT);
    digitalWrite(RELE_B_interruttore4, EEPROM_read_releBinterruttore4() ^ Flag_inversione_RELE);
#if defined(PullDown)
    BottoneAINT4.setUp(BOTTONE_A_interruttore4, HIGH);
    BottoneBINT4.setUp(BOTTONE_B_interruttore4, HIGH);
#endif
#if defined(PullUp)
    BottoneAINT4.setUp(BOTTONE_A_interruttore4, LOW);
    BottoneBINT4.setUp(BOTTONE_B_interruttore4, LOW);
#endif
  }
  if (NODI_CANCELLO > 0) {
    Serial.println("- Cancello 1 : " + String(Cancello1_Topic));
    Serial.println("RELE A cancello 1        = GPIO" + String(RELE_A_cancello1));
    Serial.println("RELE B cancello 1        = GPIO" + String(RELE_B_cancello1));
    Serial.println("Pulsante A cancello 1    = GPIO" + String(BOTTONE_A_cancello1));
    Serial.println("Pulsante B cancello 1    = GPIO" + String(BOTTONE_B_cancello1));
    pinMode(RELE_A_cancello1, OUTPUT);
    digitalWrite(RELE_A_cancello1, 0 ^ Flag_inversione_RELE);
    pinMode(RELE_B_cancello1, OUTPUT);
    digitalWrite(RELE_B_cancello1, 0 ^ Flag_inversione_RELE);
#if defined(PullDown)
    BottoneACAN1.setUp(BOTTONE_A_cancello1, HIGH);
    BottoneBCAN1.setUp(BOTTONE_B_cancello1, HIGH);
#endif
#if defined(PullUp)
    BottoneACAN1.setUp(BOTTONE_A_cancello1, LOW);
    BottoneBCAN1.setUp(BOTTONE_B_cancello1, LOW);
#endif
  }
  if (NODI_CANCELLO > 1) {
    Serial.println("- Cancello 2 : " + String(Cancello2_Topic));
    Serial.println("RELE A cancello 2        = GPIO" + String(RELE_A_cancello2));
    Serial.println("RELE B cancello 2        = GPIO" + String(RELE_B_cancello2));
    Serial.println("Pulsante A cancello 2    = GPIO" + String(BOTTONE_A_cancello2));
    Serial.println("Pulsante B cancello 2    = GPIO" + String(BOTTONE_B_cancello2));
    pinMode(RELE_A_cancello2, OUTPUT);
    digitalWrite(RELE_A_cancello2, 0 ^ Flag_inversione_RELE);
    pinMode(RELE_B_cancello2, OUTPUT);
    digitalWrite(RELE_B_cancello2, 0 ^ Flag_inversione_RELE);
#if defined(PullDown)
    BottoneACAN2.setUp(BOTTONE_A_cancello2, HIGH);
    BottoneBCAN2.setUp(BOTTONE_B_cancello2, HIGH);
#endif
#if defined(PullUp)
    BottoneACAN2.setUp(BOTTONE_A_cancello2, LOW);
    BottoneBCAN2.setUp(BOTTONE_B_cancello2, LOW);
#endif
  }
  if (NODI_TEMPERATURA > 0) {
    Serial.println("- Temperatura 1 : " + String(Temperatura1_Topic));
    Serial.println("RELE A temperatura 1      = GPIO" + String(RELE_A_temperatura1));
    Serial.println("RELE B temperatura 1      = GPIO" + String(RELE_B_temperatura1));
    Serial.println("DTH22 temperatura 1       = GPIO" + String(DHTPIN_temperatura1));
    Serial.println("SSD1306 ADDRESS temperatura 1 = 0x"   + String(I2C_DISPLAY_ADDRESS_temperatura1, HEX));
    Serial.println("SSD1306 SDA temperatura 1 = GPIO" + String(SDA_PIN_temperatura1));
    Serial.println("SSD1306 SDC temperatura 1 = GPIO" + String(SDC_PIN_temperatura1));
    pinMode(RELE_A_temperatura1, OUTPUT);
    digitalWrite(RELE_A_temperatura1, 0 ^ Flag_inversione_RELE);
    pinMode(RELE_B_temperatura1, OUTPUT);
    digitalWrite(RELE_B_temperatura1, 0 ^ Flag_inversione_RELE);
  }
  if (NODI_TEMPERATURA > 0) {                                         // Inizializzazione il display
    Serial.println("Inizializzazione il display");
    display.init();
    display.flipScreenVertically();
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 0, "Smart home");
    display.drawString(0, 25, Temperatura1_Topic);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 45, "Firmware v4");
    display.display();
    delay(2500);
  }
  // Inizializza connessioni
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  setup_wifi();
  beginOTA(OTA_hostname);
  reconnect();

  EEPROM_read_TEMPO_tapparelle_MAX();
  EEPROM_temperatura1_read();
}

void setup_wifi() {
  int i = 0;
  delay(20);
  // Serial.println("** Scan Networks **");
  // byte numSsid = WiFi.scanNetworks();
  // Serial.print("SSID List:");
  // Serial.println(numSsid);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(ssid);
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
  Serial.print("RSSI:");
  Serial.println(WiFi.RSSI());
  Serial.print("Local  IP   : ");
  Serial.println(WiFi.localIP());
  //  Serial.print("SoftAP IP   : ");
  //  Serial.println(WiFi.softAPIP());
  Serial.print("MAC address : ");
  WiFi.macAddress(mac);
  Serial.println(macToStr(mac));
  digitalWrite(Status_LED, 1 ^ Flag_inversione_Status_LED);
  dht.begin();
}

void reconnect() {
  int i = 0;
  String clientName = "ESP8266Client-";
  clientName += macToStr(mac);
  clientName += "-";
  clientName += millis();
  while (!client.connected() and i < MAX_RET_MQTT) {
    ++i;
    Serial.print("MQTT Client : ");
    Serial.println(clientName);
    Serial.print("Attempting MQTT connection... ");
    if (client.connect(string2char(clientName), mqtt_user, mqtt_password)) {
      Serial.println("MQTT connected!");
      String payload = macToStr(mac);
      payload += " start at ";
      payload += getTime();
      client.publish(ACK_Topic, (char*) payload.c_str());     // Pubblica su ACK_Topic -> MAC + " start at " + time
      delay(100);
      Serial.println(payload);
      Send_ACK();
      client.subscribe(ACK_Topic);                            // Sottoscrivi ACK_Topic
      delay(250);
      Serial.println("MQTT subscribe " + String(ACK_Topic));
      if (NODI_TAPPARELLA > 0) {
        client.subscribe(Tapparella1_Topic);                    // Sottoscrivi Tapparella1_Topic
        delay(250);
        Serial.println("MQTT subscribe " + String(Tapparella1_Topic));
      }
      if (NODI_TAPPARELLA > 1) {
        client.subscribe(Tapparella2_Topic);                    // Sottoscrivi Tapparella2_Topic
        delay(250);
        Serial.println("MQTT subscribe " + String(Tapparella2_Topic));
      }
      if (NODI_INTERRUTTORE > 0) {
        client.subscribe(Interruttore1_Topic);                  // Sottoscrivi Interruttore1_Topic
        delay(250);
        Serial.println("MQTT subscribe " + String(Interruttore1_Topic));
      }
      if (NODI_INTERRUTTORE > 1) {
        client.subscribe(Interruttore2_Topic);                  // Sottoscrivi Interruttore2_Topic
        delay(250);
        Serial.println("MQTT subscribe " + String(Interruttore2_Topic));
      }
      if (NODI_INTERRUTTORE > 2) {
        client.subscribe(Interruttore3_Topic);                  // Sottoscrivi Interruttore3_Topic
        delay(250);
        Serial.println("MQTT subscribe " + String(Interruttore3_Topic));
      }
      if (NODI_INTERRUTTORE > 3) {
        client.subscribe(Interruttore4_Topic);                  // Sottoscrivi Interruttore4_Topic
        delay(250);
        Serial.println("MQTT subscribe " + String(Interruttore4_Topic));
      }
      if (NODI_CANCELLO > 0) {
        client.subscribe(Cancello1_Topic);                  // Sottoscrivi Cancello1_Topic
        delay(250);
        Serial.println("MQTT subscribe " + String(Cancello1_Topic));
      }
      if (NODI_CANCELLO > 1) {
        client.subscribe(Cancello2_Topic);                  // Sottoscrivi Cancello2_Topic
        delay(250);
        Serial.println("MQTT subscribe " + String(Cancello2_Topic));
      }
      if (NODI_TEMPERATURA > 0) {
        client.subscribe(Temperatura1_Topic);                  // Sottoscrivi Temperatura1_Topic
        delay(250);
        Serial.println("MQTT subscribe " + String(Temperatura1_Topic));
      }
      else {
        Serial.print("MQTT failed, rc = ");
        Serial.println(client.state());
      }
    }
  }
}

// starts OTA server
void beginOTA(const char* hostname) {
  ArduinoOTA.setHostname(hostname);
  //ArduinoOTA.setPassword("admin");
  ArduinoOTA.onStart([]() {
    Serial.println("Start OTA ");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd OTA");
    delay(1000);
    Serial.println("Restart ESP");
    ESP.restart();
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
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
  String payload;
  if (NODI_TAPPARELLA > 0) {
    payload = macToStr(mac);
    payload += " alive! ";
    payload += "TAP";
    payload += " ";
    payload += Tapparella1_Topic;
    client.publish(ACK_Topic, (char*) payload.c_str());
    delay(250);
  }
  if (NODI_TAPPARELLA > 1) {
    payload = macToStr(mac);
    payload += " alive! ";
    payload += "TAP";
    payload += " ";
    payload += Tapparella2_Topic;
    client.publish(ACK_Topic, (char*) payload.c_str());
    delay(250);
  }
  if (NODI_INTERRUTTORE > 0) {
    payload = macToStr(mac);
    payload += " alive! ";
    payload += "INT";
    payload += " ";
    payload += Interruttore1_Topic;
    client.publish(ACK_Topic, (char*) payload.c_str());
    delay(250);
  }
  if (NODI_INTERRUTTORE > 1) {
    payload = macToStr(mac);
    payload += " alive! ";
    payload += "INT";
    payload += " ";
    payload += Interruttore2_Topic;
    client.publish(ACK_Topic, (char*) payload.c_str());
    delay(250);
  }
  if (NODI_INTERRUTTORE > 2) {
    payload = macToStr(mac);
    payload += " alive! ";
    payload += "INT";
    payload += " ";
    payload += Interruttore3_Topic;
    client.publish(ACK_Topic, (char*) payload.c_str());
    delay(250);
  }
  if (NODI_INTERRUTTORE > 3) {
    payload = macToStr(mac);
    payload += " alive! ";
    payload += "INT";
    payload += " ";
    payload += Interruttore4_Topic;
    client.publish(ACK_Topic, (char*) payload.c_str());
    delay(250);
  }
  if (NODI_CANCELLO > 0) {
    payload = macToStr(mac);
    payload += " alive! ";
    payload += "CAN";
    payload += " ";
    payload += Cancello1_Topic;
    client.publish(ACK_Topic, (char*) payload.c_str());
    delay(250);
  }
  if (NODI_CANCELLO > 1) {
    payload = macToStr(mac);
    payload += " alive! ";
    payload += "CAN";
    payload += " ";
    payload += Cancello2_Topic;
    client.publish(ACK_Topic, (char*) payload.c_str());
    delay(250);
  }
  if (NODI_TEMPERATURA > 0) {
    payload = macToStr(mac);
    payload += " alive! ";
    payload += "TEM";
    payload += " ";
    payload += Temperatura1_Topic;
    client.publish(ACK_Topic, (char*) payload.c_str());
    delay(250);
  }
}

void callback(char* topic, byte * message, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
  }
  Serial.println();
  String payload;

  // TAPPARELLA 1
  if (String(topic) == Tapparella1_Topic) {                                               // se arriva il comando sul topic "Tapparella_Topic"
    if ((char)message[0] == 's' & (char)message[1] == 'u' ) {                             // Topic "tapparella" = "su"
      digitalWrite(RELE_tapparella1_GIU, 0 ^ Flag_inversione_RELE);                       // RELE GIU = 0
      delay(TEMPO_RELE);                                                                  // Aspetto
      digitalWrite(RELE_tapparella1_SU, 1 ^ Flag_inversione_RELE);                        // RELE SU = 1
      In_movimento_tapparella1 = true;
      ulTimeTapparella1 = millis();
    }
    if ((char)message[0] == 'g' & (char)message[1] == 'i' & (char)message[2] == 'u') {    // Topic "tapparella" = "giu"
      digitalWrite(RELE_tapparella1_SU, 0 ^ Flag_inversione_RELE);                        // RELE SU = 0
      delay(TEMPO_RELE);                                                                  // Aspetto
      digitalWrite(RELE_tapparella1_GIU, 1 ^ Flag_inversione_RELE);                       // RELE GIU = 1
      In_movimento_tapparella1 = true;
      ulTimeTapparella1 = millis();
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'o' &
        (char)message[3] == 'p' ) {                                                       // Topic "tapparella" = "stop"
      digitalWrite(RELE_tapparella1_SU, 0 ^ Flag_inversione_RELE);                        // RELE SU = 0
      delay(TEMPO_RELE);                                                                  // Aspetto
      digitalWrite(RELE_tapparella1_GIU, 0 ^ Flag_inversione_RELE);                       // RELE GIU = 0
      In_movimento_tapparella1 = false;
    }
    if ((char)message[0] == '+') {                                                        // Topic "tapparella" = "+"
      digitalWrite(RELE_tapparella1_SU, 1 ^ Flag_inversione_RELE);                        // RELE SU = 1
      delay(TEMPO_RELE);                                                                  // Aspetto
      digitalWrite(RELE_tapparella1_GIU, 0 ^ Flag_inversione_RELE);                       // RELE GIU = 0
      String stringtmp = "";
      for (int i = 1; i < length; i++)  {
        stringtmp = stringtmp + (char)message[i];
      }
      char buf[stringtmp.length()];
      stringtmp.toCharArray(buf, stringtmp.length() + 1);
      percentualeTapparella = atol(buf);
      if (percentualeTapparella > 100) percentualeTapparella = 100;
      percentualeTapparella = TEMPO_tapparella1_MAX * percentualeTapparella / 100;
      In_movimento_tapparella1 = true;
      ulTimeTapparella1 = millis();
      ulTimeTapparella1 -= TEMPO_tapparella1_MAX;
      ulTimeTapparella1 += percentualeTapparella;
    }
    if ((char)message[0] == '-') {                                                        // Topic "tapparella" = "-"
      digitalWrite(RELE_tapparella1_SU, 0 ^ Flag_inversione_RELE);                        // RELE SU = 1
      delay(TEMPO_RELE);                                                                  // Aspetto
      digitalWrite(RELE_tapparella1_GIU, 1 ^ Flag_inversione_RELE);                       // RELE GIU = 0
      String stringtmp = "";
      for (int i = 1; i < length; i++)  {
        stringtmp = stringtmp + (char)message[i];
      }
      char buf[stringtmp.length()];
      stringtmp.toCharArray(buf, stringtmp.length() + 1);
      percentualeTapparella = atol(buf);
      if (percentualeTapparella > 100) percentualeTapparella = 100;
      percentualeTapparella = TEMPO_tapparella1_MAX * percentualeTapparella / 100;
      In_movimento_tapparella1 = true;
      ulTimeTapparella1 = millis();
      ulTimeTapparella1 -= TEMPO_tapparella1_MAX;
      ulTimeTapparella1 += percentualeTapparella;
    }
    if (((char)message[0] == 'T' or (char)message[0] == 't') &
        (char)message[1] == '=' ) {                                                        // Topic "tapparella" = "T=" o "t="
      String stringtmp = "";
      for (int i = 2; i < length; i++)  {
        stringtmp = stringtmp + (char)message[i];
      }
      char buf[stringtmp.length()];
      stringtmp.toCharArray(buf, stringtmp.length() + 1);
      TEMPO_tapparella1_MAX = atol(buf);
      if (TEMPO_tapparella1_MAX < 1) {
        TEMPO_tapparella1_MAX = 10;
      }
      if (TEMPO_tapparella1_MAX > 120) {
        TEMPO_tapparella1_MAX = 120;
      }
      TEMPO_tapparella1_MAX = TEMPO_tapparella1_MAX * 1000;
      EEPROM_write_TEMPO_tapparelle_MAX();
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] != '2' ) {   // Topic "tapparella" = "stato"
      payload = Tapparella1_Topic;
      payload += " RSSI=";
      payload += WiFi.RSSI();
      payload += "dB ";
      payload += " T=";
      payload += TEMPO_tapparella1_MAX / 1000;
      payload += "sec. SU=";
      if (digitalRead(RELE_tapparella1_SU) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE_tapparella1_SU) == false ^ Flag_inversione_RELE) payload += "OFF";
      payload += " GIU=";
      if (digitalRead(RELE_tapparella1_GIU) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE_tapparella1_GIU) == false ^ Flag_inversione_RELE) payload += "OFF";
      client.publish(ACK_Topic, (char*) payload.c_str());
      delay(100);
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] == '2' ) {   // Topic "tapparella" = "stato2"
      payload = Tapparella1_Topic;
      payload += " SU=";
      if (digitalRead(RELE_tapparella1_SU) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE_tapparella1_SU) == false ^ Flag_inversione_RELE) payload += "OFF";
      payload += " GIU=";
      if (digitalRead(RELE_tapparella1_GIU) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE_tapparella1_GIU) == false ^ Flag_inversione_RELE) payload += "OFF";
      client.publish(ACK2_Topic, (char*) payload.c_str());
      delay(100);
    }
  }

  // TAPPARELLA 2
  if (String(topic) == Tapparella2_Topic) {                                               // se arriva il comando sul topic "Tapparella_Topic"
    if ((char)message[0] == 's' & (char)message[1] == 'u' ) {                             // Topic "tapparella" = "su"
      digitalWrite(RELE_tapparella2_GIU, 0 ^ Flag_inversione_RELE);                       // RELE GIU = 0
      delay(TEMPO_RELE);                                                                  // Aspetto
      digitalWrite(RELE_tapparella2_SU, 1 ^ Flag_inversione_RELE);                        // RELE SU = 1
      In_movimento_tapparella2 = true;
      ulTimeTapparella2 = millis();
    }
    if ((char)message[0] == 'g' & (char)message[1] == 'i' & (char)message[2] == 'u') {    // Topic "tapparella" = "giu"
      digitalWrite(RELE_tapparella2_SU, 0 ^ Flag_inversione_RELE);                        // RELE SU = 0
      delay(TEMPO_RELE);                                                                  // Aspetto
      digitalWrite(RELE_tapparella2_GIU, 1 ^ Flag_inversione_RELE);                       // RELE GIU = 1
      In_movimento_tapparella2 = true;
      ulTimeTapparella2 = millis();
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'o' &
        (char)message[3] == 'p' ) {                                                       // Topic "tapparella" = "stop"
      digitalWrite(RELE_tapparella2_SU, 0 ^ Flag_inversione_RELE);                        // RELE SU = 0
      delay(TEMPO_RELE);                                                                  // Aspetto
      digitalWrite(RELE_tapparella2_GIU, 0 ^ Flag_inversione_RELE);                       // RELE GIU = 0
      In_movimento_tapparella2 = false;
    }

    if ((char)message[0] == '+') {                                                        // Topic "tapparella" = "+"
      digitalWrite(RELE_tapparella2_SU, 1 ^ Flag_inversione_RELE);                        // RELE SU = 1
      delay(TEMPO_RELE);                                                                  // Aspetto
      digitalWrite(RELE_tapparella2_GIU, 0 ^ Flag_inversione_RELE);                       // RELE GIU = 0
      String stringtmp = "";
      for (int i = 1; i < length; i++)  {
        stringtmp = stringtmp + (char)message[i];
      }
      char buf[stringtmp.length()];
      stringtmp.toCharArray(buf, stringtmp.length() + 1);
      percentualeTapparella = atol(buf);
      if (percentualeTapparella > 100) percentualeTapparella = 100;
      percentualeTapparella = TEMPO_tapparella2_MAX * percentualeTapparella / 100;
      In_movimento_tapparella1 = true;
      ulTimeTapparella2 = millis();
      ulTimeTapparella2 -= TEMPO_tapparella1_MAX;
      ulTimeTapparella2 += percentualeTapparella;
    }
    if ((char)message[0] == '-') {                                                        // Topic "tapparella" = "-"
      digitalWrite(RELE_tapparella2_SU, 0 ^ Flag_inversione_RELE);                        // RELE SU = 1
      delay(TEMPO_RELE);                                                                  // Aspetto
      digitalWrite(RELE_tapparella2_GIU, 1 ^ Flag_inversione_RELE);                       // RELE GIU = 0
      String stringtmp = "";
      for (int i = 1; i < length; i++)  {
        stringtmp = stringtmp + (char)message[i];
      }
      char buf[stringtmp.length()];
      stringtmp.toCharArray(buf, stringtmp.length() + 1);
      percentualeTapparella = atol(buf);
      if (percentualeTapparella > 100) percentualeTapparella = 100;
      percentualeTapparella = TEMPO_tapparella2_MAX * percentualeTapparella / 100;
      In_movimento_tapparella1 = true;
      ulTimeTapparella2 = millis();
      ulTimeTapparella2 -= TEMPO_tapparella1_MAX;
      ulTimeTapparella2 += percentualeTapparella;
    }
    if (((char)message[0] == 'T' or (char)message[0] == 't') &
        (char)message[1] == '=' ) {                                                       // Topic "tapparella" = "T=" o "t="
      String stringtmp = "";
      for (int i = 2; i < length; i++)  {
        stringtmp = stringtmp + (char)message[i];
      }
      char buf[stringtmp.length()];
      stringtmp.toCharArray(buf, stringtmp.length() + 1);
      TEMPO_tapparella2_MAX = atol(buf);
      if (TEMPO_tapparella2_MAX < 1) {
        TEMPO_tapparella2_MAX = 10;
      }
      if (TEMPO_tapparella2_MAX > 120) {
        TEMPO_tapparella2_MAX = 120;
      }
      TEMPO_tapparella2_MAX = TEMPO_tapparella2_MAX * 1000;
      EEPROM_write_TEMPO_tapparelle_MAX();
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] != '2' ) {   // Topic "tapparella" = "stato"
      payload = Tapparella2_Topic;
      payload += " RSSI=";
      payload += WiFi.RSSI();
      payload += "dB ";
      payload += " T=";
      payload += TEMPO_tapparella2_MAX / 1000;
      payload += "sec. SU=";
      if (digitalRead(RELE_tapparella2_SU) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE_tapparella2_SU) == false ^ Flag_inversione_RELE) payload += "OFF";
      payload += " GIU=";
      if (digitalRead(RELE_tapparella2_GIU) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE_tapparella2_GIU) == false ^ Flag_inversione_RELE) payload += "OFF";
      client.publish(ACK_Topic, (char*) payload.c_str());
      delay(100);
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] == '2' ) {     // Topic "tapparella" = "stato2"
      payload = Tapparella2_Topic;
      payload += " SU=";
      if (digitalRead(RELE_tapparella2_SU) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE_tapparella2_SU) == false ^ Flag_inversione_RELE) payload += "OFF";
      payload += " GIU=";
      if (digitalRead(RELE_tapparella2_GIU) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE_tapparella2_GIU) == false ^ Flag_inversione_RELE) payload += "OFF";
      client.publish(ACK2_Topic, (char*) payload.c_str());
      delay(100);
    }
  }

  // INTERRUTTORE 1
  if (String(topic) == Interruttore1_Topic) {                                              // se arriva il comando sul topic "Interruttore1_Topic"
    if ((char)message[0] == '1' & (char)message[1] == 'o' & (char)message[2] == 'n') {    // Topic "interruttore" = "1on"
      digitalWrite(RELE_A_interruttore1, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 1
      delay(TEMPO_RELE);                                                                  // Aspetto
      EEPROM_write_releAinterruttore1();
      EEPROM_read_releAinterruttore1();
    }
    if ((char)message[0] == '1' & (char)message[1] == 'o' & (char)message[2] == 'f' &
        (char)message[3] == 'f') {                                                        // Topic "interruttore" = "1off"
      digitalWrite(RELE_A_interruttore1, 0 ^ Flag_inversione_RELE);                                     // Spengo il RELE 1
      delay(TEMPO_RELE);                                                                  // Aspetto
      EEPROM_write_releAinterruttore1();
      EEPROM_read_releAinterruttore1();
    }
    if ((char)message[0] == '2' & (char)message[1] == 'o' & (char)message[2] == 'n') {    // Topic "interruttore" = "2on"
      digitalWrite(RELE_B_interruttore1, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 2
      delay(TEMPO_RELE);                                                                  // Aspetto
      EEPROM_write_releBinterruttore1();
    }
    if ((char)message[0] == '2' & (char)message[1] == 'o' & (char)message[2] == 'f' &
        (char)message[3] == 'f') {                                                        // Topic "interruttore" = "2off"
      digitalWrite(RELE_B_interruttore1, 0 ^ Flag_inversione_RELE);                                     // Spengo il RELE 2
      delay(TEMPO_RELE);                                                                  // Aspetto
      EEPROM_write_releBinterruttore1();
    }

    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] != '2' ) {                             // Topic "interruttore" = "stato"
      payload = Interruttore1_Topic;
      payload += " RSSI=";
      payload += WiFi.RSSI();
      payload += "dB ";
      payload += " RELE1=";
      if (digitalRead(RELE_A_interruttore1) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_A_interruttore1) ^ Flag_inversione_RELE == false ) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE_B_interruttore1) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_B_interruttore1) ^ Flag_inversione_RELE == false) payload += "OFF";
      client.publish(ACK_Topic, (char*) payload.c_str());
      delay(100);
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] == '2' ) {                             // Topic "interruttore" = "stato2"
      payload = Interruttore1_Topic;
      payload += " RELE1=";
      if (digitalRead(RELE_A_interruttore1) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_A_interruttore1) ^ Flag_inversione_RELE == false ) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE_B_interruttore1) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_B_interruttore1) ^ Flag_inversione_RELE == false) payload += "OFF";
      client.publish(ACK2_Topic, (char*) payload.c_str());
      delay(100);
    }
  }

  // INTERRUTTORE 2
  if (String(topic) == Interruttore2_Topic) {                                              // se arriva il comando sul topic "interruttore2_Topic"
    if ((char)message[0] == '1' & (char)message[1] == 'o' & (char)message[2] == 'n') {    // Topic "interruttore" = "1on"
      digitalWrite(RELE_A_interruttore2, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 1
      delay(TEMPO_RELE);                                                                  // Aspetto
      EEPROM_write_releAinterruttore2();
    }
    if ((char)message[0] == '1' & (char)message[1] == 'o' & (char)message[2] == 'f' &
        (char)message[3] == 'f') {                                                        // Topic "interruttore" = "1off"
      digitalWrite(RELE_A_interruttore2, 0 ^ Flag_inversione_RELE);                                     // Spengo il RELE 1
      delay(TEMPO_RELE);                                                                  // Aspetto
      EEPROM_write_releAinterruttore2();
    }
    if ((char)message[0] == '2' & (char)message[1] == 'o' & (char)message[2] == 'n') {    // Topic "interruttore" = "2on"
      digitalWrite(RELE_B_interruttore2, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 2
      delay(TEMPO_RELE);                                                                  // Aspetto
      EEPROM_write_releBinterruttore2();
    }
    if ((char)message[0] == '2' & (char)message[1] == 'o' & (char)message[2] == 'f' &
        (char)message[3] == 'f') {                                                        // Topic "interruttore" = "2off"
      digitalWrite(RELE_B_interruttore2, 0 ^ Flag_inversione_RELE);                                     // Spengo il RELE 2
      delay(TEMPO_RELE);                                                                  // Aspetto
      EEPROM_write_releBinterruttore2();
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] != '2' ) {                             // Topic "interruttore" = "stato"
      payload = Interruttore2_Topic;
      payload += " RSSI=";
      payload += WiFi.RSSI();
      payload += "dB ";
      payload += " RELE1=";
      if (digitalRead(RELE_A_interruttore2) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_A_interruttore2) ^ Flag_inversione_RELE == false ) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE_B_interruttore2) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_B_interruttore2) ^ Flag_inversione_RELE == false) payload += "OFF";
      client.publish(ACK_Topic, (char*) payload.c_str());
      delay(100);
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] == '2' ) {                             // Topic "interruttore" = "stato2"
      payload = Interruttore2_Topic;
      payload += " RELE1=";
      if (digitalRead(RELE_A_interruttore2) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_A_interruttore2) ^ Flag_inversione_RELE == false ) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE_B_interruttore2) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_B_interruttore2) ^ Flag_inversione_RELE == false) payload += "OFF";
      client.publish(ACK2_Topic, (char*) payload.c_str());
      delay(100);
    }
  }

  // INTERRUTTORE 3
  if (String(topic) == Interruttore3_Topic) {                                              // se arriva il comando sul topic "Interruttore3_Topic"
    if ((char)message[0] == '1' & (char)message[1] == 'o' & (char)message[2] == 'n') {    // Topic "interruttore" = "1on"
      digitalWrite(RELE_A_interruttore3, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 1
      delay(TEMPO_RELE);                                                                  // Aspetto
      EEPROM_write_releAinterruttore3();
    }
    if ((char)message[0] == '1' & (char)message[1] == 'o' & (char)message[2] == 'f' &
        (char)message[3] == 'f') {                                                        // Topic "interruttore" = "1off"
      digitalWrite(RELE_A_interruttore3, 0 ^ Flag_inversione_RELE);                                     // Spengo il RELE 1
      delay(TEMPO_RELE);                                                                  // Aspetto
      EEPROM_write_releAinterruttore3();
    }
    if ((char)message[0] == '2' & (char)message[1] == 'o' & (char)message[2] == 'n') {    // Topic "interruttore" = "2on"
      digitalWrite(RELE_B_interruttore3, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 2
      delay(TEMPO_RELE);                                                                  // Aspetto
      EEPROM_write_releBinterruttore3();
    }
    if ((char)message[0] == '2' & (char)message[1] == 'o' & (char)message[2] == 'f' &
        (char)message[3] == 'f') {                                                        // Topic "interruttore" = "2off"
      digitalWrite(RELE_B_interruttore3, 0 ^ Flag_inversione_RELE);                                     // Spengo il RELE 2
      delay(TEMPO_RELE);                                                                  // Aspetto
      EEPROM_write_releBinterruttore3();
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] != '2' ) {                             // Topic "interruttore" = "stato"
      payload = Interruttore3_Topic;
      payload += " RSSI=";
      payload += WiFi.RSSI();
      payload += "dB ";
      payload += " RELE1=";
      if (digitalRead(RELE_A_interruttore3) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_A_interruttore3) ^ Flag_inversione_RELE == false ) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE_B_interruttore3) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_B_interruttore3) ^ Flag_inversione_RELE == false) payload += "OFF";
      client.publish(ACK_Topic, (char*) payload.c_str());
      delay(100);
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] == '2' ) {                             // Topic "interruttore" = "stato2"
      payload = Interruttore3_Topic;
      payload += " RELE1=";
      if (digitalRead(RELE_A_interruttore3) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_A_interruttore3) ^ Flag_inversione_RELE == false ) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE_B_interruttore3) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_B_interruttore3) ^ Flag_inversione_RELE == false) payload += "OFF";
      client.publish(ACK2_Topic, (char*) payload.c_str());
      delay(100);
    }
  }

  // INTERRUTTORE 4
  if (String(topic) == Interruttore4_Topic) {                                              // se arriva il comando sul topic "interruttore4_Topic"
    if ((char)message[0] == '1' & (char)message[1] == 'o' & (char)message[2] == 'n') {    // Topic "interruttore" = "1on"
      digitalWrite(RELE_A_interruttore4, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 1
      delay(TEMPO_RELE);                                                                  // Aspetto
      EEPROM_write_releAinterruttore4();
    }
    if ((char)message[0] == '1' & (char)message[1] == 'o' & (char)message[2] == 'f' &
        (char)message[3] == 'f') {                                                        // Topic "interruttore" = "1off"
      digitalWrite(RELE_A_interruttore4, 0 ^ Flag_inversione_RELE);                                     // Spengo il RELE 1
      delay(TEMPO_RELE);                                                                  // Aspetto
      EEPROM_write_releAinterruttore4();
    }
    if ((char)message[0] == '2' & (char)message[1] == 'o' & (char)message[2] == 'n') {    // Topic "interruttore" = "2on"
      digitalWrite(RELE_B_interruttore4, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 2
      delay(TEMPO_RELE);                                                                  // Aspetto
      EEPROM_write_releBinterruttore4();
    }
    if ((char)message[0] == '2' & (char)message[1] == 'o' & (char)message[2] == 'f' &
        (char)message[3] == 'f') {                                                        // Topic "interruttore" = "2off"
      digitalWrite(RELE_B_interruttore4, 0 ^ Flag_inversione_RELE);                                     // Spengo il RELE 2
      delay(TEMPO_RELE);                                                                  // Aspetto
      EEPROM_write_releBinterruttore4();
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] != '2' ) {                             // Topic "interruttore" = "stato"
      payload = Interruttore4_Topic;
      payload += " RSSI=";
      payload += WiFi.RSSI();
      payload += "dB ";
      payload += " RELE1=";
      if (digitalRead(RELE_A_interruttore4) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_A_interruttore4) ^ Flag_inversione_RELE == false ) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE_B_interruttore4) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_B_interruttore4) ^ Flag_inversione_RELE == false) payload += "OFF";
      client.publish(ACK_Topic, (char*) payload.c_str());
      delay(100);
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] == '2' ) {                             // Topic "interruttore" = "stato2"
      payload = Interruttore4_Topic;
      payload += " RELE1=";
      if (digitalRead(RELE_A_interruttore4) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_A_interruttore4) ^ Flag_inversione_RELE == false ) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE_B_interruttore4) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_B_interruttore4) ^ Flag_inversione_RELE == false) payload += "OFF";
      client.publish(ACK2_Topic, (char*) payload.c_str());
      delay(100);
    }
  }

  // CANCELLO 1
  if (String(topic) == Cancello1_Topic) {                                              // se arriva il comando sul topic "Cancello1_Topic"
    if ((char)message[0] == '1' & (char)message[1] == 'o' & (char)message[2] == 'n') {    // Topic "interruttore" = "1on"
      digitalWrite(RELE_A_cancello1, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 1
      Rele_A_eccitato_cancello1 = true;
      ulTimeReleACancello1 = millis();
    }
    if ((char)message[0] == '2' & (char)message[1] == 'o' & (char)message[2] == 'n') {    // Topic "interruttore" = "2on"
      digitalWrite(RELE_B_cancello1, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 2
      Rele_B_eccitato_cancello1 = true;
      ulTimeReleBCancello1 = millis();
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] != '2' ) {                             // Topic "interruttore" = "stato"
      payload = Cancello1_Topic;
      payload += " RSSI=";
      payload += WiFi.RSSI();
      payload += "dB ";
      payload += " RELE1=";
      if (digitalRead(RELE_A_cancello1) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_A_cancello1) ^ Flag_inversione_RELE == false ) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE_B_cancello1) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_B_cancello1) ^ Flag_inversione_RELE == false) payload += "OFF";
      client.publish(ACK_Topic, (char*) payload.c_str());
      delay(100);
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] == '2' ) {                             // Topic "interruttore" = "stato2"
      payload = Cancello1_Topic;
      payload += " RELE1=";
      if (digitalRead(RELE_A_cancello1) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_A_cancello1) ^ Flag_inversione_RELE == false ) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE_B_cancello1) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_B_cancello1) ^ Flag_inversione_RELE == false) payload += "OFF";
      client.publish(ACK2_Topic, (char*) payload.c_str());
      delay(100);
    }
  }

  // CANCELLO 2
  if (String(topic) == Cancello2_Topic) {                                              // se arriva il comando sul topic "Cancello2_Topic"
    if ((char)message[0] == '1' & (char)message[1] == 'o' & (char)message[2] == 'n') {    // Topic "interruttore" = "1on"
      digitalWrite(RELE_A_cancello2, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 1
      Rele_A_eccitato_cancello2 = true;
      ulTimeReleACancello2 = millis();
    }
    if ((char)message[0] == '2' & (char)message[1] == 'o' & (char)message[2] == 'n') {    // Topic "interruttore" = "2on"
      digitalWrite(RELE_B_cancello2, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 2
      Rele_B_eccitato_cancello2 = true;
      ulTimeReleBCancello2 = millis();
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] != '2' ) {                             // Topic "interruttore" = "stato"
      payload = Cancello2_Topic;
      payload += " RSSI=";
      payload += WiFi.RSSI();
      payload += "dB ";
      payload += " RELE1=";
      if (digitalRead(RELE_A_cancello2) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_A_cancello2) ^ Flag_inversione_RELE == false ) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE_B_cancello2) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_B_cancello2) ^ Flag_inversione_RELE == false) payload += "OFF";
      client.publish(ACK_Topic, (char*) payload.c_str());
      delay(100);
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] == '2' ) {                             // Topic "interruttore" = "stato2"
      payload = Cancello2_Topic;
      payload += " RELE1=";
      if (digitalRead(RELE_A_cancello2) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_A_cancello2) ^ Flag_inversione_RELE == false ) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE_B_cancello2) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_B_cancello2) ^ Flag_inversione_RELE == false) payload += "OFF";
      client.publish(ACK2_Topic, (char*) payload.c_str());
      delay(100);
    }
  }

  // TEMPERATURA 1
  if (String(topic) == Temperatura1_Topic) {                                                                             // se arriva il comando sul topic "temperatura"
    if ((char)message[0] == 'r' & (char)message[1] == 'e' & (char)message[2] == 'a' & (char)message[3] == 'd' ) {       // Topic "temperatura" = "read"
      delay(50);
      sendTemperature();                                                                                                // sendTemperature()
      delay(50);
    }
    if ((char)message[0] == '1' & (char)message[1] == 'o' & (char)message[2] == 'n' ) {                                 // Topic "temperatura" = "1on"
      digitalWrite(RELE_A_temperatura1, 1 ^ Flag_inversione_RELE);                                                        // Accendo il RELE1
      delay(TEMPO_RELE);                                                                                                // Aspetto
    }
    if ((char)message[0] == '1' & (char)message[1] == 'o' & (char)message[2] == 'f' & (char)message[3] == 'f' ) {       // Topic "temperatura" = "1off"
      digitalWrite(RELE_A_temperatura1, 0 ^ Flag_inversione_RELE);                                                        // Spengo il RELE1
      delay(TEMPO_RELE);                                                                                                // Aspetto
    }
    if ((char)message[0] == '2' & (char)message[1] == 'o' & (char)message[2] == 'n' ) {                                 // Topic "temperatura" = "2on"
      digitalWrite(RELE_B_temperatura1, 1 ^ Flag_inversione_RELE);                                                        // Accendo il RELE2
      delay(TEMPO_RELE);                                                                                                // Aspetto
    }
    if ((char)message[0] == '2' & (char)message[1] == 'o' & (char)message[2] == 'f' & (char)message[3] == 'f' ) {       // Topic "temperatura" = "2off"
      digitalWrite(RELE_B_temperatura1, 0 ^ Flag_inversione_RELE);                                                        // Spengo il RELE2
      delay(TEMPO_RELE);                                                                                                // Aspetto
    }
    if (((char)message[0] == 'T' or (char)message[0] == 't') &
        (char)message[1] == '=' ) {                                                                                     // Topic "temperatura" = "T=" o "t="
      String stringtmp = "";
      for (int i = 2; i < length; i++)  {
        stringtmp = stringtmp + (char)message[i];
      }
      Serial.println(stringtmp);
      termostato_temperatura1 = stringtmp.toFloat();
      Serial.println(termostato_temperatura1);
      EEPROM_temperatura1_write();
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] != '2' ) {                           // Topic "Temperatura" = "stato"
      payload = Temperatura1_Topic;
      payload += " RSSI=";
      payload += WiFi.RSSI();
      payload += "dB ";
      payload += " T=";
      char tmp[4];                            // string buffer
      String stringtmp = "";                  //data on buff is copied to this string
      dtostrf(termostato_temperatura1, 4, 1, tmp);
      for (int i = 0; i < sizeof(tmp); i++)  {
        stringtmp += tmp[i];
      }
      payload += stringtmp;
      if (AUTO_temperatura1 == true) payload += " AUTO";
      else payload += " MAN";
      payload += " RELE1=";
      if (digitalRead(RELE_A_temperatura1) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE_A_temperatura1) == false ^ Flag_inversione_RELE) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE_B_temperatura1) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE_B_temperatura1) == false ^ Flag_inversione_RELE) payload += "OFF";
      client.publish(ACK_Topic, (char*) payload.c_str());
      delay(100);
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] == '2' ) {                           // Topic "tapparella" = "stato2"
      delay(50);
      sendTemperature();                                                                                                // sendTemperature()
      delay(50);
      payload = Temperatura1_Topic;
      payload += " T=";
      char tmp[4];                            // string buffer
      String stringtmp = "";                  //data on buff is copied to this string
      dtostrf(termostato_temperatura1, 4, 1, tmp);
      for (int i = 0; i < sizeof(tmp); i++)  {
        stringtmp += tmp[i];
      }
      payload += stringtmp;
      if (AUTO_temperatura1 == true) payload += " AUTO";
      else payload += " MAN";
      payload += " RELE1=";
      if (digitalRead(RELE_A_temperatura1) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE_A_temperatura1) == false ^ Flag_inversione_RELE) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE_B_temperatura1) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE_B_temperatura1) == false ^ Flag_inversione_RELE) payload += "OFF";
      client.publish(ACK_Topic, (char*) payload.c_str());
      delay(100);
    }
    if ((char)message[0] == 'a' & (char)message[1] == 'u' & (char)message[2] == 't' & (char)message[3] == 'o' ) {       // Topic "temperatura" = "auto"
      AUTO_temperatura1 = true;
      EEPROM_temperatura1_write();
    }
    if ((char)message[0] == 'm' & (char)message[1] == 'a' & (char)message[2] == 'n') {                                  // Topic "temperatura" = "man"
      AUTO_temperatura1 = false;
      EEPROM_temperatura1_write();
    }
  }

  if (String(topic) == ACK_Topic) {                                                       // se arriva il comando sul topic "ACK"
    if ((char)message[0] == 'a' & (char)message[1] == 'c' & (char)message[2] == 'k' ) {   // Topic "ack" = "ack"
      Send_ACK();
      delay(100);
    }
  }

  if ((char)message[0] == 'r' & (char)message[1] == 'e' & (char)message[2] == 's' &
      (char)message[3] == 'e' & (char)message[4] == 't' ) {                             // messaggio = "reset"
    EEPROM_clear_reset();
  }

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

  if (NODI_TEMPERATURA > 0) {
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    if (digitalRead(RELE_A_temperatura1) == true ^ Flag_inversione_RELE) {
      display.drawString(128, 0, "ON");
    }
    if (digitalRead(RELE_A_temperatura1) == false ^ Flag_inversione_RELE) {
      display.drawString(128, 0, "OFF");
    }
    if (client.connected()) {
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      display.drawString(0, 0, String(termostato_temperatura1));
      display.setTextAlignment(TEXT_ALIGN_CENTER);
      if (AUTO_temperatura1) {
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
    display.drawString(0, 45, String(t_temperatura1) + " °C");
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.drawString(128, 25, "Umidità:");
    display.drawString(128, 45, String(h_temperatura1) + " %");
    display.display();
  }

  if (TIMER.Wait(TEMPO_REFRESH_CONNESSIONE)) {
    Serial.println("Timer -> check connections");
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("ERRORE DI RETE: WiFi.status() != WL_CONNECTED");
      Serial.println("RIAVVIO L'ESP");
      ESP.restart();
      //setup_wifi();
    }
    if (!client.connected()) {
      Serial.print("ERRORE DI RETE: ");
      Serial.println(client.state());
      reconnect();
    }
  }
  client.loop();
  if (NODI_TAPPARELLA > 0) {
    if (((millis() - ulTimeTapparella1) > TEMPO_tapparella1_MAX) & In_movimento_tapparella1) {               // Tempo tapparella 1 -> spengo!
      Serial.println("Tempo tapparella 1 -> spengo!");
      digitalWrite(RELE_tapparella1_SU, 0 ^ Flag_inversione_RELE);         // RELE SU = 0
      delay(TEMPO_RELE);                                                  // Aspetto
      digitalWrite(RELE_tapparella1_GIU, 0 ^ Flag_inversione_RELE);        // RELE SU = 0
      In_movimento_tapparella1 = false;
    }
    if (((millis() - ulTimeTapparella2) > TEMPO_tapparella2_MAX) & In_movimento_tapparella2) {               // Tempo tapparella 2 -> spengo!
      Serial.println("Tempo tapparella 2 -> spengo!");
      digitalWrite(RELE_tapparella2_SU, 0 ^ Flag_inversione_RELE);         // RELE SU = 0
      delay(TEMPO_RELE);                                                  // Aspetto
      digitalWrite(RELE_tapparella2_GIU, 0 ^ Flag_inversione_RELE);        // RELE SU = 0
      In_movimento_tapparella2 = false;
    }
    if (BottoneTAP1SU.Click(TEMPO_CLICK_ON)) {                                // Bottone SU tapparella 1 premuto!
      Serial.println("Bottone SU tapparella 1 premuto!");
      digitalWrite(RELE_tapparella1_GIU, 0 ^ Flag_inversione_RELE);        // RELE GIU = 0
      delay(TEMPO_RELE);                                                  // Aspetto
      digitalWrite(RELE_tapparella1_SU, 1 ^ Flag_inversione_RELE);         // RELE SU = 1
      In_movimento_tapparella1 = true;
      ulTimeTapparella1 = millis();
    }
    if (BottoneTAP1GIU.Click(TEMPO_CLICK_ON)) {                                // Bottone GIU tapparella 1 premuto!
      Serial.println("Bottone GIU tapparella 1 premuto!");
      digitalWrite(RELE_tapparella1_SU, 0 ^ Flag_inversione_RELE);          // RELE SU = 0
      delay(TEMPO_RELE);                                                   // Aspetto
      digitalWrite(RELE_tapparella1_GIU, 1 ^ Flag_inversione_RELE);         // RELE GIU = 1
      In_movimento_tapparella1 = true;
      ulTimeTapparella1 = millis();
    }
    if (t = BottoneTAP1SU.Keep(TEMPO_CLICK_OFF)) {                            // Bottone SU tapparella 1 rilasciato!
      Serial.println("Bottone SU tapparella 1 rilasciato -> spengo!");
      digitalWrite(RELE_tapparella1_SU, 0 ^ Flag_inversione_RELE);         // RELE SU = 0
      delay(TEMPO_RELE);                                                  // Aspetto
      digitalWrite(RELE_tapparella1_GIU, 0 ^ Flag_inversione_RELE);        // RELE GIU = 0
      In_movimento_tapparella1 = false;
    }
    if (t = BottoneTAP1GIU.Keep(TEMPO_CLICK_OFF)) {                           // Bottone GIU tapparella 1 rilasciato!
      Serial.println("Bottone GIU tapparella 1 rilasciato -> spengo!");
      digitalWrite(RELE_tapparella1_SU, 0 ^ Flag_inversione_RELE);         // RELE SU = 0
      delay(TEMPO_RELE);                                                  // Aspetto
      digitalWrite(RELE_tapparella1_GIU, 0 ^ Flag_inversione_RELE);        // RELE GIU = 0
      In_movimento_tapparella1 = false;
    }
    if (BottoneTAP2SU.Click(TEMPO_CLICK_ON)) {                                // Bottone SU tapparella 2 premuto!
      Serial.println("Bottone SU tapparella 2 premuto!");
      digitalWrite(RELE_tapparella2_GIU, 0 ^ Flag_inversione_RELE);        // RELE GIU = 0
      delay(TEMPO_RELE);                                                  // Aspetto
      digitalWrite(RELE_tapparella2_SU, 1 ^ Flag_inversione_RELE);         // RELE SU = 1
      In_movimento_tapparella2 = true;
      ulTimeTapparella2 = millis();
    }
    if (BottoneTAP2GIU.Click(TEMPO_CLICK_ON)) {                                // Bottone GIU tapparella 2 premuto!
      Serial.println("Bottone GIU tapparella 2 premuto!");
      digitalWrite(RELE_tapparella2_SU, 0 ^ Flag_inversione_RELE);          // RELE SU = 0
      delay(TEMPO_RELE);                                                   // Aspetto
      digitalWrite(RELE_tapparella2_GIU, 1 ^ Flag_inversione_RELE);         // RELE GIU = 1
      In_movimento_tapparella2 = true;
      ulTimeTapparella2 = millis();
    }
    if (t = BottoneTAP2SU.Keep(TEMPO_CLICK_OFF)) {                            // Bottone SU tapparella 2 rilasciato!
      Serial.println("Bottone SU tapparella 2 rilasciato -> spengo!");
      digitalWrite(RELE_tapparella2_SU, 0 ^ Flag_inversione_RELE);         // RELE SU = 0
      delay(TEMPO_RELE);                                                  // Aspetto
      digitalWrite(RELE_tapparella2_GIU, 0 ^ Flag_inversione_RELE);        // RELE GIU = 0
      In_movimento_tapparella2 = false;
    }
    if (t = BottoneTAP2GIU.Keep(TEMPO_CLICK_OFF)) {                           // Bottone GIU tapparella 2 rilasciato!
      Serial.println("Bottone GIU tapparella 2 rilasciato -> spengo!");
      digitalWrite(RELE_tapparella2_SU, 0 ^ Flag_inversione_RELE);         // RELE SU = 0
      delay(TEMPO_RELE);                                                  // Aspetto
      digitalWrite(RELE_tapparella2_GIU, 0 ^ Flag_inversione_RELE);        // RELE GIU = 0
      In_movimento_tapparella2 = false;
    }
  }

  if (NODI_INTERRUTTORE > 0) {
    if (BottoneAINT1.Click(TEMPO_CLICK_ON)) {                                // Bottone 1 premuto!
      Serial.println("Bottone 1 premuto!");
      digitalWrite(RELE_A_interruttore1, not(digitalRead(RELE_A_interruttore1)) ^ Flag_inversione_RELE);
      EEPROM_write_releAinterruttore1();
      delay(TEMPO_RELE);
    }
    if (BottoneBINT1.Click(TEMPO_CLICK_ON)) {                                // Bottone 2 premuto!
      Serial.println("Bottone 2 premuto!");
      digitalWrite(RELE_B_interruttore1, not(digitalRead(RELE_B_interruttore1)) ^ Flag_inversione_RELE);
      EEPROM_write_releBinterruttore1();
      delay(TEMPO_RELE);
    }
    if (t = BottoneAINT1.Keep(TEMPO_CLICK_OFF)) {                            // Bottone 1 rilasciato!
      Serial.println("Bottone 1 rilasciato!");
      if (TIPO_BOTTONE_A_interruttore1 == "interruttore") {
        digitalWrite(RELE_A_interruttore1, not(digitalRead(RELE_A_interruttore1)) ^ Flag_inversione_RELE);
      }
      EEPROM_write_releAinterruttore1();
      delay(TEMPO_RELE);
    }
    if (t = BottoneBINT1.Keep(TEMPO_CLICK_OFF)) {                           // Bottone 2 rilasciato!
      Serial.println("Bottone 2 rilasciato!");
      if (TIPO_BOTTONE_B_interruttore1 == "interruttore") {
        digitalWrite(RELE_B_interruttore1, not(digitalRead(RELE_B_interruttore1)) ^ Flag_inversione_RELE);
      }
      EEPROM_write_releBinterruttore1();
      delay(TEMPO_RELE);
    }
    if (BottoneAINT2.Click(TEMPO_CLICK_ON)) {                                // Bottone 1 premuto!
      Serial.println("Bottone 1 premuto!");
      digitalWrite(RELE_A_interruttore2, not(digitalRead(RELE_A_interruttore2)) ^ Flag_inversione_RELE);
      EEPROM_write_releAinterruttore2();
      delay(TEMPO_RELE);
    }
    if (BottoneBINT2.Click(TEMPO_CLICK_ON)) {                                // Bottone 2 premuto!
      Serial.println("Bottone 2 premuto!");
      digitalWrite(RELE_B_interruttore2, not(digitalRead(RELE_B_interruttore2)) ^ Flag_inversione_RELE);
      EEPROM_write_releBinterruttore2();
      delay(TEMPO_RELE);
    }
    if (t = BottoneAINT2.Keep(TEMPO_CLICK_OFF)) {                            // Bottone 1 rilasciato!
      Serial.println("Bottone 1 rilasciato!");
      if (TIPO_BOTTONE_A_interruttore2 == "interruttore") {
        digitalWrite(RELE_A_interruttore2, not(digitalRead(RELE_A_interruttore2)) ^ Flag_inversione_RELE);
      }
      EEPROM_write_releAinterruttore2();
      delay(TEMPO_RELE);
    }
    if (t = BottoneBINT2.Keep(TEMPO_CLICK_OFF)) {                           // Bottone 2 rilasciato!
      Serial.println("Bottone 2 rilasciato!");
      if (TIPO_BOTTONE_B_interruttore2 == "interruttore") {
        digitalWrite(RELE_B_interruttore2, not(digitalRead(RELE_B_interruttore2)) ^ Flag_inversione_RELE);
      }
      EEPROM_write_releBinterruttore2();
      delay(TEMPO_RELE);
    }
    if (BottoneAINT3.Click(TEMPO_CLICK_ON)) {                                // Bottone 1 premuto!
      Serial.println("Bottone 1 premuto!");
      digitalWrite(RELE_A_interruttore3, not(digitalRead(RELE_A_interruttore3)) ^ Flag_inversione_RELE);
      EEPROM_write_releAinterruttore3();
      delay(TEMPO_RELE);
    }
    if (BottoneBINT3.Click(TEMPO_CLICK_ON)) {                                // Bottone 2 premuto!
      Serial.println("Bottone 2 premuto!");
      digitalWrite(RELE_B_interruttore3, not(digitalRead(RELE_B_interruttore3)) ^ Flag_inversione_RELE);
      EEPROM_write_releBinterruttore3();
      delay(TEMPO_RELE);
    }
    if (t = BottoneAINT3.Keep(TEMPO_CLICK_OFF)) {                            // Bottone 1 rilasciato!
      Serial.println("Bottone 1 rilasciato!");
      if (TIPO_BOTTONE_A_interruttore3 == "interruttore") {
        digitalWrite(RELE_A_interruttore3, not(digitalRead(RELE_A_interruttore3)) ^ Flag_inversione_RELE);
      }
      EEPROM_write_releAinterruttore3();
      delay(TEMPO_RELE);
    }
    if (t = BottoneBINT3.Keep(TEMPO_CLICK_OFF)) {                           // Bottone 2 rilasciato!
      Serial.println("Bottone 2 rilasciato!");
      if (TIPO_BOTTONE_B_interruttore3 == "interruttore") {
        digitalWrite(RELE_B_interruttore3, not(digitalRead(RELE_B_interruttore3)) ^ Flag_inversione_RELE);
      }
      EEPROM_write_releBinterruttore3();
      delay(TEMPO_RELE);
    }
    if (BottoneAINT4.Click(TEMPO_CLICK_ON)) {                                // Bottone 1 premuto!
      Serial.println("Bottone 1 premuto!");
      digitalWrite(RELE_A_interruttore4, not(digitalRead(RELE_A_interruttore4)) ^ Flag_inversione_RELE);
      EEPROM_write_releAinterruttore4();
      delay(TEMPO_RELE);
    }
    if (BottoneBINT4.Click(TEMPO_CLICK_ON)) {                                // Bottone 2 premuto!
      Serial.println("Bottone 2 premuto!");
      digitalWrite(RELE_B_interruttore4, not(digitalRead(RELE_B_interruttore4)) ^ Flag_inversione_RELE);
      EEPROM_write_releBinterruttore4();
      delay(TEMPO_RELE);
    }
    if (t = BottoneAINT4.Keep(TEMPO_CLICK_OFF)) {                            // Bottone 1 rilasciato!
      Serial.println("Bottone 1 rilasciato!");
      if (TIPO_BOTTONE_A_interruttore4 == "interruttore") {
        digitalWrite(RELE_A_interruttore4, not(digitalRead(RELE_A_interruttore4)) ^ Flag_inversione_RELE);
      }
      EEPROM_write_releAinterruttore4();
      delay(TEMPO_RELE);
    }
    if (t = BottoneBINT4.Keep(TEMPO_CLICK_OFF)) {                           // Bottone 2 rilasciato!
      Serial.println("Bottone 2 rilasciato!");
      if (TIPO_BOTTONE_B_interruttore4 == "interruttore") {
        digitalWrite(RELE_B_interruttore4, not(digitalRead(RELE_B_interruttore4)) ^ Flag_inversione_RELE);
      }
      EEPROM_write_releBinterruttore4();
      delay(TEMPO_RELE);
    }
  }

  if (NODI_CANCELLO > 0) {
    if (((millis() - ulTimeReleACancello1) > TEMPO_IMPULSO_cancello) & Rele_A_eccitato_cancello1) {        // Tempo rele 1 -> spengo!
      Serial.println("Tempo rele 1 -> spengo!");
      digitalWrite(RELE_A_cancello1, 0 ^ Flag_inversione_RELE);                     // Spengo il RELE 1
      Rele_A_eccitato_cancello1 = false;
    }
    if (((millis() - ulTimeReleBCancello1) > TEMPO_IMPULSO_cancello) & Rele_B_eccitato_cancello1) {        // Tempo rele 2 -> spengo!
      Serial.println("Tempo rele 2 -> spengo!");
      digitalWrite(RELE_B_cancello1, 0 ^ Flag_inversione_RELE);                     // Spengo il RELE 2
      Rele_B_eccitato_cancello1 = false;
    }
    if (BottoneACAN1.Click(TEMPO_CLICK_ON)) {                                // Bottone 1 premuto!
      Serial.println("Bottone 1 premuto!");
      digitalWrite(RELE_A_cancello1, 1 ^ Flag_inversione_RELE);
      Rele_A_eccitato_cancello1 = true;
      ulTimeReleACancello1 = millis();
    }
    if (BottoneBCAN1.Click(TEMPO_CLICK_ON)) {                                // Bottone 2 premuto!
      Serial.println("Bottone 2 premuto!");
      digitalWrite(RELE_B_cancello1, 1 ^ Flag_inversione_RELE);
      Rele_B_eccitato_cancello1 = true;
      ulTimeReleBCancello1 = millis();
    }
    if (((millis() - ulTimeReleACancello2) > TEMPO_IMPULSO_cancello) & Rele_A_eccitato_cancello2) {        // Tempo rele 1 -> spengo!
      Serial.println("Tempo rele 1 -> spengo!");
      digitalWrite(RELE_A_cancello2, 0 ^ Flag_inversione_RELE);                     // Spengo il RELE 1
      Rele_A_eccitato_cancello2 = false;
    }
    if (((millis() - ulTimeReleBCancello2) > TEMPO_IMPULSO_cancello) & Rele_B_eccitato_cancello2) {        // Tempo rele 2 -> spengo!
      Serial.println("Tempo rele 2 -> spengo!");
      digitalWrite(RELE_B_cancello2, 0 ^ Flag_inversione_RELE);                     // Spengo il RELE 2
      Rele_B_eccitato_cancello2 = false;
    }
    if (BottoneACAN2.Click(TEMPO_CLICK_ON)) {                                // Bottone 1 premuto!
      Serial.println("Bottone 1 premuto!");
      digitalWrite(RELE_A_cancello2, 1 ^ Flag_inversione_RELE);
      Rele_A_eccitato_cancello2 = true;
      ulTimeReleACancello2 = millis();
    }
    if (BottoneBCAN2.Click(TEMPO_CLICK_ON)) {                                // Bottone 2 premuto!
      Serial.println("Bottone 2 premuto!");
      digitalWrite(RELE_B_cancello2, 1 ^ Flag_inversione_RELE);
      Rele_B_eccitato_cancello2 = true;
      ulTimeReleBCancello2 = millis();
    }
  }

  if (NODI_TEMPERATURA > 0) {
    if (TIMER_TERMOSTATO1.Wait(TEMPO_TERMOSTATO)) {
      h_temperatura1 = dht.readHumidity();
      delay(50);
      t_temperatura1 = dht.readTemperature(false);                                                   // Read temperature as Celsius (the default)
      if (isnan(t_temperatura1)) {                                                                         // Check if any reads failed and exit early (to try again).
        //      String payload = macToStr(mac);
        //      payload += " Failed to read from DHT sensor!";
        //      client.publish(ACK_Topic, (char*) payload.c_str());
        Serial.println("Failed to read from DHT sensor!");
        return;
      }
      if (AUTO_temperatura1) {
        if (t_temperatura1 < (termostato_temperatura1 - soglia_temperatura1)) {
          digitalWrite(RELE_A_temperatura1, 1 ^ Flag_inversione_RELE);                                                        // Accendo il RELE1
          delay(TEMPO_RELE);                                                                                                // Aspetto
        }
        if (t_temperatura1 > (termostato_temperatura1 + soglia_temperatura1)) {
          digitalWrite(RELE_A_temperatura1, 0 ^ Flag_inversione_RELE);                                                        // Spengo il RELE1
          delay(TEMPO_RELE);                                                                                                // Aspetto
        }
      }
    }
  }
}


void sendTemperature() {
  h_temperatura1 = dht.readHumidity();
  delay(50);
  t_temperatura1 = dht.readTemperature(false);                           // Read temperature as Celsius (the default)
  delay(50);
  f_temperatura1 = dht.readTemperature(true);                            // Read temperature as Fahrenheit (isFahrenheit = true)
  if (isnan(h_temperatura1) || isnan(t_temperatura1) || isnan(f_temperatura1)) {                   // Check if any reads failed and exit early (to try again).
    String payload = Temperatura1_Topic;
    payload += " Failed to read from DHT sensor!";
    Serial.println("Failed to read from DHT sensor!");
    client.publish(ACK_Topic, (char*) payload.c_str());
    return;
  }
  hi_temperatura1 = dht.computeHeatIndex(t_temperatura1, h_temperatura1, false);                   // Compute heat index in Celsius (isFahreheit = false) float hi
  Serial.print("Humidity: ");
  Serial.print(h_temperatura1);
  Serial.print(" % ");
  Serial.print("Temperature: ");
  Serial.print(t_temperatura1);
  Serial.print(" *C ");
  Serial.print(f_temperatura1);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hi_temperatura1);
  Serial.println(" *C");
  String payload = Temperatura1_Topic;
  payload += " T:";
  payload += t_temperatura1;
  payload += "°C H:";
  payload += h_temperatura1;
  payload += "% HI:";
  payload += hi_temperatura1;
  payload += "°C";
  client.publish(ACK_Topic, (char*) payload.c_str());
}

void EEPROM_read_TEMPO_tapparelle_MAX() {                                              // Leggo dalla EEPROM TEMPO_tapparella1_MAX e TEMPO_tapparella2_MAX
  Serial.print("Reading EEPROM TEMPO_tapparella1_MAX: ");
  TEMPO_tapparella1_MAX = EEPROMReadlong(0);
  Serial.println(TEMPO_tapparella1_MAX);
  Serial.print("Reading EEPROM TEMPO_tapparella2_MAX: ");
  TEMPO_tapparella2_MAX = EEPROMReadlong(4);
  Serial.println(TEMPO_tapparella2_MAX);
  if (TEMPO_tapparella1_MAX < 1000) {
    TEMPO_tapparella1_MAX = 10000;
    EEPROM_write_TEMPO_tapparelle_MAX();
  }
  if (TEMPO_tapparella1_MAX > 60000) {
    TEMPO_tapparella1_MAX = 60000;
    EEPROM_write_TEMPO_tapparelle_MAX();
  }
  if (TEMPO_tapparella2_MAX < 1000) {
    TEMPO_tapparella2_MAX = 10000;
    EEPROM_write_TEMPO_tapparelle_MAX();
  }
  if (TEMPO_tapparella2_MAX > 60000) {
    TEMPO_tapparella2_MAX = 60000;
    EEPROM_write_TEMPO_tapparelle_MAX();
  }
}

void EEPROM_write_TEMPO_tapparelle_MAX() {                                              // Scrivo sulla EEPROM TEMPO_tapparella1_MAX e TEMPO_tapparella2_MAX
  long address = 0;
  Serial.print("Writing EEPROM TEMPO_tapparella1_MAX:");
  Serial.println(TEMPO_tapparella1_MAX);
  EEPROMWritelong(address, TEMPO_tapparella1_MAX);
  address = 4;
  Serial.print("Writing EEPROM TEMPO_tapparella2_MAX:");
  Serial.println(TEMPO_tapparella2_MAX);
  EEPROMWritelong(address, TEMPO_tapparella2_MAX);
}

int EEPROM_read_releAinterruttore1() {                                              // Leggo dalla EEPROM RELE A 1
  int tmp;
  long address = 20;
  Serial.print("Reading EEPROM stato RELE A interruttore 1: ");
  tmp = eepromReadInt(address);
  if (tmp != 0 && tmp != 1) tmp = 0;
  if (tmp == 1) Serial.println("ON");
  if (tmp == 0) Serial.println("OFF");
  return tmp;
}
int EEPROM_read_releBinterruttore1() {                                              // Leggo dalla EEPROM RELE B 1
  int tmp;
  long address = 22;
  Serial.print("Reading EEPROM stato RELE B interruttore 1: ");
  tmp = eepromReadInt(address);
  if (tmp != 0 && tmp != 1) tmp = 0;
  if (tmp == 1) Serial.println("ON");
  if (tmp == 0) Serial.println("OFF");
  return tmp;
}
int EEPROM_read_releAinterruttore2() {                                              // Leggo dalla EEPROM RELE A 2
  int tmp;
  long address = 24;
  Serial.print("Reading EEPROM stato RELE A interruttore 2: ");
  tmp = eepromReadInt(address);
  if (tmp != 0 && tmp != 1) tmp = 0;
  if (tmp == 1) Serial.println("ON");
  if (tmp == 0) Serial.println("OFF");
  return tmp;
}
int EEPROM_read_releBinterruttore2() {                                              // Leggo dalla EEPROM RELE B 2
  int tmp;
  long address = 26;
  Serial.print("Reading EEPROM stato RELE B interruttore 2: ");
  tmp = eepromReadInt(address);
  if (tmp != 0 && tmp != 1) tmp = 0;
  if (tmp == 1) Serial.println("ON");
  if (tmp == 0) Serial.println("OFF");
  return tmp;
}
int EEPROM_read_releAinterruttore3() {                                              // Leggo dalla EEPROM RELE A 3
  int tmp;
  long address = 28;
  Serial.print("Reading EEPROM stato RELE A interruttore 3: ");
  tmp = eepromReadInt(address);
  if (tmp != 0 && tmp != 1) tmp = 0;
  if (tmp == 1) Serial.println("ON");
  if (tmp == 0) Serial.println("OFF");
  return tmp;
}
int EEPROM_read_releBinterruttore3() {                                              // Leggo dalla EEPROM RELE B 3
  int tmp;
  long address = 30;
  Serial.print("Reading EEPROM stato RELE B interruttore 3: ");
  tmp = eepromReadInt(address);
  if (tmp != 0 && tmp != 1) tmp = 0;
  if (tmp == 1) Serial.println("ON");
  if (tmp == 0) Serial.println("OFF");
  return tmp;
}
int EEPROM_read_releAinterruttore4() {                                              // Leggo dalla EEPROM RELE A 4
  int tmp;
  long address = 32;
  Serial.print("Reading EEPROM stato RELE A interruttore 4: ");
  tmp = eepromReadInt(address);
  if (tmp != 0 && tmp != 1) tmp = 0;
  if (tmp == 1) Serial.println("ON");
  if (tmp == 0) Serial.println("OFF");
  return tmp;
}
int EEPROM_read_releBinterruttore4() {                                              // Leggo dalla EEPROM RELE B 4
  int tmp;
  long address = 34;
  Serial.print("Reading EEPROM stato RELE B interruttore 4: ");
  tmp = eepromReadInt(address);
  if (tmp != 0 && tmp != 1) tmp = 0;
  if (tmp == 1) Serial.println("ON");
  if (tmp == 0) Serial.println("OFF");
  return tmp;
}
void EEPROM_write_releAinterruttore1() {
  long address = 20;
  Serial.print("Writing EEPROM stato RELE A interruttore 1:");
  if (digitalRead(RELE_A_interruttore1) ^ Flag_inversione_RELE == true ) {
    int tmp = 1;
    eepromWriteInt(address, tmp );
    Serial.println("ON");
  }
  if (digitalRead(RELE_A_interruttore1) ^ Flag_inversione_RELE == false) {
    int tmp = 0;
    eepromWriteInt(address,  tmp );
    Serial.println("OFF");
  }
  EEPROM.commit();
}
void EEPROM_write_releBinterruttore1() {
  long address = 22;
  Serial.print("Writing EEPROM stato RELE B interruttore 1:");
  if (digitalRead(RELE_B_interruttore1) ^ Flag_inversione_RELE == true ) {
    int tmp = 1;
    eepromWriteInt(address, tmp );
    Serial.println("ON");
  }
  if (digitalRead(RELE_B_interruttore1) ^ Flag_inversione_RELE == false) {
    int tmp = 0;
    eepromWriteInt(address,  tmp );
    Serial.println("OFF");
  }
  EEPROM.commit();
}
void EEPROM_write_releAinterruttore2() {
  long address = 24;
  Serial.print("Writing EEPROM stato RELE A interruttore 2:");
  if (digitalRead(RELE_A_interruttore2) ^ Flag_inversione_RELE == true ) {
    int tmp = 1;
    eepromWriteInt(address, tmp );
    Serial.println("ON");
  }
  if (digitalRead(RELE_A_interruttore2) ^ Flag_inversione_RELE == false) {
    int tmp = 0;
    eepromWriteInt(address,  tmp );
    Serial.println("OFF");
  }
  EEPROM.commit();
}
void EEPROM_write_releBinterruttore2() {
  long address = 26;
  Serial.print("Writing EEPROM stato RELE B interruttore 2:");
  if (digitalRead(RELE_B_interruttore2) ^ Flag_inversione_RELE == true ) {
    int tmp = 1;
    eepromWriteInt(address, tmp );
    Serial.println("ON");
  }
  if (digitalRead(RELE_B_interruttore2) ^ Flag_inversione_RELE == false) {
    int tmp = 0;
    eepromWriteInt(address,  tmp );
    Serial.println("OFF");
  }
  EEPROM.commit();
}
void EEPROM_write_releAinterruttore3() {
  long address = 28;
  Serial.print("Writing EEPROM stato RELE A interruttore 3:");
  if (digitalRead(RELE_A_interruttore3) ^ Flag_inversione_RELE == true ) {
    int tmp = 1;
    eepromWriteInt(address, tmp );
    Serial.println("ON");
  }
  if (digitalRead(RELE_A_interruttore3) ^ Flag_inversione_RELE == false) {
    int tmp = 0;
    eepromWriteInt(address,  tmp );
    Serial.println("OFF");
  }
  EEPROM.commit();
}
void EEPROM_write_releBinterruttore3() {
  long address = 30;
  Serial.print("Writing EEPROM stato RELE B interruttore 3:");
  if (digitalRead(RELE_B_interruttore3) ^ Flag_inversione_RELE == true ) {
    int tmp = 1;
    eepromWriteInt(address, tmp );
    Serial.println("ON");
  }
  if (digitalRead(RELE_B_interruttore3) ^ Flag_inversione_RELE == false) {
    int tmp = 0;
    eepromWriteInt(address,  tmp );
    Serial.println("OFF");
  }
  EEPROM.commit();
}
void EEPROM_write_releAinterruttore4() {
  long address = 32;
  Serial.print("Writing EEPROM stato RELE A interruttore 4:");
  if (digitalRead(RELE_A_interruttore4) ^ Flag_inversione_RELE == true ) {
    int tmp = 1;
    eepromWriteInt(address, tmp );
    Serial.println("ON");
  }
  if (digitalRead(RELE_A_interruttore4) ^ Flag_inversione_RELE == false) {
    int tmp = 0;
    eepromWriteInt(address,  tmp );
    Serial.println("OFF");
  }
  EEPROM.commit();
}
void EEPROM_write_releBinterruttore4() {
  long address = 34;
  Serial.print("Writing EEPROM stato RELE B interruttore 4:");
  if (digitalRead(RELE_B_interruttore4) ^ Flag_inversione_RELE == true ) {
    int tmp = 1;
    eepromWriteInt(address, tmp );
    Serial.println("ON");
  }
  if (digitalRead(RELE_B_interruttore4) ^ Flag_inversione_RELE == false) {
    int tmp = 0;
    eepromWriteInt(address,  tmp );
    Serial.println("OFF");
  }
  EEPROM.commit();
}
void EEPROM_temperatura1_read() {
  int address = 10;
  int tmp;
  Serial.print("Reading EEPROM MAN/AUTO: ");
  tmp = eepromReadInt(address);
  if (tmp == 1) {
    AUTO_temperatura1 = true;
    Serial.println("AUTO");
  }
  if (tmp == 0) {
    AUTO_temperatura1 = false;
    Serial.println("MAN");
  }
  if ((tmp != 1 and tmp != 0) or isnan(tmp)) {
    AUTO_temperatura1 = false;
    Serial.println("MAN");
  }
  address = 12;
  Serial.print("Reading EEPROM termostato: ");
  termostato_temperatura1 = eepromReadFloat(address);
  if (termostato_temperatura1<2 or termostato_temperatura1>30 or isnan(termostato_temperatura1)) termostato_temperatura1 = 18;
  Serial.println(termostato_temperatura1);
}

void EEPROM_temperatura1_write() {
  int address = 10;
  Serial.print("Writing MAN/AUTO:");
  if (AUTO_temperatura1 == true) {
    int tmp = 1;
    eepromWriteInt(address, tmp);
    Serial.println("AUTO");
  }
  if (AUTO_temperatura1 == false) {
    int tmp = 0;
    eepromWriteInt(address, tmp);
    Serial.println("MAN");
  }
  address = 12;
  Serial.print("Writing TERMOSTATO:");
  Serial.println(termostato_temperatura1);
  eepromWriteFloat(address, termostato_temperatura1);
  EEPROM.commit();
}
void EEPROM_clear_reset() {                                              // Cancello la EEPROM e resetto l'ESP
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
  Serial.println("RESET!");
  ESP.restart();
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

//This function will write a 4 byte (32bit) long to the eeprom at
//the specified address to address + 3.
void EEPROMWritelong(int address, long value)
{
  //Decomposition from a long to 4 bytes by using bitshift.
  //One = Most significant -> Four = Least significant byte
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);
  //Write the 4 bytes into the eeprom memory.
  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
  EEPROM.commit();
}

//This function will return a 4 byte (32bit) long from the eeprom
//at the specified address to address + 3.
long EEPROMReadlong(long address)
{
  //Read the 4 bytes from the eeprom memory.
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);
  //Return the recomposed long by using bitshift.
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
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
  EEPROM.commit();
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

