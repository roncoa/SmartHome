
/*
  SmartHome v4.5

  Changelog:
  SONOFF DUAL support
  Check Flash size
  Semplificato sketch
  Controllo da pagina web
  ESP01_SERIAL_RELE support (1 o 2 relè) (solo Interruttore1 e Cancello1)
  TELNET DEBUG
  NFC support (solo Cancello1)
  Bug fix
*/


#include <ESP8266WiFi.h>
#include <TPush.h>
#include "DHT.h"
#include "SSD1306.h"

#include "SmartHome_config.h"

// VARIABILI
String        Versione = "4.5";                       // Versione SmartHome
uint8_t       mac[6];                                 // Variabile MAC ADDRESS
long          TEMPO_tapparella1_MAX = 15000;          // Variabile tempo MAX tapparella 1 in azione (in millisecondi)
long          TEMPO_tapparella2_MAX = 15000;          // Variabile tempo MAX tapparella 2 in azione (in millisecondi)
long          percentualeTapparella;                  // Variabili percentuale tapparella
boolean       In_movimento_tapparella1 = false;       // Variabile tapparella 1 in movimento
boolean       In_movimento_tapparella2 = false;       // Variabile tapparella 2 in movimento
unsigned long ulTimeTapparella1, ulTimeTapparella2;   // Variabili timers tapparelle
unsigned long ulTimeReleACancello1, ulTimeReleBCancello1, ulTimeReleACancello2, ulTimeReleBCancello2; // Variabili timers cancelli
int           t = 0;                                  // Variabile per intercettare il rilascio del pulsante
int           lamp = 0;                               // Variabile per lampeggio LED
float         t_temperatura1;                         // Variabile Temperature Celsius
float         h_temperatura1;                         // Variabile Humidity
float         f_temperatura1;                         // Variabile Temperature Fahrenheit
float         hi_temperatura1;                        // Variabile Heat index in Celsius (isFahreheit = false)
float         termostato_temperatura1 = 18.00;        // Variabile temperatura
boolean       AUTO_temperatura1 = false;              // Variabile MAN/AUTO
boolean       Rele_A_eccitato_cancello1 = false;      // Variabile rele A cancello 1
boolean       Rele_B_eccitato_cancello1 = false;      // Variabile rele B cancello 1
boolean       Rele_A_eccitato_cancello2 = false;      // Variabile rele A cancello 2
boolean       Rele_B_eccitato_cancello2 = false;      // Variabile rele B cancello 2

TPush BottoneTAP1SU, BottoneTAP1GIU, BottoneTAP2SU, BottoneTAP2GIU;
TPush BottoneAINT1, BottoneBINT1, BottoneAINT2, BottoneBINT2, BottoneAINT3, BottoneBINT3, BottoneAINT4, BottoneBINT4;
TPush BottoneACAN1, BottoneBCAN1, BottoneACAN2, BottoneBCAN2;
TPush TIMER_TERMOSTATO1;
TPush TIMER_REFRESH_CONNESSIONE, TIMER_RESET_GIORNALIERO, TIMER_LAMPEGGIO;

DHT dht(DHTPIN_temperatura1, DHTTYPE, 15);
SSD1306 display(I2C_DISPLAY_ADDRESS_temperatura1, SDA_PIN_temperatura1, SDC_PIN_temperatura1);



void setup() {
#if defined(DEBUG)
  Serial.begin(115200);
#endif
  Check_flash_chip_configuration();
  setup_EEPROM();
  setup_GPIO();

  setup_wifi();
  setup_web();
  setup_MQTT();
  setup_OTA();
  setup_telnet();

}



void loop() {

  loop_lampeggio_led();

  loop_check_low_memory();
  loop_reset_giornaliero();

  loop_wifi();
  loop_web();
  loop_MQTT();
  loop_OTA();
  loop_telnet();

  loop_tapparelle();
  loop_interruttori();
  loop_cancelli();
  loop_temperatura();
}



