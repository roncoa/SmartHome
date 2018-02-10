void Debug_MSG(String s) {
#if defined(DEBUG)
  Serial.print(s);
#endif

#if defined(TELNET_DEBUG)
  byte sbuf[s.length() + 1];
  s.getBytes(sbuf, s.length() + 1);
  for (int i = 0; i < MAX_SRV_CLIENTS; i++) {
    if (serverClients[i] && serverClients[i].connected()) {
      serverClients[i].write(sbuf, s.length() + 1);
      delay(1);
    }
  }
#endif
}

void Debug_MSG_LN(String s) {
#if defined(DEBUG)
  Serial.println(s);
#endif

#if defined(TELNET_DEBUG)
  byte sbuf[s.length() + 1];
  byte CRLF[] = {0x0d, 0x0a};
  s.getBytes(sbuf, s.length() + 1);
  for (int i = 0; i < MAX_SRV_CLIENTS; i++) {
    if (serverClients[i] && serverClients[i].connected()) {
      serverClients[i].write(sbuf, s.length() + 1);
      serverClients[i].write(CRLF, 2);
      delay(1);
    }
  }
#endif
}

void setup_GPIO() {
  Debug_MSG(" ***** SmartHome v");
  Debug_MSG(Versione);
  Debug_MSG_LN(" *****");
  Debug_MSG_LN("");
  // Inizializza GPIO
  Debug_MSG_LN("Status LED             = GPIO" + String(Status_LED));
  pinMode(Status_LED, OUTPUT);                                  // Inizializza Status_LED
  digitalWrite(Status_LED, 0 ^ Flag_inversione_Status_LED);
  if (NODI_TAPPARELLA > 0) {
    Debug_MSG_LN("- Tapparella 1 : " + String(Tapparella1_Topic));
    Debug_MSG_LN("RELE    tapparella 1 SU  = GPIO" + String(RELE_tapparella1_SU));
    Debug_MSG_LN("RELE    tapparella 1 GIU = GPIO" + String(RELE_tapparella1_GIU));
    Debug_MSG_LN("BOTTONE tapparella 1 SU  = GPIO" + String(BOTTONE_tapparella1_SU));
    Debug_MSG_LN("BOTTONE tapparella 1 GIU = GPIO" + String(BOTTONE_tapparella1_GIU));
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
    Debug_MSG_LN("- Tapparella 2 : " + String(Tapparella2_Topic));
    Debug_MSG_LN("RELE    tapparella 2 SU  = GPIO" + String(RELE_tapparella2_SU));
    Debug_MSG_LN("RELE    tapparella 2 GIU = GPIO" + String(RELE_tapparella2_GIU));
    Debug_MSG_LN("BOTTONE tapparella 2 SU  = GPIO" + String(BOTTONE_tapparella2_SU));
    Debug_MSG_LN("BOTTONE tapparella 2 GIU = GPIO" + String(BOTTONE_tapparella2_GIU));
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
    Debug_MSG_LN("- Interruttore 1 : " + String(Interruttore1_Topic));
    Debug_MSG_LN("RELE A    interruttore 1 = GPIO" + String(RELE_A_interruttore1));
    Debug_MSG_LN("RELE B    interruttore 1 = GPIO" + String(RELE_B_interruttore1));
    Debug_MSG_LN("BOTTONE A interruttore 1 = GPIO" + String(BOTTONE_A_interruttore1));
    Debug_MSG_LN("BOTTONE B interruttore 1 = GPIO" + String(BOTTONE_B_interruttore1));
    pinMode(RELE_A_interruttore1, OUTPUT);
    digitalWrite(RELE_A_interruttore1, EEPROM_read_releAinterruttore1() ^ Flag_inversione_RELE);
    pinMode(RELE_B_interruttore1, OUTPUT);
    digitalWrite(RELE_B_interruttore1, EEPROM_read_releBinterruttore1() ^ Flag_inversione_RELE);
    if (EEPROM_read_releAinterruttore1() == 1) {
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_ON();
#endif
#if defined(ESP01_SERIAL_RELE)
      ESP01_SERIAL_RELE_AON();
#endif
    }
    else {
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_OFF();
#endif
#if defined(ESP01_SERIAL_RELE)
      ESP01_SERIAL_RELE_AOFF();
#endif
    }
    if (EEPROM_read_releBinterruttore1() == 1) {
#if defined(SONOFFDUAL)
      SONOFFDUAL_B_ON();
#endif
    }
    else {
#if defined(SONOFFDUAL)
      SONOFFDUAL_B_OFF();
#endif
    }
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
    Debug_MSG_LN("- Interruttore 2 : " + String(Interruttore1_Topic));
    Debug_MSG_LN("RELE A    interruttore 2 = GPIO" + String(RELE_A_interruttore2));
    Debug_MSG_LN("RELE B    interruttore 2 = GPIO" + String(RELE_B_interruttore2));
    Debug_MSG_LN("BOTTONE A interruttore 2 = GPIO" + String(BOTTONE_A_interruttore2));
    Debug_MSG_LN("BOTTONE B interruttore 2 = GPIO" + String(BOTTONE_B_interruttore2));
    pinMode(RELE_A_interruttore2, OUTPUT);
    digitalWrite(RELE_A_interruttore2, EEPROM_read_releAinterruttore2() ^ Flag_inversione_RELE);
    pinMode(RELE_B_interruttore2, OUTPUT);
    digitalWrite(RELE_B_interruttore2, EEPROM_read_releBinterruttore2() ^ Flag_inversione_RELE);
    if (EEPROM_read_releAinterruttore2() == 1) {
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_ON();
#endif
    }
    else {
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_OFF();
#endif
    }
    if (EEPROM_read_releBinterruttore2() == 1) {
#if defined(SONOFFDUAL)
      SONOFFDUAL_B_ON();
#endif
    }
    else {
#if defined(SONOFFDUAL)
      SONOFFDUAL_B_OFF();
#endif
    }
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
    Debug_MSG_LN("- Interruttore 3 : " + String(Interruttore1_Topic));
    Debug_MSG_LN("RELE A    interruttore 3 = GPIO" + String(RELE_A_interruttore3));
    Debug_MSG_LN("RELE B    interruttore 3 = GPIO" + String(RELE_B_interruttore3));
    Debug_MSG_LN("BOTTONE A interruttore 3 = GPIO" + String(BOTTONE_A_interruttore3));
    Debug_MSG_LN("BOTTONE B interruttore 3 = GPIO" + String(BOTTONE_B_interruttore3));
    pinMode(RELE_A_interruttore3, OUTPUT);
    digitalWrite(RELE_A_interruttore3, EEPROM_read_releAinterruttore3() ^ Flag_inversione_RELE);
    pinMode(RELE_B_interruttore3, OUTPUT);
    digitalWrite(RELE_B_interruttore3, EEPROM_read_releBinterruttore3() ^ Flag_inversione_RELE);
    if (EEPROM_read_releAinterruttore3() == 1) {
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_ON();
#endif
    }
    else {
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_OFF();
#endif
    }
    if (EEPROM_read_releBinterruttore3() == 1) {
#if defined(SONOFFDUAL)
      SONOFFDUAL_B_ON();
#endif
    }
    else {
#if defined(SONOFFDUAL)
      SONOFFDUAL_B_OFF();
#endif
    }
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
    Debug_MSG_LN("- Interruttore 4 : " + String(Interruttore1_Topic));
    Debug_MSG_LN("RELE A    interruttore 4 = GPIO" + String(RELE_A_interruttore4));
    Debug_MSG_LN("RELE B    interruttore 4 = GPIO" + String(RELE_B_interruttore4));
    Debug_MSG_LN("BOTTONE A interruttore 4 = GPIO" + String(BOTTONE_A_interruttore4));
    Debug_MSG_LN("BOTTONE B interruttore 4 = GPIO" + String(BOTTONE_B_interruttore4));
    pinMode(RELE_A_interruttore4, OUTPUT);
    digitalWrite(RELE_A_interruttore4, EEPROM_read_releAinterruttore4() ^ Flag_inversione_RELE);
    pinMode(RELE_B_interruttore4, OUTPUT);
    digitalWrite(RELE_B_interruttore4, EEPROM_read_releBinterruttore4() ^ Flag_inversione_RELE);
    if (EEPROM_read_releAinterruttore4() == 1) {
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_ON();
#endif
    }
    else {
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_OFF();
#endif
    }
    if (EEPROM_read_releBinterruttore4() == 1) {
#if defined(SONOFFDUAL)
      SONOFFDUAL_B_ON();
#endif
    }
    else {
#if defined(SONOFFDUAL)
      SONOFFDUAL_B_OFF();
#endif
    }
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
#if defined(CANCELLO_NFC)
    setup_nfc();
#endif
    Debug_MSG_LN("- Cancello 1 : " + String(Cancello1_Topic));
    Debug_MSG_LN("RELE A cancello 1        = GPIO" + String(RELE_A_cancello1));
    Debug_MSG_LN("RELE B cancello 1        = GPIO" + String(RELE_B_cancello1));
    Debug_MSG_LN("Pulsante A cancello 1    = GPIO" + String(BOTTONE_A_cancello1));
    Debug_MSG_LN("Pulsante B cancello 1    = GPIO" + String(BOTTONE_B_cancello1));
    Debug_MSG_LN("NFC RST                  = GPIO" + String(RST_PIN));
    Debug_MSG_LN("NFC SDA(SS)              = GPIO" + String(SS_PIN));
    Debug_MSG_LN("NFC MOSI                 = GPIO" + String("13"));
    Debug_MSG_LN("NFC MISO                 = GPIO" + String("14"));
    Debug_MSG_LN("NFC SCK                  = GPIO" + String("15"));
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
    Debug_MSG_LN("- Cancello 2 : " + String(Cancello2_Topic));
    Debug_MSG_LN("RELE A cancello 2        = GPIO" + String(RELE_A_cancello2));
    Debug_MSG_LN("RELE B cancello 2        = GPIO" + String(RELE_B_cancello2));
    Debug_MSG_LN("Pulsante A cancello 2    = GPIO" + String(BOTTONE_A_cancello2));
    Debug_MSG_LN("Pulsante B cancello 2    = GPIO" + String(BOTTONE_B_cancello2));
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
    Debug_MSG_LN("- Temperatura 1 : " + String(Temperatura1_Topic));
    Debug_MSG_LN("RELE A temperatura 1      = GPIO" + String(RELE_A_temperatura1));
    Debug_MSG_LN("RELE B temperatura 1      = GPIO" + String(RELE_B_temperatura1));
    Debug_MSG_LN("DTH22 temperatura 1       = GPIO" + String(DHTPIN_temperatura1));
    Debug_MSG_LN("SSD1306 ADDRESS temperatura 1 = 0x"   + String(I2C_DISPLAY_ADDRESS_temperatura1, HEX));
    Debug_MSG_LN("SSD1306 SDA temperatura 1 = GPIO" + String(SDA_PIN_temperatura1));
    Debug_MSG_LN("SSD1306 SDC temperatura 1 = GPIO" + String(SDC_PIN_temperatura1));
    pinMode(RELE_A_temperatura1, OUTPUT);
    digitalWrite(RELE_A_temperatura1, 0 ^ Flag_inversione_RELE);
    pinMode(RELE_B_temperatura1, OUTPUT);
    digitalWrite(RELE_B_temperatura1, 0 ^ Flag_inversione_RELE);
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
    display.drawString(0, 45, Temperatura1_Topic);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 20, "by roncoa@gmail.com");
    display.display();
  }
  if (NODI_TAPPARELLA > 0) {
    EEPROM_read_TEMPO_tapparelle_MAX();
  }

  if (NODI_TEMPERATURA > 0) {
    EEPROM_temperatura1_read();
  }

}





void Check_flash_chip_configuration() {
  uint32_t realSize = ESP.getFlashChipRealSize();
  uint32_t ideSize = ESP.getFlashChipSize();
  FlashMode_t ideMode = ESP.getFlashChipMode();
  Serial.printf("Flash real id:   %08X\n", ESP.getFlashChipId());
  Serial.printf("Flash real size: %u\n", realSize);
  Serial.printf("Flash ide  size: %u\n", ideSize);
  Serial.printf("Flash ide speed: %u\n", ESP.getFlashChipSpeed());
  Serial.printf("Flash ide mode:  %s\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"));
  if (ideSize != realSize) {
    Debug_MSG_LN("***********************************");
    Debug_MSG_LN("* Flash Chip configuration WRONG! *");
    Debug_MSG_LN("***********************************");
    Debug_MSG_LN("");
    delay(10000);
    //return false;
  } else {
    Debug_MSG_LN("Flash Chip configuration OK.");
    Debug_MSG_LN("");
    //return true;
  }
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
  WiFiClient client2;
  int maxr = 5;
  while (!!!client2.connect("google.com", 80) and maxr > 0) {
    Debug_MSG_LN("connection failed, retrying...");
    --maxr;
  }
  if (maxr < 1) {
    client2.stop();
    return ("no time!");
  }
  client2.print("HEAD / HTTP/1.1\r\n\r\n");
  while (!!!client2.available()) {
    yield();
  }
  while (client2.available()) {
    if (client2.read() == '\n') {
      if (client2.read() == 'D') {
        if (client2.read() == 'a') {
          if (client2.read() == 't') {
            if (client2.read() == 'e') {
              if (client2.read() == ':') {
                client2.read();
                String theDate = client2.readStringUntil('\r');
                client2.stop();
                return theDate;
              }
            }
          }
        }
      }
    }
  }
}

void loop_lampeggio_led() {
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
}

void SONOFFDUAL_A_ON() {
  Serial1.begin(19230);
  Serial1.write(0xA0);
  Serial1.write(0x04);
  Serial1.write(sonoffdualcommand(1, 1)); // relè 1 ON
  Serial1.write(0xA1);
  Serial1.flush();
}
void SONOFFDUAL_B_ON() {
  Serial1.begin(19230);
  Serial1.write(0xA0);
  Serial1.write(0x04);
  Serial1.write(sonoffdualcommand(2, 1)); // relè 2 ON
  Serial1.write(0xA1);
  Serial1.flush();
}
void SONOFFDUAL_A_OFF() {
  Serial1.begin(19230);
  Serial1.write(0xA0);
  Serial1.write(0x04);
  Serial1.write(sonoffdualcommand(1, 0)); // relè 1 OFF
  Serial1.write(0xA1);
  Serial1.flush();
}
void SONOFFDUAL_B_OFF() {
  Serial1.begin(19230);
  Serial1.write(0xA0);
  Serial1.write(0x04);
  Serial1.write(sonoffdualcommand(2, 0)); // relè 2 OFF
  Serial1.write(0xA1);
  Serial1.flush();
}
void SONOFFDUAL_A_ON_B_OFF() {
  SONOFFDUAL_B_OFF();
  SONOFFDUAL_A_ON();
}
void SONOFFDUAL_A_OFF_B_ON() {
  SONOFFDUAL_A_OFF();
  SONOFFDUAL_B_ON();
}
void SONOFFDUAL_AB_ON() {
  SONOFFDUAL_A_ON();
  SONOFFDUAL_B_ON();
}
void SONOFFDUAL_AB_OFF() {
  SONOFFDUAL_A_OFF();
  SONOFFDUAL_B_OFF();
}

/*
  0x00 both off
  0x01 relay one on
  0x02 relay two on
  0x03 both relays on

  it sems that on my sonoff dual (v2.0):
  0x05 relay one on, relay two off
  0x06 relay one off, relay two on
*/
byte sonoffdualcommand(byte rele, byte stato) // rele=1,2, stato=0/1
{
  byte sstate;
  static byte switchstate[2];
  rele -= 1;
  switchstate[rele] = stato;
  sstate = switchstate[0] + (switchstate[1] << 1);
  //   Serial.write(0xA0);
  //   Serial.write(0x04);
  //   Serial.write( sstate );
  //   Serial.write(0xA1);
  //   Serial.flush();
  //   delay(1);
  return sstate;
}

void ESP01_SERIAL_RELE_AON() {  // A0 01 01 A2
  Serial.begin(9600);
  for (int i = 0; i < sizeof(ESP01_SERIAL_RELE_AON_data); i++) {
    Serial.write(ESP01_SERIAL_RELE_AON_data[i]);
  }
  Serial.flush();
}

void ESP01_SERIAL_RELE_AOFF() {  // A0 01 00 A1
  Serial.begin(9600);
  for (int i = 0; i < sizeof(ESP01_SERIAL_RELE_AOFF_data); i++) {
    Serial.write(ESP01_SERIAL_RELE_AOFF_data[i]);
  }
  Serial.flush();
}
void ESP01_SERIAL_RELE_BON() {  // A0 02 01 A3
  Serial.begin(9600);
  for (int i = 0; i < sizeof(ESP01_SERIAL_RELE_BON_data); i++) {
    Serial.write(ESP01_SERIAL_RELE_BON_data[i]);
  }
  Serial.flush();
}

void ESP01_SERIAL_RELE_BOFF() {  // A0 02 00 A2
  Serial.begin(9600);
  for (int i = 0; i < sizeof(ESP01_SERIAL_RELE_BOFF_data); i++) {
    Serial.write(ESP01_SERIAL_RELE_BOFF_data[i]);
  }
  Serial.flush();
}








void loop_tapparelle() {
  if (NODI_TAPPARELLA > 0) {
    if (((millis() - ulTimeTapparella1) > TEMPO_tapparella1_MAX) & In_movimento_tapparella1) {               // Tempo tapparella 1 -> spengo!
      Debug_MSG_LN("Tempo tapparella 1 -> spengo!");
      digitalWrite(RELE_tapparella1_SU, 0 ^ Flag_inversione_RELE);         // RELE SU = 0
      delay(TEMPO_RELE);                                                  // Aspetto
      digitalWrite(RELE_tapparella1_GIU, 0 ^ Flag_inversione_RELE);        // RELE SU = 0
#if defined(SONOFFDUAL)
      SONOFFDUAL_AB_OFF();
#endif
      In_movimento_tapparella1 = false;
    }
    if (((millis() - ulTimeTapparella2) > TEMPO_tapparella2_MAX) & In_movimento_tapparella2) {               // Tempo tapparella 2 -> spengo!
      Debug_MSG_LN("Tempo tapparella 2 -> spengo!");
      digitalWrite(RELE_tapparella2_SU, 0 ^ Flag_inversione_RELE);         // RELE SU = 0
      delay(TEMPO_RELE);                                                  // Aspetto
      digitalWrite(RELE_tapparella2_GIU, 0 ^ Flag_inversione_RELE);        // RELE SU = 0
#if defined(SONOFFDUAL)
      SONOFFDUAL_AB_OFF();
#endif
      In_movimento_tapparella2 = false;
    }
    if (BottoneTAP1SU.Click(TEMPO_CLICK_ON)) {                                // Bottone SU tapparella 1 premuto!
      Debug_MSG_LN("Bottone SU tapparella 1 premuto!");
      digitalWrite(RELE_tapparella1_GIU, 0 ^ Flag_inversione_RELE);        // RELE GIU = 0
      delay(TEMPO_RELE);                                                  // Aspetto
      digitalWrite(RELE_tapparella1_SU, 1 ^ Flag_inversione_RELE);         // RELE SU = 1
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_ON_B_OFF();
#endif
      In_movimento_tapparella1 = true;
      ulTimeTapparella1 = millis();
    }
    if (BottoneTAP1GIU.Click(TEMPO_CLICK_ON)) {                                // Bottone GIU tapparella 1 premuto!
      Debug_MSG_LN("Bottone GIU tapparella 1 premuto!");
      digitalWrite(RELE_tapparella1_SU, 0 ^ Flag_inversione_RELE);          // RELE SU = 0
      delay(TEMPO_RELE);                                                   // Aspetto
      digitalWrite(RELE_tapparella1_GIU, 1 ^ Flag_inversione_RELE);         // RELE GIU = 1
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_OFF_B_ON();
#endif
      In_movimento_tapparella1 = true;
      ulTimeTapparella1 = millis();
    }
    if (t = BottoneTAP1SU.Keep(TEMPO_CLICK_OFF)) {                            // Bottone SU tapparella 1 rilasciato!
      Debug_MSG_LN("Bottone SU tapparella 1 rilasciato -> spengo!");
      digitalWrite(RELE_tapparella1_SU, 0 ^ Flag_inversione_RELE);         // RELE SU = 0
      delay(TEMPO_RELE);                                                  // Aspetto
      digitalWrite(RELE_tapparella1_GIU, 0 ^ Flag_inversione_RELE);        // RELE GIU = 0
#if defined(SONOFFDUAL)
      SONOFFDUAL_AB_OFF();
#endif
      In_movimento_tapparella1 = false;
    }
    if (t = BottoneTAP1GIU.Keep(TEMPO_CLICK_OFF)) {                           // Bottone GIU tapparella 1 rilasciato!
      Debug_MSG_LN("Bottone GIU tapparella 1 rilasciato -> spengo!");
      digitalWrite(RELE_tapparella1_SU, 0 ^ Flag_inversione_RELE);         // RELE SU = 0
      delay(TEMPO_RELE);                                                  // Aspetto
      digitalWrite(RELE_tapparella1_GIU, 0 ^ Flag_inversione_RELE);        // RELE GIU = 0
#if defined(SONOFFDUAL)
      SONOFFDUAL_AB_OFF();
#endif
      In_movimento_tapparella1 = false;
    }
    if (BottoneTAP2SU.Click(TEMPO_CLICK_ON)) {                                // Bottone SU tapparella 2 premuto!
      Debug_MSG_LN("Bottone SU tapparella 2 premuto!");
      digitalWrite(RELE_tapparella2_GIU, 0 ^ Flag_inversione_RELE);        // RELE GIU = 0
      delay(TEMPO_RELE);                                                  // Aspetto
      digitalWrite(RELE_tapparella2_SU, 1 ^ Flag_inversione_RELE);         // RELE SU = 1
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_ON_B_OFF();
#endif
      In_movimento_tapparella2 = true;
      ulTimeTapparella2 = millis();
    }
    if (BottoneTAP2GIU.Click(TEMPO_CLICK_ON)) {                                // Bottone GIU tapparella 2 premuto!
      Debug_MSG_LN("Bottone GIU tapparella 2 premuto!");
      digitalWrite(RELE_tapparella2_SU, 0 ^ Flag_inversione_RELE);          // RELE SU = 0
      delay(TEMPO_RELE);                                                   // Aspetto
      digitalWrite(RELE_tapparella2_GIU, 1 ^ Flag_inversione_RELE);         // RELE GIU = 1
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_OFF_B_ON();
#endif
      In_movimento_tapparella2 = true;
      ulTimeTapparella2 = millis();
    }
    if (t = BottoneTAP2SU.Keep(TEMPO_CLICK_OFF)) {                            // Bottone SU tapparella 2 rilasciato!
      Debug_MSG_LN("Bottone SU tapparella 2 rilasciato -> spengo!");
      digitalWrite(RELE_tapparella2_SU, 0 ^ Flag_inversione_RELE);         // RELE SU = 0
      delay(TEMPO_RELE);                                                  // Aspetto
      digitalWrite(RELE_tapparella2_GIU, 0 ^ Flag_inversione_RELE);        // RELE GIU = 0
#if defined(SONOFFDUAL)
      SONOFFDUAL_AB_OFF();
#endif
      In_movimento_tapparella2 = false;
    }
    if (t = BottoneTAP2GIU.Keep(TEMPO_CLICK_OFF)) {                           // Bottone GIU tapparella 2 rilasciato!
      Debug_MSG_LN("Bottone GIU tapparella 2 rilasciato -> spengo!");
      digitalWrite(RELE_tapparella2_SU, 0 ^ Flag_inversione_RELE);         // RELE SU = 0
      delay(TEMPO_RELE);                                                  // Aspetto
      digitalWrite(RELE_tapparella2_GIU, 0 ^ Flag_inversione_RELE);        // RELE GIU = 0
#if defined(SONOFFDUAL)
      SONOFFDUAL_AB_OFF();
#endif
      In_movimento_tapparella2 = false;
    }
  }
}



void loop_interruttori() {
  if (NODI_INTERRUTTORE > 0) {
    if (BottoneAINT1.Click(TEMPO_CLICK_ON)) {                                // Bottone 1 premuto!
      Debug_MSG_LN("Bottone 1 premuto!");
      digitalWrite(RELE_A_interruttore1, not(digitalRead(RELE_A_interruttore1)));
#if defined(SONOFFDUAL)
      if (digitalRead(RELE_A_interruttore1)) SONOFFDUAL_A_ON();
      else SONOFFDUAL_A_OFF();
#endif
#if defined(ESP01_SERIAL_RELE)
      if (digitalRead(RELE_A_interruttore1)) ESP01_SERIAL_RELE_AON();
      else ESP01_SERIAL_RELE_AOFF();
#endif
      EEPROM_write_releAinterruttore1();
      delay(TEMPO_RELE);
    }
    if (BottoneBINT1.Click(TEMPO_CLICK_ON)) {                                // Bottone 2 premuto!
      Debug_MSG_LN("Bottone 2 premuto!");
      digitalWrite(RELE_B_interruttore1, not(digitalRead(RELE_B_interruttore1)));
#if defined(SONOFFDUAL)
      if (digitalRead(RELE_B_interruttore1)) SONOFFDUAL_B_ON();
      else SONOFFDUAL_B_OFF();
#endif
#if defined(ESP01_SERIAL_RELE)
      if (digitalRead(RELE_B_interruttore1)) ESP01_SERIAL_RELE_BON();
      else ESP01_SERIAL_RELE_BOFF();
#endif
      EEPROM_write_releBinterruttore1();
      delay(TEMPO_RELE);
    }
    if (t = BottoneAINT1.Keep(TEMPO_CLICK_OFF)) {                            // Bottone 1 rilasciato!
      Debug_MSG_LN("Bottone 1 rilasciato!");
      if (TIPO_BOTTONE_A_interruttore1 == "interruttore") {
        digitalWrite(RELE_A_interruttore1, not(digitalRead(RELE_A_interruttore1)));
#if defined(SONOFFDUAL)
        if (digitalRead(RELE_A_interruttore1)) SONOFFDUAL_A_ON();
        else SONOFFDUAL_A_OFF();
#endif
#if defined(ESP01_SERIAL_RELE)
        if (digitalRead(RELE_A_interruttore1)) ESP01_SERIAL_RELE_AON();
        else ESP01_SERIAL_RELE_AOFF();
#endif
      }
      EEPROM_write_releAinterruttore1();
      delay(TEMPO_RELE);
    }
    if (t = BottoneBINT1.Keep(TEMPO_CLICK_OFF)) {                           // Bottone 2 rilasciato!
      Debug_MSG_LN("Bottone 2 rilasciato!");
      if (TIPO_BOTTONE_B_interruttore1 == "interruttore") {
        digitalWrite(RELE_B_interruttore1, not(digitalRead(RELE_B_interruttore1)));
#if defined(SONOFFDUAL)
        if (digitalRead(RELE_B_interruttore1)) SONOFFDUAL_B_ON();
        else SONOFFDUAL_B_OFF();
#endif
#if defined(ESP01_SERIAL_RELE)
        if (digitalRead(RELE_B_interruttore1)) ESP01_SERIAL_RELE_BON();
        else ESP01_SERIAL_RELE_BOFF();
#endif
      }
      EEPROM_write_releBinterruttore1();
      delay(TEMPO_RELE);
    }
    if (BottoneAINT2.Click(TEMPO_CLICK_ON)) {                                // Bottone 1 premuto!
      Debug_MSG_LN("Bottone 1 premuto!");
      digitalWrite(RELE_A_interruttore2, not(digitalRead(RELE_A_interruttore2)));
#if defined(SONOFFDUAL)
      if (digitalRead(RELE_A_interruttore2)) SONOFFDUAL_A_ON();
      else SONOFFDUAL_A_OFF();
#endif
      EEPROM_write_releAinterruttore2();
      delay(TEMPO_RELE);
    }
    if (BottoneBINT2.Click(TEMPO_CLICK_ON)) {                                // Bottone 2 premuto!
      Debug_MSG_LN("Bottone 2 premuto!");
      digitalWrite(RELE_B_interruttore2, not(digitalRead(RELE_B_interruttore2)));
#if defined(SONOFFDUAL)
      if (digitalRead(RELE_B_interruttore2)) SONOFFDUAL_B_ON();
      else SONOFFDUAL_B_OFF();
#endif
      EEPROM_write_releBinterruttore2();
      delay(TEMPO_RELE);
    }
    if (t = BottoneAINT2.Keep(TEMPO_CLICK_OFF)) {                            // Bottone 1 rilasciato!
      Debug_MSG_LN("Bottone 1 rilasciato!");
      if (TIPO_BOTTONE_A_interruttore2 == "interruttore") {
        digitalWrite(RELE_A_interruttore2, not(digitalRead(RELE_A_interruttore2)));
#if defined(SONOFFDUAL)
        if (digitalRead(RELE_A_interruttore2)) SONOFFDUAL_A_ON();
        else SONOFFDUAL_A_OFF();
#endif
      }
      EEPROM_write_releAinterruttore2();
      delay(TEMPO_RELE);
    }
    if (t = BottoneBINT2.Keep(TEMPO_CLICK_OFF)) {                           // Bottone 2 rilasciato!
      Debug_MSG_LN("Bottone 2 rilasciato!");
      if (TIPO_BOTTONE_B_interruttore2 == "interruttore") {
        digitalWrite(RELE_B_interruttore2, not(digitalRead(RELE_B_interruttore2)));
#if defined(SONOFFDUAL)
        if (digitalRead(RELE_B_interruttore2)) SONOFFDUAL_B_ON();
        else SONOFFDUAL_B_OFF();
#endif
      }
      EEPROM_write_releBinterruttore2();
      delay(TEMPO_RELE);
    }
    if (BottoneAINT3.Click(TEMPO_CLICK_ON)) {                                // Bottone 1 premuto!
      Debug_MSG_LN("Bottone 1 premuto!");
      digitalWrite(RELE_A_interruttore3, not(digitalRead(RELE_A_interruttore3)));
#if defined(SONOFFDUAL)
      if (digitalRead(RELE_A_interruttore3)) SONOFFDUAL_A_ON();
      else SONOFFDUAL_A_OFF();
#endif
      EEPROM_write_releAinterruttore3();
      delay(TEMPO_RELE);
    }
    if (BottoneBINT3.Click(TEMPO_CLICK_ON)) {                                // Bottone 2 premuto!
      Debug_MSG_LN("Bottone 2 premuto!");
      digitalWrite(RELE_B_interruttore3, not(digitalRead(RELE_B_interruttore3)));
#if defined(SONOFFDUAL)
      if (digitalRead(RELE_B_interruttore3)) SONOFFDUAL_B_ON();
      else SONOFFDUAL_B_OFF();
#endif
      EEPROM_write_releBinterruttore3();
      delay(TEMPO_RELE);
    }
    if (t = BottoneAINT3.Keep(TEMPO_CLICK_OFF)) {                            // Bottone 1 rilasciato!
      Debug_MSG_LN("Bottone 1 rilasciato!");
      if (TIPO_BOTTONE_A_interruttore3 == "interruttore") {
        digitalWrite(RELE_A_interruttore3, not(digitalRead(RELE_A_interruttore3)));
#if defined(SONOFFDUAL)
        if (digitalRead(RELE_A_interruttore3)) SONOFFDUAL_A_ON();
        else SONOFFDUAL_A_OFF();
#endif
      }
      EEPROM_write_releAinterruttore3();
      delay(TEMPO_RELE);
    }
    if (t = BottoneBINT3.Keep(TEMPO_CLICK_OFF)) {                           // Bottone 2 rilasciato!
      Debug_MSG_LN("Bottone 2 rilasciato!");
      if (TIPO_BOTTONE_B_interruttore3 == "interruttore") {
        digitalWrite(RELE_B_interruttore3, not(digitalRead(RELE_B_interruttore3)));
#if defined(SONOFFDUAL)
        if (digitalRead(RELE_B_interruttore3)) SONOFFDUAL_B_ON();
        else SONOFFDUAL_B_OFF();
#endif
      }
      EEPROM_write_releBinterruttore3();
      delay(TEMPO_RELE);
    }
    if (BottoneAINT4.Click(TEMPO_CLICK_ON)) {                                // Bottone 1 premuto!
      Debug_MSG_LN("Bottone 1 premuto!");
      digitalWrite(RELE_A_interruttore4, not(digitalRead(RELE_A_interruttore4)));
#if defined(SONOFFDUAL)
      if (digitalRead(RELE_A_interruttore4)) SONOFFDUAL_A_ON();
      else SONOFFDUAL_A_OFF();
#endif
      EEPROM_write_releAinterruttore4();
      delay(TEMPO_RELE);
    }
    if (BottoneBINT4.Click(TEMPO_CLICK_ON)) {                                // Bottone 2 premuto!
      Debug_MSG_LN("Bottone 2 premuto!");
      digitalWrite(RELE_B_interruttore4, not(digitalRead(RELE_B_interruttore4)));
#if defined(SONOFFDUAL)
      if (digitalRead(RELE_B_interruttore4)) SONOFFDUAL_B_ON();
      else SONOFFDUAL_B_OFF();
#endif
      EEPROM_write_releBinterruttore4();
      delay(TEMPO_RELE);
    }
    if (t = BottoneAINT4.Keep(TEMPO_CLICK_OFF)) {                            // Bottone 1 rilasciato!
      Debug_MSG_LN("Bottone 1 rilasciato!");
      if (TIPO_BOTTONE_A_interruttore4 == "interruttore") {
        digitalWrite(RELE_A_interruttore4, not(digitalRead(RELE_A_interruttore4)));
#if defined(SONOFFDUAL)
        if (digitalRead(RELE_A_interruttore4)) SONOFFDUAL_A_ON();
        else SONOFFDUAL_A_OFF();
#endif
      }
      EEPROM_write_releAinterruttore4();
      delay(TEMPO_RELE);
    }
    if (t = BottoneBINT4.Keep(TEMPO_CLICK_OFF)) {                           // Bottone 2 rilasciato!
      Debug_MSG_LN("Bottone 2 rilasciato!");
      if (TIPO_BOTTONE_B_interruttore4 == "interruttore") {
        digitalWrite(RELE_B_interruttore4, not(digitalRead(RELE_B_interruttore4)));
#if defined(SONOFFDUAL)
        if (digitalRead(RELE_B_interruttore4)) SONOFFDUAL_B_ON();
        else SONOFFDUAL_B_OFF();
#endif
      }
      EEPROM_write_releBinterruttore4();
      delay(TEMPO_RELE);
    }
  }
}


void loop_cancelli() {
  if (NODI_CANCELLO > 0) {
    if (((millis() - ulTimeReleACancello1) > TEMPO_IMPULSO_cancello) & Rele_A_eccitato_cancello1) {        // Tempo rele 1 -> spengo!
      Debug_MSG_LN("Tempo rele 1 -> spengo!");
      digitalWrite(RELE_A_cancello1, 0 ^ Flag_inversione_RELE);                     // Spengo il RELE 1
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_OFF();
#endif
#if defined(ESP01_SERIAL_RELE)
      ESP01_SERIAL_RELE_AOFF();
#endif
      Rele_A_eccitato_cancello1 = false;
    }
    if (((millis() - ulTimeReleBCancello1) > TEMPO_IMPULSO_cancello) & Rele_B_eccitato_cancello1) {        // Tempo rele 2 -> spengo!
      Debug_MSG_LN("Tempo rele 2 -> spengo!");
      digitalWrite(RELE_B_cancello1, 0 ^ Flag_inversione_RELE);                     // Spengo il RELE 2
#if defined(SONOFFDUAL)
      SONOFFDUAL_B_OFF();
#endif
#if defined(ESP01_SERIAL_RELE)
      ESP01_SERIAL_RELE_BOFF();
#endif
      Rele_B_eccitato_cancello1 = false;
    }
    if (BottoneACAN1.Click(TEMPO_CLICK_ON)) {                                // Bottone 1 premuto!
      Debug_MSG_LN("Bottone 1 premuto!");
      digitalWrite(RELE_A_cancello1, 1 ^ Flag_inversione_RELE);
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_ON();
#endif
      Rele_A_eccitato_cancello1 = true;
      ulTimeReleACancello1 = millis();
    }
    if (BottoneBCAN1.Click(TEMPO_CLICK_ON)) {                                // Bottone 2 premuto!
      Debug_MSG_LN("Bottone 2 premuto!");
      digitalWrite(RELE_B_cancello1, 1 ^ Flag_inversione_RELE);
#if defined(SONOFFDUAL)
      SONOFFDUAL_B_ON();
#endif
      Rele_B_eccitato_cancello1 = true;
      ulTimeReleBCancello1 = millis();
    }
    if (((millis() - ulTimeReleACancello2) > TEMPO_IMPULSO_cancello) & Rele_A_eccitato_cancello2) {        // Tempo rele 1 -> spengo!
      Debug_MSG_LN("Tempo rele 1 -> spengo!");
      digitalWrite(RELE_A_cancello2, 0 ^ Flag_inversione_RELE);                     // Spengo il RELE 1
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_OFF();
#endif
      Rele_A_eccitato_cancello2 = false;
    }
    if (((millis() - ulTimeReleBCancello2) > TEMPO_IMPULSO_cancello) & Rele_B_eccitato_cancello2) {        // Tempo rele 2 -> spengo!
      Debug_MSG_LN("Tempo rele 2 -> spengo!");
      digitalWrite(RELE_B_cancello2, 0 ^ Flag_inversione_RELE);                     // Spengo il RELE 2
#if defined(SONOFFDUAL)
      SONOFFDUAL_B_OFF();
#endif
      Rele_B_eccitato_cancello2 = false;
    }
    if (BottoneACAN2.Click(TEMPO_CLICK_ON)) {                                // Bottone 1 premuto!
      Debug_MSG_LN("Bottone 1 premuto!");
      digitalWrite(RELE_A_cancello2, 1 ^ Flag_inversione_RELE);
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_ON();
#endif
      Rele_A_eccitato_cancello2 = true;
      ulTimeReleACancello2 = millis();
    }
    if (BottoneBCAN2.Click(TEMPO_CLICK_ON)) {                                // Bottone 2 premuto!
      Debug_MSG_LN("Bottone 2 premuto!");
      digitalWrite(RELE_B_cancello2, 1 ^ Flag_inversione_RELE);
#if defined(SONOFFDUAL)
      SONOFFDUAL_B_ON();
#endif
      Rele_B_eccitato_cancello2 = true;
      ulTimeReleBCancello2 = millis();
    }
  }
}


void loop_temperatura() {
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

    if (TIMER_TERMOSTATO1.Wait(TEMPO_TERMOSTATO)) {
      h_temperatura1 = dht.readHumidity();
      delay(50);
      t_temperatura1 = dht.readTemperature(false);                                                   // Read temperature as Celsius (the default)
      if (isnan(t_temperatura1)) {                                                                         // Check if any reads failed and exit early (to try again).
        //      String payload = macToStr(mac);
        //      payload += " Failed to read from DHT sensor!";
        //      client.publish(ACK_Topic, (char*) payload.c_str());
        Debug_MSG_LN("Failed to read from DHT sensor!");
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

