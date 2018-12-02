/*
   in SmartHome_vXX.ino
      nel void setup() inserire la riga:  setup_NuovoNodo();
      nel void loop() inserire la riga: loop_NuovoNodo();

   in mqtt.ino
      nel void Send_Alive() inserire la riga: Alive_NuovoNodo();
      nel void Send_ACK() inserire la riga: ACK_NuovoNodo();
      nel void SubscribeTopic() inserire la riga Subscribe_NuovoNodo();
      nel void callback(char* topic, byte * message, unsigned int length) inserire la riga: callback_NuovoNodo(topic, message, length);

*/


#define NuovoNodo_nodi            0                             // esempio da 0 a 3 nodi NuovoNodo
#define NuovoNodo1_Topic          "NuovoNodo/1"                 // NuovoNodo 1 Topic (non usare underscore "_")
#define NuovoNodo2_Topic          "NuovoNodo/2"                 // NuovoNodo 2 Topic (non usare underscore "_")
#define NuovoNodo3_Topic          "NuovoNodo/3"                 // NuovoNodo 3 Topic (non usare underscore "_")

String NuovoNodo_DEF        = "NEW";                            // identificativo del nodo (3 lettere) (serve all'app per riconoscere il tipo di nodo)
String NuovoNodo_MESSAGGIO1 = "messaggio1";
String NuovoNodo_MESSAGGIO2 = "messaggio2";
String NuovoNodo_Topic_ACK_suffisso = "/ack";

typedef struct {
  String topic;
  String topic_ACK;
} InfoRecordNuovoNodoType ;

InfoRecordNuovoNodoType NuovoNodo[3];

void setup_NuovoNodo() {
  NuovoNodo[0].topic = NuovoNodo1_Topic;
  NuovoNodo[1].topic = NuovoNodo2_Topic;
  NuovoNodo[2].topic = NuovoNodo3_Topic;
  NuovoNodo[0].topic_ACK = NuovoNodo1_Topic;
  NuovoNodo[1].topic_ACK = NuovoNodo2_Topic;
  NuovoNodo[2].topic_ACK = NuovoNodo3_Topic;
  NuovoNodo[0].topic_ACK += NuovoNodo_Topic_ACK_suffisso;
  NuovoNodo[1].topic_ACK += NuovoNodo_Topic_ACK_suffisso;
  NuovoNodo[2].topic_ACK += NuovoNodo_Topic_ACK_suffisso;
  
  for (int i = 0; i < NuovoNodo_nodi; i++) {
    Debug_MSG_LN("- NuovoNodo " + String(i + 1) + " : " + NuovoNodo[i].topic);
  }

  // inizializzare gli eventuali GPIO
}

void ACK_NuovoNodo() {
  String payload;
  for (int i = 0; i < NuovoNodo_nodi; i++) {
    payload = "{\"MAC_address\":\"";
    payload += macToStr(mac);
    payload += "\",\"Tipo_nodo\":\"";
    payload += NuovoNodo_DEF;
    payload += "\",\"Topic\":\"";
    payload += NuovoNodo[i].topic;
    payload += "\",\"Topic_ACK\":\"";
    payload += NuovoNodo[i].topic_ACK;
    payload += "\",\"availability_topic\":\"";
    payload += WillTopicStr;
    payload += "\"}";
    client.publish(ACK_Topic, (char*) payload.c_str());
    delay(500);
    Stato_NEW(i);
  }
}

void Alive_NuovoNodo() {

}

void Stato_NEW(int i) {
  
}

void Subscribe_NuovoNodo() {
  for (int i = 0; i < NuovoNodo_nodi; i++) {
    client.subscribe((char*) NuovoNodo[i].topic.c_str());
    delay(250);
    Debug_MSG_LN("MQTT subscribe " + String(NuovoNodo[i].topic));
  }
}

void callback_NuovoNodo(char* topic, byte * message, unsigned int length) {
  // questa funzione viene richiamata ogni volta che viene rucevuto un messaggio MQTT
  String payload;
  String message_String;
  for (int i = 0; i < NuovoNodo_nodi; i++) {
    if (String(topic) == NuovoNodo[i].topic) {
      message_String = "";
      for (int ii = 0; ii < length; ii++) {
        message_String += (char)message[ii];
      }
      if (message_String == NuovoNodo_MESSAGGIO1) {
        // azione da compiere al ricevimento di "messaggio1" sul topic NuovoNodo[i].topic
      }
      if (message_String == NuovoNodo_MESSAGGIO2) {
        // azione da compiere al ricevimento di "messaggio2" sul topic NuovoNodo[i].topic
      }
      // altre condizioni
    }
  }
}

void loop_NuovoNodo() {
  // questa funzione viene ripetuta ciclicamente
  // inserire qui la gestione degli ingressi da GPIO
}
