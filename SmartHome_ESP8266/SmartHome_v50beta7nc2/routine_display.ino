#if Display_nodi>0

#include "SSD1306.h"

String Display_DEF     = "DIS";
String Display_STATO   = "stato";
String Display_Topic_ACK_suffisso = "/ack";

typedef struct {
  String topic;
  String topic_ACK;
  String riga_1;
  String riga_2;
  String riga_3;
  String riga_4;
  int i2c;
  int sda;
  int scl;
} InfoRecordDisplayType ;

InfoRecordDisplayType Display[1];

SSD1306 display_D(Display1_I2C_DISPLAY_ADDRESS, Display1_GPIO_SDA, Display1_GPIO_SCL);

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

void setup_Display() {
  Display[0].topic = Display1_Topic;
  Display[0].topic_ACK = Display1_Topic;
  Display[0].topic_ACK += Display_Topic_ACK_suffisso;
  Display[0].i2c = Display1_I2C_DISPLAY_ADDRESS;
  Display[0].sda = Display1_GPIO_SDA;
  Display[0].scl = Display1_GPIO_SCL;
  for (int i = 0; i < Display_nodi; i++) {
    Debug_MSG_LN("- Display " + String(i + 1) + " : " + String(Display[i].topic));
    if (i < 1) {
      Debug_MSG_LN("SSD1306 ADDRESS Display " + String(i + 1) + " = 0x"   + String(Display[i].i2c, HEX));
      Debug_MSG_LN("SSD1306 SDA Display " + String(i + 1) + " = GPIO" + String(Display[i].sda));
      Debug_MSG_LN("SSD1306 SCL Display " + String(i + 1) + " = GPIO" + String(Display[i].scl));
    }
    EEPROM_Display_read(i);
  }
  Debug_MSG_LN("Inizializzazione il display");
  String s;
  s = "SmartHome v";
  s += Versione;
  display_D.init();
  display_D.flipScreenVertically();
  display_D.clear();
  display_D.setTextAlignment(TEXT_ALIGN_CENTER);
  display_D.setFont(ArialMT_Plain_10);
  display_D.drawString(64, 0, s);
  Debug_MSG_LN("Show logo");
  display_D.drawXbm(((128 - 50) / 2), 16, logo_width, logo_height, logo_bits);
  display_D.display();
  unblockingDelay(5000);
}

void ACK_Display() {
  String payload;
  for (int i = 0; i < Display_nodi; i++) {
    payload = "{\"MAC_address\":\"";
    payload += macToStr(mac);
    payload += "\",\"NODO\":\"";
    payload += Display_DEF;
    payload += "\",\"Topic\":\"";
    payload += Display[i].topic;
    payload += "\",\"Topic_ACK\":\"";
    payload += Display[i].topic_ACK;
    payload += "\",\"availability_topic\":\"";
    payload += WillTopicStr;
    payload += "\"}";
    MQTTPublish(ACK_Topic, payload);
    Stato_DIS(i);
  }
}

void HA_discovery_Display() {
}

void Stato_DIS(int i) {
  String payload;
  payload = "{\"NODO\":\"";
  payload += Display_DEF;
  payload += "\",\"RIGA1\":\"";
  payload += Display[i].riga_1;
  payload += "\",\"RIGA2\":\"";
  payload += Display[i].riga_2;
  payload += "\",\"RIGA3\":\"";
  payload += Display[i].riga_3;
  payload += "\",\"RIGA4\":\"";
  payload += Display[i].riga_4;
  payload += "\"}";
  MQTTPublish(Display[i].topic_ACK, payload);
}

void Subscribe_Display() {
  for (int i = 0; i < Display_nodi; i++) {
    MQTTSubscribe((char*) Display[i].topic.c_str());
    Debug_MSG_LN("MQTT subscribe " + String(  Display[i].topic));
  }
}

void callback_Display(char* topic, char* message) {
  String payload;
  for (int i = 0; i < Display_nodi; i++) {
    if (String(topic) ==   Display[i].topic) {
      if ((char)message[0] == '1' and (char)message[1] == '=' ) {
        String stringtmp = (String)message;
        stringtmp = stringtmp.substring(2);
        Display[i].riga_1 = stringtmp.substring(0, EEPROM_Display_len);
        Debug_MSG_LN("riga 1:" + Display[i].riga_1);
        EEPROM_Display_write(i);
        Stato_DIS(i);
      }
      if ((char)message[0] == '2' and (char)message[1] == '=' ) {
        String stringtmp = (String)message;
        stringtmp = stringtmp.substring(2);
        Display[i].riga_2 = stringtmp.substring(0, EEPROM_Display_len);
        Debug_MSG_LN("riga 2:" + Display[i].riga_2 );
        EEPROM_Display_write(i);
        Stato_DIS(i);
      }
      if ((char)message[0] == '3' and (char)message[1] == '=' ) {
        String stringtmp = (String)message;
        stringtmp = stringtmp.substring(2);
        Display[i].riga_3 = stringtmp.substring(0, EEPROM_Display_len);
        Debug_MSG_LN("riga 3:" + Display[i].riga_3);
        EEPROM_Display_write(i);
        Stato_DIS(i);
      }
      if ((char)message[0] == '4' and (char)message[1] == '=' ) {
        String stringtmp = (String)message;
        stringtmp = stringtmp.substring(2);
        Display[i].riga_4 = stringtmp.substring(0, EEPROM_Display_len);
        Debug_MSG_LN("riga 4:" + Display[i].riga_4);
        EEPROM_Display_write(i);
        Stato_DIS(i);
      }
      if ((String)message == Display_STATO) {
        Stato_DIS(i);
      }
    }
  }
}

void loop_Display() {
  display_D.clear();
  display_D.setFont(ArialMT_Plain_16);
  display_D.setTextAlignment(TEXT_ALIGN_LEFT);
  display_D.drawString(0, 0,  Display[0].riga_1);
  display_D.drawString(0, 16, Display[0].riga_2);
  display_D.drawString(0, 32, Display[0].riga_3);
  display_D.drawString(0, 48, Display[0].riga_4);
  display_D.display();
}

void EEPROM_Display_write(int i) {
  Debug_MSG_LN("Writing EEPROM Display " + String(i + 1));
  Debug_MSG("Riga 1:");
  Debug_MSG_LN(Display[i].riga_1);
  eepromWriteString((int)EEPROM_Display, EEPROM_Display_len, (char*) Display[i].riga_1.c_str());
  Debug_MSG("Riga 2:");
  Debug_MSG_LN(Display[i].riga_2);
  eepromWriteString((int)EEPROM_Display + EEPROM_Display_len, EEPROM_Display_len, (char*) Display[i].riga_2.c_str());
  Debug_MSG("Riga 3:");
  Debug_MSG_LN(Display[i].riga_3);
  eepromWriteString((int)EEPROM_Display + EEPROM_Display_len + EEPROM_Display_len, EEPROM_Display_len, (char*) Display[i].riga_3.c_str());
  Debug_MSG("Riga 4:");
  Debug_MSG_LN(Display[i].riga_4);
  eepromWriteString((int)EEPROM_Display + EEPROM_Display_len + EEPROM_Display_len + EEPROM_Display_len, EEPROM_Display_len, (char*) Display[i].riga_4.c_str());
  EEPROM.commit();
}
void EEPROM_Display_read(int i) {
  Debug_MSG_LN("Reading EEPROM Display " + String(i + 1));
  char buf[EEPROM_Display_len];
  eepromReadString((int)EEPROM_Display, EEPROM_Display_len, buf);
  Display[i].riga_1 = (String) buf;
  Debug_MSG("Riga 1:");
  Debug_MSG_LN(Display[i].riga_1);
  eepromReadString((int)EEPROM_Display + EEPROM_Display_len, EEPROM_Display_len, buf);
  Display[i].riga_2 = (String) buf;
  Debug_MSG("Riga 2:");
  Debug_MSG_LN(Display[i].riga_2);
  eepromReadString((int)EEPROM_Display + EEPROM_Display_len + EEPROM_Display_len, EEPROM_Display_len, buf);
  Display[i].riga_3 = (String) buf;
  Debug_MSG("Riga 3:");
  Debug_MSG_LN(Display[i].riga_3);
  eepromReadString((int)EEPROM_Display + EEPROM_Display_len + EEPROM_Display_len + EEPROM_Display_len, EEPROM_Display_len, buf);
  Display[i].riga_4 = (String) buf;
  Debug_MSG("Riga 4:");
  Debug_MSG_LN(Display[i].riga_4);
}
#endif
