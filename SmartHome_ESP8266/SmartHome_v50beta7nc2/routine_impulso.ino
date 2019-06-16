#if Impulso_nodi>0

#define Impulso_TEMPO_CLICK_ON           TEMPO_CLICK_ON
#define Impulso_TEMPO_CLICK_OFF          TEMPO_CLICK_OFF

//#define Impulso_mantieni_click              // se abilitato(non commentato) mantiene il relè eccitato duante la pressione del bottone e/o tag vicino

unsigned long Impulso_TEMPO_IMPULSO = 500;  // Indica la durata dell'impulso al rele' (in millisecondi)

String Impulso_DEF     = "IMP";
String Impulso_ON      = "on";
String Impulso_STATO   = "stato";
String Impulso_ACK_ON  = "on";
String Impulso_ACK_OFF = "off";
String Impulso_Topic_ACK_suffisso = "/ack";

typedef struct {
  String topic;
  String topic_ACK;
  int rele;
  int bottone;
  unsigned long ulTime;
  boolean rele_eccitato;
} InfoRecordImpulsoType ;

InfoRecordImpulsoType Impulso[10];

TPush Impulso_TPush_bottone[Impulso_nodi];
Ticker piukeyTimer, menokeyTimer;

boolean piukey_flag = false;
boolean menokey_flag = false;

void setup_Impulso() {
  Impulso[0].topic = Impulso1_Topic;
  Impulso[0].topic_ACK = Impulso1_Topic;
  Impulso[0].topic_ACK += Impulso_Topic_ACK_suffisso;
  Impulso[0].rele = Impulso1_GPIO_rele;
#ifdef Impulso1_GPIO_bottone
  Impulso[0].bottone = Impulso1_GPIO_bottone;
#else
  Impulso[0].bottone = 100;
#endif
  Impulso[0].rele_eccitato = false;

  Impulso[1].topic = Impulso2_Topic;
  Impulso[1].topic_ACK = Impulso2_Topic;
  Impulso[1].topic_ACK += Impulso_Topic_ACK_suffisso;
  Impulso[1].rele = Impulso2_GPIO_rele;
#ifdef Impulso2_GPIO_bottone
  Impulso[1].bottone = Impulso2_GPIO_bottone;
#else
  Impulso[1].bottone = 100;
#endif
  Impulso[1].rele_eccitato = false;

  Impulso[2].topic = Impulso3_Topic;
  Impulso[2].topic_ACK = Impulso3_Topic;
  Impulso[2].topic_ACK += Impulso_Topic_ACK_suffisso;
  Impulso[2].rele = Impulso3_GPIO_rele;
#ifdef Impulso3_GPIO_bottone
  Impulso[2].bottone = Impulso3_GPIO_bottone;
#else
  Impulso[2].bottone = 100;
#endif
  Impulso[2].rele_eccitato = false;

  Impulso[3].topic = Impulso4_Topic;
  Impulso[3].topic_ACK = Impulso4_Topic;
  Impulso[3].topic_ACK += Impulso_Topic_ACK_suffisso;
  Impulso[3].rele = Impulso4_GPIO_rele;
#ifdef Impulso4_GPIO_bottone
  Impulso[3].bottone = Impulso4_GPIO_bottone;
#else
  Impulso[3].bottone = 100;
#endif
  Impulso[3].rele_eccitato = false;

  Impulso[4].topic = Impulso5_Topic;
  Impulso[4].topic_ACK = Impulso5_Topic;
  Impulso[4].topic_ACK += Impulso_Topic_ACK_suffisso;
  Impulso[4].rele = Impulso5_GPIO_rele;
#ifdef Impulso5_GPIO_bottone
  Impulso[4].bottone = Impulso5_GPIO_bottone;
#else
  Impulso[4].bottone = 100;
#endif
  Impulso[4].rele_eccitato = false;

  Impulso[5].topic = Impulso6_Topic;
  Impulso[5].topic_ACK = Impulso6_Topic;
  Impulso[5].topic_ACK += Impulso_Topic_ACK_suffisso;
  Impulso[5].rele = Impulso6_GPIO_rele;
#ifdef Impulso6_GPIO_bottone
  Impulso[5].bottone = Impulso6_GPIO_bottone;
#else
  Impulso[5].bottone = 100;
#endif
  Impulso[5].rele_eccitato = false;

  Impulso[6].topic = Impulso7_Topic;
  Impulso[6].topic_ACK = Impulso7_Topic;
  Impulso[6].topic_ACK += Impulso_Topic_ACK_suffisso;
  Impulso[6].rele = Impulso7_GPIO_rele;
#ifdef Impulso7_GPIO_bottone
  Impulso[6].bottone = Impulso7_GPIO_bottone;
#else
  Impulso[6].bottone = 100;
#endif
  Impulso[6].rele_eccitato = false;

  Impulso[7].topic = Impulso8_Topic;
  Impulso[7].topic_ACK = Impulso8_Topic;
  Impulso[7].topic_ACK += Impulso_Topic_ACK_suffisso;
  Impulso[7].rele = Impulso8_GPIO_rele;
#ifdef Impulso8_GPIO_bottone
  Impulso[7].bottone = Impulso8_GPIO_bottone;
#else
  Impulso[7].bottone = 100;
#endif
  Impulso[7].rele_eccitato = false;

  Impulso[8].topic = Impulso9_Topic;
  Impulso[8].topic_ACK = Impulso9_Topic;
  Impulso[8].topic_ACK += Impulso_Topic_ACK_suffisso;
  Impulso[8].rele = Impulso9_GPIO_rele;
#ifdef Impulso9_GPIO_bottone
  Impulso[8].bottone = Impulso9_GPIO_bottone;
#else
  Impulso[8].bottone = 100;
#endif
  Impulso[8].rele_eccitato = false;

  Impulso[9].topic = Impulso10_Topic;
  Impulso[9].topic_ACK = Impulso10_Topic;
  Impulso[9].topic_ACK += Impulso_Topic_ACK_suffisso;
  Impulso[9].rele = Impulso10_GPIO_rele;
#ifdef Impulso10_GPIO_bottone
  Impulso[9].bottone = Impulso10_GPIO_bottone;
#else
  Impulso[9].bottone = 100;
#endif
  Impulso[9].rele_eccitato = false;

  for (int i = 0; i < Impulso_nodi; i++) {
    Debug_MSG_LN("Nodo Impulso " + String(i + 1) + " : " + Impulso[i].topic);
    Debug_MSG_LN(" RELE    Impulso " + String(i + 1) + "    = GPIO" + Impulso[i].rele);
    if (Impulso[i].bottone >= 99) {
      Debug_MSG_LN(" BOTTONE Impulso " + String(i + 1) + " : Nessun GPIO definito!");
    }
    else {
      Debug_MSG_LN(" BOTTONE Impulso " + String(i + 1) + "    = GPIO" + Impulso[i].bottone);
    }
#ifdef PullDown
    Impulso_TPush_bottone[i].setUp(Impulso[i].bottone, HIGH);
#endif
#ifdef PullUp
    Impulso_TPush_bottone[i].setUp(Impulso[i].bottone, LOW);
#endif
    pinMode(Impulso[i].rele, OUTPUT);
    digitalWrite(Impulso[i].rele, 0 ^ Flag_inversione_RELE);
#ifdef Impulso1_NFC
    if (i == 0) {
      Debug_MSG_LN(" NFC RST              = GPIO" + String(Impulso1_GPIO_RST));
      Debug_MSG_LN(" NFC SDA(SS)          = GPIO" + String(Impulso1_GPIO_SS));
      Debug_MSG_LN(" NFC MOSI             = GPIO" + String(Impulso1_GPIO_MOSI));
      Debug_MSG_LN(" NFC MISO             = GPIO" + String(Impulso1_GPIO_MISO));
      Debug_MSG_LN(" NFC SCK              = GPIO" + String(Impulso1_GPIO_SCK));
      setup_nfc();
    }
#endif
  }
}

void ACK_Impulso() {
  String payload;
  for (int i = 0; i < Impulso_nodi; i++) {
    payload = "{\"MAC_address\":\"";
    payload += macToStr(mac);
    payload += "\",\"NODO\":\"";
    payload += Impulso_DEF;
    payload += "\",\"Topic\":\"";
    payload += Impulso[i].topic;
    payload += "\",\"Topic_ACK\":\"";
    payload += Impulso[i].topic_ACK;
    payload += "\",\"availability_topic\":\"";
    payload += WillTopicStr;
    payload += "\"}";
    MQTTPublish(ACK_Topic, payload);
    Stato_IMP(i);
  }
}

void HA_discovery_Impulso() {
  String HAtopic;
  String HApayload;
  for (int i = 0; i < Impulso_nodi; i++) {
    Debug_MSG_LN("Creo nodo " + (String)Impulso[i].topic + " Home assistant discovery ");
    HAtopic = "homeassistant/switch/";
    HAtopic += macToStr(mac);
    HAtopic += "/";
    HAtopic += Impulso_DEF;
    HAtopic += i;
    HAtopic += "/config";
    HApayload = "{\"name\":\"";
    HApayload += Impulso[i].topic;
    HApayload += "\",\"command_topic\":\"";
    HApayload += Impulso[i].topic;
    HApayload += "\",\"payload_on\":\"";
    HApayload += "on";
    HApayload += "\",\"payload_off\":\"";
    HApayload += "off";
    HApayload += "\",\"state_topic\":\"";
    HApayload += Impulso[i].topic_ACK;
    HApayload += "\",\"state_on\":\"";
    HApayload += "on";
    HApayload += "\",\"state_off\":\"";
    HApayload += "off";
    HApayload += "\",\"value_template\":\"";
    HApayload += "{{value_json.STATO}}";
    HApayload += "\",\"availability_topic\":\"";
    HApayload += WillTopicStr;
    HApayload += "\"}";
    MQTTPublish(HAtopic, HApayload);
  }
}

void Stato_IMP(int i) {
  String payload;
  payload = "{\"NODO\":\"";
  payload += Impulso_DEF;
  payload += "\",\"STATO\":\"";
  if (digitalRead(Impulso[i].rele) ^ (Flag_inversione_RELE == true )) payload += Impulso_ACK_ON;
  if (digitalRead(Impulso[i].rele) ^ (Flag_inversione_RELE == false )) payload += Impulso_ACK_OFF;
  payload += "\"}";
  MQTTPublish(Impulso[i].topic_ACK, payload);
}

void Subscribe_Impulso() {
  String payload;
  for (int i = 0; i < Impulso_nodi; i++) {
    MQTTSubscribe(Impulso[i].topic);                  // Sottoscrivi Impulso1_Topic
    Debug_MSG_LN("MQTT subscribe " + String(Impulso[i].topic));
  }
}

void callback_Impulso(char* topic, char* message) {
  String payload;
  for (int i = 0; i < Impulso_nodi; i++) {
    if (String(topic) == Impulso[i].topic) {                                              // se arriva il comando sul topic "Impulso1_Topic"
      if ((String)message == Impulso_ON) {                  // Topic "Impulso1_Topic" = "on"
        digitalWrite(Impulso[i].rele, 1 ^ Flag_inversione_RELE);                                     // Accendo il rele
#ifdef SONOFFDUAL
        if (i = 0) SONOFFDUAL_A_ON();
        if (i = 1) SONOFFDUAL_B_ON();
#endif
#ifdef ESP01_SERIAL_RELE
        ESP01_SERIAL_RELE_A_ON();
#endif
        Impulso[i].rele_eccitato = true;
        Impulso[i].ulTime = millis();
        Stato_IMP(i);
      }
      if ((String)message == Impulso_STATO) {                                      // Topic "Impulso1_Topic" = "stato"
        Stato_IMP(i);
      }
#ifdef Impulso1_NFC
      if ((char)message[0] == '+' and (char)message[1] == 'k' and (char)message[2] == 'e' and (char)message[3] == 'y') {    // Topic "Impulso1_Topic" = "+key"
        piukey();
      }
      if ((char)message[0] == '-' and (char)message[1] == 'k' and (char)message[2] == 'e' and (char)message[3] == 'y') {    // Topic "Impulso1_Topic" = "-key"
        menokey();
      }
      if ((char)message[0] == 'X' and (char)message[1] == 'k' and (char)message[2] == 'e' and (char)message[3] == 'y') {    // Topic "Impulso1_Topic" = "Xkey"
        clearkey();
      }
#endif
    }
  }
}

void loop_Impulso() {
  String payload;

  for (int i = 0; i < Impulso_nodi; i++) {
    if (((millis() - Impulso[i].ulTime) > Impulso_TEMPO_IMPULSO) and Impulso[i].rele_eccitato) {        // Tempo rele -> spengo!
      Debug_MSG_LN("Tempo Impulso " + String(i + 1) + " -> spengo!");
      digitalWrite(Impulso[i].rele, 0 ^ Flag_inversione_RELE);                     // Spengo il rele
#ifdef SONOFFDUAL
      if (i = 0) SONOFFDUAL_A_OFF();;
      if (i = 1) SONOFFDUAL_B_OFF();
#endif
#ifdef ESP01_SERIAL_RELE
      ESP01_SERIAL_RELE_A_OFF();
#endif
      Impulso[i].rele_eccitato = false;
      Stato_IMP(i);
    }
    if (Impulso_TPush_bottone[i].Click(Impulso_TEMPO_CLICK_ON) and Impulso[i].bottone != 100) {                                // Bottone premuto!
      Debug_MSG_LN("Bottone Impulso " + String(i + 1) + " premuto!");
      digitalWrite(Impulso[i].rele, 1 ^ Flag_inversione_RELE);
#ifdef SONOFFDUAL
      if (i = 0) SONOFFDUAL_A_ON();
      if (i = 1) SONOFFDUAL_B_ON();
#endif
#ifdef ESP01_SERIAL_RELE
      ESP01_SERIAL_RELE_A_ON();
#endif
#ifndef Impulso_mantieni_click
      Impulso[i].rele_eccitato = true;
      Impulso[i].ulTime = millis();
      Stato_IMP(i);
#endif
    }
#ifdef Impulso_mantieni_click
    if (t = Impulso_TPush_bottone[i].Keep(Impulso_TEMPO_CLICK_OFF)) {
      Impulso[i].rele_eccitato = true;
      Impulso[i].ulTime = millis();
      Stato_IMP(i);
    }
#endif
  }
#ifdef Impulso1_NFC
  loop_nfc();
#endif
}
#endif

// ******************* NFC  ********************
#ifdef Impulso1_NFC
#include <SPI.h>
#include "MFRC522.h"

MFRC522 mfrc522(Impulso1_GPIO_SS, Impulso1_GPIO_RST);

void setup_nfc() {
  SPI.begin();           // Init SPI bus
  mfrc522.PCD_Init();    // Init MFRC522
}

void loop_nfc() {
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;
  Debug_MSG("Card UID:");
  String UID, EEPROMUID;
  EEPROMUID = "";
  UID = dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Debug_MSG_LN(UID);

#ifndef Impulso_mantieni_click
  mfrc522.PICC_HaltA();
#endif
  mfrc522.PCD_StopCrypto1();

  if (piukey_flag == true) {
    piukeyTimer.detach();
    piukey_flag = false;
    addKey(UID);
    return;
  }

  if (menokey_flag == true) {
    menokeyTimer.detach();
    menokey_flag = false;
    eraseKey(UID);
    return;
  }

  for (int i = EEPROM_NFC; i < EEPROM_NFC + 100; i = i + 4) {
    EEPROMUID = "";
    EEPROMUID += String(EEPROM.read(i + 0) < 0x10 ? "0" : "");
    EEPROMUID += String(EEPROM.read(i + 0), HEX);
    EEPROMUID += String(EEPROM.read(i + 1) < 0x10 ? "0" : "");
    EEPROMUID += String(EEPROM.read(i + 1), HEX);
    EEPROMUID += String(EEPROM.read(i + 2) < 0x10 ? "0" : "");
    EEPROMUID += String(EEPROM.read(i + 2), HEX);
    EEPROMUID += String(EEPROM.read(i + 3) < 0x10 ? "0" : "");
    EEPROMUID += String(EEPROM.read(i + 3), HEX);
    if (UID == EEPROMUID) {
      Debug_MSG_LN("Key " + String(((i - EEPROM_NFC) / 4) + 1) + " : " + EEPROMUID + " accesso OK!");
      digitalWrite(Impulso[0].rele, 1 ^ Flag_inversione_RELE);
#ifdef SONOFFDUAL
      SONOFFDUAL_A_ON();
#endif
#ifdef ESP01_SERIAL_RELE
      ESP01_SERIAL_RELE_AON();
#endif
      Impulso[0].rele_eccitato = true;
      Impulso[0].ulTime = millis();
      Stato_IMP(0);
      return;
    }
  }
  Debug_MSG_LN("Accesso negato!");
}

void eraseAllKey() {
  Debug_MSG("Erasing all key ");
  for (int i = EEPROM_NFC; i < EEPROM_NFC + 100; i = i + 1) {
    Debug_MSG(".");
    EEPROM.write(i, 255);
    EEPROM.commit();
  }
  Debug_MSG_LN("");
  Debug_MSG("Done!");
}

void addKey(String UID) {
  for (int i = EEPROM_NFC; i < EEPROM_NFC + 100; i = i + 4) {
    String EEPROMUID;
    EEPROMUID = "";
    EEPROMUID += String(EEPROM.read(i + 0) < 0x10 ? "0" : "");
    EEPROMUID += String(EEPROM.read(i + 0), HEX);
    EEPROMUID += String(EEPROM.read(i + 1) < 0x10 ? "0" : "");
    EEPROMUID += String(EEPROM.read(i + 1), HEX);
    EEPROMUID += String(EEPROM.read(i + 2) < 0x10 ? "0" : "");
    EEPROMUID += String(EEPROM.read(i + 2), HEX);
    EEPROMUID += String(EEPROM.read(i + 3) < 0x10 ? "0" : "");
    EEPROMUID += String(EEPROM.read(i + 3), HEX);
    if (EEPROMUID == UID) {
      Debug_MSG_LN("Key " + String(((i - EEPROM_NFC) / 4) + 1) + ": " + EEPROMUID + " -> gia presente");
      return;
    }
  }

  boolean flag = true;
  for (int i = EEPROM_NFC; i < EEPROM_NFC + 100; i = i + 4) {
    String EEPROMUID;
    EEPROMUID = "";
    EEPROMUID += String(EEPROM.read(i + 0) < 0x10 ? "0" : "");
    EEPROMUID += String(EEPROM.read(i + 0), HEX);
    EEPROMUID += String(EEPROM.read(i + 1) < 0x10 ? "0" : "");
    EEPROMUID += String(EEPROM.read(i + 1), HEX);
    EEPROMUID += String(EEPROM.read(i + 2) < 0x10 ? "0" : "");
    EEPROMUID += String(EEPROM.read(i + 2), HEX);
    EEPROMUID += String(EEPROM.read(i + 3) < 0x10 ? "0" : "");
    EEPROMUID += String(EEPROM.read(i + 3), HEX);
    if (EEPROMUID == UID) {
      flag = false;
    }
    else {
      if ((EEPROMUID == "ffffffff" or EEPROMUID == "00000000") and flag) {
        char cc[1];
        cc[0] = UID.charAt(0);
        cc[1] = UID.charAt(1);
        EEPROM.write(i + 0, strtoul(cc, NULL, 16 ));
        cc[0] = UID.charAt(2);
        cc[1] = UID.charAt(3);
        EEPROM.write(i + 1, strtoul(cc, NULL, 16 ));
        cc[0] = UID.charAt(4);
        cc[1] = UID.charAt(5);
        EEPROM.write(i + 2, strtoul(cc, NULL, 16 ));
        cc[0] = UID.charAt(6);
        cc[1] = UID.charAt(7);
        EEPROM.write(i + 3, strtoul(cc, NULL, 16 ));
        EEPROM.commit();
        Debug_MSG_LN("Key " + String(((i - EEPROM_NFC) / 4) + 1) + ": " + UID + " -> scritta!");
        flag = false;
      }
      else {
        Debug_MSG_LN("Key " + String(((i - EEPROM_NFC) / 4) + 1) + ": " + EEPROMUID);
      }
    }
  }
  if (flag) Debug_MSG_LN("Memoria key full");
}

void eraseKey(String UID) {
  boolean flag = true;
  for (int i = EEPROM_NFC; i < EEPROM_NFC + 100; i = i + 4) {
    String EEPROMUID;
    EEPROMUID = "";
    EEPROMUID += String(EEPROM.read(i + 0) < 0x10 ? "0" : "");
    EEPROMUID += String(EEPROM.read(i + 0), HEX);
    EEPROMUID += String(EEPROM.read(i + 1) < 0x10 ? "0" : "");
    EEPROMUID += String(EEPROM.read(i + 1), HEX);
    EEPROMUID += String(EEPROM.read(i + 2) < 0x10 ? "0" : "");
    EEPROMUID += String(EEPROM.read(i + 2), HEX);
    EEPROMUID += String(EEPROM.read(i + 3) < 0x10 ? "0" : "");
    EEPROMUID += String(EEPROM.read(i + 3), HEX);
    Debug_MSG("Key " + String(((i - EEPROM_NFC) / 4) + 1) + ": " + EEPROMUID);
    if (EEPROMUID == UID) {
      EEPROM.write(i, 255);
      EEPROM.write(i + 1, 255);
      EEPROM.write(i + 2, 255);
      EEPROM.write(i + 3, 255);
      EEPROM.commit();
      flag = false;
      Debug_MSG_LN(" -> ffffffff (cancellata!)");
    }
    else {
      Debug_MSG_LN("");
    }
  }
  if (flag) Debug_MSG_LN("Key non presente!");
}

String dump_byte_array(byte *buffer, byte bufferSize) {
  String s = "";
  for (byte i = 0; i < bufferSize; i++) {
    s += String(buffer[i] < 0x10 ? "0" : "");
    s += String(buffer[i], HEX);
  }
  s.toLowerCase();
  return s;
}

void piukey() {
  Debug_MSG_LN("Aggiungo key:");
  piukeyTimer.detach();
  menokeyTimer.detach();
  piukeyTimer.once(10, piumenokeyTimeout);
  piukey_flag = true;
}

void menokey() {
  Debug_MSG_LN("Cancello key:");
  piukeyTimer.detach();
  menokeyTimer.detach();
  menokeyTimer.once(10, piumenokeyTimeout);
  menokey_flag = true;
}

void piumenokeyTimeout() {
  piukey_flag = false;
  menokey_flag = false;
  Debug_MSG_LN("timeout!");
}

void clearkey() {
  Debug_MSG_LN("Cancello tutte le key:");
  eraseAllKey();
}
#endif
