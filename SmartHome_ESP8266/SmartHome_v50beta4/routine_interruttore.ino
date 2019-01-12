TPush Interruttore_TPush_bottone[Interruttore_nodi];
TPush Interruttore_TPush_stato[Interruttore_nodi];

long EEPROM_Interruttore_stato_rele = 200;
String Interruttore_DEF     = "INT";
String Interruttore_ON      = "on";
String Interruttore_OFF     = "off";
String Interruttore_TOGGLE  = "toggle";
String Interruttore_STATO   = "stato";
String Interruttore_ACK_ON  = "on";
String Interruttore_ACK_OFF = "off";
String Interruttore_Topic_ACK_suffisso = "/ack";

typedef struct {
  String topic;
  String topic_ACK;
  int rele;
  int bottone;
  int stato;
  String tipo_bottone;
  unsigned long ulTime;
} InfoRecordInterruttoreType ;

InfoRecordInterruttoreType Interruttore[10];

void setup_Interruttore() {
  Interruttore[0].topic = Interruttore1_Topic;
  Interruttore[0].topic_ACK = Interruttore1_Topic;
  Interruttore[0].topic_ACK += Interruttore_Topic_ACK_suffisso;
  Interruttore[0].rele = Interruttore1_GPIO_rele;
#ifdef Interruttore1_GPIO_bottone
  Interruttore[0].bottone = Interruttore1_GPIO_bottone;
  Interruttore[0].tipo_bottone = Interruttore1_tipo_bottone;
#else
  Interruttore[0].bottone = 100;
#endif
  Interruttore[0].stato = Interruttore1_GPIO_stato;

  Interruttore[1].topic = Interruttore2_Topic;
  Interruttore[1].topic_ACK = Interruttore2_Topic;
  Interruttore[1].topic_ACK += Interruttore_Topic_ACK_suffisso;
  Interruttore[1].rele = Interruttore2_GPIO_rele;
#ifdef Interruttore2_GPIO_bottone
  Interruttore[1].bottone = Interruttore2_GPIO_bottone;
  Interruttore[1].tipo_bottone = Interruttore2_tipo_bottone;
#else
  Interruttore[1].bottone = 100;
#endif
  Interruttore[1].stato = Interruttore2_GPIO_stato;

  Interruttore[2].topic = Interruttore3_Topic;
  Interruttore[2].topic_ACK = Interruttore3_Topic;
  Interruttore[2].topic_ACK += Interruttore_Topic_ACK_suffisso;
  Interruttore[2].rele = Interruttore3_GPIO_rele;
#ifdef Interruttore3_GPIO_bottone
  Interruttore[2].bottone = Interruttore3_GPIO_bottone;
  Interruttore[2].tipo_bottone = Interruttore3_tipo_bottone;
#else
  Interruttore[2].bottone = 100;
#endif
  Interruttore[2].stato = Interruttore3_GPIO_stato;

  Interruttore[3].topic = Interruttore4_Topic;
  Interruttore[3].topic_ACK = Interruttore4_Topic;
  Interruttore[3].topic_ACK += Interruttore_Topic_ACK_suffisso;
  Interruttore[3].rele = Interruttore4_GPIO_rele;
#ifdef Interruttore4_GPIO_bottone
  Interruttore[3].bottone = Interruttore4_GPIO_bottone;
  Interruttore[3].tipo_bottone = Interruttore4_tipo_bottone;
#else
  Interruttore[3].bottone = 100;
#endif
  Interruttore[3].stato = Interruttore4_GPIO_stato;

  Interruttore[4].topic = Interruttore5_Topic;
  Interruttore[4].topic_ACK = Interruttore5_Topic;
  Interruttore[4].topic_ACK += Interruttore_Topic_ACK_suffisso;
  Interruttore[4].rele = Interruttore5_GPIO_rele;
#ifdef Interruttore5_GPIO_bottone
  Interruttore[4].bottone = Interruttore5_GPIO_bottone;
  Interruttore[4].tipo_bottone = Interruttore5_tipo_bottone;
#else
  Interruttore[4].bottone = 100;
#endif
  Interruttore[4].stato = Interruttore5_GPIO_stato;

  Interruttore[5].topic = Interruttore6_Topic;
  Interruttore[5].topic_ACK = Interruttore6_Topic;
  Interruttore[5].topic_ACK += Interruttore_Topic_ACK_suffisso;
  Interruttore[5].rele = Interruttore6_GPIO_rele;
#ifdef Interruttore6_GPIO_bottone
  Interruttore[5].bottone = Interruttore6_GPIO_bottone;
  Interruttore[5].tipo_bottone = Interruttore6_tipo_bottone;
#else
  Interruttore[5].bottone = 100;
#endif
  Interruttore[5].stato = Interruttore6_GPIO_stato;

  Interruttore[6].topic = Interruttore7_Topic;
  Interruttore[6].topic_ACK = Interruttore7_Topic;
  Interruttore[6].topic_ACK += Interruttore_Topic_ACK_suffisso;
  Interruttore[6].rele = Interruttore7_GPIO_rele;
#ifdef Interruttore7_GPIO_bottone
  Interruttore[6].bottone = Interruttore7_GPIO_bottone;
  Interruttore[6].tipo_bottone = Interruttore7_tipo_bottone;
#else
  Interruttore[6].bottone = 100;
#endif
  Interruttore[6].stato = Interruttore7_GPIO_stato;

  Interruttore[7].topic = Interruttore8_Topic;
  Interruttore[7].topic_ACK = Interruttore8_Topic;
  Interruttore[7].topic_ACK += Interruttore_Topic_ACK_suffisso;
  Interruttore[7].rele = Interruttore8_GPIO_rele;
#ifdef Interruttore8_GPIO_bottone
  Interruttore[7].bottone = Interruttore8_GPIO_bottone;
  Interruttore[7].tipo_bottone = Interruttore8_tipo_bottone;
#else
  Interruttore[7].bottone = 100;
#endif
  Interruttore[7].stato = Interruttore8_GPIO_stato;

  Interruttore[8].topic = Interruttore9_Topic;
  Interruttore[8].topic_ACK = Interruttore9_Topic;
  Interruttore[8].topic_ACK += Interruttore_Topic_ACK_suffisso;
  Interruttore[8].rele = Interruttore9_GPIO_rele;
#ifdef Interruttore9_GPIO_bottone
  Interruttore[8].bottone = Interruttore9_GPIO_bottone;
  Interruttore[8].tipo_bottone = Interruttore9_tipo_bottone;
#else
  Interruttore[8].bottone = 100;
#endif
  Interruttore[8].stato = Interruttore9_GPIO_stato;

  Interruttore[9].topic = Interruttore10_Topic;
  Interruttore[9].topic_ACK = Interruttore10_Topic;
  Interruttore[9].topic_ACK += Interruttore_Topic_ACK_suffisso;
  Interruttore[9].rele = Interruttore10_GPIO_rele;
#ifdef Interruttore10_GPIO_bottone
  Interruttore[9].bottone = Interruttore10_GPIO_bottone;
  Interruttore[9].tipo_bottone = Interruttore10_tipo_bottone;
#else
  Interruttore[9].bottone = 100;
#endif
  Interruttore[9].stato = Interruttore10_GPIO_stato;

  for (int i = 0; i < Interruttore_nodi; i++) {
    Debug_MSG_LN("- Interruttore " + String(i + 1) + " : " + Interruttore[i].topic);
    Debug_MSG_LN("RELE    Interruttore " + String(i + 1) + " = GPIO" + String(Interruttore[i].rele));
    Debug_MSG_LN("STATO   Interruttore " + String(i + 1) + " = GPIO" + String(Interruttore[i].stato));
    if (Interruttore[i].bottone >= 99) {
      Debug_MSG_LN("BOTTONE Interruttore " + String(i + 1) + " : Nessun GPIO definito!");
    }
    else {
      Debug_MSG_LN("BOTTONE Interruttore " + String(i + 1) + " = GPIO" + String(Interruttore[i].bottone));
#if defined(PullDown)
      Interruttore_TPush_bottone[i].setUp(Interruttore[i].bottone, HIGH);
#endif
#if defined(PullUp)
      Interruttore_TPush_bottone[i].setUp(Interruttore[i].bottone, LOW);
#endif
    }

    if (Interruttore[i].rele != Interruttore[i].stato) {
#if defined(PullDown)
      Interruttore_TPush_stato[i].setUp(Interruttore[i].stato, HIGH);
#endif
#if defined(PullUp)
      Interruttore_TPush_stato[i].setUp(Interruttore[i].stato, LOW);
#endif
    }
    pinMode(Interruttore[i].rele, OUTPUT);
    digitalWrite(Interruttore[i].rele, EEPROM_READ_Interruttore_stato_rele(i) ^ Flag_inversione_RELE);

#if defined(SONOFFDUAL)
    if (EEPROM_READ_Interruttore_stato_rele(0) == 1) SONOFFDUAL_A_ON();
    if (EEPROM_READ_Interruttore_stato_rele(0) == 0) SONOFFDUAL_A_OFF();
    if (EEPROM_READ_Interruttore_stato_rele(1) == 1) SONOFFDUAL_B_ON();
    if (EEPROM_READ_Interruttore_stato_rele(1) == 0) SONOFFDUAL_B_OFF();
#endif
#if defined(ESP01_SERIAL_RELE)
    if (EEPROM_READ_Interruttore_stato_rele(0) == 1) ESP01_SERIAL_RELE_A_ON();
    if (EEPROM_READ_Interruttore_stato_rele(0) == 0) ESP01_SERIAL_RELE_A_OFF();
#endif
  }
}

void ACK_Interruttore() {
  String payload;
  for (int i = 0; i < Interruttore_nodi; i++) {
    payload = "{\"MAC_address\":\"";
    payload += macToStr(mac);
    payload += "\",\"NODO\":\"";
    payload += Interruttore_DEF;
    payload += "\",\"Topic\":\"";
    payload += Interruttore[i].topic;
    payload += "\",\"Topic_ACK\":\"";
    payload += Interruttore[i].topic_ACK;
    payload += "\",\"availability_topic\":\"";
    payload += WillTopicStr;
    payload += "\"}";
    client.publish(ACK_Topic, (char*) payload.c_str());
    delay(500);
    Stato_INT(i);
  }
}

void Alive_Interruttore() {
  String HAtopic;
  String HApayload;

#if defined(HomeAssistant_discovery)
  for (int i = 0; i < Interruttore_nodi; i++) {
    HAtopic = "homeassistant/switch/";
    HAtopic += macToStr(mac);
    HAtopic += "/";
    HAtopic += Interruttore_DEF;
    HAtopic += i;
    HAtopic += "/config";
    HApayload = "{\"name\":\"";
    HApayload += Interruttore[i].topic;
    HApayload += "\",\"command_topic\":\"";
    HApayload += Interruttore[i].topic;
    HApayload += "\",\"payload_on\":\"";
    HApayload += "on";
    HApayload += "\",\"payload_off\":\"";
    HApayload += "off";
    HApayload += "\",\"state_topic\":\"";
    HApayload += Interruttore[i].topic_ACK;
    HApayload += "\",\"state_on\":\"";
    HApayload += "on";
    HApayload += "\",\"state_off\":\"";
    HApayload += "off";
    HApayload += "\",\"value_template\":\"";
    HApayload += "{{value_json.STATO}}";

    HApayload += "\",\"availability_topic\":\"";
    HApayload += macToStr(mac);
    HApayload += "\"}";
    client.publish((char*) HAtopic.c_str(), (char*) HApayload.c_str());
    delay(250);
  }
#endif
}

void Stato_INT(int i) {
  String payload;
  payload = "{\"NODO\":\"";
  payload += Interruttore_DEF;
  payload += "\",\"STATO\":\"";
  if (digitalRead(Interruttore[i].stato) ^ Flag_inversione_RELE == true ) payload += Interruttore_ACK_ON;
  if (digitalRead(Interruttore[i].stato) ^ Flag_inversione_RELE == false ) payload += Interruttore_ACK_OFF;
  payload += "\"}";
  client.publish((char*) Interruttore[i].topic_ACK.c_str(), (char*) payload.c_str());
  delay(250);
}

void Subscribe_Interruttore() {
  for (int i = 0; i < Interruttore_nodi; i++) {
    client.subscribe((char*) Interruttore[i].topic.c_str());
    delay(250);
    Debug_MSG_LN("MQTT subscribe " + Interruttore[i].topic);
  }
}

void callback_Interruttore(char* topic, byte * message, unsigned int length) {
  String payload;
  String message_String;
  for (int i = 0; i < Interruttore_nodi; i++) {
    if (String(topic) == Interruttore[i].topic) {               // se arriva il comando sul topic "Interruttore1_Topic"
      message_String = "";
      for (int ii = 0; ii < length; ii++) {
        message_String += (char)message[ii];
      }
      if (message_String == Interruttore_ON) {                                                   // Topic "Interruttore1_Topic" = "on"
        digitalWrite(Interruttore[i].rele, 1 ^ Flag_inversione_RELE);                                     // Accendo il rele
#if defined(SONOFFDUAL)
        if (i = 0) SONOFFDUAL_A_ON();
        if (i = 1) SONOFFDUAL_B_ON();
#endif
#if defined(ESP01_SERIAL_RELE)
        ESP01_SERIAL_RELE_A_ON();
#endif
        EEPROM_WRITE_Interruttore_stato_rele(i);
        Stato_INT(i);
      }
      if (message_String == Interruttore_OFF) {                                                     // Topic "Interruttore1_Topic" = "off"
        digitalWrite(Interruttore[i].rele, 0 ^ Flag_inversione_RELE);                                     // Spengo il rele
#if defined(SONOFFDUAL)
        if (i = 0) SONOFFDUAL_A_OFF();;
        if (i = 1) SONOFFDUAL_B_OFF();
#endif
#if defined(ESP01_SERIAL_RELE)
        ESP01_SERIAL_RELE_A_OFF();
#endif
        EEPROM_WRITE_Interruttore_stato_rele(i);
        Stato_INT(i);
      }
      if (message_String == Interruttore_TOGGLE) {                                                    // Topic "Interruttore1_Topic" = "toggle"
        digitalWrite(Interruttore[i].rele, not(digitalRead(Interruttore[i].rele)));
#if defined(SONOFFDUAL)
        if (digitalRead(Interruttore[0].rele)) SONOFFDUAL_A_ON();
        else SONOFFDUAL_A_OFF();
        if (digitalRead(Interruttore[1].rele)) SONOFFDUAL_B_ON();
        else SONOFFDUAL_B_OFF();
#endif
#if defined(ESP01_SERIAL_RELE)
        if (digitalRead(Interruttore[0].rele)) ESP01_SERIAL_RELE_A_ON();
        else ESP01_SERIAL_RELE_A_OFF();
#endif
        EEPROM_WRITE_Interruttore_stato_rele(i);
        Stato_INT(i);
      }
      if (message_String == Interruttore_STATO) {                                                     // Topic "Interruttore1_Topic" = "stato"
        Stato_INT(i);
      }
    }
  }
}



void loop_Interruttore() {
  String payload;
  for (int i = 0; i < Interruttore_nodi; i++) {
    if (Interruttore_TPush_bottone[i].Click(Interruttore_TEMPO_CLICK_ON)) {                                // Bottone premuto! TEMPO_CLICK_ON
      Debug_MSG_LN("Bottone premuto!");
      digitalWrite(Interruttore[i].rele, not(digitalRead(Interruttore[i].rele)));
#if defined(SONOFFDUAL)
      if (digitalRead(Interruttore[0].rele)) SONOFFDUAL_A_ON();
      else SONOFFDUAL_A_OFF();
      if (digitalRead(Interruttore[1].rele)) SONOFFDUAL_B_ON();
      else SONOFFDUAL_B_OFF();
#endif
#if defined(ESP01_SERIAL_RELE)
      if (digitalRead(Interruttore[0].rele)) ESP01_SERIAL_RELE_A_ON();
          else ESP01_SERIAL_RELE_A_OFF();
#endif
            EEPROM_WRITE_Interruttore_stato_rele(i);
            Stato_INT(i);
      }
  if (t = Interruttore_TPush_bottone[i].Keep(Interruttore_TEMPO_CLICK_OFF)) {                            // Bottone rilasciato! TEMPO_CLICK_OFF
      Debug_MSG_LN("Bottone rilasciato!");
      if (Interruttore[i].tipo_bottone == "monostabile") {
        digitalWrite(Interruttore[i].rele, not(digitalRead(Interruttore[i].rele)));
#if defined(SONOFFDUAL)
        if (digitalRead(Interruttore[0].rele)) SONOFFDUAL_A_ON();
        else SONOFFDUAL_A_OFF();
        if (digitalRead(Interruttore[1].rele)) SONOFFDUAL_B_ON();
        else SONOFFDUAL_B_OFF();
#endif
#if defined(ESP01_SERIAL_RELE)
        if (digitalRead(Interruttore[0].rele)) ESP01_SERIAL_RELE_A_ON();
        else ESP01_SERIAL_RELE_A_OFF();
#endif
      }
      EEPROM_WRITE_Interruttore_stato_rele(i);
      Stato_INT(i);
    }
    if ((Interruttore_TPush_stato[i].Click(Interruttore_TEMPO_CLICK_ON) or Interruttore_TPush_stato[i].Keep(Interruttore_TEMPO_CLICK_OFF))) {
      Stato_INT(i);
    }
  }
}



int EEPROM_READ_Interruttore_stato_rele(int i) {                                              // Leggo dalla EEPROM RELE Interruttore
  Debug_MSG("Reading EEPROM Interruttore stato rele " + String(i + 1) + " : ");
  int tmp;
  tmp = eepromReadInt(EEPROM_Interruttore_stato_rele + (i  * 2));
  if (tmp != 0 && tmp != 1) tmp = 0;
  if (tmp == 1) Debug_MSG_LN("ON");
  if (tmp == 0) Debug_MSG_LN("OFF");
  return tmp;
}

void EEPROM_WRITE_Interruttore_stato_rele(int i) {
  Debug_MSG("Writing EEPROM Interruttore stato rele " + String(i + 1) + " : ");
  if (digitalRead(Interruttore[i].rele) ^ Flag_inversione_RELE == true ) {
    int tmp = 1;
    eepromWriteInt(EEPROM_Interruttore_stato_rele + (i  * 2), tmp );
    Debug_MSG_LN("ON");
  }
  if (digitalRead(Interruttore[i].rele) ^ Flag_inversione_RELE == false) {
    int tmp = 0;
    eepromWriteInt(EEPROM_Interruttore_stato_rele + (i  * 2),  tmp );
    Debug_MSG_LN("OFF");
  }
  EEPROM.commit();
}
