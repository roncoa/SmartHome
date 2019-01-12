#define Tapparella_TEMPO_STATO          5000

TPush Tapparella_TPush_bottone_SU[Tapparella_nodi], Tapparella_TPush_bottone_GIU[Tapparella_nodi];
TPush Tapparella_timer_STATO;

long EEPROM_Tapparella_tempo_max = 0;
long EEPROM_Tapparella_lock = 40;
long Tapparella_percentuale;
long Tapparella_tempo_max_default = 25000;       // Tempo tapparella di default (in millisecondi)

String Tapparella_DEF     = "TAP";
String Tapparella_SU      = "su";
String Tapparella_GIU     = "giu";
String Tapparella_STOP    = "stop";
String Tapparella_STATO   = "stato";
String Tapparella_ACK_SU  = "su";
String Tapparella_ACK_GIU = "giu";
String Tapparella_ACK_STOP = "stop";
String Tapparella_Topic_ACK_suffisso = "/ack";

typedef struct {
  String topic;
  String topic_ACK;
  int rele_SU;
  int rele_GIU;
  int bottone_SU;
  int bottone_GIU;
  boolean in_movimento;
  unsigned long ulTime;
  long tempo_max;
  int lock;
} InfoRecordTapparellaType ;

InfoRecordTapparellaType Tapparella[5];

void setup_Tapparella() {

  Tapparella[0].topic = Tapparella1_Topic;
  Tapparella[0].topic_ACK = Tapparella1_Topic;
  Tapparella[0].topic_ACK += Tapparella_Topic_ACK_suffisso;
  Tapparella[0].rele_SU = Tapparella1_GPIO_rele_SU;
  Tapparella[0].rele_GIU = Tapparella1_GPIO_rele_GIU;
#ifdef Tapparella1_GPIO_bottone_SU
  Tapparella[0].bottone_SU = Tapparella1_GPIO_bottone_SU;
#else
  Tapparella[0].bottone_SU = 100;
#endif
#ifdef Tapparella1_GPIO_bottone_GIU
  Tapparella[0].bottone_GIU = Tapparella1_GPIO_bottone_GIU;
#else
  Tapparella[0].bottone_GIU = 100;
#endif
  Tapparella[0].in_movimento = false;
  Tapparella[0].lock = false;

  Tapparella[1].topic = Tapparella2_Topic;
  Tapparella[1].topic_ACK = Tapparella2_Topic;
  Tapparella[1].topic_ACK += Tapparella_Topic_ACK_suffisso;
  Tapparella[1].rele_SU = Tapparella2_GPIO_rele_SU;
  Tapparella[1].rele_GIU = Tapparella2_GPIO_rele_GIU;
#ifdef Tapparella2_GPIO_bottone_SU
  Tapparella[1].bottone_SU = Tapparella2_GPIO_bottone_SU;
#else
  Tapparella[1].bottone_SU = 100;
#endif
#ifdef Tapparella2_GPIO_bottone_GIU
  Tapparella[1].bottone_GIU = Tapparella2_GPIO_bottone_GIU;
#else
  Tapparella[1].bottone_GIU = 100;
#endif
  Tapparella[1].in_movimento = false;
  Tapparella[1].lock = false;

  Tapparella[2].topic = Tapparella3_Topic;
  Tapparella[2].topic_ACK = Tapparella3_Topic;
  Tapparella[2].topic_ACK += Tapparella_Topic_ACK_suffisso;
  Tapparella[2].rele_SU = Tapparella3_GPIO_rele_SU;
  Tapparella[2].rele_GIU = Tapparella3_GPIO_rele_GIU;
#ifdef Tapparella3_GPIO_bottone_SU
  Tapparella[2].bottone_SU = Tapparella3_GPIO_bottone_SU;
#else
  Tapparella[2].bottone_SU = 100;
#endif
#ifdef Tapparella3_GPIO_bottone_GIU
  Tapparella[2].bottone_GIU = Tapparella3_GPIO_bottone_GIU;
#else
  Tapparella[2].bottone_GIU = 100;
#endif
  Tapparella[2].in_movimento = false;
  Tapparella[2].lock = false;

  Tapparella[3].topic = Tapparella4_Topic;
  Tapparella[3].topic_ACK = Tapparella4_Topic;
  Tapparella[3].topic_ACK += Tapparella_Topic_ACK_suffisso;
  Tapparella[3].rele_SU = Tapparella4_GPIO_rele_SU;
  Tapparella[3].rele_GIU = Tapparella4_GPIO_rele_GIU;
#ifdef Tapparella4_GPIO_bottone_SU
  Tapparella[3].bottone_SU = Tapparella4_GPIO_bottone_SU;
#else
  Tapparella[3].bottone_SU = 100;
#endif
#ifdef Tapparella4_GPIO_bottone_GIU
  Tapparella[3].bottone_GIU = Tapparella4_GPIO_bottone_GIU;
#else
  Tapparella[3].bottone_GIU = 100;
#endif
  Tapparella[3].in_movimento = false;
  Tapparella[3].lock = false;

  Tapparella[4].topic = Tapparella5_Topic;
  Tapparella[4].topic_ACK = Tapparella5_Topic;
  Tapparella[4].topic_ACK += Tapparella_Topic_ACK_suffisso;
  Tapparella[4].rele_SU = Tapparella5_GPIO_rele_SU;
  Tapparella[4].rele_GIU = Tapparella5_GPIO_rele_GIU;
#ifdef Tapparella5_GPIO_bottone_SU
  Tapparella[4].bottone_SU = Tapparella5_GPIO_bottone_SU;
#else
  Tapparella[4].bottone_SU = 100;
#endif
#ifdef Tapparella5_GPIO_bottone_GIU
  Tapparella[4].bottone_GIU = Tapparella5_GPIO_bottone_GIU;
#else
  Tapparella[4].bottone_GIU = 100;
#endif
  Tapparella[4].in_movimento = false;
  Tapparella[4].lock = false;


  for (int i = 0; i < Tapparella_nodi; i++) {
    Debug_MSG_LN("- Tapparella " + String(i + 1) + " : " + Tapparella[i].topic);
    Debug_MSG_LN("RELE    tapparella " + String(i + 1) + " SU  = GPIO" + String(Tapparella[i].rele_SU));
    Debug_MSG_LN("RELE    tapparella " + String(i + 1) + " GIU = GPIO" + String(Tapparella[i].rele_GIU));
    pinMode(Tapparella[i].rele_SU, OUTPUT);
    digitalWrite(Tapparella[i].rele_SU, 0 ^ Flag_inversione_RELE);
    pinMode(Tapparella[i].rele_GIU, OUTPUT);
    digitalWrite(Tapparella[i].rele_GIU, 0 ^ Flag_inversione_RELE);
    if (Tapparella[i].bottone_SU >= 99) {
      Debug_MSG_LN("BOTTONE tapparella " + String(i + 1) + " SU  = Nessun GPIO definito!");
    }
    else {
      Debug_MSG_LN("BOTTONE tapparella " + String(i + 1) + " SU  = GPIO" + String(Tapparella[i].bottone_SU));
#if defined(PullDown)
      Tapparella_TPush_bottone_SU[i].setUp(Tapparella[i].bottone_SU, HIGH);
#endif
#if defined(PullUp)
      Tapparella_TPush_bottone_SU[i].setUp(Tapparella[i].bottone_SU, LOW);
#endif
    }
    if (Tapparella[i].bottone_GIU >= 99) {
      Debug_MSG_LN("BOTTONE tapparella " + String(i + 1) + " GIU = Nessun GPIO definito!");
    }
    else {
      Debug_MSG_LN("BOTTONE tapparella " + String(i + 1) + " GIU = GPIO" + String(Tapparella[i].bottone_GIU));
#if defined(PullDown)
      Tapparella_TPush_bottone_GIU[i].setUp(Tapparella[i].bottone_GIU, HIGH);
#endif
#if defined(PullUp)
      Tapparella_TPush_bottone_GIU[i].setUp(Tapparella[i].bottone_GIU, LOW);
#endif
    }
    EEPROM_READ_Tapparella_lock(i);
    EEPROM_READ_Tapparella_tempo_max(i);
  }
}

void ACK_Tapparella() {
  String payload;
  for (int i = 0; i < Tapparella_nodi; i++) {
    payload = "{\"MAC_address\":\"";
    payload += macToStr(mac);
    payload += "\",\"NODO\":\"";
    payload += Tapparella_DEF;
    payload += "\",\"Topic\":\"";
    payload += Tapparella[i].topic;
    payload += "\",\"Topic_ACK\":\"";
    payload += Tapparella[i].topic_ACK;
    payload += "\",\"availability_topic\":\"";
    payload += WillTopicStr;
    payload += "\"}";
    client.publish(ACK_Topic, (char*) payload.c_str());
    delay(250);
    Stato_TAP (i);
  }
}

void Alive_Tapparella() {
  String HAtopic;
  String HApayload;

#if defined(HomeAssistant_discovery)

  for (int i = 0; i < Tapparella_nodi; i++) {
    HAtopic = "homeassistant/cover/";
    HAtopic += macToStr(mac);
    HAtopic += "/";
    HAtopic += Tapparella_DEF;
    HAtopic += i;
    HAtopic += "/config";
    HApayload = "{\"name\":\"";
    HApayload += Tapparella[i].topic;
    HApayload += "\",\"command_topic\":\"";
    HApayload += Tapparella[i].topic;
    HApayload += "\",\"payload_open\":\"";
    HApayload += "su";
    HApayload += "\",\"payload_close\":\"";
    HApayload += "giu";
    HApayload += "\",\"payload_stop\":\"";
    HApayload += "stop";    
    /*HApayload += "\",\"state_topic\":\"";
    HApayload += Tapparella[i].topic_ACK;
    HApayload += "\",\"state_open\":\"";
    HApayload += "su";
    HApayload += "\",\"state_closed\":\"";
    HApayload += "giu";
    HApayload += "\",\"value_template\":\"";
    HApayload += "{{value_json.STATO}}";*/
    HApayload += "\",\"availability_topic\":\"";
    HApayload += macToStr(mac);
    HApayload += "\"}";
    client.publish((char*) HAtopic.c_str(), (char*) HApayload.c_str());
    delay(250);
  }
#endif





}

void Stato_TAP (int i) {
  String payload;
  payload = "{\"NODO\":\"";
  payload += Tapparella_DEF;
  payload += "\",\"STATO\":\"";
  if ((digitalRead(Tapparella[i].rele_SU) ^ Flag_inversione_RELE == true ) and  (digitalRead(Tapparella[i].rele_GIU) ^ Flag_inversione_RELE == false )) payload += Tapparella_ACK_SU;
  if ((digitalRead(Tapparella[i].rele_SU) ^ Flag_inversione_RELE == false ) and  (digitalRead(Tapparella[i].rele_GIU) ^ Flag_inversione_RELE == true )) payload += Tapparella_ACK_GIU;
  if ((digitalRead(Tapparella[i].rele_SU) ^ Flag_inversione_RELE == false ) and  (digitalRead(Tapparella[i].rele_GIU) ^ Flag_inversione_RELE == false )) payload += Tapparella_ACK_STOP;
  payload += "\",\"RELE_SU\":\"";
  if (digitalRead(Tapparella[i].rele_SU) == true ^ Flag_inversione_RELE) payload += "on";
  if (digitalRead(Tapparella[i].rele_SU) == false ^ Flag_inversione_RELE) payload += "off";
  payload += "\",\"RELE_GIU\":\"";
  if (digitalRead(Tapparella[i].rele_GIU) == true ^ Flag_inversione_RELE) payload += "on";
  if (digitalRead(Tapparella[i].rele_GIU) == false ^ Flag_inversione_RELE) payload += "off";
  payload += "\",\"LOCK\":\"";
  if (Tapparella[i].lock == 1) payload += "true";
  if (Tapparella[i].lock == 0) payload += "false";
  payload += "\",\"T_MAX\":\"";
  payload += Tapparella[i].tempo_max / 1000;
  payload += "\"}";
  client.publish((char*) Tapparella[i].topic_ACK.c_str(), (char*) payload.c_str());
  delay(250);
}

void Subscribe_Tapparella() {
  for (int i = 0; i < Tapparella_nodi; i++) {
    client.subscribe((char*) Tapparella[i].topic.c_str());
    delay(250);
    Debug_MSG_LN("MQTT subscribe " + Tapparella[i].topic);
  }
}

void callback_Tapparella(char* topic, byte * message, unsigned int length) {
  String payload;
  String message_String;
  for (int i = 0; i < Tapparella_nodi; i++) {
    if (String(topic) == Tapparella[i].topic) {
      message_String = "";
      for (int ii = 0; ii < length; ii++) {
        message_String += (char)message[ii];
      }
      if (message_String == Tapparella_SU) {                             // Topic "Tapparella1_Topic" = "su"
        if (Tapparella[i].lock == 0) {
          digitalWrite(Tapparella[i].rele_GIU, 0 ^ Flag_inversione_RELE);                       // RELE GIU = 0
          delay(Tapparella_TEMPO_RELE);                                                                  // Aspetto
          digitalWrite(Tapparella[i].rele_SU, 1 ^ Flag_inversione_RELE);                        // RELE SU = 1
#if defined(SONOFFDUAL)
          SONOFFDUAL_A_ON_B_OFF();
#endif
          Stato_TAP (i);
          Tapparella[i].in_movimento = true;
          Tapparella[i].ulTime = millis();
        }
        else {
          Stato_TAP (i);
        }
      }
      if (message_String == Tapparella_GIU) {    // Topic "Tapparella1_Topic" = "giu"
        if (Tapparella[i].lock == 0) {
          digitalWrite(Tapparella[i].rele_SU, 0 ^ Flag_inversione_RELE);                        // RELE SU = 0
          delay(Tapparella_TEMPO_RELE);                                                                  // Aspetto
          digitalWrite(Tapparella[i].rele_GIU, 1 ^ Flag_inversione_RELE);                       // RELE GIU = 1
#if defined(SONOFFDUAL)
          SONOFFDUAL_A_OFF_B_ON();
#endif
          Stato_TAP (i);
          Tapparella[i].in_movimento = true;
          Tapparella[i].ulTime = millis();
        }
        else {
          Stato_TAP (i);
        }
      }
      if (message_String == Tapparella_STOP) {                                                       // Topic "Tapparella1_Topic" = "stop"
        digitalWrite(Tapparella[i].rele_SU, 0 ^ Flag_inversione_RELE);                        // RELE SU = 0
        delay(Tapparella_TEMPO_RELE);                                                                  // Aspetto
        digitalWrite(Tapparella[i].rele_GIU, 0 ^ Flag_inversione_RELE);                       // RELE GIU = 0
#if defined(SONOFFDUAL)
        SONOFFDUAL_AB_OFF();
#endif
        Stato_TAP (i);
        Tapparella[i].in_movimento = false;
      }
      if ((char)message[0] == '+') {                                                        // Topic "Tapparella1_Topic" = "+"
        if (Tapparella[i].lock == 0) {
          digitalWrite(Tapparella[i].rele_SU, 1 ^ Flag_inversione_RELE);                        // RELE SU = 1
          delay(Tapparella_TEMPO_RELE);                                                                  // Aspetto
          digitalWrite(Tapparella[i].rele_GIU, 0 ^ Flag_inversione_RELE);                       // RELE GIU = 0
#if defined(SONOFFDUAL)
          SONOFFDUAL_A_ON_B_OFF();
#endif
          Stato_TAP (i);
          message_String = message_String.substring(1);
          char buf[message_String.length()];
          message_String.toCharArray(buf, message_String.length() + 1);
          Tapparella_percentuale = atol(buf);
          if (Tapparella_percentuale > 100) Tapparella_percentuale = 100;
          Debug_MSG_LN("Percentuale=" + (String) Tapparella_percentuale + "%");
          Tapparella_percentuale = Tapparella[i].tempo_max * Tapparella_percentuale / 100;
          Tapparella[i].in_movimento = true;
          Tapparella[i].ulTime = millis();
          Tapparella[i].ulTime -= Tapparella[i].tempo_max;
          Tapparella[i].ulTime += Tapparella_percentuale;
        }
        else {
          Stato_TAP (i);
        }
      }
      if ((char)message[0] == '-') {                                                        // Topic "Tapparella1_Topic" = "-"
        if (Tapparella[i].lock == 0) {
          digitalWrite(Tapparella[i].rele_SU, 0 ^ Flag_inversione_RELE);                        // RELE SU = 1
          delay(Tapparella_TEMPO_RELE);                                                                  // Aspetto
          digitalWrite(Tapparella[i].rele_GIU, 1 ^ Flag_inversione_RELE);                       // RELE GIU = 0
#if defined(SONOFFDUAL)
          SONOFFDUAL_A_OFF_B_ON();
#endif
          Stato_TAP (i);
          message_String = message_String.substring(1);
          char buf[message_String.length()];
          message_String.toCharArray(buf, message_String.length() + 1);
          Tapparella_percentuale = atol(buf);
          if (Tapparella_percentuale > 100) Tapparella_percentuale = 100;
          Debug_MSG_LN("Percentuale=" + (String) Tapparella_percentuale + "%");
          Tapparella_percentuale = Tapparella[i].tempo_max * Tapparella_percentuale / 100;
          Tapparella[i].in_movimento = true;
          Tapparella[i].ulTime = millis();
          Tapparella[i].ulTime -= Tapparella[i].tempo_max;
          Tapparella[i].ulTime += Tapparella_percentuale;
        }
        else {
          Stato_TAP (i);
        }
      }
      if (((char)message[0] == 'T' or (char)message[0] == 't') &
          (char)message[1] == '=' ) {                                                        // Topic "Tapparella1_Topic" = "T=" o "t="
        String stringtmp = "";
        for (int i = 2; i < length; i++)  {
          stringtmp = stringtmp + (char)message[i];
        }
        char buf[stringtmp.length()];
        stringtmp.toCharArray(buf, stringtmp.length() + 1);
        Tapparella[i].tempo_max = atol(buf);
        if (Tapparella[i].tempo_max < 1) {
          Tapparella[i].tempo_max = 10;
        }
        if (Tapparella[i].tempo_max > 120) {
          Tapparella[i].tempo_max = 120;
        }
        Tapparella[i].tempo_max = Tapparella[i].tempo_max * 1000;
        EEPROM_WRITE_Tapparella_tempo_max(i);
        Stato_TAP (i);
      }
      if (((char)message[0] == 'L' or (char)message[0] == 'l') &
          (char)message[1] == '=' ) {                                                           // Topic "Tapparella_Topic" = "L=" o "l="
        if ((char)message[2] == '1') {
          Tapparella[i].lock = 1;
        }
        if ((char)message[2] == '0') {
          Tapparella[i].lock = 0;
        }
        EEPROM_WRITE_Tapparella_lock(i);
        Stato_TAP (i);
      }

      if (message_String == Tapparella_STATO) {                   // Topic "Tapparella1_Topic" = "stato"
        Stato_TAP(i);
      }
    }
  }
}

void loop_Tapparella() {
  String payload;
  for (int i = 0; i < Tapparella_nodi; i++) {
    if (((millis() - Tapparella[i].ulTime) > Tapparella[i].tempo_max) & Tapparella[i].in_movimento) {               // Tempo tapparella -> spengo!
      Debug_MSG_LN("Tempo tapparella " + String(i + 1) + " -> spengo!");
      digitalWrite(Tapparella[i].rele_SU, 0 ^ Flag_inversione_RELE);         // RELE SU = 0
      delay(Tapparella_TEMPO_RELE);                                                  // Aspetto
      digitalWrite(Tapparella[i].rele_GIU, 0 ^ Flag_inversione_RELE);        // RELE SU = 0
#if defined(SONOFFDUAL)
      SONOFFDUAL_AB_OFF();
#endif
      Stato_TAP (i);
      Tapparella[i].in_movimento = false;
    }
    if (Tapparella_TPush_bottone_SU[i].Click(Tapparella_TEMPO_CLICK_ON)) {                                // Bottone SU tapparella premuto!
      Debug_MSG_LN("Bottone SU tapparella " + String(i + 1) + " premuto!");
      digitalWrite(Tapparella[i].rele_GIU, 0 ^ Flag_inversione_RELE);        // RELE GIU = 0
      delay(Tapparella_TEMPO_RELE);                                                  // Aspetto
      digitalWrite(Tapparella[i].rele_SU, 1 ^ Flag_inversione_RELE);         // RELE SU = 1
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_ON_B_OFF();
#endif
      Stato_TAP (i);
      Tapparella[i].in_movimento = true;
      Tapparella[i].ulTime = millis();
    }
    if (Tapparella_TPush_bottone_GIU[i].Click(Tapparella_TEMPO_CLICK_ON)) {                                // Bottone GIU tapparella premuto!
      Debug_MSG_LN("Bottone GIU tapparella " + String(i + 1) + " premuto!");
      digitalWrite(Tapparella[i].rele_SU, 0 ^ Flag_inversione_RELE);          // RELE SU = 0
      delay(Tapparella_TEMPO_RELE);                                                   // Aspetto
      digitalWrite(Tapparella[i].rele_GIU, 1 ^ Flag_inversione_RELE);         // RELE GIU = 1
#if defined(SONOFFDUAL)
      SONOFFDUAL_A_OFF_B_ON();
#endif
      Stato_TAP (i);
      Tapparella[i].in_movimento = true;
      Tapparella[i].ulTime = millis();
    }
    if (t = Tapparella_TPush_bottone_SU[i].Keep(Tapparella_TEMPO_CLICK_OFF)) {                            // Bottone SU tapparella rilasciato!
      Debug_MSG_LN("Bottone SU tapparella " + String(i + 1) + " rilasciato -> spengo!");
      digitalWrite(Tapparella[i].rele_SU, 0 ^ Flag_inversione_RELE);         // RELE SU = 0
      delay(Tapparella_TEMPO_RELE);                                                  // Aspetto
      digitalWrite(Tapparella[i].rele_GIU, 0 ^ Flag_inversione_RELE);        // RELE GIU = 0
#if defined(SONOFFDUAL)
      SONOFFDUAL_AB_OFF();
#endif
      Stato_TAP (i);
      Tapparella[i].in_movimento = false;
    }
    if (t = Tapparella_TPush_bottone_GIU[i].Keep(Tapparella_TEMPO_CLICK_OFF)) {                           // Bottone GIU tapparella rilasciato!
      Debug_MSG_LN("Bottone GIU tapparella " + String(i + 1) + " rilasciato -> spengo!");
      digitalWrite(Tapparella[i].rele_SU, 0 ^ Flag_inversione_RELE);         // RELE SU = 0
      delay(Tapparella_TEMPO_RELE);                                                  // Aspetto
      digitalWrite(Tapparella[i].rele_GIU, 0 ^ Flag_inversione_RELE);        // RELE GIU = 0
#if defined(SONOFFDUAL)
      SONOFFDUAL_AB_OFF();
#endif
      Stato_TAP (i);
      Tapparella[i].in_movimento = false;
    }
  }
}

void EEPROM_READ_Tapparella_tempo_max(int i) {
  Debug_MSG("Reading EEPROM TEMPO tapparella " + String(i + 1) + " MAX: ");
  Tapparella[i].tempo_max = EEPROMReadlong(EEPROM_Tapparella_tempo_max + (i * 4));
  Debug_MSG_LN((String)Tapparella[i].tempo_max);
  if (Tapparella[i].tempo_max < 1) {
    Tapparella[i].tempo_max = Tapparella_tempo_max_default;
    EEPROM_WRITE_Tapparella_tempo_max(i);
  }
  if (Tapparella[i].tempo_max < 1000) {
    Tapparella[i].tempo_max = 10000;
    EEPROM_WRITE_Tapparella_tempo_max(i);
  }
  if (Tapparella[i].tempo_max > 60000) {
    Tapparella[i].tempo_max = 60000;
    EEPROM_WRITE_Tapparella_tempo_max(i);
  }
}

void EEPROM_WRITE_Tapparella_tempo_max(int i) {
  Debug_MSG("Writing EEPROM TEMPO tapparella " + String(i + 1) + " MAX: ");
  Debug_MSG_LN((String) Tapparella[i].tempo_max);
  EEPROMWritelong(EEPROM_Tapparella_tempo_max + (i * 4), Tapparella[i].tempo_max);
  EEPROM.commit();
}

void EEPROM_READ_Tapparella_lock(int i) {
  Debug_MSG("Reading EEPROM lock tapparella " + String(i + 1) + " : ");
  int tmp;
  tmp = eepromReadInt(EEPROM_Tapparella_lock + (i  * 2));
  if (tmp == 1) {
    Tapparella[i].lock = 1;
    Debug_MSG_LN("locked!");
  }
  if (tmp == 0) {
    Tapparella[i].lock = 0;
    Debug_MSG_LN("unlocked!");
  }
  if (tmp != 1 and tmp != 0) {
    Tapparella[i].lock = 0;
    Debug_MSG_LN("Wrong EEPROM lock block!");
    EEPROM_WRITE_Tapparella_lock(i);
  }
}

void EEPROM_WRITE_Tapparella_lock(int i) {
  Debug_MSG("Writing EEPROM lock tapparella " + String(i + 1) + " : ");
  int tmp = 1;
  if (Tapparella[i].lock == 1) {
    tmp = 1;
    Debug_MSG_LN("lock");
  }
  if (Tapparella[i].lock == 0) {
    tmp = 0;
    Debug_MSG_LN("unlock");
  }
  eepromWriteInt((EEPROM_Tapparella_lock + (i  * 2)), tmp);
  EEPROM.commit();
}
