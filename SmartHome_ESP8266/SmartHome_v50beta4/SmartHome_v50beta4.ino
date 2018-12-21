/*
 *****   SmartHome v5.0   *****

  Comandi MQTT verso il topic nodo Tapparella:
  su     - alza la Tapparella
  giu    - abbassa la Tapparella
  stop   - ferma la Tapparella
  +XX    - alza la Tapparella in percentuale del tempo Tapparella (XX=percentuale)
  -XX    - abbassa la Tapparella in percentuale del tempo Tapparella (XX=percentuale)
  t=XX   - tempo Tapparella (XX=tempo massimo di eccitazione rele)
  l=XX   - lock (XX=0 -> funzionamento normale (unlock) - XX=1 modo lock(i comandi MQTT ricevuti vengono ignorati))
  stato  - restituisce lo stato del nodo e dei relativi rele

  Comandi MQTT verso il topic nodo Interruttore:
  on     - eccita il rele
  off    - diseccita il rele
  toggle - inverte lo stato del rele
  stato  - restituisce lo stato del nodo e dei relativi rele

  Comandi MQTT verso il topic nodo Impulso:
  on     - eccita il rele (per un tempo impostato nel firmware)
  stato  - restituisce lo stato del nodo e dei relativi rele

  Comandi MQTT verso il topic nodo Temporizzatore:
  on     - eccita il rele (per un tempo impostabile a piacimento)
  t=XX   - tempo eccitazione rele (XX=tempo dopo il quale il rele viene diseccitato)
  stato  - restituisce lo stato del nodo e dei relativi rele

  Comandi MQTT verso il topic nodo Termostato:
  read   - restituisce la Termostato
  on     - eccita il rele del termostato
  off    - diseccita il rele del termostato
  auto   - setta modalitÃ  automatica
  man    - setta modalitÃ  manuale
  t=XX   - Termostato termostato (XX=Termostato)
  stato  - restituisce lo stato del nodo e dei relativi rele

  Comandi MQTT verso il topic nodo Sensore:
  stato  - restituisce lo stato del nodo
  
  Comandi MQTT verso il topic ACK
  ack    - interroga tutti i nodi presenti

  Comandi MQTT verso qualsiasi nodo
  reset  - resetta nodo + cancella EEPROM nodo
  reboot - riavvia il nodo
  info   - il nodo restituisce alcune info

  Changelog v5.0:
  _ Semplificato sketch
  _ Aggiunto nodo Temporizzatore
  _ Rinominato nodi
  _ Max  5 nodi Tapparella
  _ Max 10 nodi Interruttore
  _ Max 10 nodi Impulso
  _ Max  2 nodo Termostato
  _ Max 10 nodi Temporizzatore
  _ Max 10 nodi Sensore
  _ Rimappato EEPROM
  _ Aggiunto topic di ack di ogni singlolo nodo dove il nodo conferma il proprio cambiamento di stato
  _ Eliminato doppio rele in Interruttore, Termostato e Impulso (ora tutti i nodi hanno un solo rele' (tranne naturalmente Tapparella))
  _ Momentaneamente rimossa compatibiltÃ con i moduli ESP01 serial rele

  Changelog v4.5:
  _ SONOFF DUAL support
  _ Check Flash size
  _ Semplificato sketch
  _ Controllo da pagina web
  _ ESP01_SERIAL_rele support (1 o 2 relÃ¨) (solo Interruttore1 e Impulso1)
  _ TELNET DEBUG
  _ NFC support (solo Impulso1)
  _ check low memory
  _ reset giornaliero
  _ Bug fix


  TESTED ON
  Arduino:  1.8.7
  Core:     ESP8266 Community versione 2.4.0
  Scheda:   Generic ESP8266 Module, 80 MHz, ck, 26 MHz, 40MHz, Flash Mode (tabella sotto), Flash Size (tabella sotto), v2 Prebuilt (MSS=536), Disabled, None, 115200

  Nodemcu           -> Flash Mode:DIO  Flash Size:4M(1M SPIFFS)
  Electrodragon     -> Flash Mode:DIO  Flash Size:4M(1M SPIFFS)
  Hipposwitch       -> Flash Mode:DIO  Flash Size:4M(1M SPIFFS)
  GC                -> Flash Mode:DIO  Flash Size:2M(1M SPIFFS)
  ESP01             -> Flash Mode:DIO  Flash Size:1M(256k SPIFFS)
  SONOFFDUAL        -> Flash Mode:QIO  Flash Size:1M(256k SPIFFS)
  SONOFF 4CH PRO R2 -> Flash Mode:DOUT Flash Size:1M(256k SPIFFS) oppure Scheda: ESP8285
  Shelly1           -> Flash Mode:DIO  Flash Size:2M(1M SPIFFS)
*/

#include <TPush.h>
#include "SmartHome_config.h"

// MQTT topic ACK
#define ACK_Topic                 "ack"                     // ACK_Topic  (NON MODIFICARE!!!!!)

// VARIABILI
String        Versione = "5.0beta4";                       // Versione SmartHome
uint8_t       mac[6];                                 // Variabile MAC ADDRESS
int           t = 0;                                  // Variabile per intercettare il rilascio del bottone
unsigned long start_millis;

#define MinFreeHeap       5000 

TPush TIMER_REFRESH_CONNESSIONE, TIMER_LAMPEGGIO;
TPush TIMER_RESET_GIORNALIERO;

void setup() {
#if defined(DEBUG)
  Serial.begin(115200);
#endif
  Check_flash_chip_configuration();
  setup_EEPROM();
  setup_GPIO();
  
  setup_Tapparella();
  setup_Termostato();
  setup_Interruttore();
  setup_Impulso();
  setup_Temporizzatore();
  setup_Sensore();
  setup_Display();
    //setup_NuovoNodo();

  setup_wifi();
  setup_web();
  setup_OTA();
  setup_telnet();
  setup_MQTT();
}

void loop() {
  loop_lampeggio_led();
  loop_check_low_memory();
  //loop_reset_giornaliero();

  loop_wifi();
  loop_web();
  loop_OTA();
  loop_telnet();
  loop_MQTT();

  loop_Tapparella();
  loop_Termostato();
  loop_Interruttore();
  loop_Impulso();
  loop_Temporizzatore();
  loop_Sensore();
  loop_Display();
  //loop_NuovoNodo();
}
