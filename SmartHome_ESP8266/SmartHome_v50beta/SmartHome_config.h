/*

SmartHome v50

file creato con il tool per SmartHome v5.0

*/

// WIFI & MQTT server
const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORD";
const char* mqtt_server = "MQTT_SERVER";
const char* mqtt_user = "MQTT_USER";
const char* mqtt_password = "MQTT_PASSWORD";
const int   mqtt_port = MQTT_port;

// DEBUG
#define DEBUG
#define TELNET_DEBUG

// OTA
#define ArduinoIDE_OTA
#define Hostname_OTA "HostName_OTA"

// WEBSERVER
#define WEB_SERVER 

// Numero nodi da compilare
#define Tapparella_nodi 0
#define Interruttore_nodi 0
#define Impulso_nodi 0
#define Termostato_nodi 0
#define Temporizzatore_nodi 0

// GPIO
#define Status_LED_GPIO 16
#define Flag_inversione_Status_LED_GPIO 1
#define Flag_inversione_RELE 0
#define PullUp

#define ACK_Topic  "ack"                       // NON MODIFICARE

// nodi Tapparella

// Tapparella1
#define Tapparella1_Topic "Tapparella/1"
#define Tapparella1_Topic_ACK "Tapparella/1/ack"
#define Tapparella1_GPIO_rele_SU 12
#define Tapparella1_GPIO_rele_GIU 13
#define Tapparella1_GPIO_bottone_SU 4
#define Tapparella1_GPIO_bottone_GIU 5
// Tapparella2
#define Tapparella2_Topic "Tapparella/2"
#define Tapparella2_Topic_ACK "Tapparella/2/ack"
#define Tapparella2_GPIO_rele_SU 0
#define Tapparella2_GPIO_rele_GIU 0
#define Tapparella2_GPIO_bottone_SU 0
#define Tapparella2_GPIO_bottone_GIU 0
// Tapparella3
#define Tapparella3_Topic "Tapparella/3"
#define Tapparella3_Topic_ACK "Tapparella/3/ack"
#define Tapparella3_GPIO_rele_SU 0
#define Tapparella3_GPIO_rele_GIU 0
#define Tapparella3_GPIO_bottone_SU 0
#define Tapparella3_GPIO_bottone_GIU 0
// Tapparella4
#define Tapparella4_Topic "Tapparella/4"
#define Tapparella4_Topic_ACK "Tapparella/4/ack"
#define Tapparella4_GPIO_rele_SU 0
#define Tapparella4_GPIO_rele_GIU 0
#define Tapparella4_GPIO_bottone_SU 0
#define Tapparella4_GPIO_bottone_GIU 0
// Tapparella5
#define Tapparella5_Topic "Tapparella/5"
#define Tapparella5_Topic_ACK "Tapparella/5/ack"
#define Tapparella5_GPIO_rele_SU 0
#define Tapparella5_GPIO_rele_GIU 0
#define Tapparella5_GPIO_bottone_SU 0
#define Tapparella5_GPIO_bottone_GIU 0

// nodi Interruttore

// Interruttore1
#define Interruttore1_Topic "Interruttore/1"
#define Interruttore1_Topic_ACK "Interruttore/1/ack"
#define Interruttore1_GPIO_rele 12
#define Interruttore1_GPIO_bottone 4
#define Interruttore1_GPIO_stato 12
#define Interruttore1_tipo_bottone "monostabile"
// Interruttore2
#define Interruttore2_Topic "Interruttore/2"
#define Interruttore2_Topic_ACK "Interruttore/2/ack"
#define Interruttore2_GPIO_rele 0
#define Interruttore2_GPIO_bottone 0
#define Interruttore2_GPIO_stato 0
#define Interruttore2_tipo_bottone "monostabile"
// Interruttore3
#define Interruttore3_Topic "Interruttore/3"
#define Interruttore3_Topic_ACK "Interruttore/3/ack"
#define Interruttore3_GPIO_rele 0
#define Interruttore3_GPIO_bottone 0
#define Interruttore3_GPIO_stato 0
#define Interruttore3_tipo_bottone "monostabile"
// Interruttore4
#define Interruttore4_Topic "Interruttore/4"
#define Interruttore4_Topic_ACK "Interruttore/4/ack"
#define Interruttore4_GPIO_rele 0
#define Interruttore4_GPIO_bottone 0
#define Interruttore4_GPIO_stato 0
#define Interruttore4_tipo_bottone "monostabile"
// Interruttore5
#define Interruttore5_Topic "Interruttore/5"
#define Interruttore5_Topic_ACK "Interruttore/5/ack"
#define Interruttore5_GPIO_rele 0
#define Interruttore5_GPIO_bottone 0
#define Interruttore5_GPIO_stato 0
#define Interruttore5_tipo_bottone "monostabile"
// Interruttore6
#define Interruttore6_Topic "Interruttore/6"
#define Interruttore6_Topic_ACK "Interruttore/6/ack"
#define Interruttore6_GPIO_rele 0
#define Interruttore6_GPIO_bottone 0
#define Interruttore6_GPIO_stato 0
#define Interruttore6_tipo_bottone "monostabile"
// Interruttore7
#define Interruttore7_Topic "Interruttore/7"
#define Interruttore7_Topic_ACK "Interruttore/7/ack"
#define Interruttore7_GPIO_rele 0
#define Interruttore7_GPIO_bottone 0
#define Interruttore7_GPIO_stato 0
#define Interruttore7_tipo_bottone "monostabile"
// Interruttore8
#define Interruttore8_Topic "Interruttore/8"
#define Interruttore8_Topic_ACK "Interruttore/8/ack"
#define Interruttore8_GPIO_rele 0
#define Interruttore8_GPIO_bottone 0
#define Interruttore8_GPIO_stato 0
#define Interruttore8_tipo_bottone "monostabile"
// Interruttore9
#define Interruttore9_Topic "Interruttore/9"
#define Interruttore9_Topic_ACK "Interruttore/9/ack"
#define Interruttore9_GPIO_rele 0
#define Interruttore9_GPIO_bottone 0
#define Interruttore9_GPIO_stato 0
#define Interruttore9_tipo_bottone "monostabile"
// Interruttore10
#define Interruttore10_Topic "Interruttore/10"
#define Interruttore10_Topic_ACK "Interruttore/10/ack"
#define Interruttore10_GPIO_rele 0
#define Interruttore10_GPIO_bottone 0
#define Interruttore10_GPIO_stato 0
#define Interruttore10_tipo_bottone "monostabile"

// nodi Impulso

//#define Impulso1_NFC
/* wiring the MFRC522 To ESP8266 (ESP-12)
SDA(SS) = Impulso1_GPIO_SS
RST     = Impulso1_GPIO_RST
MOSI    = GPIO13
MISO    = GPIO12
SCK     = GPIO14
*/
#define Impulso1_GPIO_SS                  4             // SDA-PIN - RC522 - RFID - SPI - Modul GPIO4
#define Impulso1_GPIO_RST                 5             // RST-PIN - RC522 - RFID - SPI - Modul GPIO5
#define Impulso1_GPIO_MOSI               13             // (NON MODIFICARE)
#define Impulso1_GPIO_MISO               12             // (NON MODIFICARE)
#define Impulso1_GPIO_SCK                14             // (NON MODIFICARE)
// Impulso1
#define Impulso1_Topic "Impulso/1"
#define Impulso1_Topic_ACK "Impulso/1/ack"
#define Impulso1_GPIO_rele 12
#define Impulso1_GPIO_bottone 4
// Impulso2
#define Impulso2_Topic "Impulso/2"
#define Impulso2_Topic_ACK "Impulso/2/ack"
#define Impulso2_GPIO_rele 0
#define Impulso2_GPIO_bottone 0
// Impulso3
#define Impulso3_Topic "Impulso/3"
#define Impulso3_Topic_ACK "Impulso/3/ack"
#define Impulso3_GPIO_rele 0
#define Impulso3_GPIO_bottone 0
// Impulso4
#define Impulso4_Topic "Impulso/4"
#define Impulso4_Topic_ACK "Impulso/4/ack"
#define Impulso4_GPIO_rele 0
#define Impulso4_GPIO_bottone 0
// Impulso5
#define Impulso5_Topic "Impulso/5"
#define Impulso5_Topic_ACK "Impulso/5/ack"
#define Impulso5_GPIO_rele 0
#define Impulso5_GPIO_bottone 0
// Impulso6
#define Impulso6_Topic "Impulso/6"
#define Impulso6_Topic_ACK "Impulso/6/ack"
#define Impulso6_GPIO_rele 0
#define Impulso6_GPIO_bottone 0
// Impulso7
#define Impulso7_Topic "Impulso/7"
#define Impulso7_Topic_ACK "Impulso/7/ack"
#define Impulso7_GPIO_rele 0
#define Impulso7_GPIO_bottone 0
// Impulso8
#define Impulso8_Topic "Impulso/8"
#define Impulso8_Topic_ACK "Impulso/8/ack"
#define Impulso8_GPIO_rele 0
#define Impulso8_GPIO_bottone 0
// Impulso9
#define Impulso9_Topic "Impulso/9"
#define Impulso9_Topic_ACK "Impulso/9/ack"
#define Impulso9_GPIO_rele 0
#define Impulso9_GPIO_bottone 0
// Impulso10
#define Impulso10_Topic "Impulso/10"
#define Impulso10_Topic_ACK "Impulso/10/ack"
#define Impulso10_GPIO_rele 0
#define Impulso10_GPIO_bottone 0

// nodi Temporizzatore

// Temporizzatore1
#define Temporizzatore1_Topic "Temporizzatore/1"
#define Temporizzatore1_Topic_ACK "Temporizzatore/1/ack"
#define Temporizzatore1_GPIO_rele 12
#define Temporizzatore1_GPIO_bottone 4
// Temporizzatore2
#define Temporizzatore2_Topic "Temporizzatore/2"
#define Temporizzatore2_Topic_ACK "Temporizzatore/2/ack"
#define Temporizzatore2_GPIO_rele 0
#define Temporizzatore2_GPIO_bottone 0
// Temporizzatore3
#define Temporizzatore3_Topic "Temporizzatore/3"
#define Temporizzatore3_Topic_ACK "Temporizzatore/3/ack"
#define Temporizzatore3_GPIO_rele 0
#define Temporizzatore3_GPIO_bottone 0
// Temporizzatore4
#define Temporizzatore4_Topic "Temporizzatore/4"
#define Temporizzatore4_Topic_ACK "Temporizzatore/4/ack"
#define Temporizzatore4_GPIO_rele 0
#define Temporizzatore4_GPIO_bottone 0
// Temporizzatore5
#define Temporizzatore5_Topic "Temporizzatore/5"
#define Temporizzatore5_Topic_ACK "Temporizzatore/5/ack"
#define Temporizzatore5_GPIO_rele 0
#define Temporizzatore5_GPIO_bottone 0
// Temporizzatore6
#define Temporizzatore6_Topic "Temporizzatore/6"
#define Temporizzatore6_Topic_ACK "Temporizzatore/6/ack"
#define Temporizzatore6_GPIO_rele 0
#define Temporizzatore6_GPIO_bottone 0
// Temporizzatore7
#define Temporizzatore7_Topic "Temporizzatore/7"
#define Temporizzatore7_Topic_ACK "Temporizzatore/7/ack"
#define Temporizzatore7_GPIO_rele 0
#define Temporizzatore7_GPIO_bottone 0
// Temporizzatore8
#define Temporizzatore8_Topic "Temporizzatore/8"
#define Temporizzatore8_Topic_ACK "Temporizzatore/8/ack"
#define Temporizzatore8_GPIO_rele 0
#define Temporizzatore8_GPIO_bottone 0
// Temporizzatore9
#define Temporizzatore9_Topic "Temporizzatore/9"
#define Temporizzatore9_Topic_ACK "Temporizzatore/9/ack"
#define Temporizzatore9_GPIO_rele 0
#define Temporizzatore9_GPIO_bottone 0
// Temporizzatore10
#define Temporizzatore10_Topic "Temporizzatore/10"
#define Temporizzatore10_Topic_ACK "Temporizzatore/10/ack"
#define Temporizzatore10_GPIO_rele 0
#define Temporizzatore10_GPIO_bottone 0

// nodi Termostato

#define DHTTYPE DHT22
float   soglia_Termostato1 = 0.25;
// Termostato1
#define Termostato1_Topic "Termostato/1"
#define Termostato1_Topic_ACK "Termostato/1/ack"
#define Termostato1_GPIO_rele 12
#define Termostato1_GPIO_DHT 2
#define Termostato1_I2C_DISPLAY_ADDRESS 0x3c
#define Termostato1_GPIO_SDA 4
#define Termostato1_GPIO_SDC 5

// RETE
#define MAX_RET_WIFI                      20
#define MAX_RET_MQTT                       3

// TEMPI
#define TEMPO_REFRESH_CONNESSIONE      60000
#define Tapparella_TEMPO_RELE            100
#define Tapparella_TEMPO_CLICK_ON        150
#define Tapparella_TEMPO_CLICK_OFF       100
#define Interruttore_TEMPO_CLICK_ON        0
#define Interruttore_TEMPO_CLICK_OFF       0
#define Impulso_TEMPO_CLICK_ON            50
#define Impulso_TEMPO_IMPULSO            500
#define Temporizzatore_TEMPO_CLICK_ON     50
#define Termostato_TEMPO_TERMOSTATO     5000
