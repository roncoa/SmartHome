#include "DHT.h"
#include "SSD1306.h"

TPush Termostato1_timer;

long EEPROM_Termostato1_auto = 100;
long EEPROM_Termostato1_termostato = 102;

float         Termostato1_t;                         // Variabile Temperature Celsius
float         Termostato1_h;                         // Variabile Humidity
float         Termostato1_f;                         // Variabile Temperature Fahrenheit
float         Termostato1_hi;                        // Variabile Heat index in Celsius (isFahreheit = false)
float         Termostato1_termostato = 18.00;        // Variabile temperatura
boolean       Termostato1_AUTO = false;              // Variabile MAN/AUTO

DHT dht(Termostato1_GPIO_DHT, DHTTYPE, 15);
SSD1306 display(Termostato1_I2C_DISPLAY_ADDRESS, Termostato1_GPIO_SDA, Termostato1_GPIO_SDC);

void setup_Termostato() {
  for (int i = 0; i < Termostato_nodi; i++) {
    Debug_MSG_LN("- Temperatura " + String(i + 1) + " : " + String(Termostato1_Topic));
    Debug_MSG_LN("RELE temperatura 1      = GPIO" + String(Termostato1_GPIO_rele));
    Debug_MSG_LN("DTH22 temperatura 1       = GPIO" + String(Termostato1_GPIO_DHT));
    Debug_MSG_LN("SSD1306 ADDRESS temperatura 1 = 0x"   + String(Termostato1_I2C_DISPLAY_ADDRESS, HEX));
    Debug_MSG_LN("SSD1306 SDA temperatura 1 = GPIO" + String(Termostato1_GPIO_SDA));
    Debug_MSG_LN("SSD1306 SDC temperatura 1 = GPIO" + String(Termostato1_GPIO_SDC));
    pinMode(Termostato1_GPIO_rele, OUTPUT);
    digitalWrite(Termostato1_GPIO_rele, 0 ^ Flag_inversione_RELE);
    dht.begin();
    // Inizializzazione il display
    Debug_MSG_LN("Inizializzazione il display");
    display.init();
    display.flipScreenVertically();
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    String s;
    s = "SmartHome v";
    s += Versione;
    display.drawString(0, 0, s);
    display.drawString(0, 45, Termostato1_Topic);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 20, "by roncoa@gmail.com");
    display.display();

    EEPROM_Termostato1_read();
  }
}


void ACK_Termostato() {
  String payload;
  for (int i = 0; i < Termostato_nodi; i++) {
    payload = macToStr(mac);
    payload += " alive! TEM ";
    payload += Termostato1_Topic;
    client.publish(ACK_Topic, (char*) payload.c_str());
    delay(250);
  }
}
void Online_ACK_Termostato() {
  String payload;
  for (int i = 0; i < Termostato_nodi; i++) {
    payload = "online";
    client.publish(Termostato1_Topic_ACK, (char*) payload.c_str());
    delay(250);
  }

}
void Subscribe_Termostato() {
  for (int i = 0; i < Termostato_nodi; i++) {
    client.subscribe(Termostato1_Topic);                  // Sottoscrivi Temperatura_Topic
    delay(250);
    Debug_MSG_LN("MQTT subscribe " + String(Termostato1_Topic));
  }
}

void callback_Termostato(char* topic, byte * message, unsigned int length) {
  String payload;
  if (String(topic) == Termostato1_Topic) {                                                                             // se arriva il comando sul topic "Termostato1_Topic"
    if ((char)message[0] == 'r' & (char)message[1] == 'e' & (char)message[2] == 'a' & (char)message[3] == 'd' ) {       // Topic "Termostato1_Topic" = "read"
      delay(50);
      sendTemperature();                                                                                                // sendTemperature()
      delay(50);
    }
    if ((char)message[0] == 'o' & (char)message[1] == 'n') {                                                     // Topic "Termostato1_Topic" = "on"
      digitalWrite(Termostato1_GPIO_rele, 1 ^ Flag_inversione_RELE);                                                        // Accendo il RELE
      //  delay(TEMPO_RELE);                                                                                                // Aspetto
    }
    if ((char)message[0] == 'o' & (char)message[1] == 'f' & (char)message[2] == 'f') {                     // Topic "Termostato1_Topic" = "off"
      digitalWrite(Termostato1_GPIO_rele, 0 ^ Flag_inversione_RELE);                                                        // Spengo il RELE
      //  delay(TEMPO_RELE);                                                                                                // Aspetto
    }
    if (((char)message[0] == 'T' or (char)message[0] == 't') &
        (char)message[1] == '=' ) {                                                                                     // Topic "Termostato1_Topic" = "T=" o "t="
      String stringtmp = "";
      for (int i = 2; i < length; i++)  {
        stringtmp = stringtmp + (char)message[i];
      }
      Debug_MSG_LN(stringtmp);
      Termostato1_termostato = stringtmp.toFloat();
      Debug_MSG_LN((String) Termostato1_termostato);
      EEPROM_Termostato1_write();
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o') {                                                  // Topic "Termostato1_Topic" = "stato"
      payload = Termostato1_Topic;
      payload += " RSSI=";
      payload += WiFi.RSSI();
      payload += "dB ";
      payload += " T=";
      char tmp[4];                            // string buffer
      String stringtmp = "";                  //data on buff is copied to this string
      dtostrf(Termostato1_termostato, 4, 1, tmp);
      for (int i = 0; i < sizeof(tmp); i++)  {
        stringtmp += tmp[i];
      }
      payload += stringtmp;
      if (Termostato1_AUTO == true) payload += " AUTO";
      else payload += " MAN";
      payload += " RELE=";
      if (digitalRead(Termostato1_GPIO_rele) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(Termostato1_GPIO_rele) == false ^ Flag_inversione_RELE) payload += "OFF";
      client.publish(ACK_Topic, (char*) payload.c_str());
      delay(100);
    }
    if ((char)message[0] == 'a' & (char)message[1] == 'u' & (char)message[2] == 't' & (char)message[3] == 'o' ) {       // Topic "Termostato1_Topic" = "auto"
      Termostato1_AUTO = true;
      EEPROM_Termostato1_write();
    }
    if ((char)message[0] == 'm' & (char)message[1] == 'a' & (char)message[2] == 'n') {                                  // Topic "Termostato1_Topic" = "man"
      Termostato1_AUTO = false;
      EEPROM_Termostato1_write();
    }
  }
}

void sendTemperature() {
  Termostato1_h = dht.readHumidity();
  delay(50);
  Termostato1_t = dht.readTemperature(false);                           // Read temperature as Celsius (the default)
  delay(50);
  Termostato1_f = dht.readTemperature(true);                            // Read temperature as Fahrenheit (isFahrenheit = true)
  if (isnan(Termostato1_h) || isnan(Termostato1_t) || isnan(Termostato1_f)) {                   // Check if any reads failed and exit early (to try again).
    String payload = Termostato1_Topic;
    payload += " Failed to read from DHT sensor!";
    Debug_MSG_LN("Failed to read from DHT sensor!");
    client.publish(ACK_Topic, (char*) payload.c_str());
    return;
  }
  Termostato1_hi = dht.computeHeatIndex(Termostato1_t, Termostato1_h, false);                   // Compute heat index in Celsius (isFahreheit = false) float hi
  Debug_MSG("Humidity: ");
  Debug_MSG((String) Termostato1_h);
  Debug_MSG(" % ");
  Debug_MSG("Temperature: ");
  Debug_MSG((String) Termostato1_t);
  Debug_MSG(" *C ");
  Debug_MSG((String) Termostato1_f);
  Debug_MSG(" *F\t");
  Debug_MSG("Heat index: ");
  Debug_MSG((String) Termostato1_hi);
  Debug_MSG_LN(" *C");
  String payload = Termostato1_Topic;
  payload += " T:";
  payload += Termostato1_t;
  payload += "°C H:";
  payload += Termostato1_h;
  payload += "% HI:";
  payload += Termostato1_hi;
  payload += "°C";
  client.publish(ACK_Topic, (char*) payload.c_str());
}
void loop_Termostato() {
  for (int i = 0; i < Termostato_nodi; i++) {
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    if (digitalRead(Termostato1_GPIO_rele) == true ^ Flag_inversione_RELE) {
      display.drawString(128, 0, "ON");
    }
    if (digitalRead(Termostato1_GPIO_rele) == false ^ Flag_inversione_RELE) {
      display.drawString(128, 0, "OFF");
    }
    if (client.connected()) {
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      display.drawString(0, 0, String(Termostato1_termostato));
      display.setTextAlignment(TEXT_ALIGN_CENTER);
      if (Termostato1_AUTO) {
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
    display.drawString(0, 45, String(Termostato1_t) + " °C");
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.drawString(128, 25, "Umidità:");
    display.drawString(128, 45, String(Termostato1_h) + " %");
    display.display();

    if (Termostato1_timer.Wait(Termostato_TEMPO_TERMOSTATO)) {
      Termostato1_h = dht.readHumidity();
      delay(50);
      Termostato1_t = dht.readTemperature(false);                                                   // Read temperature as Celsius (the default)
      if (isnan(Termostato1_t)) {                                                                         // Check if any reads failed and exit early (to try again).
        //      String payload = macToStr(mac);
        //      payload += " Failed to read from DHT sensor!";
        //      client.publish(ACK_Topic, (char*) payload.c_str());
        Debug_MSG_LN("Failed to read from DHT sensor!");
        return;
      }
      if (Termostato1_AUTO) {
        if (Termostato1_t < (Termostato1_termostato - soglia_Termostato1)) {
          digitalWrite(Termostato1_GPIO_rele, 1 ^ Flag_inversione_RELE);                                                        // Accendo il RELE
          // delay(TEMPO_RELE);                                                                                                // Aspetto
        }
        if (Termostato1_t > (Termostato1_termostato + soglia_Termostato1)) {
          digitalWrite(Termostato1_GPIO_rele, 0 ^ Flag_inversione_RELE);                                                        // Spengo il RELE
          // delay(TEMPO_RELE);                                                                                                // Aspetto
        }
      }
    }
  }
}

void EEPROM_Termostato1_write() {
  Debug_MSG("Writing MAN/AUTO:");
  if (Termostato1_AUTO == true) {
    int tmp = 1;
    eepromWriteInt(EEPROM_Termostato1_auto, tmp);
    Debug_MSG_LN("AUTO");
  }
  if (Termostato1_AUTO == false) {
    int tmp = 0;
    eepromWriteInt(EEPROM_Termostato1_auto, tmp);
    Debug_MSG_LN("MAN");
  }
  Debug_MSG("Writing TERMOSTATO:");
  Debug_MSG_LN((String) Termostato1_termostato);
  eepromWriteFloat(EEPROM_Termostato1_termostato, Termostato1_termostato);
  EEPROM.commit();
}
void EEPROM_Termostato1_read() {
  int tmp;
  Debug_MSG("Reading EEPROM MAN/AUTO: ");
  tmp = eepromReadInt(EEPROM_Termostato1_auto);
  if (tmp == 1) {
    Termostato1_AUTO = true;
    Debug_MSG_LN("AUTO");
  }
  if (tmp == 0) {
    Termostato1_AUTO = false;
    Debug_MSG_LN("MAN");
  }
  if ((tmp != 1 and tmp != 0) or isnan(tmp)) {
    Termostato1_AUTO = false;
    Debug_MSG_LN("MAN");
  }
  Debug_MSG("Reading EEPROM termostato: ");
  Termostato1_termostato = eepromReadFloat(EEPROM_Termostato1_termostato);
  if (Termostato1_termostato<2 or Termostato1_termostato>30 or isnan(Termostato1_termostato)) Termostato1_termostato = 18;
  Debug_MSG_LN((String) Termostato1_termostato);
}
