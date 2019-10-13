#if IR_nodi>0

#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRutils.h>

#define TX_PIN IR1_GPIO_TX
#define RX_PIN IR1_GPIO_RX
#define CAPTURE_BUFFER_SIZE 1024
#if DECODE_AC
#define TIMEOUT 50U
#else
#define TIMEOUT 15U
#endif

IRrecv irrecv(RX_PIN, CAPTURE_BUFFER_SIZE, TIMEOUT, true);
decode_results results;
IRsend irsend(TX_PIN);

String IR_DEF     = "IR_";
String IR_STATO   = "stato";
String IR_Topic_ACK_suffisso = "/ack";

typedef struct {
  String topic;
  String topic_ACK;
  String topic_send;
  int RX;
  int TX;
} InfoRecordIRType ;

InfoRecordIRType IR[1];

void setup_IR() {
  IR[0].topic = IR1_Topic;
  IR[0].topic_ACK = IR1_Topic;
  IR[0].topic_ACK += IR_Topic_ACK_suffisso;
#ifdef IR1_Topic_Send
  IR[0].topic_send = IR1_Topic_Send;
#else
  IR[0].topic_send = "";
#endif
#ifdef IR1_GPIO_RX
  IR[0].RX = IR1_GPIO_RX;
#else
  IR[0].RX = 100;
#endif
#ifdef IR1_GPIO_TX
  IR[0].TX = IR1_GPIO_TX;
#else
  IR[0].TX = 100;
#endif

  for (int i = 0; i < IR_nodi; i++) {
    Debug_MSG_LN("Nodo IR " + String(i + 1) + " : " + String(IR[i].topic));
    if (IR[0].RX >= 99) {
      Debug_MSG_LN(" RX GPIO IR " + String(i + 1) + " : Nessun GPIO definito!");
    }
    else {
      Debug_MSG_LN(" RX GPIO IR " + String(i + 1) + " = GPIO" + String(IR[i].RX));
      irrecv.enableIRIn();  // Start up the IR receiver.
    }
    if (IR[0].TX >= 99) {
      Debug_MSG_LN(" TX GPIO IR " + String(i + 1) + " : Nessun GPIO definito!");
    }
    else {
      Debug_MSG_LN(" TX GPIO IR " + String(i + 1) + " = GPIO" + String(IR[i].TX));
      irsend.begin();       // Start up the IR sender.
    }
    if (IR[0].topic_send != "") Debug_MSG_LN(" Topic send " + String(i + 1) + " = " + String(IR[i].topic_send));
  }
}

void ACK_IR() {
  for (int i = 0; i < IR_nodi; i++) {
    DynamicJsonDocument JSONdoc(1024);
    JSONdoc["MAC_address"] = macToStr(mac);
    JSONdoc["NODO"] = IR_DEF;
    JSONdoc["Topic"] = IR[i].topic;
    JSONdoc["Topic_ACK"] = IR[i].topic_ACK;
    JSONdoc["availability_topic"] = WillTopicStr;
    String payload = "";
    serializeJson(JSONdoc, payload);
    MQTTPublish(ACK_Topic, payload);
    Stato_IR(i);
  }
}

void HA_discovery_IR() {
}

void Stato_IR(int i) {
  DynamicJsonDocument JSONdoc(1024);
  JSONdoc["NODO"] = IR_DEF;
  String tmp = "";
  if (IR[0].RX < 99) tmp += "RX";
  if (IR[0].TX < 99) tmp += "TX";
  JSONdoc["STATO"] = tmp;
  String payload = "";
  serializeJson(JSONdoc, payload);
  MQTTPublish(IR[i].topic_ACK, payload);
}

void Subscribe_IR() {
  String payload;
  for (int i = 0; i < IR_nodi; i++) {
    MQTTSubscribe(IR[i].topic);
    Debug_MSG_LN("MQTT subscribe " + String(IR[i].topic));
  }
}

void callback_IR(char* topic, char* message) {
  String payload;
  DynamicJsonDocument JSONdoc(1024);
  DeserializationError error = deserializeJson(JSONdoc, message);
  String JSON_PROTOCOL = JSONdoc["PROTOCOL"];
  String JSON_LEN = JSONdoc["LEN"];
  String JSON_RAW = JSONdoc["RAW"];
  String JSON_VALUE = JSONdoc["VALUE"];
  /*
    Serial.print(F("deserializeJson(): "));
    Serial.println(error.c_str());
    Debug_MSG_LN("JSON_PROTOCOL:" + JSON_PROTOCOL);
    Debug_MSG_LN("JSON_LEN:" + JSON_LEN);
    Debug_MSG_LN("JSON_RAW:" + JSON_RAW);
    Debug_MSG_LN("JSON_VALUE:" + JSON_VALUE);
  */
  for (int i = 0; i < IR_nodi; i++) {
    if (String(topic) == IR[i].topic) {
      if ((String)message == IR_STATO) {
        Stato_IR(i);
      }
      if (!JSONdoc["PROTOCOL"].isNull()) {
        if (JSON_PROTOCOL == "UNKNOWN") {
          Debug_MSG_LN("IR RAW send");
          Debug_MSG_LN(" Raw code: " + JSON_RAW);
          Debug_MSG_LN(" Size: " + JSON_LEN);
          uint16_t sizee = JSON_LEN.toInt() ;
          uint16_t raw_array[sizee - 1];
          String tmp_RAW = JSON_RAW;
          for (int i = 0; i < sizee - 1; i++) {
            raw_array[i] = (uint16_t)tmp_RAW.substring(0, tmp_RAW.indexOf(",")).toInt();
            //Debug_MSG_LN((String)i + "-" + tmp_RAW.substring(0, tmp_RAW.indexOf(",")));
            tmp_RAW =  tmp_RAW.substring(tmp_RAW.indexOf(",") + 1, tmp_RAW.length());
            //Debug_MSG_LN(tmp_RAW);
          }
          raw_array[sizee - 1] = (uint16_t)tmp_RAW.toInt();
          Debug_MSG_LN("IR RX disable");
          irrecv.disableIRIn();
          Debug_MSG_LN("IR send");
          irsend.sendRaw(raw_array, sizee, 38);
          Debug_MSG_LN("IR RX enable");
          irrecv.enableIRIn();
        }
        else {
          Debug_MSG_LN("IR " + JSON_PROTOCOL + " send");
          Debug_MSG_LN(" value: " + JSON_VALUE);
          uint64_t rv = strtoul(JSON_VALUE.c_str(), NULL, 0);
          int sizeee = sizeof(rv) * 8;
          Debug_MSG_LN(" Size: " + (String)sizeee);

          Debug_MSG_LN("IR RX disable");
          irrecv.disableIRIn();
          if (!JSON_VALUE.startsWith("0x")) JSON_VALUE = "0x" + JSON_VALUE;
          Debug_MSG_LN("IR send");
          //if (hasACState(protocol)) irsend.send(protocol, results.state, size / 8);
          irsend.send(strToDecodeType((char*)JSON_PROTOCOL.c_str()), rv, sizeee);
          Debug_MSG_LN("IR RX enable");
          irrecv.enableIRIn();
        }
      }
    }
  }
}

void loop_IR() {
  if (irrecv.decode(&results)) {
    Debug_MSG_LN("IR received");
    decode_type_t protocol = results.decode_type;
    String typee = typeToString(protocol);
    String rawData = "[";
    for (uint16_t i = 1; i < results.rawlen; i++) {
      uint32_t usecs;
      for (usecs = results.rawbuf[i] * RAWTICK; usecs > UINT16_MAX; usecs -= UINT16_MAX) {
        rawData += uint64ToString(UINT16_MAX);
        if (i % 2) rawData += ", 0,  ";
        else rawData += ",  0, ";
      }
      rawData += uint64ToString(usecs, 10);
      if (i < results.rawlen - 1) rawData += ", ";  // ',' not needed on the last one
      if (i % 2 == 0)  rawData += " ";  // Extra if it was even.
    }
    rawData += "]";
    DynamicJsonDocument JSONdoc(1024);
    JSONdoc["PROTOCOL"] = typee;
    JSONdoc["VALUE"] = uint64ToString(results.value, 16);
    JSONdoc["LEN"] = (String)getCorrectedRawLength(&results);
    JSONdoc["RAW"] = rawData;
    String payload;
    serializeJson(JSONdoc, payload);
    MQTTPublish(IR[0].topic_ACK, payload);
    if (IR[0].topic_send != "") MQTTPublish(IR[0].topic_send, payload);
    irrecv.resume();
  }
  yield();
}
#endif
