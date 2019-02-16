/*
  Mappa EEPROM
*/

long EEPROM_Tapparella_tempo_max = 0;
/*
  0   -> SmartHome Tapparella 1  - Tapparella_tempo_max (long 4th byte)
  1   -> SmartHome Tapparella 1  - Tapparella_tempo_max (long 3th byte)
  2   -> SmartHome Tapparella 1  - Tapparella_tempo_max (long 2nd byte)
  3   -> SmartHome Tapparella 1  - Tapparella_tempo_max (long 1st byte)
  4   -> SmartHome Tapparella 2  - Tapparella_tempo_max (long 4th byte)
  5   -> SmartHome Tapparella 2  - Tapparella_tempo_max (long 3th byte)
  6   -> SmartHome Tapparella 2  - Tapparella_tempo_max (long 2nd byte)
  7   -> SmartHome Tapparella 2  - Tapparella_tempo_max (long 1st byte)
  8   -> SmartHome Tapparella 3  - Tapparella_tempo_max (long 4th byte)
  9   -> SmartHome Tapparella 3  - Tapparella_tempo_max (long 3th byte)
  10  -> SmartHome Tapparella 3  - Tapparella_tempo_max (long 2nd byte)
  11  -> SmartHome Tapparella 3  - Tapparella_tempo_max (long 1st byte)
  12  -> SmartHome Tapparella 4  - Tapparella_tempo_max (long 4th byte)
  13  -> SmartHome Tapparella 4  - Tapparella_tempo_max (long 3th byte)
  14  -> SmartHome Tapparella 4  - Tapparella_tempo_max (long 2nd byte)
  15  -> SmartHome Tapparella 4  - Tapparella_tempo_max (long 1st byte)
  16  -> SmartHome Tapparella 5  - Tapparella_tempo_max (long 4th byte)
  17  -> SmartHome Tapparella 5  - Tapparella_tempo_max (long 3th byte)
  18  -> SmartHome Tapparella 5  - Tapparella_tempo_max (long 2nd byte)
  19  -> SmartHome Tapparella 5  - Tapparella_tempo_max (long 1st byte)
*/
long EEPROM_Tapparella_lock = 20;
/*
  20  -> SmartHome Tapparella 1 - 1=lock 0=unlock (int 1 byte)
  21  -> SmartHome Tapparella 2 - 1=lock 0=unlock (int 1 byte)
  22  -> SmartHome Tapparella 3 - 1=lock 0=unlock (int 1 byte)
  23  -> SmartHome Tapparella 4 - 1=lock 0=unlock (int 1 byte)
  24  -> SmartHome Tapparella 5 - 1=lock 0=unlock (int 1 byte)
*/
long EEPROM_Tapparella_percentuale = 25;
/*
  25  -> SmartHome Tapparella 1 - percentuale (float 4th byte)
  26  -> SmartHome Tapparella 1 - percentuale (float 3th byte)
  27  -> SmartHome Tapparella 1 - percentuale (float 2nd byte)
  28  -> SmartHome Tapparella 1 - percentuale (float 1st byte)
  29  -> SmartHome Tapparella 2 - percentuale (float 4th byte)
  30  -> SmartHome Tapparella 2 - percentuale (float 3th byte)
  31  -> SmartHome Tapparella 2 - percentuale (float 2nd byte)
  32  -> SmartHome Tapparella 2 - percentuale (float 1st byte)
  33  -> SmartHome Tapparella 3 - percentuale (float 4th byte)
  34  -> SmartHome Tapparella 3 - percentuale (float 3th byte)
  35  -> SmartHome Tapparella 3 - percentuale (float 2nd byte)
  36  -> SmartHome Tapparella 3 - percentuale (float 1st byte)
  37  -> SmartHome Tapparella 4 - percentuale (float 4th byte)
  38  -> SmartHome Tapparella 4 - percentuale (float 3th byte)
  39  -> SmartHome Tapparella 4 - percentuale (float 2nd byte)
  40  -> SmartHome Tapparella 4 - percentuale (float 1st byte)
  41  -> SmartHome Tapparella 5 - percentuale (float 4th byte)
  42  -> SmartHome Tapparella 5 - percentuale (float 3th byte)
  43  -> SmartHome Tapparella 5 - percentuale (float 2nd byte)
  44  -> SmartHome Tapparella 5 - percentuale (float 1st byte)
*/

long EEPROM_Termostato_auto_rele = 100;
/*
  100  -> SmartHome Termostato 1 - AUTO 1=AUTO 0=MAN (int 1 byte)
  101  -> SmartHome Termostato 1 - RELE 1=ON   0=OFF (int 1 byte)
  102  -> SmartHome Termostato 2 - AUTO 1=AUTO 0=MAN (int 1 byte)
  103  -> SmartHome Termostato 2 - RELE 1=ON   0=OFF (int 1 byte)
*/
long EEPROM_Termostato_termostato = 104;
/*
  104  -> SmartHome Termostato 1 - TERMOSTATO (float 4th byte)
  105  -> SmartHome Termostato 1 - TERMOSTATO (float 3th byte)
  106  -> SmartHome Termostato 1 - TERMOSTATO (float 2nd byte)
  107  -> SmartHome Termostato 1 - TERMOSTATO (float 1st byte)
  108  -> SmartHome Termostato 2 - TERMOSTATO (float 4th byte)
  109  -> SmartHome Termostato 2 - TERMOSTATO (float 3th byte)
  110  -> SmartHome Termostato 2 - TERMOSTATO (float 2nd byte)
  111  -> SmartHome Termostato 2 - TERMOSTATO (float 1st byte)
*/
long EEPROM_Interruttore_stato_rele = 200;
/*
  200  -> SmartHome RELE Interruttore  1 (int 1 byte)
  201  -> SmartHome RELE Interruttore  2 (int 1 byte)
  202  -> SmartHome RELE Interruttore  3 (int 1 byte)
  203  -> SmartHome RELE Interruttore  4 (int 1 byte)
  204  -> SmartHome RELE Interruttore  5 (int 1 byte)
  205  -> SmartHome RELE Interruttore  6 (int 1 byte)
  206  -> SmartHome RELE Interruttore  7 (int 1 byte)
  207  -> SmartHome RELE Interruttore  8 (int 1 byte)
  208  -> SmartHome RELE Interruttore  9 (int 1 byte)
  209  -> SmartHome RELE Interruttore 10 (int 1 byte)
*/
long EEPROM_Temporizzatore_tempo = 300;
/*
  300  -> SmartHome Temporizzatore 1  - Temporizzatore_tempo (long 4th byte)
  301  -> SmartHome Temporizzatore 1  - Temporizzatore_tempo (long 3th byte)
  302  -> SmartHome Temporizzatore 1  - Temporizzatore_tempo (long 2nd byte)
  303  -> SmartHome Temporizzatore 1  - Temporizzatore_tempo (long 1st byte)
  304  -> SmartHome Temporizzatore 2  - Temporizzatore_tempo (long 4th byte)
  305  -> SmartHome Temporizzatore 2  - Temporizzatore_tempo (long 3th byte)
  306  -> SmartHome Temporizzatore 2  - Temporizzatore_tempo (long 2nd byte)
  307  -> SmartHome Temporizzatore 2  - Temporizzatore_tempo (long 1st byte)
  308  -> SmartHome Temporizzatore 3  - Temporizzatore_tempo (long 4th byte)
  309  -> SmartHome Temporizzatore 3  - Temporizzatore_tempo (long 3th byte)
  310  -> SmartHome Temporizzatore 3  - Temporizzatore_tempo (long 2nd byte)
  311  -> SmartHome Temporizzatore 3  - Temporizzatore_tempo (long 1st byte)
  312  -> SmartHome Temporizzatore 4  - Temporizzatore_tempo (long 4th byte)
  313  -> SmartHome Temporizzatore 4  - Temporizzatore_tempo (long 3th byte)
  314  -> SmartHome Temporizzatore 4  - Temporizzatore_tempo (long 2nd byte)
  315  -> SmartHome Temporizzatore 4  - Temporizzatore_tempo (long 1st byte)
  316  -> SmartHome Temporizzatore 5  - Temporizzatore_tempo (long 4th byte)
  317  -> SmartHome Temporizzatore 5  - Temporizzatore_tempo (long 3th byte)
  318  -> SmartHome Temporizzatore 5  - Temporizzatore_tempo (long 2nd byte)
  319  -> SmartHome Temporizzatore 5  - Temporizzatore_tempo (long 1st byte)
*/
long EEPROM_Display = 350;
int EEPROM_Display_len = 14;
/*
  350 -> SmartHome Display 1 riga 1
  370 -> SmartHome Display 1 riga 2
  390 -> SmartHome Display 1 riga 3
*/
long EEPROM_NFC = 400;
/*
  400 -> NFC UID (4 byte)
  ...
  500 ->
*/

#include <EEPROM.h>

void setup_EEPROM() {
  // Inizializza EEPROM
  Debug_MSG_LN("EEPROM initializing");
  Debug_MSG_LN("");
  EEPROM.begin(512);
  delay(100);
}

void EEPROM_clear_reset() {                                              // Pulisce la EEPROM e resetta l'ESP
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

//This function will write a 4 byte (32bit) long to the eeprom at
//the specified address to address + 3.
void EEPROMWritelong(int address, long value) {
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
long EEPROMReadlong(long address) {
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

int eepromRead(int address) {
  int value = 0x0000;
  value = EEPROM.read(address);
  return value;
}

void eepromWrite(int address, int value) {
  EEPROM.write(address, value & 0xFF);
  EEPROM.commit();
}
