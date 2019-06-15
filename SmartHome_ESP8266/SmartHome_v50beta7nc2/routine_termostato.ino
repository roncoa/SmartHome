#if Termostato_nodi>0

#include "DHT.h"
#include "SSD1306.h"  // 128x64   giallo 0-128,0-15 - blu 0-128,16-64

#define DHTTYPE DHT22
#define Termostato_TEMPO_TERMOSTATO     5000               // non impostare un valore inferiore a 2000 (2 sec.)

#define Termostato_TEMPO_CLICK_ON           TEMPO_CLICK_ON


float   soglia_scatto_Termostato1 = 0.25;
float   soglia_aggiornamento_temperatura = 0.2;

String Termostato_DEF     = "TER";
String Termostato_ON      = "on";
String Termostato_OFF     = "off";
String Termostato_MAN     = "man";
String Termostato_AUTO    = "auto";
String Termostato_CRONO   = "crono";
String Termostato_STATO   = "stato";
String Termostato_READ    = "read";
String Termostato_ACK_ON  = "on";
String Termostato_ACK_OFF = "off";
String Termostato_ACK_MAN = "man";
String Termostato_ACK_AUTO = "auto";
String Termostato_ACK_CRONO = "crono";
String Termostato_Topic_ACK_suffisso = "/ack";

typedef struct {
  String topic;
  String topic_ACK;
  int rele;
  int bottone;
  int piu;
  int meno;
  int dht;
  int i2c;
  int sda;
  int scl;
  float t;
  float h;
  float hi;
  float ta;
  float t1;
  float t2;
  float t3;
  int crono[24];
  boolean AUTO;
  boolean CRONO;
} InfoRecordTermostatoType ;

InfoRecordTermostatoType Termostato[1];

TPush Termostato_timer[Termostato_nodi];
TPush Termostato_TPush_bottone[Termostato_nodi], Termostato_TPush_piu[Termostato_nodi], Termostato_TPush_meno[Termostato_nodi];
Ticker Tempo_pagina;
int Pagina = 1;
int Tot_pagine = 6;
DHT dht(Termostato1_GPIO_DHT, DHTTYPE);
SSD1306 display(Termostato1_I2C_DISPLAY_ADDRESS, Termostato1_GPIO_SDA, Termostato1_GPIO_SCL);

// http://www.online-utility.org/image_converter.jsp
// https://convertio.co/it/bmp-xbm/
// to create XBM files

#define logo_width 50
#define logo_height 50
static uint8_t PROGMEM logo_bits[] = {
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x00, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF,
  0x1F, 0x00, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x00, 0xF8, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x00, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFE,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0xFF, 0xFF, 0xFF, 0xFF,
  0x1F, 0xF8, 0x03, 0xFF, 0xFF, 0x7F, 0xF8, 0xE7, 0xF7, 0x03, 0xFF, 0xFF,
  0x1F, 0xF0, 0x37, 0xEC, 0x03, 0xFF, 0xFF, 0x0F, 0xE0, 0xDB, 0xDB, 0x03,
  0xFF, 0xFF, 0x87, 0x83, 0x2F, 0xF4, 0x03, 0xFF, 0xFF, 0xC1, 0x0F, 0xFF,
  0xFF, 0x03, 0xFF, 0xFF, 0xF0, 0x1F, 0x7C, 0xFE, 0x03, 0xFF, 0x7F, 0xF8,
  0x3F, 0x78, 0xFE, 0x03, 0xFF, 0x1F, 0x3C, 0xF8, 0xF0, 0xFF, 0x03, 0xFF,
  0x0F, 0x3E, 0xF9, 0xC1, 0xFF, 0x03, 0xFF, 0x83, 0x3F, 0xF8, 0x83, 0xFF,
  0x03, 0xFF, 0xC1, 0x3F, 0xF8, 0x0F, 0xFF, 0x03, 0xFF, 0xF0, 0xFF, 0xFF,
  0x1F, 0xFC, 0x03, 0x3F, 0xF8, 0xFF, 0xFF, 0x7F, 0xF8, 0x03, 0x1F, 0xFE,
  0xFF, 0xFF, 0xFF, 0xE0, 0x03, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xC1, 0x03,
  0xFF, 0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0xFF, 0x87, 0xFF, 0xFF, 0xFF,
  0xFF, 0x03, 0xFF, 0xE3, 0xFF, 0xFF, 0xCF, 0xFF, 0x03, 0xFF, 0xE3, 0x90,
  0x63, 0x84, 0xFF, 0x03, 0xFF, 0x87, 0x04, 0x4F, 0xCE, 0xFF, 0x03, 0xFF,
  0x9F, 0x24, 0x43, 0xCE, 0xFF, 0x03, 0xFF, 0x9B, 0x24, 0x49, 0xCE, 0xFF,
  0x03, 0xFF, 0xC3, 0x24, 0x43, 0x8E, 0xFF, 0x03, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0xFF, 0x67, 0xFE, 0xFF, 0xFF, 0xFF, 0x03,
  0xFF, 0x67, 0xFE, 0xFF, 0xFF, 0xFF, 0x03, 0xFF, 0x67, 0x86, 0x90, 0xE3,
  0xFF, 0x03, 0xFF, 0x07, 0x92, 0x00, 0xCB, 0xFF, 0x03, 0xFF, 0x67, 0x92,
  0x24, 0xC1, 0xFF, 0x03, 0xFF, 0x67, 0x92, 0x24, 0xF9, 0xFF, 0x03, 0xFF,
  0x67, 0x86, 0x24, 0xE3, 0xFF, 0x03, 0xFF, 0xFF, 0xFF, 0xB5, 0xEF, 0xFF,
  0x03, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0xFE, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0x01, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFC, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x00,
  0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0x00, 0x80, 0xFF, 0xFF, 0xFF, 0xFF,
  0x07, 0x00,
};

#define powerON_width 16
#define powerON_height 16
static uint8_t PROGMEM powerON_bits[] = {
  0xC0, 0x03, 0xC0, 0x03, 0xD8, 0x1B, 0xDC, 0x3B, 0xDE, 0x7B, 0xCE, 0x73,
  0xC7, 0xE3, 0xC7, 0xE3, 0xC7, 0xE3, 0xC7, 0xE3, 0x0E, 0x70, 0x0E, 0x70,
  0x3E, 0x7C, 0xFC, 0x3F, 0xF8, 0x1F, 0xE0, 0x07,
};

#define powerOFF_width 16
#define powerOFF_height 16
static uint8_t PROGMEM powerOFF_bits[] = {
  0xC0, 0xE3, 0xC0, 0xF3, 0xD8, 0xFB, 0xDC, 0x7F, 0xDE, 0x7F, 0xCE, 0x7F,
  0xC7, 0xEF, 0xC7, 0xE7, 0xE7, 0xE3, 0xF7, 0xE3, 0xFE, 0x70, 0x7E, 0x70,
  0x3E, 0x7C, 0xFF, 0x3F, 0xFF, 0x1F, 0xE7, 0x07,
};

#define wifiON_width 16
#define wifiON_height 16
static uint8_t PROGMEM wifiON_bits[] = {
  0xF0, 0x0F, 0xFC, 0x3F, 0xFE, 0x7F, 0x1E, 0x78, 0x07, 0xE0, 0xE3, 0xC7,
  0xF8, 0x1F, 0x3C, 0x3C, 0x1C, 0x38, 0xC0, 0x03, 0xE0, 0x07, 0x20, 0x04,
  0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
};

#define wifiOFF_width 16
#define wifiOFF_height 16
static uint8_t PROGMEM wifiOFF_bits[] = {
  0xF0, 0xEF, 0xFC, 0xFF, 0xFE, 0xFF, 0x1E, 0x7C, 0x07, 0xFE, 0xE3, 0xDF,
  0xF8, 0x1F, 0xFC, 0x3F, 0xFC, 0x3B, 0xF0, 0x03, 0xF8, 0x07, 0x7C, 0x04,
  0xBE, 0x01, 0x9F, 0x01, 0x8F, 0x01, 0x87, 0x01,
};

#define temperatura_width 16
#define temperatura_height 16
static uint8_t PROGMEM temperatura_bits[] = {
  0x80, 0x03, 0xC0, 0x07, 0xE0, 0x04, 0xE0, 0x07, 0xE0, 0x04, 0xE0, 0x07,
  0xE0, 0x04, 0xE0, 0x07, 0xE0, 0x04, 0xF0, 0x0F, 0xF8, 0x1F, 0xF8, 0x1F,
  0xF8, 0x1F, 0xF8, 0x1F, 0xF0, 0x0F, 0xE0, 0x07,
};

#define umidita_width 16
#define umidita_height 16
static uint8_t PROGMEM umidita_bits[] = {
  0x00, 0x00, 0x80, 0x01, 0xC0, 0x03, 0xE0, 0x07, 0x70, 0x0E, 0x30, 0x0C,
  0x38, 0x1C, 0x18, 0x18, 0x1C, 0x38, 0x1C, 0x38, 0x1C, 0x38, 0x38, 0x1C,
  0xF8, 0x1F, 0xF0, 0x0F, 0xC0, 0x03, 0x00, 0x00,
};

#define auto_width 16
#define auto_height 16
static uint8_t auto_bits[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x57, 0x77, 0x55, 0x52,
  0x55, 0x52, 0x57, 0x52, 0x55, 0x52, 0x55, 0x52, 0x75, 0x72, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

#define man_width 16
#define man_height 16
static uint8_t man_bits[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD1, 0x4B, 0x5B, 0x5A,
  0x5F, 0x5A, 0xD5, 0x6B, 0x51, 0x6A, 0x51, 0x4A, 0x51, 0x4A, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

#define crono_width 16
#define crono_height 16
static uint8_t crono_bits[] = {
  0x00, 0x00, 0xE0, 0x07, 0xF0, 0x0F, 0x98, 0x19, 0x8C, 0x31, 0x86, 0x61,
  0x86, 0x61, 0x86, 0x6F, 0x86, 0x6F, 0x06, 0x60, 0x06, 0x60, 0x0C, 0x30,
  0x18, 0x18, 0xF0, 0x0F, 0xE0, 0x07, 0x00, 0x00,
};

#define no_crono_width 16
#define no_crono_height 16
static uint8_t no_crono_bits[] = {
  0x00, 0xE0, 0xE0, 0xF7, 0xF0, 0xFF, 0x98, 0x7D, 0x8C, 0x3F, 0x86, 0x7F,
  0x86, 0x6F, 0xC6, 0x6F, 0xE6, 0x6F, 0xF6, 0x61, 0xFE, 0x60, 0x7C, 0x30,
  0x3E, 0x18, 0xFF, 0x0F, 0xEF, 0x07, 0x07, 0x00,
};

#define dot_width 10
#define dot_height 10
static uint8_t dot_bits[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0xFC, 0x00,
  0xFC, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0xFC, 0x00,
};

void setup_Termostato() {
  Termostato[0].topic = Termostato1_Topic;
  Termostato[0].topic_ACK = Termostato1_Topic;
  Termostato[0].topic_ACK += Termostato_Topic_ACK_suffisso;
  Termostato[0].rele = Termostato1_GPIO_rele;
#ifdef Termostato1_GPIO_bottone
  Termostato[0].bottone = Termostato1_GPIO_bottone;
#else
  Termostato[0].bottone = 100;
#endif
#ifdef Termostato1_GPIO_piu
  Termostato[0].piu = Termostato1_GPIO_piu;
#else
  Termostato[0].piu = 100;
#endif
#ifdef Termostato1_GPIO_meno
  Termostato[0].meno = Termostato1_GPIO_meno;
#else
  Termostato[0].meno = 100;
#endif
  Termostato[0].dht = Termostato1_GPIO_DHT;
  Termostato[0].i2c = Termostato1_I2C_DISPLAY_ADDRESS;
  Termostato[0].sda = Termostato1_GPIO_SDA;
  Termostato[0].scl = Termostato1_GPIO_SCL;
  Termostato[0].AUTO = false;
  Termostato[0].ta = 18.00;

  for (int i = 0; i < Termostato_nodi; i++) {
    Debug_MSG_LN("Nodo Termostato " + String(i + 1) + " : " + String(Termostato[i].topic));
    Debug_MSG_LN(" RELE termostato " + String(i + 1) + "      = GPIO" + String(Termostato[i].rele));
    if (Termostato[i].bottone >= 99) {
      Debug_MSG_LN(" BOTTONE termostato " + String(i + 1) + " : Nessun GPIO definito!");
    }
    else {
      Debug_MSG_LN(" BOTTONE termostato " + String(i + 1) + "   = GPIO" + String(Termostato[i].bottone));
    }

    if (Termostato[i].piu >= 99) {
      Debug_MSG_LN(" PIU termostato " + String(i + 1) + " : Nessun GPIO definito!");
    }
    else {
      Debug_MSG_LN(" PIU termostato " + String(i + 1) + "   = GPIO" + String(Termostato[i].piu));
    }

    if (Termostato[i].meno >= 99) {
      Debug_MSG_LN(" MENO termostato " + String(i + 1) + " : Nessun GPIO definito!");
    }
    else {
      Debug_MSG_LN(" MENO termostato " + String(i + 1) + "   = GPIO" + String(Termostato[i].meno));
    }
    Debug_MSG_LN(" DTH22 termostato " + String(i + 1) + "     = GPIO" + String(Termostato[i].dht));
    Debug_MSG_LN(" SSD1306 ADDRESS termostato " + String(i + 1) + " = 0x"   + String(Termostato[i].i2c, HEX));
    Debug_MSG_LN(" SSD1306 SDA termostato " + String(i + 1) + " = GPIO" + String(Termostato[i].sda));
    Debug_MSG_LN(" SSD1306 SCL termostato " + String(i + 1) + " = GPIO" + String(Termostato[i].scl));
#ifdef PullDown
    Termostato_TPush_bottone[i].setUp(Termostato[i].bottone, HIGH);
    Termostato_TPush_piu[i].setUp(Termostato[i].piu, HIGH);
    Termostato_TPush_meno[i].setUp(Termostato[i].meno, HIGH);
#endif
#ifdef PullUp
    Termostato_TPush_bottone[i].setUp(Termostato[i].bottone, LOW);
    Termostato_TPush_piu[i].setUp(Termostato[i].piu, LOW);
    Termostato_TPush_meno[i].setUp(Termostato[i].meno, LOW);
#endif

    pinMode(Termostato[i].rele, OUTPUT);
    EEPROM_Termostato_read(i);
  }
  dht.begin();
  Debug_MSG_LN("Inizializzazione il display");
  String s;
  s = "SmartHome v";
  s += Versione;
  display.init();
  display.flipScreenVertically();
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 0, s);
  Debug_MSG_LN("Show logo");
  display.drawXbm(((128 - 50) / 2), 16, logo_width, logo_height, logo_bits);
  display.display();
  unblockingDelay(5000);
  Termostato[0].t = dht.readTemperature(false);
  Termostato[0].h = dht.readHumidity();
  Termostato[0].hi = dht.computeHeatIndex(Termostato[0].t, Termostato[0].h, false);
  Debug_MSG_LN((String) Termostato[0].topic + " " + (String) Termostato[0].t + "°C - " + (String) Termostato[0].h + "%");
  Tempo_pagina.attach(5, Cambia_pagina);
}

void ACK_Termostato() {
  String payload;
  for (int i = 0; i < Termostato_nodi; i++) {
    payload = "{\"MAC_address\":\"";
    payload += macToStr(mac);
    payload += "\",\"NODO\":\"";
    payload += Termostato_DEF;
    payload += "\",\"Topic\":\"";
    payload += Termostato[i].topic;
    payload += "\",\"Topic_ACK\":\"";
    payload += Termostato[i].topic_ACK;
    payload += "\",\"availability_topic\":\"";
    payload += WillTopicStr;
    payload += "\"}";
    MQTTPublish(ACK_Topic, payload);
    Stato_TER(i);
  }
}

void HA_discovery_Termostato() {
}

void Subscribe_Termostato() {
  for (int i = 0; i < Termostato_nodi; i++) {
    MQTTSubscribe(Termostato[i].topic);
    Debug_MSG_LN("MQTT subscribe " + String(  Termostato[i].topic));
  }
}

void sendTemperature() {
  String payload;
  for (int i = 0; i < Termostato_nodi; i++) {
    float old_t = Termostato[i].t;
    float old_h = Termostato[i].h;
    Termostato[i].t = dht.readTemperature(false);
    Termostato[i].h = dht.readHumidity();
    Termostato[i].hi = dht.computeHeatIndex(Termostato[i].t, Termostato[i].h, false);
    Debug_MSG((String) Termostato[i].topic);
    if (isnan(Termostato[i].h) || isnan(Termostato[i].t)) {
      Debug_MSG_LN(" failed to read from DHT sensor!");
      return;
    }
    if (fabs(Termostato[i].t - old_t) <= soglia_aggiornamento_temperatura and fabs(Termostato[i].h - old_h) <= soglia_aggiornamento_temperatura) {
      Termostato[i].t = old_t;
      Termostato[i].h = old_h;
      Debug_MSG_LN(" old value, not updating!");
      return;
    }
    Debug_MSG_LN(" " + (String) Termostato[i].t + "°C - " + (String) Termostato[i].h + "%");
    Stato_TER(i);
  }
}

void Stato_TER(int i) {
  String payload;
  payload = "{\"NODO\":\"";
  payload += Termostato_DEF;
  payload += "\",\"T\":\"";
  payload += Termostato[i].t;
  payload += "\",\"H\":\"";
  payload += Termostato[i].h;
  payload += "\",\"HI\":\"";
  payload += Termostato[i].hi;
  payload += "\",\"STATO\":\"";
  if (!Termostato[i].CRONO && Termostato[i].AUTO) payload += Termostato_ACK_AUTO;
  if (!Termostato[i].CRONO && !Termostato[i].AUTO) payload += Termostato_ACK_MAN;
  if (Termostato[i].CRONO) payload += Termostato_ACK_CRONO;
  payload += "\",\"RELE\":\"";
  if (digitalRead(Termostato[i].rele) == true ^ Flag_inversione_RELE) payload += Termostato_ACK_ON;
  if (digitalRead(Termostato[i].rele) == false ^ Flag_inversione_RELE) payload += Termostato_ACK_OFF;
  payload += "\",\"TERMOSTATO\":\"";
  payload += (String)Termostato[i].ta;
  payload += "\"}";
  MQTTPublish(Termostato[i].topic_ACK, payload);
}

void callback_Termostato(char* topic, char* message) {
  String payload;
  for (int i = 0; i < Termostato_nodi; i++) {
    if (String(topic) ==   Termostato[i].topic) {                                                                             // se arriva il comando sul topic "Termostato1_Topic"
      if ((String)message == Termostato_READ) {       // Topic "Termostato1_Topic" = "read"
        sendTemperature();                                                                                                // sendTemperature()
        Stato_TER(i);
      }
      if ((String)message == Termostato_ON) {                                                     // Topic "Termostato1_Topic" = "on"
        // Termostato[i].AUTO = false;
        digitalWrite(Termostato[i].rele, 1 ^ Flag_inversione_RELE);                                                        // Accendo il RELE
        Stato_TER(i);
        EEPROM_Termostato_write(i);
      }
      if ((String)message == Termostato_OFF) {                     // Topic "Termostato1_Topic" = "off"
        // Termostato[i].AUTO = false;
        digitalWrite(Termostato[i].rele, 0 ^ Flag_inversione_RELE);                                                        // Spengo il RELE
        Stato_TER(i);
        EEPROM_Termostato_write(i);
      }
      if (((char)message[0] == 'T' or (char)message[0] == 't') and (char)message[1] == '=' ) {                                  // Topic "Termostato1_Topic" = "T=" o "t="
        String stringtmp = (String)message;
        stringtmp = stringtmp.substring(2);
        Termostato[i].ta = stringtmp.toFloat();
        if (Termostato[i].ta<2 or Termostato[i].ta>30 or isnan(Termostato[i].ta)) Termostato[i].ta = 18;
        Debug_MSG_LN("t=" + (String)Termostato[i].ta);
        Stato_TER(i);
        EEPROM_Termostato_write(i);
      }
      if (((char)message[0] == 'T' or (char)message[0] == 't') and (char)message[1] == '1') {                                  // Topic "Termostato1_Topic" = "T1=" o "t1="
        String stringtmp = (String)message;
        stringtmp = stringtmp.substring(3);
        Termostato[i].t1 = stringtmp.toFloat();
        if (Termostato[i].t1<2 or Termostato[i].t1>30 or isnan(Termostato[i].t1)) Termostato[i].t1 = 18;
        Debug_MSG_LN("t1=" + (String)Termostato[i].t1);
        Stato_TER(i);
        EEPROM_Termostato_write(i);
      }
      if (((char)message[0] == 'T' or (char)message[0] == 't') and (char)message[1] == '2') {                                  // Topic "Termostato1_Topic" = "T2=" o "t2="
        String stringtmp = (String)message;
        stringtmp = stringtmp.substring(3);
        Termostato[i].t2 = stringtmp.toFloat();
        if (Termostato[i].t2<2 or Termostato[i].t2>30 or isnan(Termostato[i].t2)) Termostato[i].t2 = 18;
        Debug_MSG_LN("t2=" + (String)Termostato[i].t2);
        Stato_TER(i);
        EEPROM_Termostato_write(i);
      }
      if (((char)message[0] == 'T' or (char)message[0] == 't') and (char)message[1] == '3') {                                  // Topic "Termostato1_Topic" = "T3=" o "t3="
        String stringtmp = (String)message;
        stringtmp = stringtmp.substring(3);
        Termostato[i].t3 = stringtmp.toFloat();
        if (Termostato[i].t3<2 or Termostato[i].t3>30 or isnan(Termostato[i].t3)) Termostato[i].t3 = 18;
        Debug_MSG_LN("t3=" + (String)Termostato[i].t3);
        Stato_TER(i);
        EEPROM_Termostato_write(i);
      }
      if ((char)message[0] == 'h' and (char)message[3] == '=') {  // h00=1
        String stringtmp1 = (String)message;
        String stringtmp2 = (String)message;
        stringtmp1 = stringtmp1.substring(1, 3);
        stringtmp2 = stringtmp2.substring(4, 5);
        if (stringtmp1.toInt()<0 or stringtmp2.toInt()>23) {
          Debug_MSG_LN("Parametri errati");
          return;
        }
        if (stringtmp2.toInt()<1 or stringtmp2.toInt()>3) {
          Debug_MSG_LN("Parametri errati");
          return;
        }
        Termostato[i].crono[stringtmp1.toInt()] = stringtmp2.toInt();
        Debug_MSG_LN("Ora " + (String)stringtmp1.toInt() + "= T" + (String)stringtmp2.toInt());
        Stato_TER(i);
        EEPROM_Termostato_write(i);
      }

      if ((String)message == Termostato_STATO) {                                                  // Topic "Termostato1_Topic" = "stato"
        payload =   Termostato[i].topic;
        payload += " RSSI=";
        payload += WiFi.RSSI();
        payload += "dB ";
        payload += " T=";
        char tmp[4];                            // string buffer
        String stringtmp = "";                  //data on buff is copied to this string
        dtostrf(Termostato[i].ta, 4, 1, tmp);
        for (int i = 0; i < sizeof(tmp); i++)  {
          stringtmp += tmp[i];
        }
        payload += stringtmp;
        if (Termostato[i].AUTO == true) payload += " AUTO";
        else payload += " MAN";
        payload += " RELE=";
        if (digitalRead(Termostato[i].rele) == true ^ Flag_inversione_RELE) payload += "ON";
        if (digitalRead(Termostato[i].rele) == false ^ Flag_inversione_RELE) payload += "OFF";
        MQTTPublish(ACK_Topic, payload);
      }
      if ((String)message == Termostato_AUTO) {       // Topic "Termostato1_Topic" = "auto"
        Termostato[i].AUTO = true;
        Stato_TER(i);
        EEPROM_Termostato_write(i);
      }
      if ((String)message == Termostato_MAN) {                                  // Topic "Termostato1_Topic" = "man"
        Termostato[i].AUTO = false;
        Stato_TER(i);
        EEPROM_Termostato_write(i);
      }
    }
  }
}

void loop_Termostato() {
  for (int i = 0; i < Termostato_nodi; i++) {

    if (Termostato_TPush_piu[i].Click(Termostato_TEMPO_CLICK_ON) and Termostato[i].piu != 100) {      // Piu premuto!
      Debug_MSG_LN("PIU Termostato " + String(i + 1) + " premuto!");
      Termostato[i].ta = Termostato[i].ta + 0.1;
      Debug_MSG_LN("t=" + (String)Termostato[i].ta);
      Stato_TER(i);
      EEPROM_Termostato_write(i);
    }
    if (Termostato_TPush_meno[i].Click(Termostato_TEMPO_CLICK_ON) and Termostato[i].meno != 100) {      // Meno premuto!
      Debug_MSG_LN("MENO Termostato " + String(i + 1) + " premuto!");
      Termostato[i].ta = Termostato[i].ta - 0.1;
      Debug_MSG_LN("t=" + (String)Termostato[i].ta);
      Stato_TER(i);
      EEPROM_Termostato_write(i);
    }

    if (Termostato_TPush_bottone[i].Click(Termostato_TEMPO_CLICK_ON) and Termostato[i].bottone != 100) {      // Bottone premuto!
      Debug_MSG_LN("Bottone Termostato " + String(i + 1) + " premuto!");
      int tmp = 0;
      if (!Termostato[i].CRONO && !Termostato[i].AUTO && (digitalRead(Termostato[i].rele) == true ^ Flag_inversione_RELE)) tmp = 1;
      if (!Termostato[i].CRONO && !Termostato[i].AUTO && (digitalRead(Termostato[i].rele) != true ^ Flag_inversione_RELE)) tmp = 2;
      if (!Termostato[i].CRONO && Termostato[i].AUTO ) tmp = 3;
      if (Termostato[i].CRONO) tmp = 4;

      switch (tmp) {
        case 1:
          Termostato[i].AUTO = false;
          Termostato[i].CRONO = false;
          digitalWrite(Termostato[i].rele, 0 ^ Flag_inversione_RELE);
          break;
        case 2:
          Termostato[i].AUTO = true;
          Termostato[i].CRONO = false;
          digitalWrite(Termostato[i].rele, 0 ^ Flag_inversione_RELE);
          break;
        case 3:
          Termostato[i].AUTO = false;
          Termostato[i].CRONO = true;
          break;
        case 4:
          Termostato[i].AUTO = false;
          Termostato[i].CRONO = false;
          digitalWrite(Termostato[i].rele, 1 ^ Flag_inversione_RELE);
          break;
      }
      EEPROM_Termostato_write(i);
    }

    if (Termostato_timer[i].Wait(Termostato_TEMPO_TERMOSTATO)) {
      sendTemperature();
      if (!Termostato[i].CRONO && Termostato[i].AUTO) {
        if ((Termostato[i].t < (Termostato[i].ta - soglia_scatto_Termostato1)) and (digitalRead(Termostato[i].rele) == 0  ^ Flag_inversione_RELE)) {
          digitalWrite(Termostato[i].rele, 1 ^ Flag_inversione_RELE);                                                        // Accendo il RELE
          Debug_MSG_LN("Accendo");
          Stato_TER(i);
          EEPROM_Termostato_write(i);
        }
        if ((Termostato[i].t > (Termostato[i].ta + soglia_scatto_Termostato1)) and (digitalRead(Termostato[i].rele) == 1  ^ Flag_inversione_RELE)) {
          digitalWrite(Termostato[i].rele, 0 ^ Flag_inversione_RELE);                                                        // Spengo il RELE
          Debug_MSG_LN("Spengo");
          Stato_TER(i);
          EEPROM_Termostato_write(i);
        }
      }
      if (Termostato[i].CRONO && cbtime_set) {
        now = dstAdjusted.time(nullptr);
        char buffer[80];
        strftime (buffer, 80, "%H", localtime(&now));
        String xx = (String)buffer;
        float tcrono;
        if (Termostato[i].crono[xx.toInt()] == 1) tcrono = Termostato[i].t1;
        if (Termostato[i].crono[xx.toInt()] == 2) tcrono = Termostato[i].t2;
        if (Termostato[i].crono[xx.toInt()] == 3) tcrono = Termostato[i].t3;
        if ((Termostato[i].t < (tcrono - soglia_scatto_Termostato1)) and (digitalRead(Termostato[i].rele) == 0  ^ Flag_inversione_RELE)) {
          digitalWrite(Termostato[i].rele, 1 ^ Flag_inversione_RELE);
          Debug_MSG_LN("Accendo");
          Stato_TER(i);
          EEPROM_Termostato_write(i);
        }
        if ((Termostato[i].t > (tcrono + soglia_scatto_Termostato1)) and (digitalRead(Termostato[i].rele) == 1  ^ Flag_inversione_RELE)) {
          digitalWrite(Termostato[i].rele, 0 ^ Flag_inversione_RELE);
          Debug_MSG_LN("Spengo");
          Stato_TER(i);
          EEPROM_Termostato_write(i);
        }
      }
    }

    display.clear();
    Prima_riga(i);
    if (Pagina == 1) Pagina_1(i);
    if (Pagina == 2) Pagina_2(i);
    if (Pagina == 3) Pagina_3(i);
    if (Pagina == 4) Pagina_4(i);
    if (Pagina == 5) Pagina_5(i);
    if (Pagina == 6) Pagina_6(i);
    display.display();

  }
}

void Cambia_pagina() {
  Pagina += 1;
  if (Pagina > Tot_pagine) Pagina = 1;
}

void Prima_riga(int i) {
  if (digitalRead(Termostato[i].rele) == true ^ Flag_inversione_RELE) {
    display.drawXbm(128 - 16, 0, powerON_width, powerON_height, powerON_bits);
  }
  else {
    display.drawXbm(128 - 16, 0, powerOFF_width, powerOFF_height, powerOFF_bits);
  }
  if (mqttClient.connected()) {
    display.drawXbm(0, 0, wifiON_width, wifiON_height, wifiON_bits);
  }
  else {
    display.drawXbm(0, 0, wifiOFF_width, wifiOFF_height, wifiOFF_bits);
  }
  display.drawXbm(16, 0, temperatura_width, temperatura_height, temperatura_bits);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.drawString(32, 0, String(Termostato[i].t) + "°C");
  if (!Termostato[i].CRONO && Termostato[i].AUTO) display.drawXbm(128 - 35, 0, auto_width, auto_height, auto_bits);
  if (!Termostato[i].CRONO && !Termostato[i].AUTO)    display.drawXbm(128 - 35, 0, man_width, man_height, man_bits);
  if (Termostato[i].CRONO && cbtime_set) display.drawXbm(128 - 35, 0, crono_width, crono_height, crono_bits);
  if (Termostato[i].CRONO && !cbtime_set) display.drawXbm(128 - 35, 0, no_crono_width, no_crono_height, no_crono_bits);
}

void Pagina_1(int i) {
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  if (!Termostato[i].CRONO && Termostato[i].AUTO) display.drawString(64, 16, "TA = " + String(Termostato[i].ta) + "°C");
  if (!Termostato[i].CRONO && !Termostato[i].AUTO) {
    if (digitalRead(Termostato[i].rele) == 1  ^ Flag_inversione_RELE) display.drawString(64, 16, "Manuale ON");
    else display.drawString(64, 16, "Manuale OFF");
  }
  if (Termostato[i].CRONO && cbtime_set)  {
    now = dstAdjusted.time(nullptr);
    char buffer[80];
    strftime (buffer, 80, "%H", localtime(&now));
    String xx = (String)buffer;
    String tmp, tcrono;
    if (Termostato[i].crono[xx.toInt()] == 1) tcrono = (String)Termostato[i].t1;
    if (Termostato[i].crono[xx.toInt()] == 2) tcrono = (String)Termostato[i].t2;
    if (Termostato[i].crono[xx.toInt()] == 3) tcrono = (String)Termostato[i].t3;
    display.drawString(64, 16, "T" + (String)Termostato[i].crono[xx.toInt()] + " = " + tcrono + "°C");
    strftime (buffer, 80, "%X", localtime(&now));
    display.drawString(64, 32, (String)buffer);
  }
  if (Termostato[i].CRONO && !cbtime_set) display.drawString(64, 16, "Crono -no time-");
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  // display.drawXbm(20, 32, temperatura_width, temperatura_height, temperatura_bits);
  // display.drawString(40, 32, String(Termostato[i].t) + "°C");
  display.drawXbm(20, 48, umidita_width, umidita_height, umidita_bits);
  display.drawString(40, 48, String(Termostato[i].h) + "%");
}

void Pagina_2(int i) {
  Cambia_pagina();
}

void Pagina_3(int i) {
  Cambia_pagina();
  return;
  if (!Termostato[i].CRONO or cbtime_set != true) {
    Cambia_pagina();
    return;
  }
  now = dstAdjusted.time(nullptr);
  char buffer[80];
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
  strftime (buffer, 80, "%X", localtime(&now));
  display.drawString(64, 16, (String)buffer);
  strftime (buffer, 80, "%x", localtime(&now));
  display.drawString(64, 32, (String)buffer);
  strftime (buffer, 80, "%w", localtime(&now));
  String tmp = "err.";
  if ((String)buffer == "0") tmp = "domenica";
  if ((String)buffer == "1") tmp = "lunedì";
  if ((String)buffer == "2") tmp = "martedì";
  if ((String)buffer == "3") tmp = "mercoledì";
  if ((String)buffer == "4") tmp = "giovedì";
  if ((String)buffer == "5") tmp = "venerdì";
  if ((String)buffer == "6") tmp = "sabato";
  display.drawString(64, 48, tmp);
}

void Pagina_4(int i) {
  if (!Termostato[i].CRONO or cbtime_set != true) {
    Cambia_pagina();
    return;
  }
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0,   14, "T3");
  display.drawString(0,   24, "T2");
  display.drawString(0,   34, "T1");
  display.drawString(10,  44, "0");
  display.drawString(20,  44, "1");
  display.drawString(30,  44, "2");
  display.drawString(40,  44, "3");
  display.drawString(50,  44, "4");
  display.drawString(60,  44, "5");
  display.drawString(70,  44, "6");
  display.drawString(80,  44, "7");
  display.drawString(90,  44, "8");
  display.drawString(100, 44, "9");
  display.drawString(110, 44, "1");
  display.drawString(110, 54, "0");
  display.drawString(120, 44, "1");
  display.drawString(120, 54, "1");
  for (int ii = 0; ii <= 11; ii++) {
    if (Termostato[i].crono[ii] >= 1) display.drawXbm((ii + 1) * 10, 34, dot_width, dot_height, dot_bits);
    if (Termostato[i].crono[ii] >= 2) display.drawXbm((ii + 1) * 10, 24, dot_width, dot_height, dot_bits);
    if (Termostato[i].crono[ii] == 3) display.drawXbm((ii + 1) * 10, 14, dot_width, dot_height, dot_bits);
  }
  now = dstAdjusted.time(nullptr);
  char buffer[80];
  strftime (buffer, 80, "%w", localtime(&now));
  if ((String)buffer == "0") {
    display.drawString(0,   44, "D");
    display.drawString(0,   54, "O");
  }
  if ((String)buffer == "1") {
    display.drawString(0,   44, "L");
    display.drawString(0,   54, "U");
  }
  if ((String)buffer == "2") {
    display.drawString(0,   44, "M");
    display.drawString(0,   54, "A");
  }
  if ((String)buffer == "3") {
    display.drawString(0,   44, "M");
    display.drawString(0,   54, "E");
  }
  if ((String)buffer == "4") {
    display.drawString(0,   44, "G");
    display.drawString(0,   54, "I");
  }
  if ((String)buffer == "5") {
    display.drawString(0,   44, "V");
    display.drawString(0,   54, "E");
  }
  if ((String)buffer == "6") {
    display.drawString(0,   44, "S");
    display.drawString(0,   54, "A");
  }
}

void Pagina_5(int i) {
  if (!Termostato[i].CRONO or cbtime_set != true) {
    Cambia_pagina();
    return;
  }
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0,   14, "T3");
  display.drawString(0,   24, "T2");
  display.drawString(0,   34, "T1");
  display.drawString(10,  44, "1");
  display.drawString(10,  54, "2");
  display.drawString(20,  44, "1");
  display.drawString(20,  54, "3");
  display.drawString(30,  44, "1");
  display.drawString(30,  54, "4");
  display.drawString(40,  44, "1");
  display.drawString(40,  54, "5");
  display.drawString(50,  44, "1");
  display.drawString(50,  54, "6");
  display.drawString(60,  44, "1");
  display.drawString(60,  54, "7");
  display.drawString(70,  44, "1");
  display.drawString(70,  54, "8");
  display.drawString(80,  44, "1");
  display.drawString(80,  54, "9");
  display.drawString(90,  44, "2");
  display.drawString(90,  54, "0");
  display.drawString(100, 44, "2");
  display.drawString(100, 54, "1");
  display.drawString(110, 44, "2");
  display.drawString(110, 54, "2");
  display.drawString(120, 44, "2");
  display.drawString(120, 54, "3");
  for (int ii = 12; ii <= 23; ii++) {
    if (Termostato[i].crono[ii] >= 1) display.drawXbm((ii - 11) * 10, 34, dot_width, dot_height, dot_bits);
    if (Termostato[i].crono[ii] >= 2) display.drawXbm((ii - 11) * 10, 24, dot_width, dot_height, dot_bits);
    if (Termostato[i].crono[ii] == 3) display.drawXbm((ii - 11) * 10, 14, dot_width, dot_height, dot_bits);
  }
  now = dstAdjusted.time(nullptr);
  char buffer[80];
  strftime (buffer, 80, "%w", localtime(&now));
  if ((String)buffer == "0") {
    display.drawString(0,   44, "D");
    display.drawString(0,   54, "O");
  }
  if ((String)buffer == "1") {
    display.drawString(0,   44, "L");
    display.drawString(0,   54, "U");
  }
  if ((String)buffer == "2") {
    display.drawString(0,   44, "M");
    display.drawString(0,   54, "A");
  }
  if ((String)buffer == "3") {
    display.drawString(0,   44, "M");
    display.drawString(0,   54, "E");
  }
  if ((String)buffer == "4") {
    display.drawString(0,   44, "G");
    display.drawString(0,   54, "I");
  }
  if ((String)buffer == "5") {
    display.drawString(0,   44, "V");
    display.drawString(0,   54, "E");
  }
  if ((String)buffer == "6") {
    display.drawString(0,   44, "S");
    display.drawString(0,   54, "A");
  }
}

void Pagina_6(int i) {
  if (!Termostato[i].CRONO or cbtime_set != true) {
    Cambia_pagina();
    return;
  }
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 16, "T3 = " + (String)Termostato[i].t3 + "°C");
  display.drawString(64, 32, "T2 = " + (String)Termostato[i].t2 + "°C");
  display.drawString(64, 48, "T1 = " + (String)Termostato[i].t1 + "°C");
}

void EEPROM_Termostato_write(int i) {
  Debug_MSG("Writing EEPROM MAN/AUTO/CRONO Termostato " + String(i + 1) + " :");
  if (!Termostato[i].CRONO && Termostato[i].AUTO) {
    int tmp = 1;
    eepromWrite(EEPROM_Termostato_auto_rele , tmp);
    Debug_MSG_LN("AUTO");
  }
  if (!Termostato[i].CRONO && !Termostato[i].AUTO) {
    int tmp = 0;
    eepromWrite(EEPROM_Termostato_auto_rele , tmp);
    Debug_MSG_LN("MAN");
  }
  if (Termostato[i].CRONO) {
    int tmp = 2;
    eepromWrite(EEPROM_Termostato_auto_rele , tmp);
    Debug_MSG_LN("CRONO");
  }
  Debug_MSG("Writing EEPROM RELE Termostato " + String(i + 1) + " :");
  if (digitalRead(Termostato[i].rele) == 1 ^ Flag_inversione_RELE) {
    int tmp = 1;
    eepromWrite(EEPROM_Termostato_auto_rele  + 1, tmp);
    Debug_MSG_LN("ON");
  }
  if (digitalRead(Termostato[i].rele) == 0 ^ Flag_inversione_RELE) {
    int tmp = 0;
    eepromWrite(EEPROM_Termostato_auto_rele + 1, tmp);
    Debug_MSG_LN("OFF");
  }
  Debug_MSG("Writing EEPROM TERMOSTATO " + String(i + 1) + " :");
  Debug_MSG_LN((String) Termostato[i].ta);
  eepromWriteFloat(EEPROM_Termostato_termostato , Termostato[i].ta);

  Debug_MSG("Writing EEPROM T1 " + String(i + 1) + " :");
  Debug_MSG_LN((String) Termostato[i].t1);
  eepromWriteFloat(EEPROM_Termostato_termostato + 4, Termostato[i].t1);
  Debug_MSG("Writing EEPROM T2 " + String(i + 1) + " :");
  Debug_MSG_LN((String) Termostato[i].t2);
  eepromWriteFloat(EEPROM_Termostato_termostato + 8, Termostato[i].t2);
  Debug_MSG("Writing EEPROM T3 " + String(i + 1) + " :");
  Debug_MSG_LN((String) Termostato[i].t3);
  eepromWriteFloat(EEPROM_Termostato_termostato + 12, Termostato[i].t3);

  for (int ii = 0; ii <= 23; ii++) {
    Debug_MSG("Writing EEPROM Crono " + String(i + 1) + " - " + String(ii) + " :");
    eepromWrite(EEPROM_Termostato_crono + ii, Termostato[i].crono[ii]);
    Debug_MSG_LN("T" + (String)Termostato[i].crono[ii]);
  }
  EEPROM.commit();
}

void EEPROM_Termostato_read(int i) {
  int tmp;
  Debug_MSG("Reading EEPROM MAN/AUTO/CRONO Termostato " + String(i + 1) + " :");
  tmp = eepromRead(EEPROM_Termostato_auto_rele);
  if (tmp == 1) {
    Termostato[i].AUTO = true;
    Termostato[i].CRONO = false;
    Debug_MSG_LN("AUTO");
  }
  if (tmp == 0) {
    Termostato[i].AUTO = false;
    Termostato[i].CRONO = false;
    Debug_MSG_LN("MAN");
  }
  if (tmp == 2) {
    Termostato[i].AUTO = false;
    Termostato[i].CRONO = true;
    Debug_MSG_LN("CRONO");
  }
  if ((tmp != 1 and tmp != 0 and tmp != 2) or isnan(tmp)) {
    Termostato[i].AUTO = false;
    Termostato[i].CRONO = false;
    Debug_MSG_LN("ERR -> MAN");
  }
  Debug_MSG("Reading EEPROM RELE Termostato " + String(i + 1) + " :");
  tmp = eepromRead(EEPROM_Termostato_auto_rele + 1);
  if (tmp == 1) {
    digitalWrite(Termostato[i].rele, 1 ^ Flag_inversione_RELE);
    Debug_MSG_LN("ON");
  }
  if (tmp == 0) {
    digitalWrite(Termostato[i].rele, 0 ^ Flag_inversione_RELE);
    Debug_MSG_LN("OFF");
  }
  if ((tmp != 1 and tmp != 0) or isnan(tmp)) {
    digitalWrite(Termostato[i].rele, 0 ^ Flag_inversione_RELE);
    Debug_MSG_LN("ERR -> OFF");
  }
  Debug_MSG("Reading EEPROM TERMOSTATO " + String(i + 1) + " :");
  Termostato[i].ta = eepromReadFloat(EEPROM_Termostato_termostato + (i * 4));
  if (Termostato[i].ta<2 or Termostato[i].ta>30 or isnan(Termostato[i].ta)) Termostato[i].ta = 18;
  Debug_MSG_LN((String) Termostato[i].ta);
  Debug_MSG("Reading EEPROM T1 " + String(i + 1) + " :");
  Termostato[i].t1 = eepromReadFloat(EEPROM_Termostato_termostato + 4);
  if (Termostato[i].t1<2 or Termostato[i].t1>30 or isnan(Termostato[i].t1)) Termostato[i].t1 = 18;
  Debug_MSG_LN((String) Termostato[i].t1);
  Debug_MSG("Reading EEPROM T2 " + String(i + 1) + " :");
  Termostato[i].t2 = eepromReadFloat(EEPROM_Termostato_termostato + 8);
  if (Termostato[i].t2<2 or Termostato[i].t2>30 or isnan(Termostato[i].t2)) Termostato[i].t2 = 18;
  Debug_MSG_LN((String) Termostato[i].t2);
  Debug_MSG("Reading EEPROM T3 " + String(i + 1) + " :");
  Termostato[i].t3 = eepromReadFloat(EEPROM_Termostato_termostato + 12);
  if (Termostato[i].t3<2 or Termostato[i].t3>30 or isnan(Termostato[i].t3)) Termostato[i].t3 = 18;
  Debug_MSG_LN((String) Termostato[i].t3);

  for (int ii = 0; ii <= 23; ii++) {
    Debug_MSG("Reading EEPROM Crono " + String(i + 1) + " - " + String(ii) + " :");
    Termostato[i].crono[ii] = eepromRead(EEPROM_Termostato_crono + ii);
    if (Termostato[i].crono[ii]<1 or Termostato[i].crono[ii]>3 or isnan(Termostato[i].crono[ii])) Termostato[i].crono[ii] = 1;
    Debug_MSG_LN("T" + (String)Termostato[i].crono[ii]);
  }
}

#endif
