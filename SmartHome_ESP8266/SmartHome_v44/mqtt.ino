#include <PubSubClient.h>
#define MQTT_VERSION MQTT_VERSION_3_1_1
WiFiClient espClient;
PubSubClient client(espClient);

void setup_MQTT() {
  WiFi.macAddress(mac);
  if (macToStr(mac) == "000000000000") {
    Debug_MSG("Attempting MQTT connection... ");
    Debug_MSG_LN("error: mac=null");
    return;
  }

  int i = 0;
  String clientName = "ESP8266Client-";
  clientName += macToStr(mac);
  clientName += "-";
  clientName += millis();
  while (!client.connected() and i < MAX_RET_MQTT) {
    ++i;
    Debug_MSG("MQTT Client : ");
    Debug_MSG_LN(clientName);
    Debug_MSG("Attempting MQTT connection... ");
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
    if (client.connect(string2char(clientName), mqtt_user, mqtt_password)) {
      Debug_MSG_LN("MQTT connected!");
      Send_Alive();
      Send_ACK();
      SubscribeTopic();
    }
    else {
      Debug_MSG("MQTT failed, rc = ");
      switch (client.state()) {
        case -4:
          Debug_MSG_LN("MQTT_CONNECTION_TIMEOUT");
          break;
        case -3:
          Debug_MSG_LN("MQTT_CONNECTION_LOST");
          break;
        case -2:
          Debug_MSG_LN("MQTT_CONNECT_FAILED");
          break;
        case -1:
          Debug_MSG_LN("MQTT_DISCONNECTED");
          break;
        case 0:
          Debug_MSG_LN("MQTT_CONNECTED");
          break;
        case 1:
          Debug_MSG_LN("MQTT_CONNECT_BAD_PROTOCOL");
          break;
        case 2:
          Debug_MSG_LN("MQTT_CONNECT_BAD_CLIENT_ID");
          break;
        case 3:
          Debug_MSG_LN("MQTT_CONNECT_UNAVAILABLE");
          break;
        case 4:
          Debug_MSG_LN("MQTT_CONNECT_BAD_CREDENTIALS");
          break;
        case 5:
          Debug_MSG_LN("MQTT_CONNECT_UNAUTHORIZED");
          break;
        default:
          Debug_MSG_LN("MQTT_UNKNOWN_ERROR");
          break;
      }
    }
  }
}

void loop_MQTT() {
  client.loop();
}

void Send_Alive() {
  String payload = macToStr(mac);

  payload += " ";
  payload += WiFi.localIP().toString();

  payload += " v";
  payload += Versione;
  payload += " start at ";
  payload += getTime();
  client.publish(ACK_Topic, (char*) payload.c_str());     // Pubblica su ACK_Topic -> MAC + " start at " + time
  delay(100);
  Debug_MSG_LN(payload);
}
void Send_ACK() {
  String payload;
  if (NODI_TAPPARELLA > 0) {
    payload = macToStr(mac);
    payload += " alive! TAP ";
    payload += Tapparella1_Topic;
    client.publish(ACK_Topic, (char*) payload.c_str());
    delay(250);
  }
  if (NODI_TAPPARELLA > 1) {
    payload = macToStr(mac);
    payload += " alive! TAP ";
    payload += Tapparella2_Topic;
    client.publish(ACK_Topic, (char*) payload.c_str());
    delay(250);
  }
  if (NODI_INTERRUTTORE > 0) {
    payload = macToStr(mac);
    payload += " alive! INT ";
    payload += Interruttore1_Topic;
    client.publish(ACK_Topic, (char*) payload.c_str());
    delay(250);
  }
  if (NODI_INTERRUTTORE > 1) {
    payload = macToStr(mac);
    payload += " alive! INT ";
    payload += Interruttore2_Topic;
    client.publish(ACK_Topic, (char*) payload.c_str());
    delay(250);
  }
  if (NODI_INTERRUTTORE > 2) {
    payload = macToStr(mac);
    payload += " alive! INT ";
    payload += Interruttore3_Topic;
    client.publish(ACK_Topic, (char*) payload.c_str());
    delay(250);
  }
  if (NODI_INTERRUTTORE > 3) {
    payload = macToStr(mac);
    payload += " alive! INT ";
    payload += Interruttore4_Topic;
    client.publish(ACK_Topic, (char*) payload.c_str());
    delay(250);
  }
  if (NODI_CANCELLO > 0) {
    payload = macToStr(mac);
    payload += " alive! CAN ";
    payload += Cancello1_Topic;
    client.publish(ACK_Topic, (char*) payload.c_str());
    delay(250);
  }
  if (NODI_CANCELLO > 1) {
    payload = macToStr(mac);
    payload += " alive! CAN ";
    payload += Cancello2_Topic;
    client.publish(ACK_Topic, (char*) payload.c_str());
    delay(250);
  }
  if (NODI_TEMPERATURA > 0) {
    payload = macToStr(mac);
    payload += " alive! TEM ";
    payload += Temperatura1_Topic;
    client.publish(ACK_Topic, (char*) payload.c_str());
    delay(250);
  }
}

void SubscribeTopic() {
  client.subscribe(ACK_Topic);                            // Sottoscrivi ACK_Topic
  delay(250);
  Debug_MSG_LN("MQTT subscribe " + String(ACK_Topic));
  if (NODI_TAPPARELLA > 0) {
    client.subscribe(Tapparella1_Topic);                    // Sottoscrivi Tapparella1_Topic
    delay(250);
    Debug_MSG_LN("MQTT subscribe " + String(Tapparella1_Topic));
  }
  if (NODI_TAPPARELLA > 1) {
    client.subscribe(Tapparella2_Topic);                    // Sottoscrivi Tapparella2_Topic
    delay(250);
    Debug_MSG_LN("MQTT subscribe " + String(Tapparella2_Topic));
  }
  if (NODI_INTERRUTTORE > 0) {
    client.subscribe(Interruttore1_Topic);                  // Sottoscrivi Interruttore1_Topic
    delay(250);
    Debug_MSG_LN("MQTT subscribe " + String(Interruttore1_Topic));
  }
  if (NODI_INTERRUTTORE > 1) {
    client.subscribe(Interruttore2_Topic);                  // Sottoscrivi Interruttore2_Topic
    delay(250);
    Debug_MSG_LN("MQTT subscribe " + String(Interruttore2_Topic));
  }
  if (NODI_INTERRUTTORE > 2) {
    client.subscribe(Interruttore3_Topic);                  // Sottoscrivi Interruttore3_Topic
    delay(250);
    Debug_MSG_LN("MQTT subscribe " + String(Interruttore3_Topic));
  }
  if (NODI_INTERRUTTORE > 3) {
    client.subscribe(Interruttore4_Topic);                  // Sottoscrivi Interruttore4_Topic
    delay(250);
    Debug_MSG_LN("MQTT subscribe " + String(Interruttore4_Topic));
  }
  if (NODI_CANCELLO > 0) {
    client.subscribe(Cancello1_Topic);                  // Sottoscrivi Cancello1_Topic
    delay(250);
    Debug_MSG_LN("MQTT subscribe " + String(Cancello1_Topic));
  }
  if (NODI_CANCELLO > 1) {
    client.subscribe(Cancello2_Topic);                  // Sottoscrivi Cancello2_Topic
    delay(250);
    Debug_MSG_LN("MQTT subscribe " + String(Cancello2_Topic));
  }
  if (NODI_TEMPERATURA > 0) {
    client.subscribe(Temperatura1_Topic);                  // Sottoscrivi Temperatura1_Topic
    delay(250);
    Debug_MSG_LN("MQTT subscribe " + String(Temperatura1_Topic));
  }
}

void sendTemperature() {
  h_temperatura1 = dht.readHumidity();
  delay(50);
  t_temperatura1 = dht.readTemperature(false);                           // Read temperature as Celsius (the default)
  delay(50);
  f_temperatura1 = dht.readTemperature(true);                            // Read temperature as Fahrenheit (isFahrenheit = true)
  if (isnan(h_temperatura1) || isnan(t_temperatura1) || isnan(f_temperatura1)) {                   // Check if any reads failed and exit early (to try again).
    String payload = Temperatura1_Topic;
    payload += " Failed to read from DHT sensor!";
    Debug_MSG_LN("Failed to read from DHT sensor!");
    client.publish(ACK_Topic, (char*) payload.c_str());
    return;
  }
  hi_temperatura1 = dht.computeHeatIndex(t_temperatura1, h_temperatura1, false);                   // Compute heat index in Celsius (isFahreheit = false) float hi
  Debug_MSG("Humidity: ");
  Debug_MSG((String) h_temperatura1);
  Debug_MSG(" % ");
  Debug_MSG("Temperature: ");
  Debug_MSG((String) t_temperatura1);
  Debug_MSG(" *C ");
  Debug_MSG((String) f_temperatura1);
  Debug_MSG(" *F\t");
  Debug_MSG("Heat index: ");
  Debug_MSG((String) hi_temperatura1);
  Debug_MSG_LN(" *C");
  String payload = Temperatura1_Topic;
  payload += " T:";
  payload += t_temperatura1;
  payload += "°C H:";
  payload += h_temperatura1;
  payload += "% HI:";
  payload += hi_temperatura1;
  payload += "°C";
  client.publish(ACK_Topic, (char*) payload.c_str());
}

void callback(char* topic, byte * message, unsigned int length) {
  Debug_MSG("Message arrived [");
  Debug_MSG((String) topic);
  Debug_MSG("] ");
  for (int i = 0; i < length; i++) {
    Debug_MSG((String) (char) message[i]);
  }
  Debug_MSG_LN("");
  String payload;

  // TAPPARELLA 1
  if (String(topic) == Tapparella1_Topic) {                                               // se arriva il comando sul topic "Tapparella1_Topic"
    if ((char)message[0] == 's' & (char)message[1] == 'u' ) {                             // Topic "Tapparella1_Topic" = "su"
      digitalWrite(RELE_tapparella1_GIU, 0 ^ Flag_inversione_RELE);                       // RELE GIU = 0
      delay(TEMPO_RELE);                                                                  // Aspetto
      digitalWrite(RELE_tapparella1_SU, 1 ^ Flag_inversione_RELE);                        // RELE SU = 1
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_ON_B_OFF();
#endif
      In_movimento_tapparella1 = true;
      ulTimeTapparella1 = millis();
    }
    if ((char)message[0] == 'g' & (char)message[1] == 'i' & (char)message[2] == 'u') {    // Topic "Tapparella1_Topic" = "giu"
      digitalWrite(RELE_tapparella1_SU, 0 ^ Flag_inversione_RELE);                        // RELE SU = 0
      delay(TEMPO_RELE);                                                                  // Aspetto
      digitalWrite(RELE_tapparella1_GIU, 1 ^ Flag_inversione_RELE);                       // RELE GIU = 1
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_OFF_B_ON();
#endif
      In_movimento_tapparella1 = true;
      ulTimeTapparella1 = millis();
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'o' &
        (char)message[3] == 'p' ) {                                                       // Topic "Tapparella1_Topic" = "stop"
      digitalWrite(RELE_tapparella1_SU, 0 ^ Flag_inversione_RELE);                        // RELE SU = 0
      delay(TEMPO_RELE);                                                                  // Aspetto
      digitalWrite(RELE_tapparella1_GIU, 0 ^ Flag_inversione_RELE);                       // RELE GIU = 0
#if defined(SONOFFDUAL)
      SONOFFDUAL_AB_OFF();
#endif
      In_movimento_tapparella1 = false;
    }
    if ((char)message[0] == '+') {                                                        // Topic "Tapparella1_Topic" = "+"
      digitalWrite(RELE_tapparella1_SU, 1 ^ Flag_inversione_RELE);                        // RELE SU = 1
      delay(TEMPO_RELE);                                                                  // Aspetto
      digitalWrite(RELE_tapparella1_GIU, 0 ^ Flag_inversione_RELE);                       // RELE GIU = 0
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_ON_B_OFF();
#endif
      String stringtmp = "";
      for (int i = 1; i < length; i++)  {
        stringtmp = stringtmp + (char)message[i];
      }
      char buf[stringtmp.length()];
      stringtmp.toCharArray(buf, stringtmp.length() + 1);
      percentualeTapparella = atol(buf);
      if (percentualeTapparella > 100) percentualeTapparella = 100;
      percentualeTapparella = TEMPO_tapparella1_MAX * percentualeTapparella / 100;
      In_movimento_tapparella1 = true;
      ulTimeTapparella1 = millis();
      ulTimeTapparella1 -= TEMPO_tapparella1_MAX;
      ulTimeTapparella1 += percentualeTapparella;
    }
    if ((char)message[0] == '-') {                                                        // Topic "Tapparella1_Topic" = "-"
      digitalWrite(RELE_tapparella1_SU, 0 ^ Flag_inversione_RELE);                        // RELE SU = 1
      delay(TEMPO_RELE);                                                                  // Aspetto
      digitalWrite(RELE_tapparella1_GIU, 1 ^ Flag_inversione_RELE);                       // RELE GIU = 0
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_OFF_B_ON();
#endif
      String stringtmp = "";
      for (int i = 1; i < length; i++)  {
        stringtmp = stringtmp + (char)message[i];
      }
      char buf[stringtmp.length()];
      stringtmp.toCharArray(buf, stringtmp.length() + 1);
      percentualeTapparella = atol(buf);
      if (percentualeTapparella > 100) percentualeTapparella = 100;
      percentualeTapparella = TEMPO_tapparella1_MAX * percentualeTapparella / 100;
      In_movimento_tapparella1 = true;
      ulTimeTapparella1 = millis();
      ulTimeTapparella1 -= TEMPO_tapparella1_MAX;
      ulTimeTapparella1 += percentualeTapparella;
    }
    if (((char)message[0] == 'T' or (char)message[0] == 't') &
        (char)message[1] == '=' ) {                                                        // Topic "Tapparella1_Topic" = "T=" o "t="
      String stringtmp = "";
      for (int i = 2; i < length; i++)  {
        stringtmp = stringtmp + (char)message[i];
      }
      char buf[stringtmp.length()];
      stringtmp.toCharArray(buf, stringtmp.length() + 1);
      TEMPO_tapparella1_MAX = atol(buf);
      if (TEMPO_tapparella1_MAX < 1) {
        TEMPO_tapparella1_MAX = 10;
      }
      if (TEMPO_tapparella1_MAX > 120) {
        TEMPO_tapparella1_MAX = 120;
      }
      TEMPO_tapparella1_MAX = TEMPO_tapparella1_MAX * 1000;
      EEPROM_write_TEMPO_tapparelle_MAX();
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] != '2' ) {   // Topic "Tapparella1_Topic" = "stato"
      payload = Tapparella1_Topic;
      payload += " RSSI=";
      payload += WiFi.RSSI();
      payload += "dB ";
      payload += " T=";
      payload += TEMPO_tapparella1_MAX / 1000;
      payload += "sec. SU=";
      if (digitalRead(RELE_tapparella1_SU) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE_tapparella1_SU) == false ^ Flag_inversione_RELE) payload += "OFF";
      payload += " GIU=";
      if (digitalRead(RELE_tapparella1_GIU) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE_tapparella1_GIU) == false ^ Flag_inversione_RELE) payload += "OFF";
      client.publish(ACK_Topic, (char*) payload.c_str());
      delay(100);
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] == '2' ) {   // Topic "Tapparella1_Topic" = "stato2"
      payload = Tapparella1_Topic;
      payload += " SU=";
      if (digitalRead(RELE_tapparella1_SU) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE_tapparella1_SU) == false ^ Flag_inversione_RELE) payload += "OFF";
      payload += " GIU=";
      if (digitalRead(RELE_tapparella1_GIU) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE_tapparella1_GIU) == false ^ Flag_inversione_RELE) payload += "OFF";
      client.publish(ACK2_Topic, (char*) payload.c_str());
      delay(100);
    }
  }

  // TAPPARELLA 2
  if (String(topic) == Tapparella2_Topic) {                                               // se arriva il comando sul topic "Tapparella2_Topic"
    if ((char)message[0] == 's' & (char)message[1] == 'u' ) {                             // Topic "Tapparella2_Topic" = "su"
      digitalWrite(RELE_tapparella2_GIU, 0 ^ Flag_inversione_RELE);                       // RELE GIU = 0
      delay(TEMPO_RELE);                                                                  // Aspetto
      digitalWrite(RELE_tapparella2_SU, 1 ^ Flag_inversione_RELE);                        // RELE SU = 1
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_ON_B_OFF();
#endif
      In_movimento_tapparella2 = true;
      ulTimeTapparella2 = millis();
    }
    if ((char)message[0] == 'g' & (char)message[1] == 'i' & (char)message[2] == 'u') {    // Topic "Tapparella2_Topic" = "giu"
      digitalWrite(RELE_tapparella2_SU, 0 ^ Flag_inversione_RELE);                        // RELE SU = 0
      delay(TEMPO_RELE);                                                                  // Aspetto
      digitalWrite(RELE_tapparella2_GIU, 1 ^ Flag_inversione_RELE);                       // RELE GIU = 1
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_OFF_B_ON();
#endif
      In_movimento_tapparella2 = true;
      ulTimeTapparella2 = millis();
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'o' &
        (char)message[3] == 'p' ) {                                                       // Topic "Tapparella2_Topic" = "stop"
      digitalWrite(RELE_tapparella2_SU, 0 ^ Flag_inversione_RELE);                        // RELE SU = 0
      delay(TEMPO_RELE);                                                                  // Aspetto
      digitalWrite(RELE_tapparella2_GIU, 0 ^ Flag_inversione_RELE);                       // RELE GIU = 0
#if defined(SONOFFDUAL)
      SONOFFDUAL_AB_OFF();
#endif
      In_movimento_tapparella2 = false;
    }

    if ((char)message[0] == '+') {                                                        // Topic "Tapparella2_Topic" = "+"
      digitalWrite(RELE_tapparella2_SU, 1 ^ Flag_inversione_RELE);                        // RELE SU = 1
      delay(TEMPO_RELE);                                                                  // Aspetto
      digitalWrite(RELE_tapparella2_GIU, 0 ^ Flag_inversione_RELE);                       // RELE GIU = 0
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_ON_B_OFF();
#endif
      String stringtmp = "";
      for (int i = 1; i < length; i++)  {
        stringtmp = stringtmp + (char)message[i];
      }
      char buf[stringtmp.length()];
      stringtmp.toCharArray(buf, stringtmp.length() + 1);
      percentualeTapparella = atol(buf);
      if (percentualeTapparella > 100) percentualeTapparella = 100;
      percentualeTapparella = TEMPO_tapparella2_MAX * percentualeTapparella / 100;
      In_movimento_tapparella1 = true;
      ulTimeTapparella2 = millis();
      ulTimeTapparella2 -= TEMPO_tapparella1_MAX;
      ulTimeTapparella2 += percentualeTapparella;
    }
    if ((char)message[0] == '-') {                                                        // Topic "Tapparella2_Topic" = "-"
      digitalWrite(RELE_tapparella2_SU, 0 ^ Flag_inversione_RELE);                        // RELE SU = 1
      delay(TEMPO_RELE);                                                                  // Aspetto
      digitalWrite(RELE_tapparella2_GIU, 1 ^ Flag_inversione_RELE);                       // RELE GIU = 0
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_OFF_B_ON();
#endif
      String stringtmp = "";
      for (int i = 1; i < length; i++)  {
        stringtmp = stringtmp + (char)message[i];
      }
      char buf[stringtmp.length()];
      stringtmp.toCharArray(buf, stringtmp.length() + 1);
      percentualeTapparella = atol(buf);
      if (percentualeTapparella > 100) percentualeTapparella = 100;
      percentualeTapparella = TEMPO_tapparella2_MAX * percentualeTapparella / 100;
      In_movimento_tapparella1 = true;
      ulTimeTapparella2 = millis();
      ulTimeTapparella2 -= TEMPO_tapparella1_MAX;
      ulTimeTapparella2 += percentualeTapparella;
    }
    if (((char)message[0] == 'T' or (char)message[0] == 't') &
        (char)message[1] == '=' ) {                                                       // Topic "Tapparella2_Topic" = "T=" o "t="
      String stringtmp = "";
      for (int i = 2; i < length; i++)  {
        stringtmp = stringtmp + (char)message[i];
      }
      char buf[stringtmp.length()];
      stringtmp.toCharArray(buf, stringtmp.length() + 1);
      TEMPO_tapparella2_MAX = atol(buf);
      if (TEMPO_tapparella2_MAX < 1) {
        TEMPO_tapparella2_MAX = 10;
      }
      if (TEMPO_tapparella2_MAX > 120) {
        TEMPO_tapparella2_MAX = 120;
      }
      TEMPO_tapparella2_MAX = TEMPO_tapparella2_MAX * 1000;
      EEPROM_write_TEMPO_tapparelle_MAX();
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] != '2' ) {   // Topic "Tapparella2_Topic" = "stato"
      payload = Tapparella2_Topic;
      payload += " RSSI=";
      payload += WiFi.RSSI();
      payload += "dB ";
      payload += " T=";
      payload += TEMPO_tapparella2_MAX / 1000;
      payload += "sec. SU=";
      if (digitalRead(RELE_tapparella2_SU) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE_tapparella2_SU) == false ^ Flag_inversione_RELE) payload += "OFF";
      payload += " GIU=";
      if (digitalRead(RELE_tapparella2_GIU) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE_tapparella2_GIU) == false ^ Flag_inversione_RELE) payload += "OFF";
      client.publish(ACK_Topic, (char*) payload.c_str());
      delay(100);
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] == '2' ) {     // Topic "Tapparella2_Topic" = "stato2"
      payload = Tapparella2_Topic;
      payload += " SU=";
      if (digitalRead(RELE_tapparella2_SU) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE_tapparella2_SU) == false ^ Flag_inversione_RELE) payload += "OFF";
      payload += " GIU=";
      if (digitalRead(RELE_tapparella2_GIU) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE_tapparella2_GIU) == false ^ Flag_inversione_RELE) payload += "OFF";
      client.publish(ACK2_Topic, (char*) payload.c_str());
      delay(100);
    }
  }

  // INTERRUTTORE 1
  if (String(topic) == Interruttore1_Topic) {                                              // se arriva il comando sul topic "Interruttore1_Topic"
    if ((char)message[0] == '1' & (char)message[1] == 'o' & (char)message[2] == 'n') {    // Topic "Interruttore1_Topic" = "1on"
      digitalWrite(RELE_A_interruttore1, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 1
      delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_ON();
#endif
#if defined(ESP01_SERIAL_RELE)
      ESP01_SERIAL_RELE_AON();
#endif
      EEPROM_write_releAinterruttore1();
    }
    if ((char)message[0] == '1' & (char)message[1] == 'o' & (char)message[2] == 'f' &
        (char)message[3] == 'f') {                                                        // Topic "Interruttore1_Topic" = "1off"
      digitalWrite(RELE_A_interruttore1, 0 ^ Flag_inversione_RELE);                                     // Spengo il RELE 1
      delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_OFF();
#endif
#if defined(ESP01_SERIAL_RELE)
      ESP01_SERIAL_RELE_AOFF();
#endif
      EEPROM_write_releAinterruttore1();
    }
    if ((char)message[0] == '2' & (char)message[1] == 'o' & (char)message[2] == 'n') {    // Topic "Interruttore1_Topic" = "2on"
      digitalWrite(RELE_B_interruttore1, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 2
      delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
      SONOFFDUAL_B_ON();
#endif
#if defined(ESP01_SERIAL_RELE)
      ESP01_SERIAL_RELE_BON();
#endif
      EEPROM_write_releBinterruttore1();
    }
    if ((char)message[0] == '2' & (char)message[1] == 'o' & (char)message[2] == 'f' &
        (char)message[3] == 'f') {                                                        // Topic "Interruttore1_Topic" = "2off"
      digitalWrite(RELE_B_interruttore1, 0 ^ Flag_inversione_RELE);                                     // Spengo il RELE 2
      delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
      SONOFFDUAL_B_OFF();
#endif
#if defined(ESP01_SERIAL_RELE)
      ESP01_SERIAL_RELE_AOFF();
#endif
      EEPROM_write_releBinterruttore1();
    }

    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] != '2' ) {                             // Topic "Interruttore1_Topic" = "stato"
      payload = Interruttore1_Topic;
      payload += " RSSI=";
      payload += WiFi.RSSI();
      payload += "dB ";
      payload += " RELE1=";
      if (digitalRead(RELE_A_interruttore1) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_A_interruttore1) ^ Flag_inversione_RELE == false ) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE_B_interruttore1) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_B_interruttore1) ^ Flag_inversione_RELE == false) payload += "OFF";
      client.publish(ACK_Topic, (char*) payload.c_str());
      delay(100);
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] == '2' ) {                             // Topic "Interruttore1_Topic" = "stato2"
      payload = Interruttore1_Topic;
      payload += " RELE1=";
      if (digitalRead(RELE_A_interruttore1) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_A_interruttore1) ^ Flag_inversione_RELE == false ) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE_B_interruttore1) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_B_interruttore1) ^ Flag_inversione_RELE == false) payload += "OFF";
      client.publish(ACK2_Topic, (char*) payload.c_str());
      delay(100);
    }
  }

  // INTERRUTTORE 2
  if (String(topic) == Interruttore2_Topic) {                                              // se arriva il comando sul topic "interruttore2_Topic"
    if ((char)message[0] == '1' & (char)message[1] == 'o' & (char)message[2] == 'n') {    // Topic "interruttore2_Topic" = "1on"
      digitalWrite(RELE_A_interruttore2, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 1
      delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_ON();
#endif
      EEPROM_write_releAinterruttore2();
    }
    if ((char)message[0] == '1' & (char)message[1] == 'o' & (char)message[2] == 'f' &
        (char)message[3] == 'f') {                                                        // Topic "interruttore2_Topic" = "1off"
      digitalWrite(RELE_A_interruttore2, 0 ^ Flag_inversione_RELE);                                     // Spengo il RELE 1
      delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_OFF();
#endif
      EEPROM_write_releAinterruttore2();
    }
    if ((char)message[0] == '2' & (char)message[1] == 'o' & (char)message[2] == 'n') {    // Topic "interruttore2_Topic" = "2on"
      digitalWrite(RELE_B_interruttore2, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 2
      delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
      SONOFFDUAL_B_ON();
#endif
      EEPROM_write_releBinterruttore2();
    }
    if ((char)message[0] == '2' & (char)message[1] == 'o' & (char)message[2] == 'f' &
        (char)message[3] == 'f') {                                                        // Topic "interruttore2_Topic" = "2off"
      digitalWrite(RELE_B_interruttore2, 0 ^ Flag_inversione_RELE);                                     // Spengo il RELE 2
      delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
      SONOFFDUAL_B_OFF();
#endif
      EEPROM_write_releBinterruttore2();
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] != '2' ) {                             // Topic "interruttore2_Topic" = "stato"
      payload = Interruttore2_Topic;
      payload += " RSSI=";
      payload += WiFi.RSSI();
      payload += "dB ";
      payload += " RELE1=";
      if (digitalRead(RELE_A_interruttore2) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_A_interruttore2) ^ Flag_inversione_RELE == false ) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE_B_interruttore2) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_B_interruttore2) ^ Flag_inversione_RELE == false) payload += "OFF";
      client.publish(ACK_Topic, (char*) payload.c_str());
      delay(100);
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] == '2' ) {                             // Topic "interruttore2_Topic" = "stato2"
      payload = Interruttore2_Topic;
      payload += " RELE1=";
      if (digitalRead(RELE_A_interruttore2) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_A_interruttore2) ^ Flag_inversione_RELE == false ) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE_B_interruttore2) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_B_interruttore2) ^ Flag_inversione_RELE == false) payload += "OFF";
      client.publish(ACK2_Topic, (char*) payload.c_str());
      delay(100);
    }
  }

  // INTERRUTTORE 3
  if (String(topic) == Interruttore3_Topic) {                                              // se arriva il comando sul topic "Interruttore3_Topic"
    if ((char)message[0] == '1' & (char)message[1] == 'o' & (char)message[2] == 'n') {    // Topic "Interruttore3_Topic" = "1on"
      digitalWrite(RELE_A_interruttore3, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 1
      delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_ON();
#endif
      EEPROM_write_releAinterruttore3();
    }
    if ((char)message[0] == '1' & (char)message[1] == 'o' & (char)message[2] == 'f' &
        (char)message[3] == 'f') {                                                        // Topic "Interruttore3_Topic" = "1off"
      digitalWrite(RELE_A_interruttore3, 0 ^ Flag_inversione_RELE);                                     // Spengo il RELE 1
      delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_OFF();
#endif
      EEPROM_write_releAinterruttore3();
    }
    if ((char)message[0] == '2' & (char)message[1] == 'o' & (char)message[2] == 'n') {    // Topic "Interruttore3_Topic" = "2on"
      digitalWrite(RELE_B_interruttore3, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 2
      delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
      SONOFFDUAL_B_ON();
#endif
      EEPROM_write_releBinterruttore3();
    }
    if ((char)message[0] == '2' & (char)message[1] == 'o' & (char)message[2] == 'f' &
        (char)message[3] == 'f') {                                                        // Topic "Interruttore3_Topic" = "2off"
      digitalWrite(RELE_B_interruttore3, 0 ^ Flag_inversione_RELE);                                     // Spengo il RELE 2
      delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
      SONOFFDUAL_B_OFF();
#endif
      EEPROM_write_releBinterruttore3();
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] != '2' ) {                             // Topic "Interruttore3_Topic" = "stato"
      payload = Interruttore3_Topic;
      payload += " RSSI=";
      payload += WiFi.RSSI();
      payload += "dB ";
      payload += " RELE1=";
      if (digitalRead(RELE_A_interruttore3) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_A_interruttore3) ^ Flag_inversione_RELE == false ) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE_B_interruttore3) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_B_interruttore3) ^ Flag_inversione_RELE == false) payload += "OFF";
      client.publish(ACK_Topic, (char*) payload.c_str());
      delay(100);
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] == '2' ) {                             // Topic "Interruttore3_Topic" = "stato2"
      payload = Interruttore3_Topic;
      payload += " RELE1=";
      if (digitalRead(RELE_A_interruttore3) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_A_interruttore3) ^ Flag_inversione_RELE == false ) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE_B_interruttore3) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_B_interruttore3) ^ Flag_inversione_RELE == false) payload += "OFF";
      client.publish(ACK2_Topic, (char*) payload.c_str());
      delay(100);
    }
  }

  // INTERRUTTORE 4
  if (String(topic) == Interruttore4_Topic) {                                              // se arriva il comando sul topic "interruttore4_Topic"
    if ((char)message[0] == '1' & (char)message[1] == 'o' & (char)message[2] == 'n') {    // Topic "interruttore4_Topic" = "1on"
      digitalWrite(RELE_A_interruttore4, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 1
      delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_ON();
#endif
      EEPROM_write_releAinterruttore4();
    }
    if ((char)message[0] == '1' & (char)message[1] == 'o' & (char)message[2] == 'f' &
        (char)message[3] == 'f') {                                                        // Topic "interruttore4_Topic" = "1off"
      digitalWrite(RELE_A_interruttore4, 0 ^ Flag_inversione_RELE);                                     // Spengo il RELE 1
      delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_OFF();
#endif
      EEPROM_write_releAinterruttore4();
    }
    if ((char)message[0] == '2' & (char)message[1] == 'o' & (char)message[2] == 'n') {    // Topic "interruttore4_Topic" = "2on"
      digitalWrite(RELE_B_interruttore4, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 2
      delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
      SONOFFDUAL_B_ON();
#endif
      EEPROM_write_releBinterruttore4();
    }
    if ((char)message[0] == '2' & (char)message[1] == 'o' & (char)message[2] == 'f' &
        (char)message[3] == 'f') {                                                        // Topic "interruttore4_Topic" = "2off"
      digitalWrite(RELE_B_interruttore4, 0 ^ Flag_inversione_RELE);                                     // Spengo il RELE 2
      delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
      SONOFFDUAL_B_OFF();
#endif
      EEPROM_write_releBinterruttore4();
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] != '2' ) {                             // Topic "interruttore4_Topic" = "stato"
      payload = Interruttore4_Topic;
      payload += " RSSI=";
      payload += WiFi.RSSI();
      payload += "dB ";
      payload += " RELE1=";
      if (digitalRead(RELE_A_interruttore4) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_A_interruttore4) ^ Flag_inversione_RELE == false ) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE_B_interruttore4) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_B_interruttore4) ^ Flag_inversione_RELE == false) payload += "OFF";
      client.publish(ACK_Topic, (char*) payload.c_str());
      delay(100);
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] == '2' ) {                             // Topic "interruttore4_Topic" = "stato2"
      payload = Interruttore4_Topic;
      payload += " RELE1=";
      if (digitalRead(RELE_A_interruttore4) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_A_interruttore4) ^ Flag_inversione_RELE == false ) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE_B_interruttore4) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_B_interruttore4) ^ Flag_inversione_RELE == false) payload += "OFF";
      client.publish(ACK2_Topic, (char*) payload.c_str());
      delay(100);
    }
  }

  // CANCELLO 1
  if (String(topic) == Cancello1_Topic) {                                              // se arriva il comando sul topic "Cancello1_Topic"
    if ((char)message[0] == '1' & (char)message[1] == 'o' & (char)message[2] == 'n') {    // Topic "Cancello1_Topic" = "1on"
      digitalWrite(RELE_A_cancello1, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 1
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_ON();
#endif
#if defined(ESP01_SERIAL_RELE)
      ESP01_SERIAL_RELE_AON();
#endif
      Rele_A_eccitato_cancello1 = true;
      ulTimeReleACancello1 = millis();
    }
    if ((char)message[0] == '2' & (char)message[1] == 'o' & (char)message[2] == 'n') {    // Topic "Cancello1_Topic" = "2on"
      digitalWrite(RELE_B_cancello1, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 2
#if defined(SONOFFDUAL)
      SONOFFDUAL_B_ON();
#endif
#if defined(ESP01_SERIAL_RELE)
      ESP01_SERIAL_RELE_BON();
#endif
      Rele_B_eccitato_cancello1 = true;
      ulTimeReleBCancello1 = millis();
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] != '2' ) {                             // Topic "Cancello1_Topic" = "stato"
      payload = Cancello1_Topic;
      payload += " RSSI=";
      payload += WiFi.RSSI();
      payload += "dB ";
      payload += " RELE1=";
      if (digitalRead(RELE_A_cancello1) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_A_cancello1) ^ Flag_inversione_RELE == false ) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE_B_cancello1) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_B_cancello1) ^ Flag_inversione_RELE == false) payload += "OFF";
      client.publish(ACK_Topic, (char*) payload.c_str());
      delay(100);
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] == '2' ) {                             // Topic "Cancello1_Topic" = "stato2"
      payload = Cancello1_Topic;
      payload += " RELE1=";
      if (digitalRead(RELE_A_cancello1) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_A_cancello1) ^ Flag_inversione_RELE == false ) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE_B_cancello1) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_B_cancello1) ^ Flag_inversione_RELE == false) payload += "OFF";
      client.publish(ACK2_Topic, (char*) payload.c_str());
      delay(100);
    }
  }

  // CANCELLO 2
  if (String(topic) == Cancello2_Topic) {                                              // se arriva il comando sul topic "Cancello2_Topic"
    if ((char)message[0] == '1' & (char)message[1] == 'o' & (char)message[2] == 'n') {    // Topic "Cancello2_Topic" = "1on"
      digitalWrite(RELE_A_cancello2, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 1
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_ON();
#endif
      Rele_A_eccitato_cancello2 = true;
      ulTimeReleACancello2 = millis();
    }
    if ((char)message[0] == '2' & (char)message[1] == 'o' & (char)message[2] == 'n') {    // Topic "Cancello2_Topic" = "2on"
      digitalWrite(RELE_B_cancello2, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 2
#if defined(SONOFFDUAL)
      SONOFFDUAL_B_ON();
#endif
      Rele_B_eccitato_cancello2 = true;
      ulTimeReleBCancello2 = millis();
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] != '2' ) {                             // Topic "Cancello2_Topic" = "stato"
      payload = Cancello2_Topic;
      payload += " RSSI=";
      payload += WiFi.RSSI();
      payload += "dB ";
      payload += " RELE1=";
      if (digitalRead(RELE_A_cancello2) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_A_cancello2) ^ Flag_inversione_RELE == false ) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE_B_cancello2) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_B_cancello2) ^ Flag_inversione_RELE == false) payload += "OFF";
      client.publish(ACK_Topic, (char*) payload.c_str());
      delay(100);
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] == '2' ) {                             // Topic "Cancello2_Topic" = "stato2"
      payload = Cancello2_Topic;
      payload += " RELE1=";
      if (digitalRead(RELE_A_cancello2) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_A_cancello2) ^ Flag_inversione_RELE == false ) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE_B_cancello2) ^ Flag_inversione_RELE == true ) payload += "ON";
      if (digitalRead(RELE_B_cancello2) ^ Flag_inversione_RELE == false) payload += "OFF";
      client.publish(ACK2_Topic, (char*) payload.c_str());
      delay(100);
    }
  }

  // TEMPERATURA 1
  if (String(topic) == Temperatura1_Topic) {                                                                             // se arriva il comando sul topic "Temperatura1_Topic"
    if ((char)message[0] == 'r' & (char)message[1] == 'e' & (char)message[2] == 'a' & (char)message[3] == 'd' ) {       // Topic "Temperatura1_Topic" = "read"
      delay(50);
      sendTemperature();                                                                                                // sendTemperature()
      delay(50);
    }
    if ((char)message[0] == '1' & (char)message[1] == 'o' & (char)message[2] == 'n' ) {                                 // Topic "Temperatura1_Topic" = "1on"
      digitalWrite(RELE_A_temperatura1, 1 ^ Flag_inversione_RELE);                                                        // Accendo il RELE1
      delay(TEMPO_RELE);                                                                                                // Aspetto
    }
    if ((char)message[0] == '1' & (char)message[1] == 'o' & (char)message[2] == 'f' & (char)message[3] == 'f' ) {       // Topic "Temperatura1_Topic" = "1off"
      digitalWrite(RELE_A_temperatura1, 0 ^ Flag_inversione_RELE);                                                        // Spengo il RELE1
      delay(TEMPO_RELE);                                                                                                // Aspetto
    }
    if ((char)message[0] == '2' & (char)message[1] == 'o' & (char)message[2] == 'n' ) {                                 // Topic "Temperatura1_Topic" = "2on"
      digitalWrite(RELE_B_temperatura1, 1 ^ Flag_inversione_RELE);                                                        // Accendo il RELE2
      delay(TEMPO_RELE);                                                                                                // Aspetto
    }
    if ((char)message[0] == '2' & (char)message[1] == 'o' & (char)message[2] == 'f' & (char)message[3] == 'f' ) {       // Topic "Temperatura1_Topic" = "2off"
      digitalWrite(RELE_B_temperatura1, 0 ^ Flag_inversione_RELE);                                                        // Spengo il RELE2
      delay(TEMPO_RELE);                                                                                                // Aspetto
    }
    if (((char)message[0] == 'T' or (char)message[0] == 't') &
        (char)message[1] == '=' ) {                                                                                     // Topic "Temperatura1_Topic" = "T=" o "t="
      String stringtmp = "";
      for (int i = 2; i < length; i++)  {
        stringtmp = stringtmp + (char)message[i];
      }
      Debug_MSG_LN(stringtmp);
      termostato_temperatura1 = stringtmp.toFloat();
      Debug_MSG_LN((String) termostato_temperatura1);
      EEPROM_temperatura1_write();
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] != '2' ) {                           // Topic "Temperatura1_Topic" = "stato"
      payload = Temperatura1_Topic;
      payload += " RSSI=";
      payload += WiFi.RSSI();
      payload += "dB ";
      payload += " T=";
      char tmp[4];                            // string buffer
      String stringtmp = "";                  //data on buff is copied to this string
      dtostrf(termostato_temperatura1, 4, 1, tmp);
      for (int i = 0; i < sizeof(tmp); i++)  {
        stringtmp += tmp[i];
      }
      payload += stringtmp;
      if (AUTO_temperatura1 == true) payload += " AUTO";
      else payload += " MAN";
      payload += " RELE1=";
      if (digitalRead(RELE_A_temperatura1) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE_A_temperatura1) == false ^ Flag_inversione_RELE) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE_B_temperatura1) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE_B_temperatura1) == false ^ Flag_inversione_RELE) payload += "OFF";
      client.publish(ACK_Topic, (char*) payload.c_str());
      delay(100);
    }
    if ((char)message[0] == 's' & (char)message[1] == 't' & (char)message[2] == 'a' &
        (char)message[3] == 't' & (char)message[4] == 'o' & (char)message[5] == '2' ) {                           // Topic "Temperatura1_Topic" = "stato2"
      delay(50);
      sendTemperature();                                                                                                // sendTemperature()
      delay(50);
      payload = Temperatura1_Topic;
      payload += " T=";
      char tmp[4];                            // string buffer
      String stringtmp = "";                  //data on buff is copied to this string
      dtostrf(termostato_temperatura1, 4, 1, tmp);
      for (int i = 0; i < sizeof(tmp); i++)  {
        stringtmp += tmp[i];
      }
      payload += stringtmp;
      if (AUTO_temperatura1 == true) payload += " AUTO";
      else payload += " MAN";
      payload += " RELE1=";
      if (digitalRead(RELE_A_temperatura1) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE_A_temperatura1) == false ^ Flag_inversione_RELE) payload += "OFF";
      payload += " RELE2=";
      if (digitalRead(RELE_B_temperatura1) == true ^ Flag_inversione_RELE) payload += "ON";
      if (digitalRead(RELE_B_temperatura1) == false ^ Flag_inversione_RELE) payload += "OFF";
      client.publish(ACK_Topic, (char*) payload.c_str());
      delay(100);
    }
    if ((char)message[0] == 'a' & (char)message[1] == 'u' & (char)message[2] == 't' & (char)message[3] == 'o' ) {       // Topic "Temperatura1_Topic" = "auto"
      AUTO_temperatura1 = true;
      EEPROM_temperatura1_write();
    }
    if ((char)message[0] == 'm' & (char)message[1] == 'a' & (char)message[2] == 'n') {                                  // Topic "Temperatura1_Topic" = "man"
      AUTO_temperatura1 = false;
      EEPROM_temperatura1_write();
    }
  }

  if (String(topic) == ACK_Topic) {                                                       // se arriva il comando sul topic "ACK"
    if ((char)message[0] == 'a' & (char)message[1] == 'c' & (char)message[2] == 'k' ) {   // Topic "ack" = "ack"
      Send_ACK();
      delay(100);
    }
  }

  if ((char)message[0] == 'r' & (char)message[1] == 'e' & (char)message[2] == 's' &
      (char)message[3] == 'e' & (char)message[4] == 't' ) {                             // messaggio = "reset"
    EEPROM_clear_reset();
  }
  if ((char)message[0] == 'r' & (char)message[1] == 'e' & (char)message[2] == 'b' &
      (char)message[3] == 'o' & (char)message[4] == 'o' & (char)message[5] == 't' ) {                             // messaggio = "reboot"
    delay(500);
    ESP.restart();
  }

}




