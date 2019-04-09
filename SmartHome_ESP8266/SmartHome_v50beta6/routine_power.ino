#if Power_nodi>0

/*
  Power1_device
  1 = PZEM004
  2 = BlitzWolf
  3 = Sonoff POW
*/

// #define Power_DISPLAY "Display/1"
#define Power_TEMPO                5000

#include <TPush.h>

TPush Power_timer;

#if Power1_device  == 1             // PZEM004
#include <PZEM004T.h>
HardwareSerial hwserial(UART0);     // Use hwserial UART0 at pins GPIO1 (TX) and GPIO3 (RX)
PZEM004T pzem(&hwserial);           // Attach PZEM to hwserial
IPAddress ip(0, 0, 0, 1);
bool pzemrdy = false;
#endif

#if Power1_device  == 2             // BlitzWolf
#include "HLW8012.h"
/*
  GPIO 12  SEL Pin HJL-01
  GPIO 14  CF1 Pin HJL-01
  GPIO 15  relay (high act.)
  GPIO 13  button (low act.)
  GPIO 2   blue LED (low act.)
  GPIO 0   red LED (low act.)
  GPIO 5   CF Pin HJL-01 IO4
*/
// GPIOs
#define SEL_PIN                         12
#define CF1_PIN                         14
#define CF_PIN                          5
#define CURRENT_MODE                    HIGH
// These are the nominal values for the resistors in the circuit
#define CURRENT_RESISTOR                0.001
#define VOLTAGE_RESISTOR_UPSTREAM       ( 5 * 470000 ) // Real: 2280k
#define VOLTAGE_RESISTOR_DOWNSTREAM     ( 1000 ) // Real 1.009k
HLW8012 hlw8012;
#endif

#if Power1_device  == 3             // Sonoff POW
#include "HLW8012.h"
/*
  GPIO 05 SEL Pin HLW8012 Sel input (Sonoff Pow)
  GPIO 13 CF1 Pin HLW8012 CF1 voltage / current (Sonoff Pow)
  GPIO 14 CF Pin HLW8012 CF power (Sonoff Pow)
*/
// GPIOs
#define SEL_PIN                         5
#define CF1_PIN                         13
#define CF_PIN                          14
#define CURRENT_MODE                    HIGH
// These are the nominal values for the resistors in the circuit
#define CURRENT_RESISTOR                0.001
#define VOLTAGE_RESISTOR_UPSTREAM       ( 5 * 470000 ) // Real: 2280k
#define VOLTAGE_RESISTOR_DOWNSTREAM     ( 1000 ) // Real 1.009k
HLW8012 hlw8012;
#endif


float tV, tA, tW, tWh;

String Power_DEF        = "POW";                            // identificativo del nodo (3 lettere) (serve all'app per riconoscere il tipo di nodo)
String Power_Topic_ACK_suffisso = "/ack";
String Power_STATO   = "stato";

typedef struct {
  String topic;
  String topic_ACK;
  float V = 0.0;
  float A = 0.0;
  float W = 0.0;
  float Wh = 0.0;
} InfoRecordPowerType ;

InfoRecordPowerType Power[1];

void setup_Power() {
  Power[0].topic = Power1_Topic;
  Power[0].topic_ACK = Power1_Topic;
  Power[0].topic_ACK += Power_Topic_ACK_suffisso;

  for (int i = 0; i < Power_nodi; i++) {
    Debug_MSG_LN("- Power " + String(i + 1) + " : " + Power[i].topic);
#if defined(Power_DISPLAY)
    Debug_MSG_LN("- Display " + String(i + 1) + " : " + Power_DISPLAY);
#endif

#if Power1_device  == 1
    //hwserial.swap();           // (optionally) swap hw_serial pins to gpio13(rx),15(tx)
#endif

#if Power1_device  == 2 or Power1_device  == 3
    hlw8012.begin(CF_PIN, CF1_PIN, SEL_PIN, CURRENT_MODE, false, 500000);
    hlw8012.setResistors(CURRENT_RESISTOR, VOLTAGE_RESISTOR_UPSTREAM, VOLTAGE_RESISTOR_DOWNSTREAM);
    Debug_MSG_LN("[HLW] Default current multiplier : " + (String)hlw8012.getCurrentMultiplier());
    Debug_MSG_LN("[HLW] Default voltage multiplier : " + (String)hlw8012.getVoltageMultiplier());
    Debug_MSG_LN("[HLW] Default power multiplier   : " + (String)hlw8012.getPowerMultiplier());
#endif

  }
}

void ACK_Power() {
  String payload;
  for (int i = 0; i < Power_nodi; i++) {
    payload = "{\"MAC_address\":\"";
    payload += macToStr(mac);
    payload += "\",\"NODO\":\"";
    payload += Power_DEF;
    payload += "\",\"Topic\":\"";
    payload += Power[i].topic;
    payload += "\",\"Topic_ACK\":\"";
    payload += Power[i].topic_ACK;
    payload += "\",\"availability_topic\":\"";
    payload += WillTopicStr;
    payload += "\"}";
    client.publish(ACK_Topic, (char*) payload.c_str());
    delay(500);
    Stato_Power(i);
  }
}

void HA_discovery_Power() {

}

void Stato_Power(int i) {
  String payload;
  payload = "{\"NODO\":\"";
  payload += Power_DEF;
  payload += "\",\"V\":\"";
  payload += Power[i].V;
  payload += "\",\"A\":\"";
  payload += Power[i].A;
  payload += "\",\"W\":\"";
  payload += Power[i].W;
  payload += "\",\"Wh\":\"";
  payload += Power[i].Wh;
  payload += "\"}";
  client.publish((char*) Power[i].topic_ACK.c_str(), (char*) payload.c_str());
  delay(250);

#if defined(Power_DISPLAY)
  payload = "1=" + (String)Power[i].V + "V";
  client.publish((char*)Power_DISPLAY, (char*) payload.c_str());
  delay(100);
  payload = "2=" + (String)Power[i].W + "W";
  client.publish((char*)Power_DISPLAY, (char*) payload.c_str());
  delay(100);
  payload = "3=" + (String)Power[i].Wh + "Wh";
  client.publish((char*)Power_DISPLAY, (char*) payload.c_str());
  delay(100);
#endif
}

void Subscribe_Power() {
  for (int i = 0; i < Power_nodi; i++) {
    client.subscribe((char*) Power[i].topic.c_str());
    delay(250);
    Debug_MSG_LN("MQTT subscribe " + String(Power[i].topic));
  }
}

void callback_Power(char* topic, byte * message, unsigned int length) {
  String payload;
  String message_String;
  for (int i = 0; i < Power_nodi; i++) {
    if (String(topic) == Power[i].topic) {
      message_String = "";
      for (int ii = 0; ii < length; ii++) {
        message_String += (char)message[ii];
      }
      if (message_String == Power_STATO) {
        Stato_Power(i);
      }
#if Power1_device  == 2
      if ((char)message[0] == 'C') {
        message_String = message_String.substring(1);
        char buf[message_String.length()];
        message_String.toCharArray(buf, message_String.length() + 1);
        int val_cal = atol(buf);
        Debug_MSG_LN("Calibro il BlitzWolf con " + (String)val_cal + "W");
        calibrate(val_cal);
      }
#endif

    }
  }
}

void loop_Power() {
#if Power1_device == 1
  if (!pzemrdy) {
    pzemrdy = pzem.setAddress(ip);
    unblockingDelay(1000);
    return;
  }
#endif
  if (Power_timer.Wait(Power_TEMPO)) {
#if Power1_device == 1
    Debug_MSG_LN("Leggo valori dal PZEM_004");
    tV = pzem.voltage(ip);
    Debug_MSG_LN((String) tV + "V");
    tA = pzem.current(ip);
    Debug_MSG_LN((String) tA + "A");
    tW = pzem.power(ip);
    Debug_MSG_LN((String) tW + "W");
    tWh = pzem.energy(ip);
    Debug_MSG_LN((String) tWh + "Wh");
    if (isnan(tV) || isnan(tA) || isnan(tW) || isnan(tWh) || tV < 0 || tA < 0 || tW < 0 || tWh < 0) {
      Debug_MSG_LN("- nan o <0 -");
      return;
    }
    if ((Power[0].V != tV) or (Power[0].A != tA) or (Power[0].W != tW) or (Power[0].Wh != tWh)) {
      Debug_MSG_LN("OK");
      Power[0].V = tV;
      Power[0].A = tA;
      Power[0].W = tW;
      Power[0].Wh = tWh;
      Stato_Power(0);
    }
#endif

#if Power1_device  == 2 or Power1_device  == 3
    if (Power1_device == 2) Debug_MSG_LN("Leggo valori dal BlitzWolf");
    if (Power1_device == 3) Debug_MSG_LN("Leggo valori dal Sonoff POW");
    tV = hlw8012.getVoltage();
    Debug_MSG_LN((String) tV + "V");
    tA = hlw8012.getCurrent();
    Debug_MSG_LN((String) tA + "A");
    tW = hlw8012.getActivePower();
    Debug_MSG_LN((String) tW + "W");
    if (isnan(tV) || isnan(tA) || isnan(tW) || tV < 0 || tA < 0 || tW < 0 ) {
      Debug_MSG_LN("- nan o <0 -");
      return;
    }
    if ((Power[0].V != tV) or (Power[0].A != tA) or (Power[0].W != tW) or (Power[0].Wh != tWh)) {
      Debug_MSG_LN("OK");
      Power[0].V = tV;
      Power[0].A = tA;
      Power[0].W = tW;
      Stato_Power(0);
    }

    Debug_MSG_LN("[HLW] Active Power (W)    : " + (String)hlw8012.getActivePower());
    Debug_MSG_LN("[HLW] Voltage (V)         : " + (String)hlw8012.getVoltage());
    Debug_MSG_LN("[HLW] Current (A)         : " + (String)hlw8012.getCurrent());
    Debug_MSG_LN("[HLW] Apparent Power (VA) : " + (String)hlw8012.getApparentPower());
    Debug_MSG_LN("[HLW] Power Factor (%)    : " + (String)(100 * hlw8012.getPowerFactor()));
    hlw8012.toggleMode();
#endif
  }
}
#if Power1_device  == 2 or Power1_device  == 3
void calibrate(int Val_cal) {
  // Let's first read power, current and voltage
  // with an interval in between to allow the signal to stabilise:
  hlw8012.getActivePower();
  hlw8012.setMode(MODE_CURRENT);
  unblockingDelay(2000);
  hlw8012.getCurrent();
  hlw8012.setMode(MODE_VOLTAGE);
  unblockingDelay(2000);
  hlw8012.getVoltage();
  // Calibrate using a Val_cal W (pure resistive) on a 230V line
  hlw8012.expectedActivePower(Val_cal);
  hlw8012.expectedVoltage(230.0);
  hlw8012.expectedCurrent(Val_cal / 230.0);
  // Show corrected factors
  Debug_MSG_LN("[HLW] New current multiplier : " + (String)hlw8012.getCurrentMultiplier());
  Debug_MSG_LN("[HLW] New voltage multiplier : " + (String)hlw8012.getVoltageMultiplier());
  Debug_MSG_LN("[HLW] New power multiplier   : " + (String)hlw8012.getPowerMultiplier());
}

#endif
#endif
