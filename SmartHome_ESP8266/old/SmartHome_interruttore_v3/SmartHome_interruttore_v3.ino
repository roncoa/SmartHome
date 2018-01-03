/*
  SmartHome interruttore V 3.0

  Comandi da inviare al topic "Interruttore_Topic":
  1on           -> comando ON 1
  1off          -> comando OFF 1
  2on           -> comando ON 2
  2off          -> comando OFF 2
  stato         -> restituisce sul topic ACK lo stato dei relè e per quanto tempo la tapparella può restare in azione (in sec.)
  stato2        -> restituisce sul topic ACK2 lo stato dei relè per l'app
  reset         -> pulisce la memoria EEPROM e resetta l'ESC
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <TPush.h>
#include <EEPROM.h>

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
//#define ESP01                                     // Commentare l'hardware non corrente
#define NODEMCU                                   // Commentare l'hardware non corrente
//#define ELECTRODRAGON                             // Commentare l'hardware non corrente

// Tipo nodo
#define TIPO_NODO         "INT"                   // "TAP"->tapparella "TEM"->temperatura "INT"->interruttore "CAN"->cancello

// MQTT topic
#define Interruttore_Topic  "interruttore/xxxxx"        // Interruttore_Topic
#define ACK_Topic           "ack"                 // ACK_Topic
#define ACK2_Topic        "ack2"                  // ACK2_Topic

// TEMPI
#define MAX_RET_WIFI                20            // Indica per quante volte ritenta di connettersi al WIFI
#define MAX_RET_MQTT                3             // Indica per quante volte ritenta di connettersi al server MQTT
#define TEMPO_REFRESH_CONNESSIONE   60000         // Indica il timeout di refresh della connessione (60000=1 min.)
#define TEMPO_CLICK_ON              150           // Indica il tempo minimo di pressione bottone
#define TEMPO_CLICK_OFF             100           // Indica il tempo minimo di rilascio bottone
#define TEMPO_RELE                  200           // Indica il tempo tra una commutazione RELE e la successiva

// GPIO
#if defined(ESP01)
#define Flag_inversione_RELE        1             // Inversione del segnale di uscita RELE       (0=normale - 1=invertito)  
#define Flag_inversione_Status_LED  1             // Inversione del segnale di uscita Status_LED (0=normale - 1=invertito)
#define Status_LED                  4             // BUILTIN_LED : nodemcu->GPIO16 - ESP01->GPIO1(TX) 
#define RELE_1                      0             // RELE 1
#define RELE_2                      2             // RELE 2
#define BOTTONE_1                   1             // Pulsante 1
#define BOTTONE_2                   3             // Pulsante 2
#define PullUp                                    // resistenza PullUp verso vcc
//#define PullDown                                // resistenza PullDown verso gnd
#endif
#if defined(NODEMCU)
#define Flag_inversione_RELE        1             // Inversione del segnale di uscita RELE       (0=normale - 1=invertito)  
#define Flag_inversione_Status_LED  0             // Inversione del segnale di uscita Status_LED (0=normale - 1=invertito)
#define Status_LED                  12            // BUILTIN_LED : nodemcu->GPIO16 - ESP01->GPIO1(TX) 
#define RELE_1                      16            // RELE 1
#define RELE_2                      13            // RELE 2
#define BOTTONE_1                   0             // Pulsante 1
#define BOTTONE_2                   2             // Pulsante 2
#define PullUp                                    // resistenza PullUp verso vcc
//#define PullDown                                // resistenza PullDown verso gnd
#endif
#if defined(ELECTRODRAGON)
#define Flag_inversione_RELE        1             // Inversione del segnale di uscita RELE       (0=normale - 1=invertito)
#define Flag_inversione_Status_LED  1             // Inversione del segnale di uscita Status_LED (0=normale - 1=invertito)
#define Status_LED                  16            // BUILTIN_LED : nodemcu->GPIO16 - ESP01->GPIO1(TX)
#define RELE_1                      12            // RELE 1
#define RELE_2                      13            // RELE 2
#define BOTTONE_1                   0             // Pulsante 1
#define BOTTONE_2                   2             // Pulsante 2
#define PullUp                                    // resistenza PullUp verso vcc
//#define PullDown                                // resistenza PullDown verso gnd
#endif

// VARIABILI
uint8_t       mac[6];                             // MAC ADDRESS
long          TEMPO_MAX = 10000;                  // Indica per quanto tempo la tapparella può restare in azione (in millisecondi)
int           t = 0;                              // Usata per intercettare il rilascio del pulsante
int           lamp = 0;

WiFiClient espClient;
PubSubClient client(espClient);
TPush Bottone1, Bottone2, TIMER, T_LAMPEGGIO;

void setup() {
#if defined(DEBUG)
  Serial.begin(115200);
#endif
  Serial.println();
  Serial.println(" ***** SmartHome interruttore v3 *****");
  Serial.println("Status_LED = GPIO" + String(Status_LED));
  Serial.println("RELE 1     = GPIO" + String(RELE_1));
  Serial.println("RELE 2     = GPIO" + String(RELE_2));
  Serial.println("BOTTONE 1  = GPIO" + String(BOTTONE_1));
  Serial.println("BOTTONE 2  = GPIO" + String(BOTTONE_2));

  // Inizializza EEPROM
  EEPROM.begin(512);
  delay(10);

  // Inizializza Status_LED
  pinMode(Status_LED, OUTPUT);
  digitalWrite(Status_LED, 0 ^ Flag_inversione_Status_LED);

  // Inizializza GPIO
  pinMode(RELE_1, OUTPUT);
  digitalWrite(RELE_1, EEPROM_read_rele1() ^ Flag_inversione_RELE);
  pinMode(RELE_2, OUTPUT);
  digitalWrite(RELE_2, EEPROM_read_rele2() ^ Flag_inversione_RELE);
#if defined(PullDown)
  Bottone1.setUp(BOTTONE_1, HIGH);
  Bottone2.setUp(BOTTONE_2, HIGH);
#endif
#if defined(PullUp)
  Bottone1.setUp(BOTTONE_1, LOW);
  Bottone2.setUp(BOTTONE_2, LOW);
#endif

  // Inizializza connessioni
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  setup_wifi();
  beginOTA(Interruttore_Topic);
  reconnect();

}

void setup_wifi() {
  int i = 0;
  delay(10);
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
  Serial.print("Local  IP   : ");
  Serial.println(WiFi.localIP());
  //  Serial.print("SoftAP IP   : ");
  //  Serial.println(WiFi.softAPIP());
  Serial.print("MAC address : ");
  WiFi.macAddress(mac);
  Serial.println(macToStr(mac));
  digitalWrite(Status_LED, 1 ^ Flag_inversione_Status_LED);
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
      Serial.println("connected!");
      String payload = macToStr(mac);
      payload += " start at ";
      payload += getTime();
      client.publish(ACK_Topic, (char*) payload.c_str());     // Pubblica su ACK_Topic -> MAC + " start at " + time
      delay(50);
      Send_ACK();
      client.subscribe(ACK_Topic);                            // Sottoscrivi ACK_Topic
      client.subscribe(Interruttore_Topic);                     // Sottoscrivi Interruttore_Topic
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
  String payload = macToStr(mac);
  payload += " alive! ";
  payload += TIPO_NODO;
  payload += " ";
  payload += Interruttore_Topic;
  client.publish(ACK_Topic, (char*) payload.c_str());     // Pubblica su ACK_Topic -> MAC + " alive! " TIPO_NODO + " " + Interruttore_Topic
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
  }
  Serial.println();
  // digitalWrite(Status_LED, 0 ^ Flag_inversione_Status_LED);
  String payload;
  if (String(topic) == Interruttore_Topic) {                                              // se arriva il comando sul topic "Interruttore_Topic"
    if ((char)message[0] == '1' & (char)message[1] == 'o' & (char)message[2] == 'n') {    // Topic "interruttore" = "1on"
      digitalWrite(RELE_1, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 1
      delay(TEMPO_RELE);                                                                  // Aspetto
      EEPROM_write_rele1();
    }
    if ((char)message[0] == '1' & (char)message[1] == 'o' & (char)message[2] == 'f' &
        (char)message[3] == 'f') {                                                        // Topic "interruttore" = "1off"
      digitalWrite(RELE_1, 0 ^ Flag_inversione_RELE);                                     // Spengo il RELE 1
      delay(TEMPO_RELE);                                                                  // Aspetto
      EEPROM_write_rele1();
    }
    if ((char)message[0] == '2' & (char)message[1] == 'o' & (char)message[2] == 'n') {    // Topic "interruttore" = "2on"
      digitalWrite(RELE_2, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 2
      delay(TEMPO_RELE);                                                                  // Aspetto
      EEPROM_write_rele2();
    }
    if ((char)message[0] == '2' & (char)message[1] == 'o' & (char)message[2] == 'f' &
        (char)message[3] == 'f') {                                                        // Topic "interruttore" = "2off"
      digitalWrite(RELE_2, 0 ^ Flag_inversione_RELE);                                     // Spengo il RELE 2
      delay(TEMPO_RELE);                                                                  // Aspetto
      EEPROM_write_rele2();
    }

    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] != '2' ) {                             // Topic "interruttore" = "stato"
      payload = macToStr(mac);
      payload += " RSSI=";
      payload += WiFi.RSSI();
      payload += "dB ";
      payload += " RELE1=";
      if (digitalRead(RELE_1) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_1) ^ Flag_inversione_RELE == false ) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE_2) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_2) ^ Flag_inversione_RELE == false) payload += "OFF";
      client.publish(ACK_Topic, (char*) payload.c_str());
      delay(100);
    }

    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] == '2' ) {                             // Topic "interruttore" = "stato2"
      payload = macToStr(mac);
      payload += " RELE1=";
      if (digitalRead(RELE_1) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_1) ^ Flag_inversione_RELE == false ) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE_2) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_2) ^ Flag_inversione_RELE == false) payload += "OFF";
      client.publish(ACK2_Topic, (char*) payload.c_str());
      delay(100);
    }




    if ((char)message[0] == 'r' & (char)message[1] == 'e' & (char)message[2] == 's' &
        (char)message[3] == 'e' & (char)message[4] == 't' ) {                             // Topic "interruttore" = "reset"
      EEPROM_clear_reset();
    }
  }
  if (String(topic) == ACK_Topic) {                                                       // se arriva il comando sul topic "ACK"
    if ((char)message[0] == 'a' & (char)message[1] == 'c' & (char)message[2] == 'k' ) {   // Topic "ack" = "ack"
      Send_ACK();
      delay(100);
    }
  }
  // digitalWrite(Status_LED, 1 ^ Flag_inversione_Status_LED);
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
  if (Bottone1.Click(TEMPO_CLICK_ON)) {                                // Bottone 1 premuto!
    Serial.println("Bottone 1 premuto!");
    digitalWrite(RELE_1, digitalRead(RELE_1) ^ Flag_inversione_RELE);
    EEPROM_write_rele1();
    delay(TEMPO_RELE);
  }
  if (Bottone2.Click(TEMPO_CLICK_ON)) {                                // Bottone 2 premuto!
    Serial.println("Bottone 2 premuto!");
    digitalWrite(RELE_2, digitalRead(RELE_2) ^ Flag_inversione_RELE);
    EEPROM_write_rele2();
    delay(TEMPO_RELE);
  }
  if (t = Bottone1.Keep(TEMPO_CLICK_OFF)) {                            // Bottone 1 rilasciato!
    Serial.println("Bottone 1 rilasciato!");
    digitalWrite(RELE_1, digitalRead(RELE_1) ^ Flag_inversione_RELE);
    EEPROM_write_rele1();
    delay(TEMPO_RELE);
  }
  if (t = Bottone2.Keep(TEMPO_CLICK_OFF)) {                           // Bottone 2 rilasciato!
    Serial.println("Bottone 2 rilasciato!");
    digitalWrite(RELE_1, digitalRead(RELE_1) ^ Flag_inversione_RELE);
    EEPROM_write_rele1();
    delay(TEMPO_RELE);
  }
  client.loop();
}

/*
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

*/
int EEPROM_read_rele1() {                                              // Leggo dalla EEPROM RELE 1
  int tmp;
  long address = 20;
  Serial.print("Reading EEPROM RELE 1 : ");
  tmp = eepromReadInt(address);
  if (tmp != 0 && tmp != 1) tmp = 0;
  if (tmp == 1) Serial.println("ON");
  if (tmp == 0) Serial.println("OFF");
  return tmp;
}

void EEPROM_write_rele1() {                                              // Scrivo sulla EEPROM RELE 1
  long address = 20;
  Serial.print("Writing EEPROM RELE 1 :");
  if (digitalRead(RELE_1) ^ Flag_inversione_RELE == true ) {
    int tmp = 1;
    eepromWriteInt(address, tmp );
    Serial.println("ON");
  }
  if (digitalRead(RELE_1) ^ Flag_inversione_RELE == false) {
    int tmp = 0;
    eepromWriteInt(address,  tmp );
    Serial.println("OFF");
  }
  EEPROM.commit();
}

int EEPROM_read_rele2() {                                              // Leggo dalla EEPROM RELE 2
  int tmp;
  int address = 22;
  Serial.print("Reading EEPROM RELE 2 : ");
  tmp = eepromReadInt(address);
  if (tmp != 0 && tmp != 1) tmp = 0;
  if (tmp == 1) Serial.println("ON");
  if (tmp == 0) Serial.println("OFF");
  return tmp;
}

void EEPROM_write_rele2() {                                              // Scrivo sulla EEPROM RELE 2
  int address = 22;
  Serial.print("Writing EEPROM RELE 2 :");
  if (digitalRead(RELE_2) ^ Flag_inversione_RELE == true) {
    int tmp = 1;
    eepromWriteInt(address,  tmp );
    Serial.println("ON");
  }
  if (digitalRead(RELE_2) ^ Flag_inversione_RELE == false) {
    int tmp = 0;
    eepromWriteInt(address,  tmp );
    Serial.println("OFF");
  }
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
