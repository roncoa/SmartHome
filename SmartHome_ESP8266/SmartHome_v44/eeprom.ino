/*
  SmartHome mappa della EEPROM dell'ESP8266

  0  -> SmartHome tapparella 1  - TEMPO_MAX (long 4th byte)
  1  -> SmartHome tapparella 1  - TEMPO_MAX (long 3th byte)
  2  -> SmartHome tapparella 1  - TEMPO_MAX (long 2nd byte)
  3  -> SmartHome tapparella 1  - TEMPO_MAX (long 1st byte)
  4  -> SmartHome tapparella 2  - TEMPO_MAX (long 4th byte)
  5  -> SmartHome tapparella 2  - TEMPO_MAX (long 3th byte)
  6  -> SmartHome tapparella 2  - TEMPO_MAX (long 2nd byte)
  7  -> SmartHome tapparella 2  - TEMPO_MAX (long 1st byte)

  10 -> SmartHome temperatura 1 - AUTO 1=AUTO 0=MAN (int 2nd byte)
  11 -> SmartHome temperatura 1 - AUTO 1=AUTO 0=MAN (int 1st byte)
  12 -> SmartHome temperatura 1 - TERMOSTATO (float 4th byte)
  13 -> SmartHome temperatura 1 - TERMOSTATO (float 3th byte)
  14 -> SmartHome temperatura 1 - TERMOSTATO (float 2nd byte)
  15 -> SmartHome temperatura 1 - TERMOSTATO (float 1st byte)

  20 -> SmartHome RELE A interruttore 1 (int 2nd byte)
  21 -> SmartHome RELE A interruttore 1 (int 1st byte)
  22 -> SmartHome RELE B interruttore 1 (int 2nd byte)
  23 -> SmartHome RELE B interruttore 1 (int 1st byte)
  24 -> SmartHome RELE A interruttore 2 (int 2nd byte)
  25 -> SmartHome RELE A interruttore 2 (int 1st byte)
  26 -> SmartHome RELE B interruttore 2 (int 2nd byte)
  27 -> SmartHome RELE B interruttore 2 (int 1st byte)
  28 -> SmartHome RELE A interruttore 3 (int 2nd byte)
  29 -> SmartHome RELE A interruttore 3 (int 1st byte)
  30 -> SmartHome RELE B interruttore 3 (int 2nd byte)
  31 -> SmartHome RELE B interruttore 3 (int 1st byte)
  32 -> SmartHome RELE A interruttore 4 (int 2nd byte)
  33 -> SmartHome RELE A interruttore 4 (int 1st byte)
  34 -> SmartHome RELE B interruttore 4 (int 2nd byte)
  35 -> SmartHome RELE B interruttore 4 (int 1st byte)

*/

#include <EEPROM.h>

void setup_EEPROM() {
  // Inizializza EEPROM
  EEPROM.begin(512);
  delay(100);
}

void EEPROM_clear_reset() {                                              // Cancello la EEPROM e resetto l'ESP
  EEPROM.begin(512);
  // write a 0 to all 512 bytes of the EEPROM
  Debug_MSG("EEPROM clear:");
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 0);
    Debug_MSG(".");
  }
  Debug_MSG_LN("");
  Debug_MSG_LN("EEPROM CLEARED!");
  EEPROM.end();
  Debug_MSG_LN("RESET!");
  delay(500);
  ESP.restart();
}

void EEPROM_read_TEMPO_tapparelle_MAX() {                                              // Leggo dalla EEPROM TEMPO_tapparella1_MAX e TEMPO_tapparella2_MAX
  Debug_MSG("Reading EEPROM TEMPO_tapparella1_MAX: ");
  TEMPO_tapparella1_MAX = EEPROMReadlong(0);
  Debug_MSG_LN((String) TEMPO_tapparella1_MAX);
  Debug_MSG("Reading EEPROM TEMPO_tapparella2_MAX: ");
  TEMPO_tapparella2_MAX = EEPROMReadlong(4);
  Debug_MSG_LN((String) TEMPO_tapparella2_MAX);
  if (TEMPO_tapparella1_MAX < 1000) {
    TEMPO_tapparella1_MAX = 10000;
    EEPROM_write_TEMPO_tapparelle_MAX();
  }
  if (TEMPO_tapparella1_MAX > 60000) {
    TEMPO_tapparella1_MAX = 60000;
    EEPROM_write_TEMPO_tapparelle_MAX();
  }
  if (TEMPO_tapparella2_MAX < 1000) {
    TEMPO_tapparella2_MAX = 10000;
    EEPROM_write_TEMPO_tapparelle_MAX();
  }
  if (TEMPO_tapparella2_MAX > 60000) {
    TEMPO_tapparella2_MAX = 60000;
    EEPROM_write_TEMPO_tapparelle_MAX();
  }
}

void EEPROM_write_TEMPO_tapparelle_MAX() {                                              // Scrivo sulla EEPROM TEMPO_tapparella1_MAX e TEMPO_tapparella2_MAX
  long address = 0;
  Debug_MSG("Writing EEPROM TEMPO_tapparella1_MAX:");
  Debug_MSG_LN((String) TEMPO_tapparella1_MAX);
  EEPROMWritelong(address, TEMPO_tapparella1_MAX);
  address = 4;
  Debug_MSG("Writing EEPROM TEMPO_tapparella2_MAX:");
  Debug_MSG_LN((String) TEMPO_tapparella2_MAX);
  EEPROMWritelong(address, TEMPO_tapparella2_MAX);
}

int EEPROM_read_releAinterruttore1() {                                              // Leggo dalla EEPROM RELE A 1
  int tmp;
  long address = 20;
  Debug_MSG("Reading EEPROM stato RELE A interruttore 1: ");
  tmp = eepromReadInt(address);
  if (tmp != 0 && tmp != 1) tmp = 0;
  if (tmp == 1) Debug_MSG_LN("ON");
  if (tmp == 0) Debug_MSG_LN("OFF");
  return tmp;
}
int EEPROM_read_releBinterruttore1() {                                              // Leggo dalla EEPROM RELE B 1
  int tmp;
  long address = 22;
  Debug_MSG("Reading EEPROM stato RELE B interruttore 1: ");
  tmp = eepromReadInt(address);
  if (tmp != 0 && tmp != 1) tmp = 0;
  if (tmp == 1) Debug_MSG_LN("ON");
  if (tmp == 0) Debug_MSG_LN("OFF");
  return tmp;
}
int EEPROM_read_releAinterruttore2() {                                              // Leggo dalla EEPROM RELE A 2
  int tmp;
  long address = 24;
  Debug_MSG("Reading EEPROM stato RELE A interruttore 2: ");
  tmp = eepromReadInt(address);
  if (tmp != 0 && tmp != 1) tmp = 0;
  if (tmp == 1) Debug_MSG_LN("ON");
  if (tmp == 0) Debug_MSG_LN("OFF");
  return tmp;
}
int EEPROM_read_releBinterruttore2() {                                              // Leggo dalla EEPROM RELE B 2
  int tmp;
  long address = 26;
  Debug_MSG("Reading EEPROM stato RELE B interruttore 2: ");
  tmp = eepromReadInt(address);
  if (tmp != 0 && tmp != 1) tmp = 0;
  if (tmp == 1) Debug_MSG_LN("ON");
  if (tmp == 0) Debug_MSG_LN("OFF");
  return tmp;
}
int EEPROM_read_releAinterruttore3() {                                              // Leggo dalla EEPROM RELE A 3
  int tmp;
  long address = 28;
  Debug_MSG("Reading EEPROM stato RELE A interruttore 3: ");
  tmp = eepromReadInt(address);
  if (tmp != 0 && tmp != 1) tmp = 0;
  if (tmp == 1) Debug_MSG_LN("ON");
  if (tmp == 0) Debug_MSG_LN("OFF");
  return tmp;
}
int EEPROM_read_releBinterruttore3() {                                              // Leggo dalla EEPROM RELE B 3
  int tmp;
  long address = 30;
  Debug_MSG("Reading EEPROM stato RELE B interruttore 3: ");
  tmp = eepromReadInt(address);
  if (tmp != 0 && tmp != 1) tmp = 0;
  if (tmp == 1) Debug_MSG_LN("ON");
  if (tmp == 0) Debug_MSG_LN("OFF");
  return tmp;
}
int EEPROM_read_releAinterruttore4() {                                              // Leggo dalla EEPROM RELE A 4
  int tmp;
  long address = 32;
  Debug_MSG("Reading EEPROM stato RELE A interruttore 4: ");
  tmp = eepromReadInt(address);
  if (tmp != 0 && tmp != 1) tmp = 0;
  if (tmp == 1) Debug_MSG_LN("ON");
  if (tmp == 0) Debug_MSG_LN("OFF");
  return tmp;
}
int EEPROM_read_releBinterruttore4() {                                              // Leggo dalla EEPROM RELE B 4
  int tmp;
  long address = 34;
  Debug_MSG("Reading EEPROM stato RELE B interruttore 4: ");
  tmp = eepromReadInt(address);
  if (tmp != 0 && tmp != 1) tmp = 0;
  if (tmp == 1) Debug_MSG_LN("ON");
  if (tmp == 0) Debug_MSG_LN("OFF");
  return tmp;
}
void EEPROM_write_releAinterruttore1() {
  long address = 20;
  Debug_MSG("Writing EEPROM stato RELE A interruttore 1:");
  if (digitalRead(RELE_A_interruttore1) ^ Flag_inversione_RELE == true ) {
    int tmp = 1;
    eepromWriteInt(address, tmp );
    Debug_MSG_LN("ON");
  }
  if (digitalRead(RELE_A_interruttore1) ^ Flag_inversione_RELE == false) {
    int tmp = 0;
    eepromWriteInt(address,  tmp );
    Debug_MSG_LN("OFF");
  }
  EEPROM.commit();
}
void EEPROM_write_releBinterruttore1() {
  long address = 22;
  Debug_MSG("Writing EEPROM stato RELE B interruttore 1:");
  if (digitalRead(RELE_B_interruttore1) ^ Flag_inversione_RELE == true ) {
    int tmp = 1;
    eepromWriteInt(address, tmp );
    Debug_MSG_LN("ON");
  }
  if (digitalRead(RELE_B_interruttore1) ^ Flag_inversione_RELE == false) {
    int tmp = 0;
    eepromWriteInt(address,  tmp );
    Debug_MSG_LN("OFF");
  }
  EEPROM.commit();
}
void EEPROM_write_releAinterruttore2() {
  long address = 24;
  Debug_MSG("Writing EEPROM stato RELE A interruttore 2:");
  if (digitalRead(RELE_A_interruttore2) ^ Flag_inversione_RELE == true ) {
    int tmp = 1;
    eepromWriteInt(address, tmp );
    Debug_MSG_LN("ON");
  }
  if (digitalRead(RELE_A_interruttore2) ^ Flag_inversione_RELE == false) {
    int tmp = 0;
    eepromWriteInt(address,  tmp );
    Debug_MSG_LN("OFF");
  }
  EEPROM.commit();
}
void EEPROM_write_releBinterruttore2() {
  long address = 26;
  Debug_MSG("Writing EEPROM stato RELE B interruttore 2:");
  if (digitalRead(RELE_B_interruttore2) ^ Flag_inversione_RELE == true ) {
    int tmp = 1;
    eepromWriteInt(address, tmp );
    Debug_MSG_LN("ON");
  }
  if (digitalRead(RELE_B_interruttore2) ^ Flag_inversione_RELE == false) {
    int tmp = 0;
    eepromWriteInt(address,  tmp );
    Debug_MSG_LN("OFF");
  }
  EEPROM.commit();
}
void EEPROM_write_releAinterruttore3() {
  long address = 28;
  Debug_MSG("Writing EEPROM stato RELE A interruttore 3:");
  if (digitalRead(RELE_A_interruttore3) ^ Flag_inversione_RELE == true ) {
    int tmp = 1;
    eepromWriteInt(address, tmp );
    Debug_MSG_LN("ON");
  }
  if (digitalRead(RELE_A_interruttore3) ^ Flag_inversione_RELE == false) {
    int tmp = 0;
    eepromWriteInt(address,  tmp );
    Debug_MSG_LN("OFF");
  }
  EEPROM.commit();
}
void EEPROM_write_releBinterruttore3() {
  long address = 30;
  Debug_MSG("Writing EEPROM stato RELE B interruttore 3:");
  if (digitalRead(RELE_B_interruttore3) ^ Flag_inversione_RELE == true ) {
    int tmp = 1;
    eepromWriteInt(address, tmp );
    Debug_MSG_LN("ON");
  }
  if (digitalRead(RELE_B_interruttore3) ^ Flag_inversione_RELE == false) {
    int tmp = 0;
    eepromWriteInt(address,  tmp );
    Debug_MSG_LN("OFF");
  }
  EEPROM.commit();
}
void EEPROM_write_releAinterruttore4() {
  long address = 32;
  Debug_MSG("Writing EEPROM stato RELE A interruttore 4:");
  if (digitalRead(RELE_A_interruttore4) ^ Flag_inversione_RELE == true ) {
    int tmp = 1;
    eepromWriteInt(address, tmp );
    Debug_MSG_LN("ON");
  }
  if (digitalRead(RELE_A_interruttore4) ^ Flag_inversione_RELE == false) {
    int tmp = 0;
    eepromWriteInt(address,  tmp );
    Debug_MSG_LN("OFF");
  }
  EEPROM.commit();
}
void EEPROM_write_releBinterruttore4() {
  long address = 34;
  Debug_MSG("Writing EEPROM stato RELE B interruttore 4:");
  if (digitalRead(RELE_B_interruttore4) ^ Flag_inversione_RELE == true ) {
    int tmp = 1;
    eepromWriteInt(address, tmp );
    Debug_MSG_LN("ON");
  }
  if (digitalRead(RELE_B_interruttore4) ^ Flag_inversione_RELE == false) {
    int tmp = 0;
    eepromWriteInt(address,  tmp );
    Debug_MSG_LN("OFF");
  }
  EEPROM.commit();
}

void EEPROM_temperatura1_read() {
  int address = 10;
  int tmp;
  Debug_MSG("Reading EEPROM MAN/AUTO: ");
  tmp = eepromReadInt(address);
  if (tmp == 1) {
    AUTO_temperatura1 = true;
    Debug_MSG_LN("AUTO");
  }
  if (tmp == 0) {
    AUTO_temperatura1 = false;
    Debug_MSG_LN("MAN");
  }
  if ((tmp != 1 and tmp != 0) or isnan(tmp)) {
    AUTO_temperatura1 = false;
    Debug_MSG_LN("MAN");
  }
  address = 12;
  Debug_MSG("Reading EEPROM termostato: ");
  termostato_temperatura1 = eepromReadFloat(address);
  if (termostato_temperatura1<2 or termostato_temperatura1>30 or isnan(termostato_temperatura1)) termostato_temperatura1 = 18;
  Debug_MSG_LN((String) termostato_temperatura1);
}

void EEPROM_temperatura1_write() {
  int address = 10;
  Debug_MSG("Writing MAN/AUTO:");
  if (AUTO_temperatura1 == true) {
    int tmp = 1;
    eepromWriteInt(address, tmp);
    Debug_MSG_LN("AUTO");
  }
  if (AUTO_temperatura1 == false) {
    int tmp = 0;
    eepromWriteInt(address, tmp);
    Debug_MSG_LN("MAN");
  }
  address = 12;
  Debug_MSG("Writing TERMOSTATO:");
  Debug_MSG_LN((String) termostato_temperatura1);
  eepromWriteFloat(address, termostato_temperatura1);
  EEPROM.commit();
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
int eepromReadInt(int address) {
  int value = 0x0000;
  value = value | (EEPROM.read(address) << 8);
  value = value | EEPROM.read(address + 1);
  return value;
}
void eepromWriteInt(int address, int value) {
  EEPROM.write(address, (value >> 8) & 0xFF );
  EEPROM.write(address + 1, value & 0xFF);
  EEPROM.commit();
}
float eepromReadFloat(int address) {
  union u_tag {
    byte b[4];
    float fval;
  } u;
  u.b[0] = EEPROM.read(address);
  u.b[1] = EEPROM.read(address + 1);
  u.b[2] = EEPROM.read(address + 2);
  u.b[3] = EEPROM.read(address + 3);
  return u.fval;
}
void eepromWriteFloat(int address, float value) {
  union u_tag {
    byte b[4];
    float fval;
  } u;
  u.fval = value;
  EEPROM.write(address  , u.b[0]);
  EEPROM.write(address + 1, u.b[1]);
  EEPROM.write(address + 2, u.b[2]);
  EEPROM.write(address + 3, u.b[3]);
}




