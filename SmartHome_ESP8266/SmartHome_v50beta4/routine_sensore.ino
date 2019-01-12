TPush Sensore_TPush_sensore[Sensore_nodi];

String Sensore_DEF     = "SEN";
String Sensore_STATO   = "stato";
String Sensore_ACK_ON  = "on";
String Sensore_ACK_OFF = "off";
String Sensore_Topic_ACK_suffisso = "/ack";

typedef struct {
  String topic;
  String topic_ACK;
  String topic_risposta;
  String payload_risposta_ON;
  String payload_risposta_OFF;
  int sensore;
  String stato;
} InfoRecordSensoreType ;

InfoRecordSensoreType Sensore[10];

void setup_Sensore() {
  Sensore[0].topic = Sensore1_Topic;
  Sensore[0].topic_ACK = Sensore1_Topic;
  Sensore[0].topic_ACK += Sensore_Topic_ACK_suffisso;
  Sensore[0].sensore = Sensore1_GPIO_sensore;
  Sensore[0].stato = Sensore_ACK_OFF;
  Sensore[0].topic_risposta = Sensore1_Topic_risposta;
  Sensore[0].payload_risposta_ON = Sensore1_ON_risposta;
  Sensore[0].payload_risposta_OFF = Sensore1_OFF_risposta;

  Sensore[1].topic = Sensore2_Topic;
  Sensore[1].topic_ACK = Sensore2_Topic;
  Sensore[1].topic_ACK += Sensore_Topic_ACK_suffisso;
  Sensore[1].sensore = Sensore2_GPIO_sensore;
  Sensore[1].stato = Sensore_ACK_OFF;
  Sensore[1].topic_risposta = Sensore2_Topic_risposta;
  Sensore[1].payload_risposta_ON = Sensore2_ON_risposta;
  Sensore[1].payload_risposta_OFF = Sensore2_OFF_risposta;

  Sensore[2].topic = Sensore3_Topic;
  Sensore[2].topic_ACK = Sensore3_Topic;
  Sensore[2].topic_ACK += Sensore_Topic_ACK_suffisso;
  Sensore[2].sensore = Sensore3_GPIO_sensore;
  Sensore[2].stato = Sensore_ACK_OFF;
  Sensore[2].topic_risposta = Sensore3_Topic_risposta;
  Sensore[2].payload_risposta_ON = Sensore3_ON_risposta;
  Sensore[2].payload_risposta_OFF = Sensore3_OFF_risposta;

  Sensore[3].topic = Sensore4_Topic;
  Sensore[3].topic_ACK = Sensore4_Topic;
  Sensore[3].topic_ACK += Sensore_Topic_ACK_suffisso;
  Sensore[3].sensore = Sensore4_GPIO_sensore;
  Sensore[3].stato = Sensore_ACK_OFF;
  Sensore[3].topic_risposta = Sensore4_Topic_risposta;
  Sensore[3].payload_risposta_ON = Sensore4_ON_risposta;
  Sensore[3].payload_risposta_OFF = Sensore4_OFF_risposta;

  Sensore[4].topic = Sensore5_Topic;
  Sensore[4].topic_ACK = Sensore5_Topic;
  Sensore[4].topic_ACK += Sensore_Topic_ACK_suffisso;
  Sensore[4].sensore = Sensore5_GPIO_sensore;
  Sensore[4].stato = Sensore_ACK_OFF;
  Sensore[4].topic_risposta = Sensore5_Topic_risposta;
  Sensore[4].payload_risposta_ON = Sensore5_ON_risposta;
  Sensore[4].payload_risposta_OFF = Sensore5_OFF_risposta;

  Sensore[5].topic = Sensore6_Topic;
  Sensore[5].topic_ACK = Sensore6_Topic;
  Sensore[5].topic_ACK += Sensore_Topic_ACK_suffisso;
  Sensore[5].sensore = Sensore6_GPIO_sensore;
  Sensore[5].stato = Sensore_ACK_OFF;
  Sensore[5].topic_risposta = Sensore6_Topic_risposta;
  Sensore[5].payload_risposta_ON = Sensore6_ON_risposta;
  Sensore[5].payload_risposta_OFF = Sensore6_OFF_risposta;

  Sensore[6].topic = Sensore7_Topic;
  Sensore[6].topic_ACK = Sensore7_Topic;
  Sensore[6].topic_ACK += Sensore_Topic_ACK_suffisso;
  Sensore[6].sensore = Sensore7_GPIO_sensore;
  Sensore[6].stato = Sensore_ACK_OFF;
  Sensore[6].topic_risposta = Sensore7_Topic_risposta;
  Sensore[6].payload_risposta_ON = Sensore7_ON_risposta;
  Sensore[6].payload_risposta_OFF = Sensore7_OFF_risposta;

  Sensore[7].topic = Sensore8_Topic;
  Sensore[7].topic_ACK = Sensore8_Topic;
  Sensore[7].topic_ACK += Sensore_Topic_ACK_suffisso;
  Sensore[7].sensore = Sensore8_GPIO_sensore;
  Sensore[7].stato = Sensore_ACK_OFF;
  Sensore[7].topic_risposta = Sensore8_Topic_risposta;
  Sensore[7].payload_risposta_ON = Sensore8_ON_risposta;
  Sensore[7].payload_risposta_OFF = Sensore8_OFF_risposta;

  Sensore[8].topic = Sensore9_Topic;
  Sensore[8].topic_ACK = Sensore9_Topic;
  Sensore[8].topic_ACK += Sensore_Topic_ACK_suffisso;
  Sensore[8].sensore = Sensore9_GPIO_sensore;
  Sensore[8].stato = Sensore_ACK_OFF;
  Sensore[8].topic_risposta = Sensore9_Topic_risposta;
  Sensore[8].payload_risposta_ON = Sensore9_ON_risposta;
  Sensore[8].payload_risposta_OFF = Sensore9_OFF_risposta;

  Sensore[9].topic = Sensore10_Topic;
  Sensore[9].topic_ACK = Sensore10_Topic;
  Sensore[9].topic_ACK += Sensore_Topic_ACK_suffisso;
  Sensore[9].sensore = Sensore10_GPIO_sensore;
  Sensore[9].stato = Sensore_ACK_OFF;
  Sensore[9].topic_risposta = Sensore10_Topic_risposta;
  Sensore[9].payload_risposta_ON = Sensore10_ON_risposta;
  Sensore[9].payload_risposta_OFF = Sensore10_OFF_risposta;

  for (int i = 0; i < Sensore_nodi; i++) {
    Debug_MSG_LN("- Sensore " + String(i + 1) + " : " + Sensore[i].topic);
    Debug_MSG_LN("Sensore " + String(i + 1) + " SU  = GPIO" + String(Sensore[i].sensore));

#if defined(PullDown)
    Sensore_TPush_sensore[i].setUp(Sensore[i].sensore, HIGH);
#endif
#if defined(PullUp)
    Sensore_TPush_sensore[i].setUp(Sensore[i].sensore, LOW);
#endif
  }
}

void ACK_Sensore() {
  String payload;
  for (int i = 0; i < Sensore_nodi; i++) {
    payload = "{\"MAC_address\":\"";
    payload += macToStr(mac);
    payload += "\",\"NODO\":\"";
    payload += Sensore_DEF;
    payload += "\",\"Topic\":\"";
    payload += Sensore[i].topic;
    payload += "\",\"Topic_ACK\":\"";
    payload += Sensore[i].topic_ACK;
    payload += "\",\"availability_topic\":\"";
    payload += WillTopicStr;
    payload += "\"}";
    client.publish(ACK_Topic, (char*) payload.c_str());
    delay(500);
    Stato_SEN(i);
  }
}

void Alive_Sensore() {
}

void Stato_SEN(int i) {
  String payload;
  payload = "{\"NODO\":\"";
  payload += Sensore_DEF;
  payload += "\",\"STATO\":\"";
  payload += Sensore[i].stato;
  payload += "\"}";
  client.publish((char*) Sensore[i].topic_ACK.c_str(), (char*) payload.c_str());
  delay(250);

  if (Sensore[i].topic_risposta == "") return;
  if (Sensore[i].topic_risposta == "" and Sensore[i].payload_risposta_ON == "") return;
  if (Sensore[i].topic_risposta == "" and Sensore[i].payload_risposta_OFF == "") return;
  if (Sensore[i].stato == Sensore_ACK_ON) payload = Sensore[i].payload_risposta_ON;
  if (Sensore[i].stato == Sensore_ACK_OFF) payload = Sensore[i].payload_risposta_OFF;
  client.publish((char*) Sensore[i].topic_risposta.c_str(), (char*) payload.c_str());
  delay(250);

}

void Subscribe_Sensore() {
  for (int i = 0; i < Sensore_nodi; i++) {
    client.subscribe((char*) Sensore[i].topic.c_str());
    delay(250);
    Debug_MSG_LN("MQTT subscribe " + Sensore[i].topic);
  }
}

void callback_Sensore(char* topic, byte * message, unsigned int length) {
  String payload;
  String message_String;
  for (int i = 0; i < Sensore_nodi; i++) {
    if (String(topic) == Sensore[i].topic) {               // se arriva il comando sul topic "Sensore1_Topic"
      message_String = "";
      for (int ii = 0; ii < length; ii++) {
        message_String += (char)message[ii];
      }
      if (message_String == Sensore_STATO) {                                                     // Topic "Sensore1_Topic" = "stato"
        Stato_SEN(i);
      }
    }
  }
}



void loop_Sensore() {
  String payload;
  for (int i = 0; i < Sensore_nodi; i++) {
    if (Sensore_TPush_sensore[i].Click(Sensore_TEMPO_CLICK_ON)) {
      Debug_MSG_LN("Sensore ON!");
      Sensore[i].stato = Sensore_ACK_ON;
      Stato_SEN(i);
    }
    if (t = Sensore_TPush_sensore[i].Keep(Sensore_TEMPO_CLICK_OFF)) {
      Debug_MSG_LN("Sensore OFF!");
      Sensore[i].stato = Sensore_ACK_OFF;
      Stato_SEN(i);
    }
  }
}
