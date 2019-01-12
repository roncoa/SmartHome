#include <Time.h>
#include <TimeLib.h>
#include <Timezone.h>

int           lamp = 0;                               // Variabile per lampeggio LED

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

/*void loop_reset_giornaliero() {
  if (TIMER_RESET_GIORNALIERO.Wait(1000 * 60 * 60 * 24)) { // 1 giorno=86400000
    Debug_MSG_LN("Reset giornaliero");
    Debug_MSG_LN("RIAVVIO L'ESP");
    delay(250);
    WiFi.disconnect(true);
    delay(250);
    ESP.restart();
  }
}*/

void loop_check_low_memory() {
  if (ESP.getFreeHeap() < MinFreeHeap) {
    Debug_MSG("FreeHeap:");
    Debug_MSG_LN((String) ESP.getFreeHeap());
    Debug_MSG_LN("Poca memoria");
    Debug_MSG_LN("RIAVVIO L'ESP");
    delay(250);
    WiFi.disconnect(true);
    delay(250);
    ESP.restart();
  }
}

void setup_GPIO() {
  Debug_MSG(" ***** SmartHome v");
  Debug_MSG(Versione);
  Debug_MSG_LN(" *****");
  Debug_MSG_LN("");
  Debug_MSG_LN("Status LED             = GPIO" + String(Status_LED_GPIO));
  pinMode(Status_LED_GPIO, OUTPUT);                                  // Inizializza Status_LED_GPIO
  digitalWrite(Status_LED_GPIO, 0 ^ Flag_inversione_Status_LED_GPIO);
}

void Check_flash_chip_configuration() {
  uint32_t realSize = ESP.getFlashChipRealSize();
  uint32_t ideSize = ESP.getFlashChipSize();
  FlashMode_t ideMode = ESP.getFlashChipMode();
#if defined(DEBUG)
  Serial.printf("Flash real id:   %08X\n", ESP.getFlashChipId());
  Serial.printf("Flash real size: %u\n", realSize);
  Serial.printf("Flash ide  size: %u\n", ideSize);
  Serial.printf("Flash ide speed: %u\n", ESP.getFlashChipSpeed());
  Serial.printf("Flash ide mode:  %s\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"));
  Serial.printf("FreeHeap: %u\n", ESP.getFreeHeap());
#endif
  if (ideSize != realSize) {
    Debug_MSG_LN("***********************************");
    Debug_MSG_LN("* Flash Chip configuration WRONG! *");
    Debug_MSG_LN("***********************************");
    Debug_MSG_LN("");
    delay(5000);
    //return false;
  } else {
    Debug_MSG_LN("Flash Chip configuration OK.");
    Debug_MSG_LN("");
    //return true;
  }
}

void loop_lampeggio_led() {
  if (TIMER_LAMPEGGIO.Wait(250)) {
    lamp = lamp + 1;
    if (lamp > 6) lamp = 0;
    if (lamp > 1 and !client.connected()) lamp = 0;
    if (lamp == 0) digitalWrite(Status_LED_GPIO, 1 ^ Flag_inversione_Status_LED_GPIO);
    if (lamp == 1) digitalWrite(Status_LED_GPIO, 0 ^ Flag_inversione_Status_LED_GPIO);
    if (lamp == 2) digitalWrite(Status_LED_GPIO, 1 ^ Flag_inversione_Status_LED_GPIO);
    if (lamp == 3) digitalWrite(Status_LED_GPIO, 0 ^ Flag_inversione_Status_LED_GPIO);
    if (lamp == 4) digitalWrite(Status_LED_GPIO, 0 ^ Flag_inversione_Status_LED_GPIO);
    if (lamp == 5) digitalWrite(Status_LED_GPIO, 0 ^ Flag_inversione_Status_LED_GPIO);
    if (lamp == 6) digitalWrite(Status_LED_GPIO, 0 ^ Flag_inversione_Status_LED_GPIO);
  }
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

String printDigits(int digits) {
  if (digits < 10)    return ('0' + (String) digits);
  return ((String) digits);
}

String getTime() {
  TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120}; //Central European Summer Time
  TimeChangeRule CET  = {"CET ", Last, Sun, Oct, 3, 60};  //Central European Standard Time
  Timezone CE(CEST, CET);
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  delay(1000);
  time_t Ora_Locale = 0;
  time_t Ora_UTC = 0;
  TimeChangeRule *tcr; //pointer to the time change rule, use to get the TZ abbrev
  Ora_UTC = time(nullptr);
  Ora_Locale = CE.toLocal(Ora_UTC, &tcr);
  if (Ora_Locale < 40000) return ("DATE/TIME not synchronized");
  setTime(Ora_Locale);
  return ((String) printDigits(hour()) + ":" + printDigits(minute()) + ":" + printDigits(second()) + " " + dayShortStr(weekday()) + " " + printDigits(day()) + "/" + monthShortStr(month()) + "/" + year());
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
  Serial.write(sonoffdualcommand(1, 1)); // relÃ¨ 1 ON
  Serial.write(0xA1);
  Serial.flush();
}
void SONOFFDUAL_B_ON() {
  Serial.begin(19230);
  Serial.write(0xA0);
  Serial.write(0x04);
  Serial.write(sonoffdualcommand(2, 1)); // relÃ¨ 2 ON
  Serial.write(0xA1);
  Serial.flush();
}
void SONOFFDUAL_A_OFF() {
  Serial.begin(19230);
  Serial.write(0xA0);
  Serial.write(0x04);
  Serial.write(sonoffdualcommand(1, 0)); // relÃ¨ 1 OFF
  Serial.write(0xA1);
  Serial.flush();
}
void SONOFFDUAL_B_OFF() {
  Serial.begin(19230);
  Serial.write(0xA0);
  Serial.write(0x04);
  Serial.write(sonoffdualcommand(2, 0)); // relÃ¨ 2 OFF
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
byte sonoffdualcommand(byte rele, byte stato) {// rele=1,2, stato=0/1
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

/*void Reset_HA() {
  String HAtopic;
  String HApayload;
  for (int i = 0; i < 10; i++) {
    HAtopic = "homeassistant/switch/";
    HAtopic += Interruttore_DEF;
    HAtopic += i;
    HAtopic += "-";
    HAtopic += macToStr(mac);
    HAtopic += "/config";
    HApayload = "";
    client.publish((char*) HAtopic.c_str(), (char*) HApayload.c_str());
    delay(100);
  }
  for (int i = 0; i < 5; i++) {
    HAtopic = "homeassistant/cover/";
    HAtopic += macToStr(mac);
    HAtopic += "/";
    HAtopic += Tapparella_DEF;
    HAtopic += i;
    HAtopic += "/config";
    HApayload = "";
    client.publish((char*) HAtopic.c_str(), (char*) HApayload.c_str());
    delay(100);
  }
  for (int i = 0; i < 1; i++) {
    HAtopic = "homeassistant/climate/";
    HAtopic += Termostato_DEF;
    HAtopic += "i";
    HAtopic += "-";
    HAtopic += macToStr(mac);
    HAtopic += "/config";
    HApayload = "";
    client.publish((char*) HAtopic.c_str(), (char*) HApayload.c_str());
    delay(250);
  }
}*/
