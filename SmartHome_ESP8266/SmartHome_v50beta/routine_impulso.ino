TPush Impulso_TPush_bottone[Impulso_nodi];

String Impulso_DEF     = "CAN";
String Impulso_ON      = "1on";
String Impulso_ACK_ON  = "1on";
String Impulso_ACK_OFF = "1off";
String Impulso_ACK_ONLINE = "online";
String Impulso_STATO   = "stato";

typedef struct {
  String topic;
  String topic_ACK;
  int rele;
  int bottone;
  unsigned long ulTime;
  boolean rele_eccitato;
} InfoRecordImpulsoType ;

InfoRecordImpulsoType Impulso[10];

void setup_Impulso() {
  Impulso[0].topic = Impulso1_Topic;
  Impulso[1].topic = Impulso2_Topic;
  Impulso[2].topic = Impulso3_Topic;
  Impulso[3].topic = Impulso4_Topic;
  Impulso[4].topic = Impulso5_Topic;
  Impulso[5].topic = Impulso6_Topic;
  Impulso[6].topic = Impulso7_Topic;
  Impulso[7].topic = Impulso8_Topic;
  Impulso[8].topic = Impulso9_Topic;
  Impulso[9].topic = Impulso10_Topic;
  Impulso[0].topic_ACK = Impulso1_Topic_ACK;
  Impulso[1].topic_ACK = Impulso2_Topic_ACK;
  Impulso[2].topic_ACK = Impulso3_Topic_ACK;
  Impulso[3].topic_ACK = Impulso4_Topic_ACK;
  Impulso[4].topic_ACK = Impulso5_Topic_ACK;
  Impulso[5].topic_ACK = Impulso6_Topic_ACK;
  Impulso[6].topic_ACK = Impulso7_Topic_ACK;
  Impulso[7].topic_ACK = Impulso8_Topic_ACK;
  Impulso[8].topic_ACK = Impulso9_Topic_ACK;
  Impulso[9].topic_ACK = Impulso10_Topic_ACK;
  Impulso[0].rele = Impulso1_GPIO_rele;
  Impulso[1].rele = Impulso2_GPIO_rele;
  Impulso[2].rele = Impulso3_GPIO_rele;
  Impulso[3].rele = Impulso4_GPIO_rele;
  Impulso[4].rele = Impulso5_GPIO_rele;
  Impulso[5].rele = Impulso6_GPIO_rele;
  Impulso[6].rele = Impulso7_GPIO_rele;
  Impulso[7].rele = Impulso8_GPIO_rele;
  Impulso[8].rele = Impulso9_GPIO_rele;
  Impulso[9].rele = Impulso10_GPIO_rele;
  Impulso[0].bottone = Impulso1_GPIO_bottone;
  Impulso[1].bottone = Impulso2_GPIO_bottone;
  Impulso[2].bottone = Impulso3_GPIO_bottone;
  Impulso[3].bottone = Impulso4_GPIO_bottone;
  Impulso[4].bottone = Impulso5_GPIO_bottone;
  Impulso[5].bottone = Impulso6_GPIO_bottone;
  Impulso[6].bottone = Impulso7_GPIO_bottone;
  Impulso[7].bottone = Impulso8_GPIO_bottone;
  Impulso[8].bottone = Impulso9_GPIO_bottone;
  Impulso[9].bottone = Impulso10_GPIO_bottone;
  Impulso[0].rele_eccitato = false;
  Impulso[1].rele_eccitato = false;
  Impulso[2].rele_eccitato = false;
  Impulso[3].rele_eccitato = false;
  Impulso[4].rele_eccitato = false;
  Impulso[5].rele_eccitato = false;
  Impulso[6].rele_eccitato = false;
  Impulso[7].rele_eccitato = false;
  Impulso[8].rele_eccitato = false;
  Impulso[9].rele_eccitato = false;

  for (int i = 0; i < Impulso_nodi; i++) {
    Debug_MSG_LN("- Impulso " + String(i + 1) + " : " + Impulso[i].topic);
    Debug_MSG_LN("RELE Impulso " + String(i + 1) + "        = GPIO" + Impulso[i].rele);
    Debug_MSG_LN("BOTTONE Impulso " + String(i + 1) + "1    = GPIO" + Impulso[i].bottone);
#if defined(Impulso1_NFC) & i==1
    Debug_MSG_LN("NFC RST                = GPIO" + String(Impulso1_GPIO_RST));
    Debug_MSG_LN("NFC SDA(SS)            = GPIO" + String(Impulso1_GPIO_SS));
    Debug_MSG_LN("NFC MOSI               = GPIO" + String(Impulso1_GPIO_MOSI));
    Debug_MSG_LN("NFC MISO               = GPIO" + String(Impulso1_GPIO_MISO));
    Debug_MSG_LN("NFC SCK                = GPIO" + String(Impulso1_GPIO_SCK));
    setup_nfc();
#endif
    pinMode(Impulso[i].rele, OUTPUT);
    digitalWrite(Impulso[i].rele, 0 ^ Flag_inversione_RELE);
#if defined(PullDown)
    Impulso_TPush_bottone[i].setUp(Impulso[i].bottone, HIGH);
#endif
#if defined(PullUp)
    Impulso_TPush_bottone[i].setUp(Impulso[i].bottone, LOW);
#endif
  }
}

void ACK_Impulso() {
  String payload;
  for (int i = 0; i < Impulso_nodi; i++) {
    payload = macToStr(mac);
    payload += " alive! " + Impulso_DEF + " ";
    payload += Impulso[i].topic;
    client.publish(ACK_Topic, (char*) payload.c_str());
    delay(250);
    payload = Impulso_ACK_OFF;
    client.publish((char*) Impulso[i].topic_ACK.c_str(), (char*) payload.c_str());
    delay(250);
  }
}
void Online_ACK_Impulso() {
  String payload;
  for (int i = 0; i < Impulso_nodi; i++) {
    payload = Impulso_ACK_ONLINE;
    client.publish((char*) Impulso[i].topic_ACK.c_str(), (char*) payload.c_str());
    delay(250);
  }
}

void Subscribe_Impulso() {
  for (int i = 0; i < Impulso_nodi; i++) {
    client.subscribe((char*) Impulso[i].topic.c_str());                  // Sottoscrivi Impulso1_Topic
    delay(250);
    Debug_MSG_LN("MQTT subscribe " + String(Impulso[i].topic));
  }
}

void callback_Impulso(char* topic, byte * message, unsigned int length) {
  String payload;
  String message_String;
  for (int i = 0; i < Impulso_nodi; i++) {
    if (String(topic) == Impulso[i].topic) {                                              // se arriva il comando sul topic "Impulso1_Topic"
      message_String = "";
      for (int ii = 0; ii < length; ii++) {
        message_String += (char)message[ii];
      }
      if (message_String == Impulso_ON) {                  // Topic "Impulso1_Topic" = "on"
        digitalWrite(Impulso[i].rele, 1 ^ Flag_inversione_RELE);                                     // Accendo il rele
        payload = Impulso_ACK_ON;
        client.publish((char*) Impulso[i].topic_ACK.c_str(), (char*) payload.c_str());
#if defined(SONOFFDUAL)
        if (i = 0) SONOFFDUAL_A_ON();
        if (i = 1) SONOFFDUAL_B_ON();
#endif
        /*#if defined(ESP01_SERIAL_RELE)
           ESP01_SERIAL_RELE_AON();
          #endif*/
        Impulso[i].rele_eccitato = true;
        Impulso[i].ulTime = millis();
      }

      if (message_String == Impulso_STATO) {                                      // Topic "Impulso1_Topic" = "stato"
        payload = Impulso[i].topic;
        payload += " RSSI=";
        payload += WiFi.RSSI();
        payload += "dB ";
        payload += " RELE=";
        if (digitalRead(Impulso[i].rele) ^ Flag_inversione_RELE == true ) payload += "ON";
        if (digitalRead(Impulso[i].rele) ^ Flag_inversione_RELE == false ) payload += "OFF";
        client.publish(ACK_Topic, (char*) payload.c_str());
        delay(100);
      }

#if defined(Impulso1_NFC)
      if ((char)message[0] == '+' & (char)message[1] == 'k' & (char)message[2] == 'e' & (char)message[3] == 'y') {    // Topic "Impulso1_Topic" = "+key"
        piukey();
      }
      if ((char)message[0] == '-' & (char)message[1] == 'k' & (char)message[2] == 'e' & (char)message[3] == 'y') {    // Topic "Impulso1_Topic" = "-key"
        menokey();
      }
      if ((char)message[0] == 'X' & (char)message[1] == 'k' & (char)message[2] == 'e' & (char)message[3] == 'y') {    // Topic "Impulso1_Topic" = "Xkey"
        clearkey();
      }
#endif
    }
  }
}

void loop_Impulso() {
  String payload;
#if defined(Impulso1_NFC)
  loop_nfc();
#endif
  for (int i = 0; i < Impulso_nodi; i++) {
    if (((millis() - Impulso[i].ulTime) > Impulso_TEMPO_IMPULSO) & Impulso[i].rele_eccitato) {        // Tempo rele -> spengo!
      Debug_MSG_LN("Tempo rele -> spengo!");
      digitalWrite(Impulso[i].rele, 0 ^ Flag_inversione_RELE);                     // Spengo il rele
      payload = Impulso_ACK_OFF;
      client.publish((char*) Impulso[i].topic_ACK.c_str(), (char*) payload.c_str());
#if defined(SONOFFDUAL)
      if (i = 0) SONOFFDUAL_A_OFF();;
      if (i = 1) SONOFFDUAL_B_OFF();
#endif
      /*#if defined(ESP01_SERIAL_RELE)
        ESP01_SERIAL_RELE_A_OFF();
        #endif*/
      Impulso[i].rele_eccitato = false;
    }
    if (Impulso_TPush_bottone[i].Click(Impulso_TEMPO_CLICK_ON)) {                                // Bottone premuto!
      Debug_MSG_LN("Bottone premuto!");
      digitalWrite(Impulso[i].rele, 1 ^ Flag_inversione_RELE);
      payload = Impulso_ACK_ON;
      client.publish((char*) Impulso[i].topic_ACK.c_str(), (char*) payload.c_str());
#if defined(SONOFFDUAL)
      if (i = 0) SONOFFDUAL_A_ON();
      if (i = 1) SONOFFDUAL_B_ON();
#endif
      /*#if defined(SONOFFDUAL)
          SONOFFDUAL_A_ON();
        #endif*/
      Impulso[i].rele_eccitato = true;
      Impulso[i].ulTime = millis();
    }
  }
}
