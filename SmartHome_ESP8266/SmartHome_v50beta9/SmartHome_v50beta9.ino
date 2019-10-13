/*

 ************************************
 *****   SmartHome v5.0 beta 9  *****
 ************************************

  Testato con:
  Arduino IDE : -> 1.8.9
  ESP8266 Core: ESP8266 Community versione -> 2.5.2
  Scheda      : Generic ESP8266 Module, 115200, 80 MHz, 26 MHz, Flash Size (tabella sotto), Flash Mode (tabella sotto), 40MHz, nodemcu,
                Disabled, Nessuno, v2 Lower Memory, Flash, Disabled, 2, Only Sketch, nonos-sdk 2.2.1 (legacy), All SSL cipehers (most compatible)

  Settaggi Flash Mode e Flash Size di Arduino IDE:
  Nodemcu           -> Flash Mode:DIO  Flash Size:4M
  ESP01             -> Flash Mode:DIO  Flash Size:1M
  Electrodragon     -> Flash Mode:DIO  Flash Size:4M
  Hipposwitch       -> Flash Mode:DIO  Flash Size:4M
  GC                -> Flash Mode:DIO  Flash Size:2M
  SONOFFDUAL        -> Flash Mode:QIO  Flash Size:1M
  SONOFF 4CH PRO R2 -> Flash Mode:DOUT Flash Size:1M oppure Scheda: ESP8285
  Shelly1           -> Flash Mode:DIO  Flash Size:2M
  Shelly2           -> Flash Mode:DIO  Flash Size:2M
  Shelly2.5         -> Flash Mode:DIO  Flash Size:2M
  BlitzWolf         -> Flash Mode:DIO  Flash Size:1M
  DE004             -> Flash Mode:DIO  Flash Size:1M

  Librerie core ESP8266:
  <ESP8266WiFi.h>
  <ESP8266WebServer.h>
  <EEPROM.h>
  <ArduinoOTA.h>
  <Ticker.h>
  <SPI.h>
  <time.h>
  <sys/time.h>
  <coredecls.h>
  
  Librerie esterne:
  <PubSubClient.h>       https://github.com/knolleary/pubsubclient
  <simpleDSTadjust.h>    https://github.com/neptune2/simpleDSTadjust
  <TPush.h>              https://github.com/TheTrigger/TPush
  "AsyncPing.h"          https://github.com/akaJes/AsyncPing
  "MFRC522.h"            https://github.com/miguelbalboa/rfid (Impulso NFC)
  "DHT.h"                https://github.com/adafruit/DHT-sensor-library + https://github.com/adafruit/Adafruit_Sensor (Termostato)
  "SSD1306.h"            https://github.com/ThingPulse/esp8266-oled-ssd1306 (Display - Termostato)
  <PZEM004T.h>           https://github.com/olehs/PZEM004T (Power)
  "HLW8012.h"            https://github.com/xoseperez/hlw8012 (Power)
  <IRremoteESP8266.h>     (IR)

  GPIO CONSIGLIATI:
                  Nodemcu ESP01 Elctrodragon Hipposwitch Shelly1 Shelly2 SONOFFDUAL SONOFF4CHPROR2
             OUT1 12      0     12           12          4       4       14         12
             OUT2 13      2     13           14          -       5       15         5
             OUT3 -       -     -            -           -       -       -          4
             OUT4 -       -     -            -           -       -       -          15
              IN1 4       1     4            ?           5       12      4          0
              IN2 5       3     5            ?           -       14      5          9
              IN3 -       -     -            -           -       -       -          10
              IN4 -       -     -            -           -       -       -          14
  GPIO Status_LED 16      4     16           15          16      16      13         13
  Inv. Status_LED 1       1     0            0           1       1       1          1
        Inv. RELE 1       1     0            0           0       0       0          0
                                PullUp       ?           PullUp  PullUp  PullUp     PullUp

  PullUp   -> resistenza verso vcc, GPIO che chiude verso gnd
  PullDown -> resistenza verso gnd, GPIO che chiude verso vcc

  Comandi MQTT verso il topic nodo Tapparella:
  su     - alza la Tapparella
  giu    - abbassa la Tapparella
  stop   - ferma la Tapparella
  +XX    - alza la Tapparella in percentuale del tempo Tapparella (XX=percentuale)
  -XX    - abbassa la Tapparella in percentuale del tempo Tapparella (XX=percentuale)
  t=XX   - tempo Tapparella (XX=tempo massimo di eccitazione rele)
  l=XX   - lock (XX=0 -> funzionamento normale (unlock) - XX=1 modo lock(i comandi MQTT ricevuti vengono ignorati))
  %=XX   - porta la Tapparella alla percentuale XX
  stato  - restituisce lo stato del nodo e dei relativi rele

  Comandi MQTT verso il topic nodo Interruttore:
  on     - eccita il relè
  off    - diseccita il relè
  toggle - inverte lo stato del rele
  stato  - restituisce lo stato del nodo e dei relativi rele

  Comandi MQTT verso il topic nodo Impulso:
  on     - eccita il relè (per un tempo impostato nel firmware)
  stato  - restituisce lo stato del nodo e dei relativi rele

  Comandi MQTT verso il topic nodo Temporizzatore:
  on     - eccita il relè (per un tempo impostabile a piacimento)
  t=XX   - tempo eccitazione rele (XX=tempo dopo il quale il relè viene diseccitato)
  stato  - restituisce lo stato del nodo e dei relativi rele

  Comandi MQTT verso il topic nodo Termostato:
  read   - restituisce la Termostato
  on     - eccita il relè del termostato
  off    - diseccita il relè del termostato
  auto   - setta modalità automatica
  man    - setta modalità manuale
  t=XX   - Termostato termostato (XX=Termostato)
  stato  - restituisce lo stato del nodo e dei relativi rele

  Comandi MQTT verso il topic nodo Sensore:
  stato  - restituisce lo stato del nodo

  Comandi MQTT verso il topic nodo Power:
  stato  - restituisce lo stato del nodo
  CXX    - Calibra dispositivo (XX=Watt di calibrazione)

  Comandi MQTT verso il topic nodo IR:
  stato  - restituisce lo stato del nodo
  
  Comandi MQTT verso il topic ACK
  ack    - interroga tutti i nodi presenti
  info   - tutti i nodi restituiscono alcune info

  Comandi MQTT verso qualsiasi nodo
  reset  - resetta nodo + cancella EEPROM nodo
  reboot - riavvia il nodo
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ticker.h>
#include <TPush.h>
#include <EEPROM.h>
#include <ArduinoJson.h>

#include "SmartHome_config.h"

#define ACK_Topic                 "ack"               // [Default "ack"] Topic di ACK (NON MODIFICARE!!!!!)
#define MinFreeHeap               10000               // [Default 10000] valore minimo di Heap sotto al quale il dispositivo viene resettato
#define TEMPO_CLICK_ON            150                 // [Default 150] tempo click ON  (msec.) almeno 10
#define TEMPO_CLICK_OFF           100                 // [Default 100] tempo click OFF (msec.) almeno 5

String        Versione = "5.0beta9";
uint8_t       mac[6];
int           t = 0;
unsigned long start_millis;

void setup() {
#ifdef DEBUG
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  // Serial.setDebugOutput(true);
  // WiFi.printDiag(Serial);
#endif
#ifdef DEBUG1
  Serial1.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  // Serial1.setDebugOutput(true);
#endif

  Show_vesione();
  Check_flash_chip_configuration();
  Check_PubSubClient();
  setup_EEPROM();
  setup_Status_LED();

#if Tapparella_nodi>0
  setup_Tapparella();
#endif
#if Termostato_nodi>0
  setup_Termostato();
#endif
#if Interruttore_nodi>0
  setup_Interruttore();
#endif
#if Impulso_nodi>0
  setup_Impulso();
#endif
#if Temporizzatore_nodi>0
  setup_Temporizzatore();
#endif
#if Sensore_nodi>0
  setup_Sensore();
#endif
#if Display_nodi>0
  setup_Display();
#endif
#if Power_nodi>0
  setup_Power();
#endif
#if IR_nodi>0
  setup_IR();
#endif

  setup_reti();
}

void loop() {
  loop_Status_LED();
  loop_check_low_memory();

#ifdef RESET_GIORNALIERO
  loop_reset_giornaliero();
#endif
#if Tapparella_nodi>0
  loop_Tapparella();
#endif
#if Termostato_nodi>0
  loop_Termostato();
#endif
#if Interruttore_nodi>0
  loop_Interruttore();
#endif
#if Impulso_nodi>0
  loop_Impulso();
#endif
#if Temporizzatore_nodi>0
  loop_Temporizzatore();
#endif
#if Sensore_nodi>0
  loop_Sensore();
#endif
#if Display_nodi>0
  loop_Display();
#endif
#if Power_nodi>0
  loop_Power();
#endif
#if IR_nodi>0
  loop_IR();
#endif

  loop_reti();
#ifdef WEB_SERVER
  loop_web();
#endif
#ifdef TELNET_DEBUG
  loop_telnet();
#endif
#ifdef ArduinoIDE_OTA
  loop_OTA();
#endif
}

#if (MQTT_MAX_PACKET_SIZE < 1024)
#error In PubSubClient.h MQTT_MAX_PACKET_SIZE deve essere >= 1024
#endif

#if (defined(PullUp) and defined(PullDown)) or (!defined(PullUp) and !defined(PullDown))
#error In SmartHome_config.h definire correttamete PullUp o PullDown per i GPIO
#endif

#if (defined(SONOFFDUAL) and defined(ESP01_SERIAL_RELE))
#error In SmartHome_config.h definire solo un dispositivo con GPIO non standard (SONOFFDUAL o ESP01_SERIAL_RELE)
#endif

#if (defined(SONOFFDUAL) or defined(ESP01_SERIAL_RELE))
#undef DEBUG
#endif

#if (Power_nodi>0 and Power1_device == 1 and (Power1_GPIO_RX == 3 or Power1_GPIO_TX == 1) and defined(DEBUG))
#error PZEM-004 con RX=3 e TX=1 non può usare il serial DEBUG
#endif

#if (Tapparella_nodi>5 or Interruttore_nodi>10 or Impulso_nodi>10 or Termostato_nodi>1 or Temporizzatore_nodi>10 or Sensore_nodi>10 or Display_nodi>1 or Power_nodi>1 or IR_nodi>1)
#error Nodi definiti in numero errato
#endif

#if Impulso_nodi<1
#undef Impulso1_NFC
#endif
