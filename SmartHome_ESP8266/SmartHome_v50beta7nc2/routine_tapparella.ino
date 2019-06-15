#if Tapparella_nodi>0

#define Tapparella_percentuale

#define Tapparella_TEMPO_INVERSIONE_RELE          150
#define Tapparella_TEMPO_CLICK_ON                 TEMPO_CLICK_ON
#define Tapparella_TEMPO_CLICK_OFF                TEMPO_CLICK_OFF

long Tapparella_tempo_max_default = 25000;                                       // Tempo tapparella di default (in millisecondi)

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
  float percentuale;
  float percentuale_inizio_movimento;
  unsigned long ulTime;
  unsigned long millis_target;
  long tempo_max;
  int lock;
} InfoRecordTapparellaType ;

InfoRecordTapparellaType Tapparella[5];

TPush Tapparella_TPush_bottone_SU[Tapparella_nodi], Tapparella_TPush_bottone_GIU[Tapparella_nodi], Taparella_timer_refresh_perc[Tapparella_nodi];

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
    Debug_MSG_LN("Nodo Tapparella " + String(i + 1) + " : " + Tapparella[i].topic);
    Debug_MSG_LN(" RELE    tapparella " + String(i + 1) + " SU  = GPIO" + String(Tapparella[i].rele_SU));
    Debug_MSG_LN(" RELE    tapparella " + String(i + 1) + " GIU = GPIO" + String(Tapparella[i].rele_GIU));
    pinMode(Tapparella[i].rele_SU, OUTPUT);
    digitalWrite(Tapparella[i].rele_SU, 0 ^ Flag_inversione_RELE);
    pinMode(Tapparella[i].rele_GIU, OUTPUT);
    digitalWrite(Tapparella[i].rele_GIU, 0 ^ Flag_inversione_RELE);
    if (Tapparella[i].bottone_SU >= 99) {
      Debug_MSG_LN(" BOTTONE tapparella " + String(i + 1) + " SU  = Nessun GPIO definito!");
    }
    else {
      Debug_MSG_LN(" BOTTONE tapparella " + String(i + 1) + " SU  = GPIO" + String(Tapparella[i].bottone_SU));
    }
#ifdef PullDown
    Tapparella_TPush_bottone_SU[i].setUp(Tapparella[i].bottone_SU, HIGH);
#endif
#ifdef PullUp
    Tapparella_TPush_bottone_SU[i].setUp(Tapparella[i].bottone_SU, LOW);
#endif
    if (Tapparella[i].bottone_GIU >= 99) {
      Debug_MSG_LN(" BOTTONE tapparella " + String(i + 1) + " GIU = Nessun GPIO definito!");
    }
    else {
      Debug_MSG_LN(" BOTTONE tapparella " + String(i + 1) + " GIU = GPIO" + String(Tapparella[i].bottone_GIU));
    }
#ifdef PullDown
    Tapparella_TPush_bottone_GIU[i].setUp(Tapparella[i].bottone_GIU, HIGH);
#endif
#ifdef PullUp
    Tapparella_TPush_bottone_GIU[i].setUp(Tapparella[i].bottone_GIU, LOW);
#endif
    Tapparella[i].millis_target = 0;
    EEPROM_READ_Tapparella_tempo_max(i);
    EEPROM_READ_Tapparella_lock(i);
    EEPROM_READ_Tapparella_percentuale(i);
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
    MQTTPublish(ACK_Topic, payload);
    Stato_TAP (i);
  }
}

void HA_discovery_Tapparella() {
  String HAtopic;
  String HApayload;
  for (int i = 0; i < Tapparella_nodi; i++) {
    Debug_MSG_LN("Creo nodo " + (String)Tapparella[i].topic + " Home assistant discovery ");
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
    HApayload += "\",\"payload_open\":\"su";
    HApayload += "\",\"payload_close\":\"giu";
    HApayload += "\",\"payload_stop\":\"stop";
#ifdef Tapparella_percentuale
    HApayload += "\",\"position_topic\":\"";
    HApayload += Tapparella[i].topic_ACK;
    HApayload += "\",\"value_template\":\"";
    HApayload += "{{value_json.PERCENTUALE}}";
    HApayload += "\",\"set_position_topic\":\"";
    HApayload += Tapparella[i].topic;
    HApayload += "\",\"set_position_template\":\"";
    HApayload += "%={{position}}";
#endif
    HApayload += "\",\"availability_topic\":\"";
    HApayload += WillTopicStr;
    HApayload += "\"}";
    MQTTPublish(HAtopic, HApayload);
  }
}

void Stato_TAP (int i) {
  String payload;
  payload = "{\"NODO\":\"";
  payload += Tapparella_DEF;
  payload += "\",\"STATO\":\"";
  if ((digitalRead(Tapparella[i].rele_SU) ^ (Flag_inversione_RELE == true)) and  (digitalRead(Tapparella[i].rele_GIU) ^ (Flag_inversione_RELE == false ))) payload += Tapparella_ACK_SU;
  if ((digitalRead(Tapparella[i].rele_SU) ^ (Flag_inversione_RELE == false)) and  (digitalRead(Tapparella[i].rele_GIU) ^ (Flag_inversione_RELE == true ))) payload += Tapparella_ACK_GIU;
  if ((digitalRead(Tapparella[i].rele_SU) ^ (Flag_inversione_RELE == false)) and  (digitalRead(Tapparella[i].rele_GIU) ^ (Flag_inversione_RELE == false ))) payload += Tapparella_ACK_STOP;
  payload += "\",\"RELE_SU\":\"";
  if ((digitalRead(Tapparella[i].rele_SU) == true) ^ Flag_inversione_RELE) payload += "on";
  if ((digitalRead(Tapparella[i].rele_SU) == false) ^ Flag_inversione_RELE) payload += "off";
  payload += "\",\"RELE_GIU\":\"";
  if ((digitalRead(Tapparella[i].rele_GIU) == true) ^ Flag_inversione_RELE) payload += "on";
  if ((digitalRead(Tapparella[i].rele_GIU) == false) ^ Flag_inversione_RELE) payload += "off";
  payload += "\",\"LOCK\":\"";
  if (Tapparella[i].lock == 1) payload += "true";
  if (Tapparella[i].lock == 0) payload += "false";
  payload += "\",\"T_MAX\":\"";
  payload += Tapparella[i].tempo_max / 1000;
#ifdef Tapparella_percentuale
  payload += "\",\"PERCENTUALE\":\"";
  payload += (int)Tapparella[i].percentuale;
#endif
  payload += "\"}";
  MQTTPublish(Tapparella[i].topic_ACK, payload);
}

void Subscribe_Tapparella() {
  for (int i = 0; i < Tapparella_nodi; i++) {
    MQTTSubscribe((char*) Tapparella[i].topic.c_str());
    Debug_MSG_LN("MQTT subscribe " + Tapparella[i].topic);
  }
}

void callback_Tapparella(char* topic, char* message) {
  String payload;
  for (int i = 0; i < Tapparella_nodi; i++) {
    if (String(topic) == Tapparella[i].topic) {
      if ((String)message == Tapparella_SU) {                                                                      // Topic "Tapparella1_Topic" = "su"
        if (Tapparella[i].lock == 0) {
          Tapparella_cmd_SU(i, Tapparella[i].tempo_max);
        }
        else {
          Debug_MSG_LN("Tapparella " + String(i + 1) + " LOCK");
          Stato_TAP (i);
        }
      }
      if ((String)message == Tapparella_GIU) {                                                                     // Topic "Tapparella1_Topic" = "giu"
        if (Tapparella[i].lock == 0) {
          Tapparella_cmd_GIU(i, Tapparella[i].tempo_max);
        }
        else {
          Debug_MSG_LN("Tapparella " + String(i + 1) + " LOCK");
          Stato_TAP (i);
        }
      }
      if ((String)message == Tapparella_STOP) {                                                                    // Topic "Tapparella1_Topic" = "stop"
        Tapparella_cmd_STOP(i);
      }
      if ((char)message[0] == '+') {                                                                              // Topic "Tapparella1_Topic" = "+"
        String stringtmp = (String)message;
        stringtmp = stringtmp.substring(1);
        char buf[stringtmp.length()];
        stringtmp.toCharArray(buf, stringtmp.length() + 1);
        unsigned long percentuale_target = atol(buf);
        if (percentuale_target > 100) percentuale_target = 100;
        Debug_MSG_LN("Percentuale=" + (String)percentuale_target + "%");
        if (Tapparella[i].lock == 0) {
          Tapparella_cmd_SU(i, Tapparella[i].tempo_max * percentuale_target / 100);
        }
        else {
          Debug_MSG_LN("Tapparella " + String(i + 1) + " LOCK");
          Stato_TAP (i);
        }
      }
      if ((char)message[0] == '-') {                                                                              // Topic "Tapparella1_Topic" = "-"
        String stringtmp = (String)message;
        stringtmp = stringtmp.substring(1);
        char buf[stringtmp.length()];
        stringtmp.toCharArray(buf, stringtmp.length() + 1);
        unsigned long percentuale_target = atol(buf);
        if (percentuale_target > 100) percentuale_target = 100;
        Debug_MSG_LN("Percentuale=" + (String)percentuale_target + "%");
        if (Tapparella[i].lock == 0) {
          Tapparella_cmd_GIU(i, Tapparella[i].tempo_max * percentuale_target / 100);
        }
        else {
          Debug_MSG_LN("Tapparella " + String(i + 1) + " LOCK");
          Stato_TAP (i);
        }
      }
#ifdef Tapparella_percentuale
      if (((char)message[0] == '%') and ((char)message[1] == '=')) {                                              // Topic "Tapparella1_Topic" = "%="
        String stringtmp = (String)message;
        stringtmp = stringtmp.substring(2);
        char buf[stringtmp.length()];
        stringtmp.toCharArray(buf, stringtmp.length() + 1);
        unsigned long percentuale_target = atol(buf);
        if (percentuale_target > Tapparella[i].percentuale) {
          if (Tapparella[i].lock == 0) {
            Tapparella_cmd_SU(i, Tapparella[i].tempo_max * (percentuale_target - Tapparella[i].percentuale) / 100);
          }
          else {
            Debug_MSG_LN("Tapparella " + String(i + 1) + " LOCK");
            Stato_TAP (i);
          }
        }
        if (percentuale_target < Tapparella[i].percentuale) {
          if (Tapparella[i].lock == 0) {
            Tapparella_cmd_GIU(i, Tapparella[i].tempo_max * (Tapparella[i].percentuale - percentuale_target) / 100);
          }
          else {
            Debug_MSG_LN("Tapparella " + String(i + 1) + " LOCK");
            Stato_TAP (i);
          }
        }
      }
#endif
      if (((char)message[0] == 'T' or (char)message[0] == 't') and ((char)message[1] == '=' )) {                  // Topic "Tapparella1_Topic" = "T=" o "t="
        String stringtmp = (String)message;
        stringtmp = stringtmp.substring(2);
        char buf[stringtmp.length()];
        stringtmp.toCharArray(buf, stringtmp.length() + 1);
        Tapparella[i].tempo_max = atol(buf);
        if (Tapparella[i].tempo_max < 5) {
          Tapparella[i].tempo_max = 5;
        }
        if (Tapparella[i].tempo_max > 120) {
          Tapparella[i].tempo_max = 120;
        }
        Tapparella[i].tempo_max = Tapparella[i].tempo_max * 1000;
        EEPROM_WRITE_Tapparella_tempo_max(i);
        Stato_TAP (i);
      }
      if (((char)message[0] == 'L' or (char)message[0] == 'l') and ((char)message[1] == '=')) {                   // Topic "Tapparella_Topic" = "L=" o "l="
        if ((char)message[2] == '1') {
          Tapparella[i].lock = 1;
        }
        if ((char)message[2] == '0') {
          Tapparella[i].lock = 0;
        }
        EEPROM_WRITE_Tapparella_lock(i);
        Stato_TAP (i);
      }
      if ((String)message == Tapparella_STATO) {                                                                   // Topic "Tapparella1_Topic" = "stato"
        Stato_TAP(i);
      }
    }
  }
}


void loop_Tapparella() {
  String payload;
  for (int i = 0; i < Tapparella_nodi; i++) {
    if (((millis() - Tapparella[i].ulTime) > Tapparella[i].millis_target) and Tapparella[i].in_movimento) {       // Tempo tapparella -> STOP!
      Tapparella_cmd_STOP(i);
    }
    if (Tapparella_TPush_bottone_SU[i].Click(Tapparella_TEMPO_CLICK_ON) and Tapparella[i].bottone_SU != 100) {    // Bottone SU tapparella premuto!
      Tapparella_cmd_SU(i, Tapparella[i].tempo_max);
    }
    if (Tapparella_TPush_bottone_GIU[i].Click(Tapparella_TEMPO_CLICK_ON) and Tapparella[i].bottone_GIU != 100) {  // Bottone GIU tapparella premuto!
      Tapparella_cmd_GIU(i, Tapparella[i].tempo_max);
    }
    if ((t = Tapparella_TPush_bottone_SU[i].Keep(Tapparella_TEMPO_CLICK_OFF))) {                                  // Bottone SU tapparella rilasciato!
      Tapparella_cmd_STOP(i);
    }
    if ((t = Tapparella_TPush_bottone_GIU[i].Keep(Tapparella_TEMPO_CLICK_OFF))) {                                 // Bottone GIU tapparella rilasciato!
      Tapparella_cmd_STOP(i);
    }


    if (Taparella_timer_refresh_perc[i].Wait(1000)) {
      if (Tapparella[i].in_movimento) {
        calcola_percentuale(i);
      }
    }
  }
}

void Tapparella_cmd_SU (int i, unsigned long target) {
  Debug_MSG_LN("Tapparella " + String(i + 1) + " SU");
  Tapparella[i].percentuale_inizio_movimento = Tapparella[i].percentuale;
  if (Tapparella[i].in_movimento == true) calcola_percentuale(i);
  digitalWrite(Tapparella[i].rele_SU, 0 ^ Flag_inversione_RELE);                        // RELE SU = 0
  digitalWrite(Tapparella[i].rele_GIU, 0 ^ Flag_inversione_RELE);                       // RELE GIU = 0
  unblockingDelay(Tapparella_TEMPO_INVERSIONE_RELE);
  digitalWrite(Tapparella[i].rele_SU, 1 ^ Flag_inversione_RELE);                        // RELE SU = 1
#ifdef SONOFFDUAL
  SONOFFDUAL_A_ON_B_OFF();
#endif
  Tapparella[i].in_movimento = true;
  Tapparella[i].ulTime = millis();
  Tapparella[i].millis_target = target;
  Stato_TAP (i);
}

void Tapparella_cmd_GIU (int i, unsigned long target) {
  Debug_MSG_LN("Tapparella " + String(i + 1) + " GIU");
  Tapparella[i].percentuale_inizio_movimento = Tapparella[i].percentuale;
  if (Tapparella[i].in_movimento == true) calcola_percentuale(i);
  digitalWrite(Tapparella[i].rele_SU, 0 ^ Flag_inversione_RELE);                        // RELE SU = 0
  digitalWrite(Tapparella[i].rele_GIU, 0 ^ Flag_inversione_RELE);                       // RELE GIU = 0
  unblockingDelay(Tapparella_TEMPO_INVERSIONE_RELE);
  digitalWrite(Tapparella[i].rele_GIU, 1 ^ Flag_inversione_RELE);                       // RELE GIU = 1
#ifdef SONOFFDUAL
  SONOFFDUAL_A_OFF_B_ON();
#endif
  Tapparella[i].in_movimento = true;
  Tapparella[i].ulTime = millis();
  Tapparella[i].millis_target = target;
  Stato_TAP (i);
}

void Tapparella_cmd_STOP (int i) {
  Debug_MSG_LN("Tempo tapparella " + String(i + 1) + " -> STOP!");
  if (Tapparella[i].in_movimento == true) calcola_percentuale(i);
  digitalWrite(Tapparella[i].rele_SU, 0 ^ Flag_inversione_RELE);                        // RELE SU = 0
  digitalWrite(Tapparella[i].rele_GIU, 0 ^ Flag_inversione_RELE);                       // RELE GIU = 0
  //unblockingDelay(Tapparella_TEMPO_INVERSIONE_RELE);
#ifdef SONOFFDUAL
  SONOFFDUAL_AB_OFF();
#endif
  Tapparella[i].in_movimento = false;
  Tapparella[i].millis_target = 0;
  EEPROM_WRITE_Tapparella_percentuale(i);
  Stato_TAP (i);
}

void calcola_percentuale(int i) {
#ifdef Tapparella_percentuale
  float TM = Tapparella[i].tempo_max;
  float MUT = millis() - Tapparella[i].ulTime;
  if (digitalRead(Tapparella[i].rele_SU) == (1 ^ Flag_inversione_RELE)) {
    Tapparella[i].percentuale = Tapparella[i].percentuale_inizio_movimento  + (MUT / TM * 100);
    if ( Tapparella[i].percentuale > 100) {
      Tapparella[i].percentuale = 100;
    }
  }
  if (digitalRead(Tapparella[i].rele_GIU) == (1 ^ Flag_inversione_RELE)) {
    Tapparella[i].percentuale = Tapparella[i].percentuale_inizio_movimento - (MUT / TM * 100);
    if ( Tapparella[i].percentuale < 0) {
      Tapparella[i].percentuale = 0;
    }
  }
  Debug_MSG_LN("Tapparella " + String(i + 1) + " al " + (String)Tapparella[i].percentuale + " %");
  String payload = "{\"PERCENTUALE\":\"";
  payload += (int)Tapparella[i].percentuale;
  payload += "\"}";
  MQTTPublish(Tapparella[i].topic_ACK, payload);
#endif
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
  tmp = eepromRead(EEPROM_Tapparella_lock + i);
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
  eepromWrite((EEPROM_Tapparella_lock + i), tmp);
  EEPROM.commit();
}

void EEPROM_READ_Tapparella_percentuale(int i) {
  Debug_MSG("Reading EEPROM percentuale Tapparella " + String(i + 1) + " :");
  Tapparella[i].percentuale = eepromReadFloat(EEPROM_Tapparella_percentuale + (i * 4));
  if (Tapparella[i].percentuale<0 or Tapparella[i].percentuale>100 or isnan(Tapparella[i].percentuale)) Tapparella[i].percentuale = 0;
  Debug_MSG_LN((String) Tapparella[i].percentuale + "%");
}

void EEPROM_WRITE_Tapparella_percentuale(int i) {
  Debug_MSG_LN("Writing EEPROM percentuale Tapparella " + String(i + 1) + " :" + (String) Tapparella[i].percentuale + "%");
  eepromWriteFloat(EEPROM_Tapparella_percentuale + (i * 4), Tapparella[i].percentuale);
  EEPROM.commit();
}

#endif
