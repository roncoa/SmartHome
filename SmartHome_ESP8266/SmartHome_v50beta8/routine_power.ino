#if Power_nodi>0

/*
  Power1_device
  1 = PZEM004
  2 = PZEM004 V3
  3 = BlitzWolf
  4 = Sonoff POW
*/

//#define Power_DISPLAY "Display/power"
#define Power_TEMPO                5000

TPush Power_timer;

#if Power1_device  == 1             // PZEM004
#include <PZEM004T.h>
PZEM004T pzem(Power1_GPIO_RX, Power1_GPIO_TX); // (3,1)(RX,TX) connect to TX,RX of PZEM
IPAddress ip_pzem004(0, 0, 0, 1);
#endif

#if Power1_device  == 2             // PZEM004 V3
#include <PZEM004Tv30.h>
PZEM004Tv30 pzem(Power1_GPIO_RX, Power1_GPIO_TX); // (3,1)(RX,TX) connect to TX,RX of PZEM
uint8_t addr_pzem004v3 = 0x01;
#endif

#if Power1_device  == 3             // BlitzWolf
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

#if Power1_device  == 4             // Sonoff POW
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
boolean Started = false;
boolean KWh = true;
int old_day, old_week, old_month;

typedef struct {
  String topic;
  String topic_ACK;
  float V = 0.0;
  float A = 0.0;
  float W = 0.0;
  float Wh = 0.0;
  float VA = 0.0;
  float VAr = 0.0;
  float PF = 0.0;
  float freq = 0.0;
  float Wh_parziale = 0.0;
  float Wh_oggi = 0.0;
  float Wh_settimanale = 0.0;
  float Wh_mensile = 0.0;

  float Wh_ieri = 0.0;
  float Wh_settimana_scorsa = 0.0;
  float Wh_mese_scorso = 0.0;

  float Wh_zero_parziale = 0.0;
  float Wh_zero_oggi = 0.0;
  float Wh_zero_settimanale = 0.0;
  float Wh_zero_mensile = 0.0;

} InfoRecordPowerType ;

InfoRecordPowerType Power[1];

void setup_Power() {
  Power[0].topic = Power1_Topic;
  Power[0].topic_ACK = Power1_Topic;
  Power[0].topic_ACK += Power_Topic_ACK_suffisso;
  for (int i = 0; i < Power_nodi; i++) {
    Debug_MSG_LN("Nodo Power " + String(i + 1) + " : " + Power[i].topic);
#if defined(Power_DISPLAY)
    Debug_MSG_LN(" Display Power " + String(i + 1) + " : " + Power_DISPLAY);
#endif

#if Power1_device  == 1
    pzem.setAddress(ip_pzem004);
#endif
#if Power1_device  == 2
  //  pzem.setAddress(0x01);
#endif
#if Power1_device  == 3 or Power1_device  == 4
    hlw8012.begin(CF_PIN, CF1_PIN, SEL_PIN, CURRENT_MODE, false, 500000);
    hlw8012.setResistors(CURRENT_RESISTOR, VOLTAGE_RESISTOR_UPSTREAM, VOLTAGE_RESISTOR_DOWNSTREAM);
    Debug_MSG_LN(" [HLW] Default current multiplier : " + (String)hlw8012.getCurrentMultiplier());
    Debug_MSG_LN(" [HLW] Default voltage multiplier : " + (String)hlw8012.getVoltageMultiplier());
    Debug_MSG_LN(" [HLW] Default power multiplier   : " + (String)hlw8012.getPowerMultiplier());
#endif
  }
  EEPROM_READ_Power_zero(0);
  EEPROM_READ_Power_Wh_KWh();
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
    MQTTPublish(ACK_Topic, payload);
    Stato_POW(i);
  }
}

void HA_discovery_Power() {

}

void Stato_POW(int i) {
  String payload;
  payload = "{\"NODO\":\"";
  payload += Power_DEF;
  payload += "\",\"V\":\"";
  payload += Power[i].V;
  payload += "\",\"A\":\"";
  payload += Power[i].A;
  payload += "\",\"W\":\"";
  payload += Power[i].W;
  payload += "\",\"VA\":\"";
  payload += Power[i].VA;
  payload += "\",\"VAr\":\"";
  payload += Power[i].VAr;
  payload += "\",\"PF\":\"";
  payload += Power[i].PF;
  payload += "\",\"freq\":\"";
  payload += Power[i].freq;
  payload += "\",\"Wh\":\"";
  payload += Power[0].Wh;
  payload += "\",\"WhParziale\":\"";
  payload += Power[0].Wh_parziale;
  payload += "\",\"WhOggi\":\"";
  payload += Power[0].Wh_oggi;
  payload += "\",\"WhSett\":\"";
  payload += Power[0].Wh_settimanale;
  payload += "\",\"WhMese\":\"";
  payload += Power[0].Wh_mensile;
  payload += "\",\"WhIeri\":\"";
  payload += Power[0].Wh_ieri;
  payload += "\",\"WhSettSc\":\"";
  payload += Power[0].Wh_settimana_scorsa;
  payload += "\",\"WhMeseSc\":\"";
  payload += Power[0].Wh_mese_scorso;
  payload += "\",\"KWh\":\"";
  payload += (String)(Power[0].Wh / 1000);
  payload += "\",\"KWhParziale\":\"";
  payload += (String)(Power[0].Wh_parziale / 1000);
  payload += "\",\"KWhOggi\":\"";
  payload += (String)(Power[0].Wh_oggi / 1000);
  payload += "\",\"KWhSett\":\"";
  payload += (String)(Power[0].Wh_settimanale / 1000);
  payload += "\",\"KWhMese\":\"";
  payload += (String)(Power[0].Wh_mensile / 1000);
  payload += "\",\"KWhIeri\":\"";
  payload += (String)(Power[0].Wh_ieri / 1000);
  payload += "\",\"KWhSettSc\":\"";
  payload += (String)(Power[0].Wh_settimana_scorsa / 1000);
  payload += "\",\"KWhMeseSc\":\"";
  payload += (String)(Power[0].Wh_mese_scorso / 1000);
  payload += "\"}";
  MQTTPublish(Power[i].topic_ACK, payload);
#if defined(Power_DISPLAY)
  payload = "1= " + (String)Power[i].V + " V";
  MQTTPublish(Power_DISPLAY, payload);
  payload = "2= " + (String)Power[i].A + " A";
  MQTTPublish(Power_DISPLAY, payload);
  payload = "3= " + (String)Power[i].W + " W";
  MQTTPublish(Power_DISPLAY, payload);
  if (KWh) payload = "4= " + (String)Power[0].Wh + " KWh";
  else payload = "4= " + (String)(Power[0].Wh / 1000) + " Wh";
  MQTTPublish(Power_DISPLAY, payload);
#endif
}

void Subscribe_Power() {
  for (int i = 0; i < Power_nodi; i++) {
    MQTTSubscribe(Power[i].topic);
    Debug_MSG_LN("MQTT subscribe " + String(Power[i].topic));
  }
}

void callback_Power(char* topic, char* message) {
  String payload;
  for (int i = 0; i < Power_nodi; i++) {
    if (String(topic) == Power[i].topic) {
      if ((String)message == Power_STATO) {
        Stato_POW(i);
      }
#if Power1_device  == 1 or Power1_device  == 2
      if ((char)message[0] == 'z') {
        if (KWh) Debug_MSG_LN("Azzero KWh");
        else Debug_MSG_LN("Azzero Wh");
        Power[0].Wh_zero_parziale = Power[0].Wh;
        EEPROM_WRITE_Power_zero(0);
        EEPROM_READ_Power_zero(0);
      }
#endif
#if Power1_device  == 3
      if ((char)message[0] == 'c') {
        String stringtmp = (String)message;
        stringtmp = stringtmp.substring(1);
        char buf[stringtmp.length()];
        stringtmp.toCharArray(buf, stringtmp.length() + 1);
        int val_cal = atol(buf);
        Debug_MSG_LN("Calibro il BlitzWolf con " + (String)val_cal + "W");
        calibrate(val_cal);
      }
#endif
    }
  }
}

void loop_Power() {

  int new_day, new_week, new_month;
  if (cbtime_set == true) {
    now = dstAdjusted.time(nullptr);
    char buffer[80];
    String tmp;
    strftime (buffer, 80, "%j", localtime(&now)); // %j Day of year as decimal number (001-366)
    tmp = (String)buffer;
    new_day = tmp.toInt();
    strftime (buffer, 80, "%W", localtime(&now)); // %W Week of year as decimal number, Monday as first day of week (00-51)
    tmp = (String)buffer;
    new_week = tmp.toInt();
    strftime (buffer, 80, "%m", localtime(&now)); // %m Month as decimal number (01-12)
    tmp = (String)buffer;
    new_month = tmp.toInt();
    if (!Started) {
      Started = true;
      old_day = new_day;
      old_week = new_week;
      old_month = new_month;
    }
    if (new_day != old_day) {
      Debug_MSG_LN("*** new day ***");
      old_day = new_day;
      Power[0].Wh_ieri = Power[0].Wh - Power[0].Wh_zero_oggi;
      Power[0].Wh_zero_oggi = Power[0].Wh;
      EEPROM_WRITE_Power_zero(0);
      EEPROM_READ_Power_zero(0);
    }
    if (new_week != old_week) {
      Debug_MSG_LN("*** new week ***");
      old_week = new_week;
      Power[0].Wh_settimana_scorsa = Power[0].Wh - Power[0].Wh_zero_settimanale;
      Power[0].Wh_zero_settimanale = Power[0].Wh;
      EEPROM_WRITE_Power_zero(0);
      EEPROM_READ_Power_zero(0);
    }
    if (new_month != old_month) {
      Debug_MSG_LN("*** new month ***");
      old_month = new_month;
      Power[0].Wh_mese_scorso = Power[0].Wh - Power[0].Wh_zero_mensile;
      Power[0].Wh_zero_mensile = Power[0].Wh;
      EEPROM_WRITE_Power_zero(0);
      EEPROM_READ_Power_zero(0);
    }
  }

  if (Power_timer.Wait(Power_TEMPO)) {
#if Power1_device == 1
    Debug_MSG_LN("Leggo valori dal PZEM_004");
    tV = pzem.voltage(ip_pzem004);
    Debug_MSG_LN((String) tV + "V");
    tA = pzem.current(ip_pzem004);
    Debug_MSG_LN((String) tA + "A");
    tW = pzem.power(ip_pzem004);
    Debug_MSG_LN((String) tW + "W");
    tWh = pzem.energy(ip_pzem004);
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
      Power[0].PF = tW / (tV * tA);
      if (isnan(Power[0].PF)) Power[0].PF=0;
      Power[0].VA = Power[0].A * Power[0].V;
      Power[0].VAr = Power[0].VA * sin(acos(Power[0].PF));
      Power[0].Wh_parziale = Power[0].Wh - Power[0].Wh_zero_parziale;
      Power[0].Wh_oggi = Power[0].Wh - Power[0].Wh_zero_oggi;
      Power[0].Wh_settimanale = Power[0].Wh - Power[0].Wh_zero_settimanale;
      Power[0].Wh_mensile = Power[0].Wh - Power[0].Wh_zero_mensile;
      if (Power[0].Wh_parziale < 0) Power[0].Wh_parziale = 0;
      if (Power[0].Wh_oggi < 0) Power[0].Wh_oggi = 0;
      if (Power[0].Wh_settimanale < 0) Power[0].Wh_settimanale = 0;
      if (Power[0].Wh_mensile < 0) Power[0].Wh_mensile = 0;
      Stato_POW(0);
    }
#endif

#if Power1_device  == 2
    Debug_MSG_LN("Leggo valori dal PZEM_004 V3");
    tV = pzem.voltage();
    Debug_MSG_LN((String) tV + "V");
    tA = pzem.current();
    Debug_MSG_LN((String) tA + "A");
    tW = pzem.power();
    Debug_MSG_LN((String) tW + "W");
    tWh = pzem.energy();
    Debug_MSG_LN((String) tWh + "Wh");
    Power[0].PF = pzem.pf();
    Debug_MSG_LN((String) Power[0].PF + " (cosfi)");
    Power[0].freq = pzem.frequency();
    Debug_MSG_LN((String) Power[0].freq + "Hz");
    if (isnan(tV) || isnan(tA) || isnan(tW) || isnan(tWh) || isnan(Power[0].PF) || isnan(Power[0].freq) || tV < 0 || tA < 0 || tW < 0 || tWh < 0 || Power[0].PF < 0 || tV < Power[0].freq) {
      Debug_MSG_LN("- nan o <0 -");
      return;
    }
    if ((Power[0].V != tV) or (Power[0].A != tA) or (Power[0].W != tW) or (Power[0].Wh != tWh)) {
      Debug_MSG_LN("OK");
      Power[0].V = tV;
      Power[0].A = tA;
      Power[0].W = tW;
      Power[0].Wh = tWh;
      Power[0].VA = Power[0].A * Power[0].V;
      Power[0].VAr = Power[0].VA * sin(acos(Power[0].PF));
      Power[0].Wh_parziale = Power[0].Wh - Power[0].Wh_zero_parziale;
      Power[0].Wh_oggi = Power[0].Wh - Power[0].Wh_zero_oggi;
      Power[0].Wh_settimanale = Power[0].Wh - Power[0].Wh_zero_settimanale;
      Power[0].Wh_mensile = Power[0].Wh - Power[0].Wh_zero_mensile;
      if (Power[0].Wh_parziale < 0) Power[0].Wh_parziale = 0;
      if (Power[0].Wh_oggi < 0) Power[0].Wh_oggi = 0;
      if (Power[0].Wh_settimanale < 0) Power[0].Wh_settimanale = 0;
      if (Power[0].Wh_mensile < 0) Power[0].Wh_mensile = 0;
      Stato_POW(0);
    }
#endif

#if Power1_device  == 3 or Power1_device  == 4
    if (Power1_device == 3) Debug_MSG_LN("Leggo valori dal BlitzWolf");
    if (Power1_device == 4) Debug_MSG_LN("Leggo valori dal Sonoff POW");
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
      Stato_POW(0);
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

#if Power1_device  == 3 or Power1_device  == 4
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

void EEPROM_READ_Power_zero(int i) {
  Debug_MSG_LN("Reading EEPROM Power " + String(i + 1) + " :");
  Power[0].Wh_zero_parziale = eepromReadFloat(EEPROM_Power_zero);
  Power[0].Wh_zero_oggi = eepromReadFloat(EEPROM_Power_zero + 4);
  Power[0].Wh_zero_settimanale = eepromReadFloat(EEPROM_Power_zero + 8);
  Power[0].Wh_zero_mensile = eepromReadFloat(EEPROM_Power_zero + 12);
  Power[0].Wh_ieri = eepromReadFloat(EEPROM_Power_zero + 16);
  Power[0].Wh_settimana_scorsa = eepromReadFloat(EEPROM_Power_zero + 20);
  Power[0].Wh_mese_scorso = eepromReadFloat(EEPROM_Power_zero + 24);
  if (Power[0].Wh_zero_parziale < 0 or isnan(Power[0].Wh_zero_parziale)) Power[0].Wh_zero_parziale = 0;
  if (Power[0].Wh_zero_oggi <= 0 or isnan(Power[0].Wh_zero_oggi)) Power[0].Wh_zero_oggi = Power[0].Wh;
  if (Power[0].Wh_zero_settimanale <= 0 or isnan(Power[0].Wh_zero_settimanale)) Power[0].Wh_zero_settimanale = Power[0].Wh;
  if (Power[0].Wh_zero_mensile <= 0 or isnan(Power[0].Wh_zero_mensile)) Power[0].Wh_zero_mensile = Power[0].Wh;
  if (Power[0].Wh_ieri <= 0 or isnan(Power[0].Wh_ieri)) Power[0].Wh_ieri = 0;
  if (Power[0].Wh_settimana_scorsa <= 0 or isnan(Power[0].Wh_settimana_scorsa)) Power[0].Wh_settimana_scorsa = 0;
  if (Power[0].Wh_mese_scorso <= 0 or isnan(Power[0].Wh_mese_scorso)) Power[0].Wh_mese_scorso = 0;
  Debug_MSG_LN(" Wh_zero_parziale:" + (String) Power[0].Wh_zero_parziale);
  Debug_MSG_LN(" Wh_zero_oggi:" + (String) Power[0].Wh_zero_oggi);
  Debug_MSG_LN(" Wh_zero_settimanale:" + (String) Power[0].Wh_zero_settimanale);
  Debug_MSG_LN(" Wh_zero_mensile:" + (String) Power[0].Wh_zero_mensile);
  Debug_MSG_LN(" Wh_ieri:" + (String) Power[0].Wh_ieri);
  Debug_MSG_LN(" Wh_settimana_scorsa:" + (String) Power[0].Wh_settimana_scorsa);
  Debug_MSG_LN(" Wh_mese_scorso:" + (String) Power[0].Wh_mese_scorso);
}

void EEPROM_WRITE_Power_zero(int i) {
  Debug_MSG_LN("Writing EEPROM Power " + String(i + 1) + " :");
  eepromWriteFloat(EEPROM_Power_zero, Power[0].Wh_zero_parziale);
  eepromWriteFloat(EEPROM_Power_zero + 4, Power[0].Wh_zero_oggi);
  eepromWriteFloat(EEPROM_Power_zero + 8, Power[0].Wh_zero_settimanale);
  eepromWriteFloat(EEPROM_Power_zero + 12, Power[0].Wh_zero_mensile);
  eepromWriteFloat(EEPROM_Power_zero + 16, Power[0].Wh_ieri);
  eepromWriteFloat(EEPROM_Power_zero + 20, Power[0].Wh_settimana_scorsa);
  eepromWriteFloat(EEPROM_Power_zero + 24, Power[0].Wh_mese_scorso);
  Debug_MSG_LN(" Wh_zero_parziale:" + (String)Power[0].Wh);
  Debug_MSG_LN(" Wh_zero_oggi:" + (String)Power[0].Wh_zero_oggi);
  Debug_MSG_LN(" Wh_zero_settimanale:" + (String)Power[0].Wh_zero_settimanale);
  Debug_MSG_LN(" Wh_zero_mensile:" + (String)Power[0].Wh_zero_mensile);
  Debug_MSG_LN(" Wh_ieri:" + (String)Power[0].Wh_ieri);
  Debug_MSG_LN(" Wh_settimana_scorsa:" + (String)Power[0].Wh_settimana_scorsa);
  Debug_MSG_LN(" Wh_mese_scorso:" + (String)Power[0].Wh_mese_scorso);
}

void EEPROM_READ_Power_Wh_KWh() {
  Debug_MSG("Reading EEPROM Power Wh_KWh :");
  int tmp = eepromRead(EEPROM_Power_Wh_KWh);
  if (tmp<0 or tmp>1 or isnan(tmp)) tmp = 0;
  if (tmp == 0) {
    KWh = false;
    Debug_MSG_LN("Wh");
  }
  if (tmp == 1) {
    KWh = true;
    Debug_MSG_LN("KWh");
  }
}

void EEPROM_WRITE_Power_Wh_KWh() {
  Debug_MSG("Writing EEPROM Power Wh_KWh :");
  if (!KWh) {
    eepromWrite(EEPROM_Power_Wh_KWh, 0);
    Debug_MSG_LN("Wh");
  }
  if (KWh) {
    eepromWrite(EEPROM_Power_Wh_KWh, 1);
    Debug_MSG_LN("KWh");
  }
}

#endif
