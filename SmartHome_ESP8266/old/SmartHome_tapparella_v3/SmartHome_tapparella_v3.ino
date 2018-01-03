/*
  SmartHome tapparella V 3.0

  Comandi da inviare al topic "Tapparella_Topic":
  su            -> comando SU
  giu           -> comando GIU
  stop          -> comando STOP
  t=XX o T=XX   -> XX Indica per quanto tempo la tapparella può restare in azione (in sec.)
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
#define DEBUG                                      // Commentare questa riga per disabilitare il SERIAL DEBUG

// HARDWARE
//#define ESP01                                     // Commentare l'hardware non corrente
//#define NODEMCU                                   // Commentare l'hardware non corrente
#define ELECTRODRAGON                               // Commentare l'hardware non corrente

// Tipo nodo
#define TIPO_NODO         "TAP"                   // "TAP"->tapparella "TEM"->temperatura "INT"->interruttore "CAN"->cancello

// MQTT topic
#define Tapparella_Topic  "tapparella/XXXXX"        // Tapparella_Topic (non usare underscore "_")
#define ACK_Topic         "ack"                   // ACK_Topic
#define ACK2_Topic        "ack2"                  // ACK2_Topic


// TEMPI
#define MAX_RET_WIFI                15            // Indica per quante volte ritenta di connettersi al WIFI (20)
#define MAX_RET_MQTT                2             // Indica per quante volte ritenta di connettersi al server MQTT (3)
#define TEMPO_REFRESH_CONNESSIONE   60000         // Indica il timeout di refresh della connessione (60000=1 min.)
#define TEMPO_CLICK_ON              250           // Indica il tempo minimo di pressione bottone (150)
#define TEMPO_CLICK_OFF             250           // Indica il tempo minimo di rilascio bottone (100)
#define TEMPO_RELE                  100           // Indica il tempo tra una commutazione RELE e la successiva (200)

// GPIO
#if defined(ESP01)
#define Flag_inversione_RELE        1             // Inversione del segnale di uscita RELE       (0=normale - 1=invertito)  
#define Flag_inversione_Status_LED  1             // Inversione del segnale di uscita Status_LED (0=normale - 1=invertito)
#define Status_LED                  4             // BUILTIN_LED : nodemcu->GPIO16 - ESP01->GPIO1(TX) 
#define RELE_tapparella_SU          0             // RELE abilitazione
#define RELE_tapparella_GIU         2             // RELE SU/GIU
#define BOTTONE_tapparella_SU       1             // Pulsante tapparella SU
#define BOTTONE_tapparella_GIU      3             // Pulsante tapparella GIU
#define PullUp                                    // resistenza PullUp verso vcc
//#define PullDown                                // resistenza PullDown verso gnd
#endif

#if defined(NODEMCU)
#define Flag_inversione_RELE        1             // Inversione del segnale di uscita RELE       (0=normale - 1=invertito)  
#define Flag_inversione_Status_LED  1             // Inversione del segnale di uscita Status_LED (0=normale - 1=invertito)
#define Status_LED                  16            // BUILTIN_LED : nodemcu->GPIO16 - ESP01->GPIO1(TX) 
#define RELE_tapparella_SU          12            // RELE abilitazione
#define RELE_tapparella_GIU         13            // RELE SU/GIU
#define BOTTONE_tapparella_SU       4             // Pulsante tapparella SU
#define BOTTONE_tapparella_GIU      5             // Pulsante tapparella GIU
#define PullUp                                    // resistenza PullUp verso vcc
//#define PullDown                                // resistenza PullDown verso gnd
#endif

#if defined(ELECTRODRAGON)
#define Flag_inversione_RELE        0             // Inversione del segnale di uscita RELE       (0=normale - 1=invertito)
#define Flag_inversione_Status_LED  0             // Inversione del segnale di uscita Status_LED (0=normale - 1=invertito)
#define Status_LED                  16            // BUILTIN_LED : nodemcu->GPIO16 - ESP01->GPIO1(TX)
#define RELE_tapparella_SU          12            // RELE abilitazione
#define RELE_tapparella_GIU         13            // RELE SU/GIU
#define BOTTONE_tapparella_SU       4             // Pulsante tapparella SU
#define BOTTONE_tapparella_GIU      5             // Pulsante tapparella GIU
#define PullUp                                    // resistenza PullUp verso vcc
//#define PullDown                                // resistenza PullDown verso gnd
#endif

// VARIABILI
uint8_t       mac[6];                             // MAC ADDRESS
long          TEMPO_MAX = 10000;                  // Indica per quanto tempo la tapparella può restare in azione (in millisecondi)
boolean       In_movimento = false;               // Indica se la tapparella e' in movimento
int           t = 0;                              // Usata per intercettare il rilascio del pulsante
int           lamp = 0;
unsigned long ulTime;

WiFiClient espClient;
PubSubClient client(espClient);
TPush BottoneSU, BottoneGIU, TIMER, T_LAMPEGGIO;

void setup() {
#if defined(DEBUG)
  Serial.begin(115200);
#endif
  Serial.println();
  Serial.println(" ***** SmartHome tapparella v3 *****");
  Serial.println("Nodo: " + String(Tapparella_Topic));
  Serial.println("Status LED             = GPIO" + String(Status_LED));
  Serial.println("RELE    tapparella SU  = GPIO" + String(RELE_tapparella_SU));
  Serial.println("RELE    tapparella GIU = GPIO" + String(RELE_tapparella_GIU));
  Serial.println("BOTTONE tapparella SU  = GPIO" + String(BOTTONE_tapparella_SU));
  Serial.println("BOTTONE tapparella GIU = GPIO" + String(BOTTONE_tapparella_GIU));

  // Inizializza EEPROM
  EEPROM.begin(512);
  delay(10);

  // Inizializza Status_LED
  pinMode(Status_LED, OUTPUT);
  digitalWrite(Status_LED, 0 ^ Flag_inversione_Status_LED);

  // Inizializza GPIO
  pinMode(RELE_tapparella_SU, OUTPUT);
  digitalWrite(RELE_tapparella_SU, 0 ^ Flag_inversione_RELE);
  pinMode(RELE_tapparella_GIU, OUTPUT);
  digitalWrite(RELE_tapparella_GIU, 0 ^ Flag_inversione_RELE);
#if defined(PullDown)
  BottoneSU.setUp(BOTTONE_tapparella_SU, HIGH);
  BottoneGIU.setUp(BOTTONE_tapparella_GIU, HIGH);
#endif
#if defined(PullUp)
  BottoneSU.setUp(BOTTONE_tapparella_SU, LOW);
  BottoneGIU.setUp(BOTTONE_tapparella_GIU, LOW);
#endif

  // Inizializza connessioni
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  setup_wifi();
  beginOTA(Tapparella_Topic);
  reconnect();

  // Leggi TEMPO_MAX dalla EEPROM
  EEPROM_read_tempo();
}

void setup_wifi() {
  int i = 0;
  delay(10);


  Serial.println("** Scan Networks **");
  byte numSsid = WiFi.scanNetworks();

  Serial.print("SSID List:");
  Serial.println(numSsid);


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
      delay(50);
      Send_ACK();
      client.subscribe(ACK_Topic);                            // Sottoscrivi ACK_Topic
      client.subscribe(Tapparella_Topic);                     // Sottoscrivi Tapparella_Topic
    }
    else {
      Serial.print("MQTT failed, rc = ");
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
  payload += Tapparella_Topic;
  client.publish(ACK_Topic, (char*) payload.c_str());     // Pubblica su ACK_Topic -> MAC + " alive! " TIPO_NODO + " " + Tapparella_Topic
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
  if (String(topic) == Tapparella_Topic) {                                              // se arriva il comando sul topic "Tapparella_Topic"
    if ((char)message[0] == 's' & (char)message[1] == 'u' ) {                           // Topic "tapparella" = "su"
      digitalWrite(RELE_tapparella_GIU, 0 ^ Flag_inversione_RELE);                      // RELE GIU = 0
      delay(TEMPO_RELE);                                                                // Aspetto
      digitalWrite(RELE_tapparella_SU, 1 ^ Flag_inversione_RELE);                       // RELE SU = 1
      In_movimento = true;
      ulTime = millis();
    }
    if ((char)message[0] == 'g' & (char)message[1] == 'i' & (char)message[2] == 'u') {  // Topic "tapparella" = "giu"
      digitalWrite(RELE_tapparella_SU, 0 ^ Flag_inversione_RELE);                       // RELE SU = 0
      delay(TEMPO_RELE);                                                                // Aspetto
      digitalWrite(RELE_tapparella_GIU, 1 ^ Flag_inversione_RELE);                      // RELE GIU = 1
      In_movimento = true;
      ulTime = millis();
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'o' &
        (char)message[3] == 'p' ) {                                                     // Topic "tapparella" = "stop"
      digitalWrite(RELE_tapparella_SU, 0 ^ Flag_inversione_RELE);                       // RELE SU = 0
      delay(TEMPO_RELE);                                                                // Aspetto
      digitalWrite(RELE_tapparella_GIU, 0 ^ Flag_inversione_RELE);                      // RELE GIU = 0
      In_movimento = false;
    }
    if (((char)message[0] == 'T' or (char)message[0] == 't') &
        (char)message[1] == '=' ) {                                                     // Topic "tapparella" = "T=" o "t="
      String stringtmp = "";
      for (int i = 2; i < length; i++)  {
        stringtmp = stringtmp + (char)message[i];
      }
      char buf[stringtmp.length()];
      stringtmp.toCharArray(buf, stringtmp.length() + 1);
      TEMPO_MAX = atol(buf);
      if (TEMPO_MAX < 1) {
        TEMPO_MAX = 10;
      }
      if (TEMPO_MAX > 120) {
        TEMPO_MAX = 120;
      }
      TEMPO_MAX = TEMPO_MAX * 1000;
      EEPROM_write_tempo();
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] != '2' ) {                           // Topic "tapparella" = "stato"
      payload = macToStr(mac);
      payload += " RSSI=";
      payload += WiFi.RSSI();
      payload += "dB ";
      payload += " T=";
      payload += TEMPO_MAX / 1000;
      payload += "sec. SU=";
      if (digitalRead(RELE_tapparella_SU) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE_tapparella_SU) == false ^ Flag_inversione_RELE) payload += "OFF";
      payload += " GIU=";
      if (digitalRead(RELE_tapparella_GIU) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE_tapparella_GIU) == false ^ Flag_inversione_RELE) payload += "OFF";
      client.publish(ACK_Topic, (char*) payload.c_str());
      delay(100);
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] == '2' ) {                           // Topic "tapparella" = "stato2"
      payload = macToStr(mac);
      payload += " SU=";
      if (digitalRead(RELE_tapparella_SU) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE_tapparella_SU) == false ^ Flag_inversione_RELE) payload += "OFF";
      payload += " GIU=";
      if (digitalRead(RELE_tapparella_GIU) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE_tapparella_GIU) == false ^ Flag_inversione_RELE) payload += "OFF";
      client.publish(ACK2_Topic, (char*) payload.c_str());
      delay(100);
    }



    if ((char)message[0] == 'r' & (char)message[1] == 'e' & (char)message[2] == 's' &
        (char)message[3] == 'e' & (char)message[4] == 't' ) {                           // Topic "temperatura" = "reset"
      EEPROM_clear_reset();
    }
  }
  if (String(topic) == ACK_Topic) {                                                     // se arriva il comando sul topic "ACK"
    if ((char)message[0] == 'a' & (char)message[1] == 'c' & (char)message[2] == 'k' ) { // Topic "ack" = "ack"
      Send_ACK();
      delay(100);
    }
  }
  //digitalWrite(Status_LED, 1 ^ Flag_inversione_Status_LED);
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

  if (((millis() - ulTime) > TEMPO_MAX) & In_movimento) {               // Tempo -> spengo!
    Serial.println("Tempo -> spengo!");
    digitalWrite(RELE_tapparella_SU, 0 ^ Flag_inversione_RELE);         // RELE SU = 0
    delay(TEMPO_RELE);                                                  // Aspetto
    digitalWrite(RELE_tapparella_GIU, 0 ^ Flag_inversione_RELE);        // RELE SU = 0
    In_movimento = false;
  }

  if (BottoneSU.Click(TEMPO_CLICK_ON)) {                                // Bottone SU premuto!
    Serial.println("Bottone SU premuto!");
    digitalWrite(RELE_tapparella_GIU, 0 ^ Flag_inversione_RELE);        // RELE GIU = 0
    delay(TEMPO_RELE);                                                  // Aspetto
    digitalWrite(RELE_tapparella_SU, 1 ^ Flag_inversione_RELE);         // RELE SU = 1
    In_movimento = true;
    ulTime = millis();
  }
  if (BottoneGIU.Click(TEMPO_CLICK_ON)) {                                // Bottone GIU premuto!
    Serial.println("Bottone GIU premuto!");
    digitalWrite(RELE_tapparella_SU, 0 ^ Flag_inversione_RELE);          // RELE SU = 0
    delay(TEMPO_RELE);                                                   // Aspetto
    digitalWrite(RELE_tapparella_GIU, 1 ^ Flag_inversione_RELE);         // RELE GIU = 1
    In_movimento = true;
    ulTime = millis();
  }
  if (t = BottoneSU.Keep(TEMPO_CLICK_OFF)) {                            // Bottone SU rilasciato!
    Serial.println("BottoneSU OFF -> spengo!");
    digitalWrite(RELE_tapparella_SU, 0 ^ Flag_inversione_RELE);         // RELE SU = 0
    delay(TEMPO_RELE);                                                  // Aspetto
    digitalWrite(RELE_tapparella_GIU, 0 ^ Flag_inversione_RELE);        // RELE GIU = 0
    In_movimento = false;
  }
  if (t = BottoneGIU.Keep(TEMPO_CLICK_OFF)) {                           // Bottone GIU rilasciato!
    Serial.println("BottoneGIU OFF -> spengo!");
    digitalWrite(RELE_tapparella_SU, 0 ^ Flag_inversione_RELE);         // RELE SU = 0
    delay(TEMPO_RELE);                                                  // Aspetto
    digitalWrite(RELE_tapparella_GIU, 0 ^ Flag_inversione_RELE);        // RELE GIU = 0
    In_movimento = false;
  }
  client.loop();
}

void EEPROM_read_tempo() {                                              // Leggo dalla EEPROM TEMPO_MAX
  Serial.print("Reading EEPROM TEMPO_MAX: ");
  TEMPO_MAX = EEPROMReadlong(0);
  Serial.println(TEMPO_MAX);
  if (TEMPO_MAX < 1000) {
    TEMPO_MAX = 10000;
    EEPROM_write_tempo();
  }
  if (TEMPO_MAX > 60000) {
    TEMPO_MAX = 60000;
    EEPROM_write_tempo();
  }
}

void EEPROM_write_tempo() {                                              // Scrivo sulla EEPROM TEMPO_MAX
  long address = 0;
  Serial.print("Writing EEPROM TEMPO_MAX:");
  Serial.println(TEMPO_MAX);
  EEPROMWritelong(address, TEMPO_MAX);
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
