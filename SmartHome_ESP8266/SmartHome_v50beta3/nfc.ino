#include <SPI.h>
#include "MFRC522.h"

/* wiring the MFRC522 to ESP8266 (ESP-12)
  RST     = GPIO5
  SDA(SS) = GPIO4
  MOSI    = GPIO13
  MISO    = GPIO12
  SCK     = GPIO14
  GND     = GND
  3.3V    = 3.3V
*/

MFRC522 mfrc522(Impulso1_GPIO_SS, Impulso1_GPIO_RST); // Create MFRC522 instance

void setup_nfc() {
#if defined(Impulso1_NFC)
  SPI.begin();           // Init SPI bus
  mfrc522.PCD_Init();    // Init MFRC522
#endif
}

void loop_nfc() {
#if defined(Impulso1_NFC)
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    delay(50);
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }
  // Show some details of the PICC (that is: the tag/card)
  Debug_MSG("Card UID:");
  String UID, EEPROMUID;
  EEPROMUID = "";
  UID = dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Debug_MSG_LN(UID);
  delay(250);
  for (int i = 400; i < 500; i = i + 4) {
    EEPROMUID = "";
    EEPROMUID += String(EEPROM.read(i) < 0x10 ? "0" : "");
    EEPROMUID += String(EEPROM.read(i), HEX);
    EEPROMUID += String(EEPROM.read(i + 1) < 0x10 ? "0" : "");
    EEPROMUID += String(EEPROM.read(i + 1), HEX);
    EEPROMUID += String(EEPROM.read(i + 2) < 0x10 ? "0" : "");
    EEPROMUID += String(EEPROM.read(i + 2), HEX);
    EEPROMUID += String(EEPROM.read(i + 3) < 0x10 ? "0" : "");
    EEPROMUID += String(EEPROM.read(i + 3), HEX);
    Debug_MSG("Key ");
    Debug_MSG(String((i - 96) / 4));
    Debug_MSG(" : ");
    Debug_MSG(EEPROMUID);
    if (UID == EEPROMUID) {
      Debug_MSG_LN(" accesso OK!");
      digitalWrite(Impulso1_GPIO_rele, 1 ^ Flag_inversione_RELE);                                     // Accendo il rele
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_ON();
#endif
#if defined(ESP01_SERIAL_RELE)
      ESP01_SERIAL_RELE_AON();
#endif
      Rele_eccitato_Impulso1 = true;
      ulTimeReleImpulso1 = millis();
      return;
    }
    else {
      Debug_MSG_LN(" accesso negato!");
    }
  }
  delay(1000);
#endif
}

void eraseAllKey() {
  Debug_MSG("Erasing all key ");
  for (int i = 400; i < 500; i = i + 1) {
    Debug_MSG(".");
    EEPROM.write(i, 255);
    EEPROM.commit();
  }
  Debug_MSG_LN("");
  Debug_MSG("Done!");
}

void addKey(String UID) {
  for (int i = 400; i < 500; i = i + 4) {
    String EEPROMUID;
    EEPROMUID = "";
    EEPROMUID += String(EEPROM.read(i), HEX);
    EEPROMUID += String(EEPROM.read(i + 1), HEX);
    EEPROMUID += String(EEPROM.read(i + 2), HEX);
    EEPROMUID += String(EEPROM.read(i + 3), HEX);
    if (EEPROMUID == UID) {
      Debug_MSG_LN("Key già  presente");
      return;
    }
    if (EEPROMUID == "ffffffff" or EEPROMUID == "0000") {
      char cc[1];
      cc[0] = UID.charAt(0);
      cc[1] = UID.charAt(1);
      EEPROM.write(i, strtoul(cc, NULL, 16 ));
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
      Debug_MSG_LN("Key scritta!");
      return;
    }
  }
  Debug_MSG_LN("Memoria key full");
}

void eraseKey(String UID) {
  for (int i = 400; i < 500; i = i + 4) {
    String EEPROMUID;
    EEPROMUID = "";
    EEPROMUID += String(EEPROM.read(i), HEX);
    EEPROMUID += String(EEPROM.read(i + 1), HEX);
    EEPROMUID += String(EEPROM.read(i + 2), HEX);
    EEPROMUID += String(EEPROM.read(i + 3), HEX);
    if (EEPROMUID == UID) {
      EEPROM.write(i, 255);
      EEPROM.write(i + 1, 255);
      EEPROM.write(i + 2, 255);
      EEPROM.write(i + 3, 255);
      EEPROM.commit();
      Debug_MSG_LN("Key cancellata!");
    }
  }
}

// Helper routine to dump a byte array as hex values to Serial
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
  unsigned long ulTimeNFC;
  ulTimeNFC = millis();
  String UID;
  Debug_MSG_LN("Aggiungo key");
  while ( millis() < ulTimeNFC + 10000) {
    UID = dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    delay(100);
    if (mfrc522.PICC_IsNewCardPresent() and mfrc522.PICC_ReadCardSerial()) {
      addKey( UID);
      Debug_MSG("Key scritta: ");
      Debug_MSG_LN(UID);
      return;
    }
  }
  Debug_MSG_LN("Aggiungo key timeout");
}

void menokey() {
  unsigned long ulTimeNFC;
  ulTimeNFC = millis();
  String UID;
  Debug_MSG_LN("Impulso key");
  while ( millis() < ulTimeNFC + 10000) {
    UID = dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    delay(100);
    if (mfrc522.PICC_IsNewCardPresent() and mfrc522.PICC_ReadCardSerial()) {
      eraseKey( UID);
      Debug_MSG("Key cancellata: ");
      Debug_MSG_LN(UID);
      return;
    }
  }
  Debug_MSG_LN("Impulso key timeout");
}

void clearkey() {
  eraseAllKey();
}
