#include "SSD1306.h"

long EEPROM_Display = 350;
int EEPROM_Display_len = 14;

String Display_DEF     = "DIS";
String Display_STATO   = "stato";
String Display_Topic_ACK_suffisso = "/ack";

typedef struct {
  String topic;
  String topic_ACK;
  String riga_1;
  String riga_2;
  String riga_3;
  int i2c;
  int sda;
  int sdc;
} InfoRecordDisplayType ;

InfoRecordDisplayType Display[1];

SSD1306 display_D(Display1_I2C_DISPLAY_ADDRESS, Display1_GPIO_SDA, Display1_GPIO_SDC);

void setup_Display() {
  Display[0].topic = Display1_Topic;
  Display[0].topic_ACK = Display1_Topic;
  Display[0].topic_ACK += Display_Topic_ACK_suffisso;
  Display[0].i2c = Display1_I2C_DISPLAY_ADDRESS;
  Display[0].sda = Display1_GPIO_SDA;
  Display[0].sdc = Display1_GPIO_SDC;
  for (int i = 0; i < Display_nodi; i++) {
    Debug_MSG_LN("- Display " + String(i + 1) + " : " + String(Display[i].topic));
    if (i < 1) {
      Debug_MSG_LN("SSD1306 ADDRESS Display " + String(i + 1) + " = 0x"   + String(Display[i].i2c, HEX));
      Debug_MSG_LN("SSD1306 SDA Display " + String(i + 1) + " = GPIO" + String(Display[i].sda));
      Debug_MSG_LN("SSD1306 SDC Display " + String(i + 1) + " = GPIO" + String(Display[i].sdc));
    }
    EEPROM_Display_read(i);
  }
  Debug_MSG_LN("Inizializzazione il display");
  display_D.init();
  display_D.flipScreenVertically();
  display_D.clear();
  display_D.setFont(ArialMT_Plain_16);
  display_D.setTextAlignment(TEXT_ALIGN_LEFT);
  String s;
  s = "SmartHome v";
  s += Versione;
  display_D.drawString(0, 0, s);
  display_D.setFont(ArialMT_Plain_10);
  display_D.drawString(0, 20, "by roncoa@gmail.com");
  display_D.display();
  delay(1000);
  refresh_Display();
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
    client.publish(ACK_Topic, (char*) payload.c_str());
    delay(250);
    Stato_DIS(i);
  }
}
void Alive_Display() {
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
  payload += "\"}";
  client.publish((char*) Display[i].topic_ACK.c_str(), (char*) payload.c_str());
  delay(250);
}
void Subscribe_Display() {
  for (int i = 0; i < Display_nodi; i++) {
    client.subscribe((char*) Display[i].topic.c_str());
    delay(250);
    Debug_MSG_LN("MQTT subscribe " + String(  Display[i].topic));
  }
}
void refresh_Display() {
  display_D.clear();
  display_D.setFont(ArialMT_Plain_16);
  display_D.setTextAlignment(TEXT_ALIGN_LEFT);
  display_D.drawString(0, 0, Display[0].riga_1);
  display_D.drawString(0, 27, Display[0].riga_2);
  display_D.drawString(0, 45, Display[0].riga_3);
  display_D.display();
}
void callback_Display(char* topic, byte * message, unsigned int length) {
  String payload;
  String message_String;
  for (int i = 0; i < Display_nodi; i++) {
    if (String(topic) ==   Display[i].topic) {
      message_String = "";
      for (int ii = 0; ii < length; ii++) {
        message_String += (char)message[ii];
      }
      if ((char)message[0] == '1' & (char)message[1] == '=' ) {
        String stringtmp = "";
        for (int i = 2; i < length; i++)  {
          stringtmp = stringtmp + (char)message[i];
        }
        Display[i].riga_1 = stringtmp.substring(0,EEPROM_Display_len);
        Debug_MSG_LN("riga 1:" + Display[i].riga_1);
        EEPROM_Display_write(i);
        Stato_DIS(i);
      }
      if ((char)message[0] == '2' & (char)message[1] == '=' ) {
        String stringtmp = "";
        for (int i = 2; i < length; i++)  {
          stringtmp = stringtmp + (char)message[i];
        }
        Display[i].riga_2 = stringtmp.substring(0,EEPROM_Display_len);
        Debug_MSG_LN("riga 2:" + Display[i].riga_2 );
        EEPROM_Display_write(i);
        Stato_DIS(i);
      }
      if ((char)message[0] == '3' & (char)message[1] == '=' ) {
        String stringtmp = "";
        for (int i = 2; i < length; i++)  {
          stringtmp = stringtmp + (char)message[i];
        }
        Display[i].riga_3 = stringtmp.substring(0,EEPROM_Display_len);
        Debug_MSG_LN("riga 3:" + Display[i].riga_3);
        EEPROM_Display_write(i);
        Stato_DIS(i);
      }
      if (message_String == Display_STATO) {
        Stato_DIS(i);
      }
      refresh_Display();
    }
  }
}
void loop_Display() {
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
  eepromWriteString((int)EEPROM_Display + EEPROM_Display_len+EEPROM_Display_len, EEPROM_Display_len, (char*) Display[i].riga_3.c_str());
  EEPROM.commit();
}
void EEPROM_Display_read(int i) {
  Debug_MSG_LN("Reading EEPROM Display " + String(i + 1));
  char buf[9];
  eepromReadString((int)EEPROM_Display, 10, buf);
  Display[i].riga_1 = (String) buf;
  Debug_MSG("Riga 1:");
  Debug_MSG_LN(Display[i].riga_1);
  eepromReadString((int)EEPROM_Display + EEPROM_Display_len, EEPROM_Display_len, buf);
  Display[i].riga_2 = (String) buf;
  Debug_MSG("Riga 2:");
  Debug_MSG_LN(Display[i].riga_2);
  eepromReadString((int)EEPROM_Display + EEPROM_Display_len+EEPROM_Display_len, EEPROM_Display_len, buf);
  Display[i].riga_3 = (String) buf;
  Debug_MSG("Riga 3:");
  Debug_MSG_LN(Display[i].riga_3);
}
void eepromReadString(int offset, int bytes, char *buf) {
  char c = 0;
  for (int i = offset; i < (offset + bytes); i++) {
    c = EEPROM.read(i);
    buf[i - offset] = c;
    if (c == 0) break;
  }
}
void eepromWriteString(int offset, int bytes, char *buf) {
  char c = 0;
  for (int i = 0; i < bytes; i++) {
    c = buf[i];
    EEPROM.write(offset + i, c);
  }
}
