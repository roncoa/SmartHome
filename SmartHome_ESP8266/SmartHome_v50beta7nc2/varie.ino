int           lamp = 0;                               // Variabile per lampeggio LED

TPush TIMER_RESET_GIORNALIERO, TIMER_Status_LED;

void Debug_MSG(String s) {
#ifdef DEBUG
  Serial.print(s);
#endif
#ifdef DEBUG1
  Serial1.print(s);
#endif
#ifdef TELNET_DEBUG
  if (serverClient && serverClient.connected()) {
    serverClient.print(string2char(s));
  }
#endif
}

void Debug_MSG_LN(String s) {
#ifdef DEBUG
  Serial.println(s);
#endif
#ifdef DEBUG1
  Serial1.println(s);
#endif
#ifdef TELNET_DEBUG
  if (serverClient && serverClient.connected()) {
    serverClient.println(string2char(s));
  }
#endif
}

void Show_vesione() {
  Debug_MSG_LN("");
  String tmp = "";
  for (int i = 0; i < Versione.length(); i++) {
    tmp += "*";
  }
  Debug_MSG_LN("   ***********************" + tmp);
  Debug_MSG_LN("   ***** SmartHome v" + (String)Versione + " *****");
  Debug_MSG_LN("   ***********************" + tmp);
  Debug_MSG_LN("");
}

void loop_reset_giornaliero() {
  if (TIMER_RESET_GIORNALIERO.Wait(1000 * 60 * 60 * 24)) { // 1 giorno=86400000
    Debug_MSG_LN("Reset giornaliero");
    Debug_MSG_LN("RIAVVIO L'ESP");
    WiFi.disconnect(true);
    cmd_reboot();
  }
}

void loop_check_low_memory() {
  if (ESP.getFreeHeap() < MinFreeHeap) {
    Debug_MSG("FreeHeap:");
    Debug_MSG_LN((String) ESP.getFreeHeap());
    Debug_MSG_LN("Poca memoria");
    Debug_MSG_LN("RIAVVIO L'ESP");
    WiFi.disconnect(true);
    cmd_reboot();
  }
}

void setup_Status_LED() {
#ifdef Status_LED_GPIO
  Debug_MSG_LN("Status LED             = GPIO" + String(Status_LED_GPIO));
  pinMode(Status_LED_GPIO, OUTPUT);
  digitalWrite(Status_LED_GPIO, 0 ^ Flag_inversione_Status_LED_GPIO);
#else
  Debug_MSG_LN("Status LED             : Nessun GPIO definito!");
#endif
}

void Check_flash_chip_configuration() {
  uint32_t realSize = ESP.getFlashChipRealSize();
  uint32_t ideSize = ESP.getFlashChipSize();
  FlashMode_t ideMode = ESP.getFlashChipMode();
  Debug_MSG_LN("Check flash chip configuration:");
  Debug_MSG_LN(" Flash real id: " + (String)ESP.getFlashChipId());
  Debug_MSG_LN(" Flash real size: " + (String)realSize);
  Debug_MSG_LN(" Flash ide  size: " + (String)ideSize);
  Debug_MSG_LN(" Flash ide speed: " + (String)ESP.getFlashChipSpeed());
  String tmp = (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN");
  Debug_MSG_LN(" Flash ide mode:  " + tmp);
  Debug_MSG_LN(" FreeHeap: " + (String)ESP.getFreeHeap());
  if (ideSize != realSize) {
    Debug_MSG_LN("");
    Debug_MSG_LN("***********************************");
    Debug_MSG_LN("* Flash Chip configuration WRONG! *");
    Debug_MSG_LN("***********************************");
    Debug_MSG_LN("");
    unblockingDelay(5000);
  }
  else {
    Debug_MSG_LN("Flash Chip configuration OK.");
  }
}

void loop_Status_LED() {
#ifdef Status_LED_GPIO
  if (TIMER_Status_LED.Wait(250)) {
    lamp = lamp + 1;
    if (lamp > 6) lamp = 0;
    if (lamp > 1 and !mqttClient.connected()) lamp = 0;
    if (lamp == 0) digitalWrite(Status_LED_GPIO, 1 ^ Flag_inversione_Status_LED_GPIO);
    if (lamp == 1) digitalWrite(Status_LED_GPIO, 0 ^ Flag_inversione_Status_LED_GPIO);
    if (lamp == 2) digitalWrite(Status_LED_GPIO, 1 ^ Flag_inversione_Status_LED_GPIO);
    if (lamp == 3) digitalWrite(Status_LED_GPIO, 0 ^ Flag_inversione_Status_LED_GPIO);
    if (lamp == 4) digitalWrite(Status_LED_GPIO, 0 ^ Flag_inversione_Status_LED_GPIO);
    if (lamp == 5) digitalWrite(Status_LED_GPIO, 0 ^ Flag_inversione_Status_LED_GPIO);
    if (lamp == 6) digitalWrite(Status_LED_GPIO, 0 ^ Flag_inversione_Status_LED_GPIO);
  }
#endif
}

ADC_MODE(ADC_VCC);
String Vcc() {
  float currentVcc = 0.00f;
  String StrVcc;
  currentVcc = ESP.getVcc();
  StrVcc = (String)(currentVcc / 1024.00f) + "V";
  return StrVcc;
}

String macToStr(const uint8_t* mac) {
  String result;
  for (int i = 0; i < 6; ++i) {
    if (mac[i] < char(16)) result += "0";
    result += String(mac[i], 16);
    //    if (i < 5) result += ':';
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
  return ("");
  /*
    WiFiClient client2;
    int maxr = 15;
    while (!!!client2.connect("google.it", 80) and maxr > 0) {
    --maxr;
    }
    Debug_MSG_LN("");
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
  */
}

// SONOFFDUAL
/*
  0x00 both off
  0x01 relay one on
  0x02 relay two on
  0x03 both relays on
  it sems that on my sonoff dual (v2.0):
  0x05 relay one on, relay two off
  0x06 relay one off, relay two on
*/

void SONOFFDUAL_A_ON() {
  Serial.begin(19230);
  Serial.write(0xA0);
  Serial.write(0x04);
  Serial.write(sonoffdualcommand(1, 1)); // rele' 1 ON
  Serial.write(0xA1);
  Serial.flush();
}

void SONOFFDUAL_B_ON() {
  Serial.begin(19230);
  Serial.write(0xA0);
  Serial.write(0x04);
  Serial.write(sonoffdualcommand(2, 1)); // rele' 2 ON
  Serial.write(0xA1);
  Serial.flush();
}

void SONOFFDUAL_A_OFF() {
  Serial.begin(19230);
  Serial.write(0xA0);
  Serial.write(0x04);
  Serial.write(sonoffdualcommand(1, 0)); // rele' 1 OFF
  Serial.write(0xA1);
  Serial.flush();
}

void SONOFFDUAL_B_OFF() {
  Serial.begin(19230);
  Serial.write(0xA0);
  Serial.write(0x04);
  Serial.write(sonoffdualcommand(2, 0)); // rele' 2 OFF
  Serial.write(0xA1);
  Serial.flush();
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

byte sonoffdualcommand(byte rele, byte stato) {      // rele=1,2, stato=0/1
  byte sstate;
  static byte switchstate[2];
  rele -= 1;
  switchstate[rele] = stato;
  sstate = switchstate[0] + (switchstate[1] << 1);
  return sstate;
}

// ESP01_SERIAL_RELE
byte ESP01_SERIAL_RELE1_ON_data[]  = {0xA0, 0x01, 0x01, 0xA2};
byte ESP01_SERIAL_RELE1_OFF_data[] = {0xA0, 0x01, 0x00, 0xA1};
byte ESP01_SERIAL_RELE2_ON_data[]  = {0xA0, 0x02, 0x01, 0xA3};
byte ESP01_SERIAL_RELE2_OFF_data[] = {0xA0, 0x02, 0x00, 0xA2};

void ESP01_SERIAL_RELE_A_ON() {  // A0 01 01 A2
  Serial.begin(9600);
  for (int i = 0; i < sizeof(ESP01_SERIAL_RELE1_ON_data); i++) {
    Serial.write(ESP01_SERIAL_RELE1_ON_data[i]);
  }
  Serial.flush();
}

void ESP01_SERIAL_RELE_A_OFF() {  // A0 01 00 A1
  Serial.begin(9600);
  for (int i = 0; i < sizeof(ESP01_SERIAL_RELE1_OFF_data); i++) {
    Serial.write(ESP01_SERIAL_RELE1_OFF_data[i]);
  }
  Serial.flush();
}

void ESP01_SERIAL_RELE_B_ON() {  // A0 02 01 A3
  Serial.begin(9600);
  for (int i = 0; i < sizeof(ESP01_SERIAL_RELE2_ON_data); i++) {
    Serial.write(ESP01_SERIAL_RELE2_ON_data[i]);
  }
  Serial.flush();
}

void ESP01_SERIAL_RELE_B_OFF() {  // A0 02 00 A2
  Serial.begin(9600);
  for (int i = 0; i < sizeof(ESP01_SERIAL_RELE2_OFF_data); i++) {
    Serial.write(ESP01_SERIAL_RELE2_OFF_data[i]);
  }
  Serial.flush();
}
void unblockingDelay(unsigned long mseconds) {
  unsigned long timeout = millis();
  while ((millis() - timeout) < mseconds) delay(1);
}
