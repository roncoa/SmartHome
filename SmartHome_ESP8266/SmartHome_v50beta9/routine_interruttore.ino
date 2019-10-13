#if Interruttore_nodi>0

#define Interruttore_TEMPO_CLICK_ON      TEMPO_CLICK_ON
#define Interruttore_TEMPO_CLICK_OFF     TEMPO_CLICK_OFF

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

TPush Interruttore_TPush_bottone[Interruttore_nodi], Interruttore_TPush_stato[Interruttore_nodi];

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
#ifdef Interruttore1_GPIO_stato
  Interruttore[0].stato = Interruttore1_GPIO_stato;
#else
  Interruttore[0].stato = Interruttore1_GPIO_rele;
#endif

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
#ifdef Interruttore2_GPIO_stato
  Interruttore[1].stato = Interruttore2_GPIO_stato;
#else
  Interruttore[1].stato = Interruttore2_GPIO_rele;
#endif

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
#ifdef Interruttore3_GPIO_stato
  Interruttore[2].stato = Interruttore3_GPIO_stato;
#else
  Interruttore[2].stato = Interruttore3_GPIO_rele;
#endif

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
#ifdef Interruttore4_GPIO_stato
  Interruttore[3].stato = Interruttore4_GPIO_stato;
#else
  Interruttore[3].stato = Interruttore4_GPIO_rele;
#endif

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
#ifdef Interruttore5_GPIO_stato
  Interruttore[4].stato = Interruttore5_GPIO_stato;
#else
  Interruttore[4].stato = Interruttore5_GPIO_rele;
#endif

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
#ifdef Interruttore6_GPIO_stato
  Interruttore[5].stato = Interruttore6_GPIO_stato;
#else
  Interruttore[5].stato = Interruttore6_GPIO_rele;
#endif

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
#ifdef Interruttore7_GPIO_stato
  Interruttore[6].stato = Interruttore7_GPIO_stato;
#else
  Interruttore[6].stato = Interruttore7_GPIO_rele;
#endif

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
#ifdef Interruttore8_GPIO_stato
  Interruttore[7].stato = Interruttore8_GPIO_stato;
#else
  Interruttore[7].stato = Interruttore8_GPIO_rele;
#endif

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
#ifdef Interruttore9_GPIO_stato
  Interruttore[8].stato = Interruttore9_GPIO_stato;
#else
  Interruttore[8].stato = Interruttore9_GPIO_rele;
#endif

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
#ifdef Interruttore10_GPIO_stato
  Interruttore[9].stato = Interruttore10_GPIO_stato;
#else
  Interruttore[9].stato = Interruttore10_GPIO_rele;
#endif

  for (int i = 0; i < Interruttore_nodi; i++) {
    Debug_MSG_LN("Nodo Interruttore " + String(i + 1) + " : " + Interruttore[i].topic);
    Debug_MSG_LN(" RELE    Interruttore " + String(i + 1) + " = GPIO" + String(Interruttore[i].rele));
    if (Interruttore[i].rele == Interruttore[i].stato) {
      Debug_MSG_LN(" STATO   Interruttore " + String(i + 1) + " = RELE Interruttore " + String(i + 1));
    }
    else {
      Debug_MSG_LN(" STATO   Interruttore " + String(i + 1) + " = GPIO" + String(Interruttore[i].stato));
    }
    if (Interruttore[i].bottone >= 99) {
      Debug_MSG_LN(" BOTTONE Interruttore " + String(i + 1) + " : Nessun GPIO definito!");
    }
    else {
      Debug_MSG_LN(" BOTTONE Interruttore " + String(i + 1) + " = GPIO" + String(Interruttore[i].bottone));
    }
#ifdef PullDown
    Interruttore_TPush_bottone[i].setUp(Interruttore[i].bottone, HIGH);
    Debug_MSG_LN(" Bottone PullDown");
    if (Interruttore[i].rele != Interruttore[i].stato) {
      Interruttore_TPush_stato[i].setUp(Interruttore[i].stato, HIGH);
      Debug_MSG_LN(" Stato   PullDown");
    }
#endif
#ifdef PullUp
    Interruttore_TPush_bottone[i].setUp(Interruttore[i].bottone, LOW);
    Debug_MSG_LN(" Bottone PullUp");
    if (Interruttore[i].rele != Interruttore[i].stato) {
      Interruttore_TPush_stato[i].setUp(Interruttore[i].stato, LOW);
      Debug_MSG_LN(" Stato   PullUp");
    }
#endif

    pinMode(Interruttore[i].rele, OUTPUT);
    digitalWrite(Interruttore[i].rele, EEPROM_READ_Interruttore_stato_rele(i) ^ Flag_inversione_RELE);
#ifdef SONOFFDUAL
    if (EEPROM_READ_Interruttore_stato_rele(0) == 1) SONOFFDUAL_A_ON();
    if (EEPROM_READ_Interruttore_stato_rele(0) == 0) SONOFFDUAL_A_OFF();
    if (EEPROM_READ_Interruttore_stato_rele(1) == 1) SONOFFDUAL_B_ON();
    if (EEPROM_READ_Interruttore_stato_rele(1) == 0) SONOFFDUAL_B_OFF();
#endif
#ifdef ESP01_SERIAL_RELE
    if (EEPROM_READ_Interruttore_stato_rele(0) == 1) ESP01_SERIAL_RELE_A_ON();
    if (EEPROM_READ_Interruttore_stato_rele(0) == 0) ESP01_SERIAL_RELE_A_OFF();
#endif
  }
}

void ACK_Interruttore() {
  for (int i = 0; i < Interruttore_nodi; i++) {
    DynamicJsonDocument JSONdoc(1024);
    JSONdoc["MAC_address"] = macToStr(mac);
    JSONdoc["NODO"] = Interruttore_DEF;
    JSONdoc["Topic"] = Interruttore[i].topic;
    JSONdoc["Topic_ACK"] = Interruttore[i].topic_ACK;
    JSONdoc["availability_topic"] = WillTopicStr;
    String payload = "";
    serializeJson(JSONdoc, payload);
    MQTTPublish(ACK_Topic, payload);
    Stato_INT(i);
  }
}

void HA_discovery_Interruttore() {
  String HAtopic;
  String HApayload;
  for (int i = 0; i < Interruttore_nodi; i++) {
    Debug_MSG_LN("Creo nodo " + (String)Interruttore[i].topic + " Home assistant discovery ");
    HAtopic = "homeassistant/switch/";
    HAtopic += macToStr(mac);
    HAtopic += "/";
    HAtopic += Interruttore_DEF;
    HAtopic += i;
    HAtopic += "/config";
    DynamicJsonDocument JSONdoc(1024);
    JSONdoc["name"] = Interruttore[i].topic;
    JSONdoc["command_topic"] = Interruttore[i].topic;
    JSONdoc["payload_on"] = "on";
    JSONdoc["payload_off"] = "off";
    JSONdoc["state_topic"] = Interruttore[i].topic_ACK;
    JSONdoc["state_on"] = "on";
    JSONdoc["state_off"] = "off";
    JSONdoc["value_template"] = "{{value_json.STATO}}";
    JSONdoc["availability_topic"] = WillTopicStr;
    String HApayload = "";
    serializeJson(JSONdoc, HApayload);
    MQTTPublish(HAtopic, HApayload);
  }
}

void Stato_INT(int i) {
  DynamicJsonDocument JSONdoc(1024);
  JSONdoc["NODO"] = Interruttore_DEF;
  if (Interruttore[i].stato == Interruttore[i].rele) {
    if (digitalRead(Interruttore[i].stato) ^ (Flag_inversione_RELE == true )) JSONdoc["STATO"] = Interruttore_ACK_ON;
    if (digitalRead(Interruttore[i].stato) ^ (Flag_inversione_RELE == false )) JSONdoc["STATO"] = Interruttore_ACK_OFF;
  }
  else {
    if (digitalRead(Interruttore[i].stato)) {
#ifdef PullUp
      JSONdoc["STATO"] = Interruttore_ACK_OFF;
#endif
#ifdef PullDown
      JSONdoc["STATO"] = Interruttore_ACK_ON;
#endif
    }
    else  {
#ifdef PullUp
      JSONdoc["STATO"] = Interruttore_ACK_ON;
#endif
#ifdef PullDown
      JSONdoc["STATO"] = Interruttore_ACK_OFF;
#endif
    }
  }
  String payload = "";
  serializeJson(JSONdoc, payload);
  MQTTPublish(Interruttore[i].topic_ACK, payload);

  //  handle_IndexRedir();

}

void Subscribe_Interruttore() {
  for (int i = 0; i < Interruttore_nodi; i++) {
    MQTTSubscribe(Interruttore[i].topic);
    Debug_MSG_LN("MQTT subscribe " + Interruttore[i].topic);
  }
}

void callback_Interruttore(char* topic, char* message) {
  String payload;
  for (int i = 0; i < Interruttore_nodi; i++) {
    if (String(topic) == Interruttore[i].topic) {               // se arriva il comando sul topic "Interruttore1_Topic"
      if ((String)message == Interruttore_ON) {                                                   // Topic "Interruttore1_Topic" = "on"
        digitalWrite(Interruttore[i].rele, 1 ^ Flag_inversione_RELE);
#ifdef SONOFFDUAL
        if (i = 0) SONOFFDUAL_A_ON();
        if (i = 1) SONOFFDUAL_B_ON();
#endif
#ifdef ESP01_SERIAL_RELE
        ESP01_SERIAL_RELE_A_ON();
#endif
      }
      if ((String)message == Interruttore_OFF) {                                                     // Topic "Interruttore1_Topic" = "off"
        digitalWrite(Interruttore[i].rele, 0 ^ Flag_inversione_RELE);
#ifdef SONOFFDUAL
        if (i = 0) SONOFFDUAL_A_OFF();
        if (i = 1) SONOFFDUAL_B_OFF();
#endif
#ifdef ESP01_SERIAL_RELE
        ESP01_SERIAL_RELE_A_OFF();
#endif
      }
      if ((String)message == Interruttore_TOGGLE) {                                                    // Topic "Interruttore1_Topic" = "toggle"
        digitalWrite(Interruttore[i].rele, not(digitalRead(Interruttore[i].rele)));
#ifdef SONOFFDUAL
        if (digitalRead(Interruttore[0].rele)) SONOFFDUAL_A_ON();
        else SONOFFDUAL_A_OFF();
        if (digitalRead(Interruttore[1].rele)) SONOFFDUAL_B_ON();
        else SONOFFDUAL_B_OFF();
#endif
#ifdef ESP01_SERIAL_RELE
        if (digitalRead(Interruttore[0].rele)) ESP01_SERIAL_RELE_A_ON();
        else ESP01_SERIAL_RELE_A_OFF();
#endif
      }
      if ((String)message == Interruttore_STATO) {                                                     // Topic "Interruttore1_Topic" = "stato"
        Stato_INT(i);
        return;
      }
      EEPROM_WRITE_Interruttore_stato_rele(i);
      Stato_INT(i);
    }
  }
}

void loop_Interruttore() {
  String payload;
  for (int i = 0; i < Interruttore_nodi; i++) {
    if (Interruttore_TPush_bottone[i].Click(Interruttore_TEMPO_CLICK_ON) and Interruttore[i].bottone != 100) {                                // Bottone premuto! TEMPO_CLICK_ON
      Debug_MSG_LN("Bottone Interruttore " + String(i + 1) + " premuto!");
      digitalWrite(Interruttore[i].rele, not(digitalRead(Interruttore[i].rele)));
#ifdef SONOFFDUAL
      if (digitalRead(Interruttore[0].rele)) SONOFFDUAL_A_ON();
      else SONOFFDUAL_A_OFF();
      if (digitalRead(Interruttore[1].rele)) SONOFFDUAL_B_ON();
      else SONOFFDUAL_B_OFF();
#endif
#ifdef ESP01_SERIAL_RELE
      if (digitalRead(Interruttore[0].rele)) ESP01_SERIAL_RELE_A_ON();
      else ESP01_SERIAL_RELE_A_OFF();
#endif
      EEPROM_WRITE_Interruttore_stato_rele(i);
      Stato_INT(i);
    }
    if (t = Interruttore_TPush_bottone[i].Keep(Interruttore_TEMPO_CLICK_OFF)) {                            // Bottone rilasciato! TEMPO_CLICK_OFF
      Debug_MSG_LN("Bottone Interruttore " + String(i + 1) + " rilasciato!");
      if (Interruttore[i].tipo_bottone == "monostabile") {
        digitalWrite(Interruttore[i].rele, not(digitalRead(Interruttore[i].rele)));
#ifdef SONOFFDUAL
        if (digitalRead(Interruttore[0].rele)) SONOFFDUAL_A_ON();
        else SONOFFDUAL_A_OFF();
        if (digitalRead(Interruttore[1].rele)) SONOFFDUAL_B_ON();
        else SONOFFDUAL_B_OFF();
#endif
#ifdef ESP01_SERIAL_RELE
        if (digitalRead(Interruttore[0].rele)) ESP01_SERIAL_RELE_A_ON();
        else ESP01_SERIAL_RELE_A_OFF();
#endif
      }
      EEPROM_WRITE_Interruttore_stato_rele(i);
      Stato_INT(i);
    }
    if (Interruttore_TPush_stato[i].Click(Interruttore_TEMPO_CLICK_ON)) {
      Debug_MSG_LN("Stato Interruttore " + String(i + 1) + " ON");
      Stato_INT(i);
    }
    if (t = Interruttore_TPush_stato[i].Keep(Interruttore_TEMPO_CLICK_OFF)) {
      Debug_MSG_LN("Stato Interruttore " + String(i + 1) + " OFF");
      Stato_INT(i);
    }
  }
}



int EEPROM_READ_Interruttore_stato_rele(int i) {                                              // Leggo dalla EEPROM RELE Interruttore
  Debug_MSG("Reading EEPROM Interruttore stato rele " + String(i + 1) + " : ");
  int tmp;
  tmp = eepromRead(EEPROM_Interruttore_stato_rele + i);
  if (tmp != 0 and tmp != 1) tmp = 0;
  if (tmp == 1) Debug_MSG_LN("ON");
  if (tmp == 0) Debug_MSG_LN("OFF");
  return tmp;
}

void EEPROM_WRITE_Interruttore_stato_rele(int i) {
  Debug_MSG("Writing EEPROM Interruttore stato rele " + String(i + 1) + " : ");
  if (digitalRead(Interruttore[i].rele) ^ (Flag_inversione_RELE == true )) {
    int tmp = 1;
    eepromWrite(EEPROM_Interruttore_stato_rele + i, tmp );
    Debug_MSG_LN("ON");
  }
  if (digitalRead(Interruttore[i].rele) ^ (Flag_inversione_RELE == false)) {
    int tmp = 0;
    eepromWriteInt(EEPROM_Interruttore_stato_rele + (i  * 2),  tmp );
    Debug_MSG_LN("OFF");
  }
  EEPROM.commit();
}
#endif
