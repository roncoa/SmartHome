/*
  Electrodragon -> Flash Mode:QIO Flash Size:4M(1M SPIFFS)
  ESP01         -> Flash Mode:DIO Flash Size:1M(256k SPIFFS)
  SONOFFDUAL    -> Flash Mode:DIO Flash Size:1M(64K SPIFFS)
*/


// DEBUG
#define DEBUG                                       // Commentare questa riga per disabilitare il SERIAL DEBUG
#define TELNET_DEBUG                                // Commentare questa riga per disabilitare il TELNET DEBUG

// MQTT server
  const char* ssid = "WIFI_SSID" ;                    // WIFI SSID
  const char* password = "WIFI_PASSWORD";             // WIFI password
  const char* mqtt_server = "MQTT_SERVER";            // MQTT server
  const char* mqtt_user = "MQTT_USER";                // MQTT user
  const char* mqtt_password = "MQTT_PASSWORD";        // MQTT password
  const int   mqtt_port = MQTT_port;                  // MQTT port
  //const char* mqtt_server = "192.168.1.200";          // MQTT server raspberry
  //const int   mqtt_port = 1883;                       // MQTT port raspberry

// Numero nodi da compilare
#define NODI_TAPPARELLA     1                       // Max 2 nodi tapparella (consigliato usarne max 1)
#define NODI_INTERRUTTORE   0                       // Max 4 nodi interruttore
#define NODI_CANCELLO       0                       // Max 2 nodi cancello
#define NODI_TEMPERATURA    0                       // Max 1 nodo temperatura

// MQTT topic
#define Tapparella1_Topic   "tapparella/1"          // Tapparella 1 Topic (non usare underscore "_")
#define Tapparella2_Topic   "tapparella/2"          // Tapparella 2 Topic (non usare underscore "_")
#define Interruttore1_Topic "interruttore/1"        // Interruttore 1 Topic (non usare underscore "_")
#define Interruttore2_Topic "interruttore/2"        // Interruttore 2 Topic (non usare underscore "_")
#define Interruttore3_Topic "interruttore/3"        // Interruttore 3 Topic (non usare underscore "_")
#define Interruttore4_Topic "interruttore/4"        // Interruttore 4 Topic (non usare underscore "_")
#define Cancello1_Topic     "cancello/1"            // Cancello 1 Topic (non usare underscore "_")
#define Cancello2_Topic     "cancello/2"            // Cancello 2 Topic (non usare underscore "_")
#define Temperatura1_Topic  "temperatura/1"         // Temperatura 1 Topic (non usare underscore "_")
#define ACK_Topic           "ack"                   // ACK_Topic  (NON MODIFICARE!!!!!)
#define ACK2_Topic          "ack2"                  // ACK2_Topic (NON MODIFICARE!!!!!)
#define OTA_hostname        "OTAhostname"           // Hostname OTA

// GPIO
//#define SONOFFDUAL                                  // Il modulo è un SONOFF DUAL  (commentare se non è un SONOFF DUAL)
//#define ESP01_SERIAL_RELE                           // Il modulo è un ESP01 RELE  (commentare se non è un ESP01 RELE)
#define Flag_inversione_RELE         1              // Inversione del segnale di uscita RELE       (0=normale - 1=invertito) nodemcu=1 - ESP01=1 - SONOFFDUAL=0 - elctrodragon=0
#define Flag_inversione_Status_LED   1              // Inversione del segnale di uscita Status_LED (0=normale - 1=invertito) nodemcu=1 - ESP01=1 - SONOFFDUAL=1 - elctrodragon=0
#define Status_LED                   16             // BUILTIN_LED nodemcu=16 - ESP01=4 - SONOFFDUAL=13 - elctrodragon=16
#define RELE_tapparella1_SU          12             // RELE tapparella 1 SU      nodemcu=12 - ESP01=0 - SONOFFDUAL=14 - elctrodragon=12
#define RELE_tapparella1_GIU         13             // RELE tapparella 1 GIU     nodemcu=13 - ESP01=2 - SONOFFDUAL=15 - elctrodragon=13
#define BOTTONE_tapparella1_SU        4             // Pulsante tapparella 1 SU  nodemcu=4 - ESP01=1 - SONOFFDUAL=4 - elctrodragon=4
#define BOTTONE_tapparella1_GIU       5             // Pulsante tapparella 1 GIU nodemcu=5 - ESP01=3 - SONOFFDUAL=5 - elctrodragon=5
#define RELE_tapparella2_SU          12             // RELE tapparella 2 SU
#define RELE_tapparella2_GIU         13             // RELE tapparella 2 GIU
#define BOTTONE_tapparella2_SU        4             // Pulsante tapparella 2 SU
#define BOTTONE_tapparella2_GIU       5             // Pulsante tapparella 2 GIU
#define RELE_A_interruttore1         12             // RELE A interruttore 1
#define RELE_B_interruttore1         13             // RELE B interruttore 1
#define RELE_A_interruttore2         12             // RELE A interruttore 2
#define RELE_B_interruttore2         13             // RELE B interruttore 2
#define RELE_A_interruttore3         12             // RELE A interruttore 3
#define RELE_B_interruttore3         13             // RELE B interruttore 3
#define RELE_A_interruttore4         12             // RELE A interruttore 4
#define RELE_B_interruttore4         13             // RELE B interruttore 4
#define BOTTONE_A_interruttore1       4             // Pulsante A interruttore 1
#define BOTTONE_B_interruttore1       5             // Pulsante B interruttore 1
#define BOTTONE_A_interruttore2       4             // Pulsante A interruttore 2
#define BOTTONE_B_interruttore2       5             // Pulsante B interruttore 2
#define BOTTONE_A_interruttore3       4             // Pulsante A interruttore 3
#define BOTTONE_B_interruttore3       5             // Pulsante B interruttore 3
#define BOTTONE_A_interruttore4       4             // Pulsante A interruttore 4
#define BOTTONE_B_interruttore4       5             // Pulsante B interruttore 4
#define TIPO_BOTTONE_A_interruttore1 "interruttore" // Interruttore/pulsante A interruttore 1   ("interruttore" o "pulsante")
#define TIPO_BOTTONE_B_interruttore1 "pulsante" // Interruttore/pulsante B interruttore 1   ("interruttore" o "pulsante")
#define TIPO_BOTTONE_A_interruttore2 "interruttore" // Interruttore/pulsante A interruttore 2   ("interruttore" o "pulsante")
#define TIPO_BOTTONE_B_interruttore2 "interruttore" // Interruttore/pulsante B interruttore 2   ("interruttore" o "pulsante")
#define TIPO_BOTTONE_A_interruttore3 "interruttore" // Interruttore/pulsante A interruttore 3   ("interruttore" o "pulsante")
#define TIPO_BOTTONE_B_interruttore3 "interruttore" // Interruttore/pulsante B interruttore 3   ("interruttore" o "pulsante")
#define TIPO_BOTTONE_A_interruttore4 "interruttore" // Interruttore/pulsante A interruttore 4   ("interruttore" o "pulsante")
#define TIPO_BOTTONE_B_interruttore4 "interruttore" // Interruttore/pulsante B interruttore 4   ("interruttore" o "pulsante")
#define RELE_A_cancello1              2             // RELE A cancello 1
#define RELE_B_cancello1             13             // RELE B cancello 1
#define BOTTONE_A_cancello1           4             // Pulsante A cancello 1
#define BOTTONE_B_cancello1           5             // Pulsante B cancello 1
#define RELE_A_cancello2             12             // RELE A cancello 2
#define RELE_B_cancello2             13             // RELE B cancello 2
#define BOTTONE_A_cancello2           4             // Pulsante A cancello 2
#define BOTTONE_B_cancello2           5             // Pulsante B cancello 2
#define RELE_A_temperatura1          12             // RELE A temperatura 1 (termoststo)
#define RELE_B_temperatura1          13             // RELE B temperatura 1 (AUX)
#define DHTPIN_temperatura1           2             // pin DHT22 temperatura 1 (2)
#define I2C_DISPLAY_ADDRESS_temperatura1 0x3c       // indirizzo I2C Display temperatura 1
#define SDA_PIN_temperatura1          4             // pin SDA Display temperatura 1 (4)
#define SDC_PIN_temperatura1          5             // pin SDC Display temperatura 1 (5)
//#define CANCELLO_NFC
/* wiring the MFRC522 to ESP8266 (ESP-12)
  RST     = RST_PIN
  SDA(SS) = SS_PIN
  MOSI    = GPIO13
  MISO    = GPIO12
  SCK     = GPIO14
*/
#define RST_PIN                       5             // RST-PIN - RC522 - RFID - SPI - Modul GPIO5 
#define SS_PIN                        4             // SDA-PIN - RC522 - RFID - SPI - Modul GPIO4 
#define PullUp                                      // resistenza PullUp verso vcc   (commentare solo uno dei due (PullUp o PullDown))
//#define PullDown                                   // resistenza PullDown verso gnd (commentare solo uno dei due (PullUp o PullDown))

byte ESP01_SERIAL_RELE_AON_data[]  = {0xA0, 0x01, 0x01, 0xA2};
byte ESP01_SERIAL_RELE_AOFF_data[] = {0xA0, 0x01, 0x00, 0xA1};
byte ESP01_SERIAL_RELE_BON_data[]  = {0xA0, 0x02, 0x01, 0xA3};
byte ESP01_SERIAL_RELE_BOFF_data[] = {0xA0, 0x02, 0x00, 0xA2};


// TEMPI
#define MAX_RET_WIFI                20              // Indica per quante volte ritenta di connettersi al WIFI (20)
#define MAX_RET_MQTT                3               // Indica per quante volte ritenta di connettersi al server MQTT (3)
#define TEMPO_REFRESH_CONNESSIONE   60000           // Indica il timeout di refresh della connessione (60000=1 min.)
#define TEMPO_CLICK_ON              250             // Indica il tempo minimo di pressione bottone (150)
#define TEMPO_CLICK_OFF             250             // Indica il tempo minimo di rilascio bottone (100)
#define TEMPO_RELE                  100             // Indica il tempo tra una commutazione RELE e la successiva (200)
#define TEMPO_TERMOSTATO            5000            // Indica il tempo di refresh del termostato in AUTO
#define TEMPO_IMPULSO_cancello      500             // Indica la durata dell'impulso al rele' (in millisecondi)

// Sensore temperatura
#define DHTTYPE                     DHT22           // tipo sensore DHT 22  (AM2302)
float         soglia_temperatura1 = 0.25;           // soglia temperatura termostato +/- 0.25

#if (defined(PullUp) and defined(PullDown)) or (!defined(PullUp) and !defined(PullDown))
#error        In SmartHome.h nella sezione /GPIO commentare solo uno dei due (PullUp o PullDown)
#endif

#if (defined(SONOFFDUAL) and defined(ESP01_SERIAL_RELE))
#error        In SmartHome.h nella sezione /GPIO definire solo 1 tipo di hardware (SONOFFDUAL o ESP01_SERIAL_RELE)
#endif

#if defined(ESP01_SERIAL_RELE)
#undef DEBUG
#endif



