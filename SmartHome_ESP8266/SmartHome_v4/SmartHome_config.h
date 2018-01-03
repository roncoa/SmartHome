// DEBUG
#define DEBUG                                       // Commentare questa riga per disabilitare il SERIAL DEBUG

// MQTT server
const char* ssid = "**********" ;                   // WIFI SSID
const char* password = "**********";                // WIFI password
const char* mqtt_server = "***.******.***";         // MQTT server
const char* mqtt_user = "**********";               // MQTT user
const char* mqtt_password = "**********";           // MQTT password
const int   mqtt_port = ****;                       // MQTT port
//const char* mqtt_server = "192.168.1.200";        // MQTT server raspberry
//const int   mqtt_port = 1883;                     // MQTT port raspberry

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
#define OTA_hostname        "OTA"                   // Hostname OTA

// GPIO
#define Flag_inversione_RELE         1              // Inversione del segnale di uscita RELE       (0=normale - 1=invertito) nodemcu=1 - ESP01=1 - SONOFFDUAL=0 - elctrodragon=0
#define Flag_inversione_Status_LED   0              // Inversione del segnale di uscita Status_LED (0=normale - 1=invertito) nodemcu=1 - ESP01=1 - SONOFFDUAL=1 - elctrodragon=0
#define Status_LED                   BUILTIN_LED    // BUILTIN_LED nodemcu=16 - ESP01=4 - SONOFFDUAL=13 - elctrodragon=16
#define RELE_tapparella1_SU          12             // RELE tapparella 1 SU      nodemcu=12 - ESP01=0 - SONOFFDUAL=14 - elctrodragon=12
#define RELE_tapparella1_GIU         13             // RELE tapparella 1 GIU     nodemcu=13 - ESP01=2 - SONOFFDUAL=15 - elctrodragon=13
#define BOTTONE_tapparella1_SU        0             // Pulsante tapparella 1 SU  nodemcu=4 - ESP01=1 - SONOFFDUAL=4 - elctrodragon=4
#define BOTTONE_tapparella1_GIU       2             // Pulsante tapparella 1 GIU nodemcu=5 - ESP01=3 - SONOFFDUAL=5 - elctrodragon=5
#define RELE_tapparella2_SU          14             // RELE tapparella 2 SU
#define RELE_tapparella2_GIU         15             // RELE tapparella 2 GIU
#define BOTTONE_tapparella2_SU        0             // Pulsante tapparella 2 SU
#define BOTTONE_tapparella2_GIU       2             // Pulsante tapparella 2 GIU
#define RELE_A_interruttore1         12             // RELE A interruttore 1
#define RELE_B_interruttore1         13             // RELE B interruttore 1
#define RELE_A_interruttore2         14             // RELE A interruttore 2
#define RELE_B_interruttore2         15             // RELE B interruttore 2
#define RELE_A_interruttore3         12             // RELE A interruttore 3
#define RELE_B_interruttore3         13             // RELE B interruttore 3
#define RELE_A_interruttore4         14             // RELE A interruttore 4
#define RELE_B_interruttore4         15             // RELE B interruttore 4
#define BOTTONE_A_interruttore1       0             // Pulsante A interruttore 1
#define BOTTONE_B_interruttore1       2             // Pulsante B interruttore 1
#define BOTTONE_A_interruttore2       0             // Pulsante A interruttore 2
#define BOTTONE_B_interruttore2       2             // Pulsante B interruttore 2
#define BOTTONE_A_interruttore3       0             // Pulsante A interruttore 3
#define BOTTONE_B_interruttore3       2             // Pulsante B interruttore 3
#define BOTTONE_A_interruttore4       0             // Pulsante A interruttore 4
#define BOTTONE_B_interruttore4       2             // Pulsante B interruttore 4
#define TIPO_BOTTONE_A_interruttore1 "interruttore" // Interruttore/pulsante A interruttore 1   ("interruttore" o "pulsante")
#define TIPO_BOTTONE_B_interruttore1 "interruttore" // Interruttore/pulsante B interruttore 1   ("interruttore" o "pulsante")
#define TIPO_BOTTONE_A_interruttore2 "interruttore" // Interruttore/pulsante A interruttore 2   ("interruttore" o "pulsante")
#define TIPO_BOTTONE_B_interruttore2 "interruttore" // Interruttore/pulsante B interruttore 2   ("interruttore" o "pulsante")
#define TIPO_BOTTONE_A_interruttore3 "interruttore" // Interruttore/pulsante A interruttore 3   ("interruttore" o "pulsante")
#define TIPO_BOTTONE_B_interruttore3 "interruttore" // Interruttore/pulsante B interruttore 3   ("interruttore" o "pulsante")
#define TIPO_BOTTONE_A_interruttore4 "interruttore" // Interruttore/pulsante A interruttore 4   ("interruttore" o "pulsante")
#define TIPO_BOTTONE_B_interruttore4 "interruttore" // Interruttore/pulsante B interruttore 4   ("interruttore" o "pulsante")
#define RELE_A_cancello1             12             // RELE A cancello 1
#define RELE_B_cancello1             13             // RELE B cancello 1
#define BOTTONE_A_cancello1           0             // Pulsante A cancello 1
#define BOTTONE_B_cancello1           2             // Pulsante B cancello 1
#define RELE_A_cancello2             14             // RELE A cancello 2
#define RELE_B_cancello2             15             // RELE B cancello 2
#define BOTTONE_A_cancello2           0             // Pulsante A cancello 2
#define BOTTONE_B_cancello2           2             // Pulsante B cancello 2
#define RELE_A_temperatura1          12             // RELE A temperatura 1 (termoststo)
#define RELE_B_temperatura1          13             // RELE B temperatura 1 (AUX)
#define DHTPIN_temperatura1          2              // pin DHT22 temperatura 1
#define I2C_DISPLAY_ADDRESS_temperatura1 0x3c       // indirizzo I2C Display temperatura 1
#define SDA_PIN_temperatura1         4              // pin SDA Display temperatura 1
#define SDC_PIN_temperatura1         5              // pin SDCDisplay temperatura 1
#define PullUp                                      // resistenza PullUp verso vcc   (commentare solo uno dei due (PullUp o PullDown))
//#define PullDown                                   // resistenza PullDown verso gnd (commentare solo uno dei due (PullUp o PullDown))

// TEMPI
#define MAX_RET_WIFI                15              // Indica per quante volte ritenta di connettersi al WIFI (20)
#define MAX_RET_MQTT                2               // Indica per quante volte ritenta di connettersi al server MQTT (3)
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


