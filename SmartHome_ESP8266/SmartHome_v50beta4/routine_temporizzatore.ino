TPush Temporizzatore_TPush_bottone[Temporizzatore_nodi];

long EEPROM_Temporizzatore_tempo = 300;
unsigned long Temporizzatore_tempo_default = 10000;

String Temporizzatore_DEF     = "TEM";
String Temporizzatore_ON      = "on";
String Temporizzatore_STATO   = "stato";
String Temporizzatore_ACK_ON  = "on";
String Temporizzatore_ACK_OFF = "off";
String Temporizzatore_Topic_ACK_suffisso = "/ack";

typedef struct {
  String topic;
  String topic_ACK;
  int rele;
  int bottone;
  unsigned long ulTime;
  boolean rele_eccitato;
  unsigned long tempo;
} InfoRecordTemporizzatoreType ;

InfoRecordTemporizzatoreType Temporizzatore[10];

void setup_Temporizzatore() {
  Temporizzatore[0].topic = Temporizzatore1_Topic;
  Temporizzatore[0].topic_ACK = Temporizzatore1_Topic;
  Temporizzatore[0].topic_ACK += Temporizzatore_Topic_ACK_suffisso;
  Temporizzatore[0].rele = Temporizzatore1_GPIO_rele;
#ifdef Temporizzatore1_GPIO_bottone
  Temporizzatore[0].bottone = Temporizzatore1_GPIO_bottone;
#else
  Temporizzatore[0].bottone = 100;
#endif
  Temporizzatore[0].rele_eccitato = false;

  Temporizzatore[1].topic = Temporizzatore2_Topic;
  Temporizzatore[1].topic_ACK = Temporizzatore2_Topic;
  Temporizzatore[1].topic_ACK += Temporizzatore_Topic_ACK_suffisso;
  Temporizzatore[1].rele = Temporizzatore2_GPIO_rele;
#ifdef Temporizzatore2_GPIO_bottone
  Temporizzatore[1].bottone = Temporizzatore2_GPIO_bottone;
#else
  Temporizzatore[1].bottone = 100;
#endif
  Temporizzatore[1].rele_eccitato = false;

  Temporizzatore[2].topic = Temporizzatore3_Topic;
  Temporizzatore[2].topic_ACK = Temporizzatore3_Topic;
  Temporizzatore[2].topic_ACK += Temporizzatore_Topic_ACK_suffisso;
  Temporizzatore[2].rele = Temporizzatore3_GPIO_rele;
#ifdef Temporizzatore3_GPIO_bottone
  Temporizzatore[2].bottone = Temporizzatore3_GPIO_bottone;
#else
  Temporizzatore[2].bottone = 100;
#endif
  Temporizzatore[2].rele_eccitato = false;

  Temporizzatore[3].topic = Temporizzatore4_Topic;
  Temporizzatore[3].topic_ACK = Temporizzatore4_Topic;
  Temporizzatore[3].topic_ACK += Temporizzatore_Topic_ACK_suffisso;
  Temporizzatore[3].rele = Temporizzatore4_GPIO_rele;
#ifdef Temporizzatore4_GPIO_bottone
  Temporizzatore[3].bottone = Temporizzatore4_GPIO_bottone;
#else
  Temporizzatore[3].bottone = 100;
#endif
  Temporizzatore[3].rele_eccitato = false;

  Temporizzatore[4].topic = Temporizzatore5_Topic;
  Temporizzatore[4].topic_ACK = Temporizzatore5_Topic;
  Temporizzatore[4].topic_ACK += Temporizzatore_Topic_ACK_suffisso;
  Temporizzatore[4].rele = Temporizzatore5_GPIO_rele;
#ifdef Temporizzatore5_GPIO_bottone
  Temporizzatore[4].bottone = Temporizzatore5_GPIO_bottone;
#else
  Temporizzatore[4].bottone = 100;
#endif
  Temporizzatore[4].rele_eccitato = false;

  Temporizzatore[5].topic = Temporizzatore6_Topic;
  Temporizzatore[5].topic_ACK = Temporizzatore6_Topic;
  Temporizzatore[5].topic_ACK += Temporizzatore_Topic_ACK_suffisso;
  Temporizzatore[5].rele = Temporizzatore6_GPIO_rele;
#ifdef Temporizzatore6_GPIO_bottone
  Temporizzatore[5].bottone = Temporizzatore6_GPIO_bottone;
#else
  Temporizzatore[5].bottone = 100;
#endif
  Temporizzatore[5].rele_eccitato = false;

  Temporizzatore[6].topic = Temporizzatore7_Topic;
  Temporizzatore[6].topic_ACK = Temporizzatore7_Topic;
  Temporizzatore[6].topic_ACK += Temporizzatore_Topic_ACK_suffisso;
  Temporizzatore[6].rele = Temporizzatore7_GPIO_rele;
#ifdef Temporizzatore7_GPIO_bottone
  Temporizzatore[6].bottone = Temporizzatore7_GPIO_bottone;
#else
  Temporizzatore[6].bottone = 100;
#endif
  Temporizzatore[6].rele_eccitato = false;

  Temporizzatore[7].topic = Temporizzatore8_Topic;
  Temporizzatore[7].topic_ACK = Temporizzatore8_Topic;
  Temporizzatore[7].topic_ACK += Temporizzatore_Topic_ACK_suffisso;
  Temporizzatore[7].rele = Temporizzatore8_GPIO_rele;
#ifdef Temporizzatore8_GPIO_bottone
  Temporizzatore[7].bottone = Temporizzatore8_GPIO_bottone;
#else
  Temporizzatore[7].bottone = 100;
#endif
  Temporizzatore[7].rele_eccitato = false;

  Temporizzatore[8].topic = Temporizzatore9_Topic;
  Temporizzatore[8].topic_ACK = Temporizzatore9_Topic;
  Temporizzatore[8].topic_ACK += Temporizzatore_Topic_ACK_suffisso;
  Temporizzatore[8].rele = Temporizzatore9_GPIO_rele;
#ifdef Temporizzatore9_GPIO_bottone
  Temporizzatore[8].bottone = Temporizzatore9_GPIO_bottone;
#else
  Temporizzatore[8].bottone = 100;
#endif
  Temporizzatore[8].rele_eccitato = false;

  Temporizzatore[9].topic = Temporizzatore10_Topic;
  Temporizzatore[9].topic_ACK = Temporizzatore10_Topic;
  Temporizzatore[9].topic_ACK += Temporizzatore_Topic_ACK_suffisso;
  Temporizzatore[9].rele = Temporizzatore10_GPIO_rele;
#ifdef Temporizzatore10_GPIO_bottone
  Temporizzatore[9].bottone = Temporizzatore10_GPIO_bottone;
#else
  Temporizzatore[9].bottone = 100;
#endif
  Temporizzatore[9].rele_eccitato = false;

  for (int i = 0; i < Temporizzatore_nodi; i++) {
    Debug_MSG_LN("- Temporizzatore " + String(i + 1) + " : " + Temporizzatore[i].topic);
    Debug_MSG_LN("RELE Temporizzatore " + String(i + 1) + "        = GPIO" + Temporizzatore[i].rele);
    if (Temporizzatore[i].bottone >= 99) {
      Debug_MSG_LN("BOTTONE Temporizzatore " + String(i + 1) + " : Nessun GPIO definito!");
    }
    else {
      Debug_MSG_LN("BOTTONE Temporizzatore " + String(i + 1) + "     = GPIO" + Temporizzatore[i].bottone);
#if defined(PullDown)
      Temporizzatore_TPush_bottone[i].setUp(Temporizzatore[i].bottone, HIGH);
#endif
#if defined(PullUp)
      Temporizzatore_TPush_bottone[i].setUp(Temporizzatore[i].bottone, LOW);
#endif
    }
    pinMode(Temporizzatore[i].rele, OUTPUT);
    digitalWrite(Temporizzatore[i].rele, 0 ^ Flag_inversione_RELE);
    EEPROM_READ_Temporizzatore_tempo(i);
  }
}

void ACK_Temporizzatore() {
  String payload;
  for (int i = 0; i < Temporizzatore_nodi; i++) {
    payload = "{\"MAC_address\":\"";
    payload += macToStr(mac);
    payload += "\",\"NODO\":\"";
    payload += Temporizzatore_DEF;
    payload += "\",\"Topic\":\"";
    payload += Temporizzatore[i].topic;
    payload += "\",\"Topic_ACK\":\"";
    payload += Temporizzatore[i].topic_ACK;
    payload += "\",\"availability_topic\":\"";
    payload += WillTopicStr;
    payload += "\"}";
    client.publish(ACK_Topic, (char*) payload.c_str());
    delay(500);
    Stato_TEM(i);
  }
}

void Alive_Temporizzatore() {
  String HAtopic;
  String HApayload;

#if defined(HomeAssistant_discovery)
  for (int i = 0; i < Temporizzatore_nodi; i++) {
    HAtopic = "homeassistant/switch/";
    HAtopic += macToStr(mac);
    HAtopic += "/";
    HAtopic += Temporizzatore_DEF;
    HAtopic += i;
    HAtopic += "/config";
    HApayload = "{\"name\":\"";
    HApayload += Temporizzatore[i].topic;
    HApayload += "\",\"command_topic\":\"";
    HApayload += Temporizzatore[i].topic;
    HApayload += "\",\"payload_on\":\"";
    HApayload += "on";
    HApayload += "\",\"payload_off\":\"";
    HApayload += "off";
    HApayload += "\",\"state_topic\":\"";
    HApayload += Temporizzatore[i].topic_ACK;
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

void Stato_TEM(int i) {
  String payload;
  payload = "{\"NODO\":\"";
  payload += Temporizzatore_DEF;
  payload += "\",\"STATO\":\"";
  if (digitalRead(Temporizzatore[i].rele) ^ Flag_inversione_RELE == true ) payload += Temporizzatore_ACK_ON;
  if (digitalRead(Temporizzatore[i].rele) ^ Flag_inversione_RELE == false ) payload += Temporizzatore_ACK_OFF;
  payload += "\",\"TEMPO\":\"";
  payload += Temporizzatore[i].tempo;
  payload += "\"}";
  client.publish((char*) Temporizzatore[i].topic_ACK.c_str(), (char*) payload.c_str());
  delay(250);
}

void Subscribe_Temporizzatore() {
  String payload;
  for (int i = 0; i < Temporizzatore_nodi; i++) {
    client.subscribe((char*) Temporizzatore[i].topic.c_str());                  // Sottoscrivi Temporizzatore1_Topic
    delay(250);
    Debug_MSG_LN("MQTT subscribe " + String(Temporizzatore[i].topic));
  }
}

void callback_Temporizzatore(char* topic, byte * message, unsigned int length) {
  String payload;
  String message_String;
  for (int i = 0; i < Temporizzatore_nodi; i++) {
    if (String(topic) == Temporizzatore[i].topic) {                                              // se arriva il comando sul topic "Temporizzatore1_Topic"
      message_String = "";
      for (int ii = 0; ii < length; ii++) {
        message_String += (char)message[ii];
      }
      if (message_String == Temporizzatore_ON) {                  // Topic "Temporizzatore1_Topic" = "on"
        digitalWrite(Temporizzatore[i].rele, 1 ^ Flag_inversione_RELE);                                     // Accendo il rele

#if defined(SONOFFDUAL)
        if (i = 0) SONOFFDUAL_A_ON();
        if (i = 1) SONOFFDUAL_B_ON();
#endif
#if defined(ESP01_SERIAL_RELE)
        ESP01_SERIAL_RELE_A_ON();
#endif
        Temporizzatore[i].rele_eccitato = true;
        Temporizzatore[i].ulTime = millis();
        Stato_TEM(i);
      }

      if (message_String == Temporizzatore_STATO) {                                      // Topic "Temporizzatore_Topic" = "stato"
        Stato_TEM(i);
      }
      if (((char)message[0] == 'T' or (char)message[0] == 't') &
          (char)message[1] == '=' ) {                                                        // Topic "Temporizzatore_Topic" = "T=" o "t="
        String stringtmp = "";
        for (int i = 2; i < length; i++)  {
          stringtmp = stringtmp + (char)message[i];
        }
        char buf[stringtmp.length()];
        stringtmp.toCharArray(buf, stringtmp.length() + 1);
        Temporizzatore[i].tempo = atol(buf);
        if (Temporizzatore[i].tempo < 1) {
          Temporizzatore[i].tempo = 1;
        }
        if (Temporizzatore[i].tempo > 3600) {
          Temporizzatore[i].tempo = 3600;
        }
        Temporizzatore[i].tempo = Temporizzatore[i].tempo * 1000;
        EEPROM_WRITE_Temporizzatore_tempo(i);
        Stato_TEM(i);
      }
    }
  }
}

void loop_Temporizzatore() {
  String payload;
  for (int i = 0; i < Temporizzatore_nodi; i++) {
    if (((millis() - Temporizzatore[i].ulTime) > Temporizzatore[i].tempo) & Temporizzatore[i].rele_eccitato) {        // Tempo rele -> spengo!
      Debug_MSG_LN("Tempo rele -> spengo!");
      digitalWrite(Temporizzatore[i].rele, 0 ^ Flag_inversione_RELE);                     // Spengo il rele
#if defined(SONOFFDUAL)
      if (i = 0) SONOFFDUAL_A_OFF();
      if (i = 1) SONOFFDUAL_B_OFF();
#endif
#if defined(ESP01_SERIAL_RELE)
      ESP01_SERIAL_RELE_A_OFF();
#endif
      Temporizzatore[i].rele_eccitato = false;
      Stato_TEM(i);
    }
    if (Temporizzatore_TPush_bottone[i].Click(Temporizzatore_TEMPO_CLICK_ON)) {                                // Bottone premuto!
      Debug_MSG_LN("Bottone premuto!");
      digitalWrite(Temporizzatore[i].rele, 1 ^ Flag_inversione_RELE);
#if defined(SONOFFDUAL)
      if (i = 0) SONOFFDUAL_A_ON();
      if (i = 1) SONOFFDUAL_B_ON();
#endif
#if defined(ESP01_SERIAL_RELE)
      ESP01_SERIAL_RELE_A_ON();
#endif
      Temporizzatore[i].rele_eccitato = true;
      Temporizzatore[i].ulTime = millis();
      Stato_TEM(i);
    }
  }
}

void EEPROM_READ_Temporizzatore_tempo(int i) {
  Debug_MSG("Reading EEPROM Temporizzatore tempo" + String(i + 1) + " : ");
  Temporizzatore[i].tempo = EEPROMReadlong(EEPROM_Temporizzatore_tempo + (i * 4));
  Debug_MSG_LN((String)Temporizzatore[i].tempo);
  if (Temporizzatore[i].tempo < 1) {
    Temporizzatore[i].tempo = Temporizzatore_tempo_default;
    EEPROM_WRITE_Temporizzatore_tempo(i);
  }
  if (Temporizzatore[i].tempo > 3600) {
    Temporizzatore[i].tempo = Temporizzatore_tempo_default;
    EEPROM_WRITE_Temporizzatore_tempo(i);
  }
}

void EEPROM_WRITE_Temporizzatore_tempo(int i) {
  Debug_MSG("Writing EEPROM Temporizzatore tempo" + String(i + 1) + " : ");
  Debug_MSG_LN((String) Temporizzatore[i].tempo);
  EEPROMWritelong(EEPROM_Temporizzatore_tempo + (i * 4), Temporizzatore[i].tempo);
  EEPROM.commit();
}
