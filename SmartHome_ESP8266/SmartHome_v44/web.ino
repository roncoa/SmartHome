#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
//#include <ESP8266mDNS.h>

//MDNSResponder mdns;
ESP8266WebServer webserver(80);

String webPage = "";
String webPage_REBOOT = "";


void setup_web() {

    
//  if (mdns.begin("esp8266"),WiFi.localIP()) {
//    Debug_MSG_LN("MDNS responder started");
//  }
//  else {
//    Debug_MSG_LN("MDNS responder NOT started");
//  }
//        mdns.addService("http", "tcp", 80);

  webPage += "<html>";
  webPage += "<head><title>SmartHome v" + Versione + "</title></head>";
  webPage += "<body><h1>SmartHome v" + Versione + " </h1>";
  if (NODI_TAPPARELLA > 0) {
    webPage += "<p>" + (String)Tapparella1_Topic + "  <a href=\"tap1su\"><button>SU</button></a>&nbsp;<a href=\"tap1giu\"><button>GIU</button></a>&nbsp;<a href=\"tap1stop\"><button>STOP</button></a></p>";
  }
  if (NODI_TAPPARELLA > 1) {
    webPage += "<p>" + (String)Tapparella2_Topic + "  <a href=\"tap2su\"><button>SU</button></a>&nbsp;<a href=\"tap2giu\"><button>GIU</button></a>&nbsp;<a href=\"tap2stop\"><button>STOP</button></a></p>";
  }
  if (NODI_INTERRUTTORE > 0) {
    webPage += "<p>" + (String)Interruttore1_Topic + "  <a href=\"int1aon\"><button>Rele 1 ON</button></a>&nbsp;<a href=\"int1aoff\"><button>Rele 1 OFF</button></a>&nbsp;<a href=\"int1bon\"><button>Rele 2 ON</button></a>&nbsp;<a href=\"int1boff\"><button>Rele 2 OFF</button></a></p>";
  }
  if (NODI_INTERRUTTORE > 1) {
    webPage += "<p>" + (String)Interruttore2_Topic + "  <a href=\"int2aon\"><button>Rele 1 ON</button></a>&nbsp;<a href=\"int2aoff\"><button>Rele 1 OFF</button></a>&nbsp;<a href=\"int2bon\"><button>Rele 2 ON</button></a>&nbsp;<a href=\"int2boff\"><button>Rele 2 OFF</button></a></p>";
  }
  if (NODI_INTERRUTTORE > 2) {
    webPage += "<p>" + (String)Interruttore3_Topic + "  <a href=\"int3aon\"><button>Rele 1 ON</button></a>&nbsp;<a href=\"int3aoff\"><button>Rele 1 OFF</button></a>&nbsp;<a href=\"int3bon\"><button>Rele 2 ON</button></a>&nbsp;<a href=\"int3boff\"><button>Rele 2 OFF</button></a></p>";
  }
  if (NODI_INTERRUTTORE > 3) {
    webPage += "<p>" + (String)Interruttore4_Topic + "  <a href=\"int4aon\"><button>Rele 1 ON</button></a>&nbsp;<a href=\"int4aoff\"><button>Rele 1 OFF</button></a>&nbsp;<a href=\"int4bon\"><button>Rele 2 ON</button></a>&nbsp;<a href=\"int4boff\"><button>Rele 2 OFF</button></a></p>";
  }
  if (NODI_CANCELLO > 0) {
    webPage += "<p>" + (String)Cancello1_Topic + "  <a href=\"can1aon\"><button>Rele 1 ON</button></a>&nbsp;<a href=\"can1bon\"><button>Rele 2 ON</button></a></p>";
  }
  if (NODI_CANCELLO > 1) {
    webPage += "<p>" + (String)Cancello2_Topic + "  <a href=\"can2aon\"><button>Rele 1 ON</button></a>&nbsp;<a href=\"can2bon\"><button>Rele 2 ON</button></a></p>";
  }
  webPage += "<p>UTILITY  <a href=\"reboot\"><button>Reboot</button></a>&nbsp</p>";
  webPage_REBOOT += "</body></html>";

  webPage_REBOOT += "<html>";

  webPage_REBOOT += "<head><meta http-equiv=""refresh"" content=""10;url=/"" />";
  //  webPage_REBOOT += "<head><meta http-equiv=""refresh"" content=""10;url=http://" + WiFi.localIP().toString() + """ />";
  webPage_REBOOT += "<title>SmartHome v" + Versione +  "</title></head>";
  webPage_REBOOT += "<body><h1>REBOOTING in 10 sec.</h1>";
  webPage_REBOOT += "</body></html>";

  webserver.on("/", []() {
    webserver.send(200, "text/html", webPage);
  });

  // tapparella 1
  webserver.on("/tap1su", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /tap1su");
    digitalWrite(RELE_tapparella1_GIU, 0 ^ Flag_inversione_RELE);                       // RELE GIU = 0
    delay(TEMPO_RELE);                                                                  // Aspetto
    digitalWrite(RELE_tapparella1_SU, 1 ^ Flag_inversione_RELE);                        // RELE SU = 1
#if defined(SONOFFDUAL)
    SONOFFDUAL_A_ON_B_OFF();
#endif
    In_movimento_tapparella1 = true;
    ulTimeTapparella1 = millis();
  });
  webserver.on("/tap1giu", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /tap1giu");
    digitalWrite(RELE_tapparella1_SU, 0 ^ Flag_inversione_RELE);                        // RELE SU = 0
    delay(TEMPO_RELE);                                                                  // Aspetto
    digitalWrite(RELE_tapparella1_GIU, 1 ^ Flag_inversione_RELE);                       // RELE GIU = 1
#if defined(SONOFFDUAL)
    SONOFFDUAL_A_OFF_B_ON();
#endif
    In_movimento_tapparella1 = true;
    ulTimeTapparella1 = millis();
  });
  webserver.on("/tap1stop", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /tap1stop");
    digitalWrite(RELE_tapparella1_SU, 0 ^ Flag_inversione_RELE);                        // RELE SU = 0
    delay(TEMPO_RELE);                                                                  // Aspetto
    digitalWrite(RELE_tapparella1_GIU, 0 ^ Flag_inversione_RELE);                       // RELE GIU = 0
#if defined(SONOFFDUAL)
    SONOFFDUAL_AB_OFF();
#endif
    In_movimento_tapparella1 = false;
  });

  // tapparella 2
  webserver.on("/tap2su", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /tap2su");
    digitalWrite(RELE_tapparella2_GIU, 0 ^ Flag_inversione_RELE);                       // RELE GIU = 0
    delay(TEMPO_RELE);                                                                  // Aspetto
    digitalWrite(RELE_tapparella2_SU, 1 ^ Flag_inversione_RELE);                        // RELE SU = 1
#if defined(SONOFFDUAL)
    SONOFFDUAL_A_ON_B_OFF();
#endif
    In_movimento_tapparella2 = true;
    ulTimeTapparella2 = millis();
  });
  webserver.on("/tap2giu", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /tap2giu");
    digitalWrite(RELE_tapparella2_SU, 0 ^ Flag_inversione_RELE);                        // RELE SU = 0
    delay(TEMPO_RELE);                                                                  // Aspetto
    digitalWrite(RELE_tapparella2_GIU, 1 ^ Flag_inversione_RELE);                       // RELE GIU = 1
#if defined(SONOFFDUAL)
    SONOFFDUAL_A_OFF_B_ON();
#endif
    In_movimento_tapparella2 = true;
    ulTimeTapparella2 = millis();
  });
  webserver.on("/tap2stop", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /tap2stop");
    digitalWrite(RELE_tapparella2_SU, 0 ^ Flag_inversione_RELE);                        // RELE SU = 0
    delay(TEMPO_RELE);                                                                  // Aspetto
    digitalWrite(RELE_tapparella2_GIU, 0 ^ Flag_inversione_RELE);                       // RELE GIU = 0
#if defined(SONOFFDUAL)
    SONOFFDUAL_AB_OFF();
#endif
    In_movimento_tapparella2 = false;
  });

  //interruttore 1
  webserver.on("/int1aon", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int1aon");
    digitalWrite(RELE_A_interruttore1, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 1
    delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
    SONOFFDUAL_A_ON();
#endif
#if defined(ESP01_SERIAL_RELE)
    ESP01_SERIAL_RELE_AON();
#endif
    EEPROM_write_releAinterruttore1();
  });
  webserver.on("/int1bon", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int1bon");
    digitalWrite(RELE_B_interruttore1, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 2
    delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
    SONOFFDUAL_B_ON();
#endif
    EEPROM_write_releBinterruttore1();
  });
  webserver.on("/int1aoff", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int1aoff");
    digitalWrite(RELE_A_interruttore1, 0 ^ Flag_inversione_RELE);                                     // Spengo il RELE 1
    delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
    SONOFFDUAL_A_OFF();
#endif
#if defined(ESP01_SERIAL_RELE)
    ESP01_SERIAL_RELE_AOFF();
#endif
    EEPROM_write_releAinterruttore1();
  });
  webserver.on("/int1boff", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int1boff");
    digitalWrite(RELE_B_interruttore1, 0 ^ Flag_inversione_RELE);                                     // Spengo il RELE 2
    delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
    SONOFFDUAL_B_OFF();
#endif
    EEPROM_write_releBinterruttore1();
  });

  //interruttore 2
  webserver.on("/int2aon", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int2aon");
    digitalWrite(RELE_A_interruttore2, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 1
    delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
    SONOFFDUAL_A_ON();
#endif
    EEPROM_write_releAinterruttore2();
  });
  webserver.on("/int2bon", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int2bon");
    digitalWrite(RELE_B_interruttore2, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 2
    delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
    SONOFFDUAL_B_ON();
#endif
    EEPROM_write_releBinterruttore2();
  });
  webserver.on("/int2aoff", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int2aoff");
    digitalWrite(RELE_A_interruttore2, 0 ^ Flag_inversione_RELE);                                     // Spengo il RELE 1
    delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
    SONOFFDUAL_A_OFF();
#endif
    EEPROM_write_releAinterruttore2();
  });
  webserver.on("/int2boff", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int2boff");
    digitalWrite(RELE_B_interruttore2, 0 ^ Flag_inversione_RELE);                                     // Spengo il RELE 2
    delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
    SONOFFDUAL_B_OFF();
#endif
    EEPROM_write_releBinterruttore2();
  });

  //interruttore 3
  webserver.on("/int3aon", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int3aon");
    digitalWrite(RELE_A_interruttore3, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 1
    delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
    SONOFFDUAL_A_ON();
#endif
    EEPROM_write_releAinterruttore3();
  });
  webserver.on("/int3bon", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int3bon");
    digitalWrite(RELE_B_interruttore3, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 2
    delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
    SONOFFDUAL_B_ON();
#endif
    EEPROM_write_releBinterruttore3();
  });
  webserver.on("/int3aoff", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int3aoff");
    digitalWrite(RELE_A_interruttore3, 0 ^ Flag_inversione_RELE);                                     // Spengo il RELE 1
    delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
    SONOFFDUAL_A_OFF();
#endif
    EEPROM_write_releAinterruttore3();
  });
  webserver.on("/int3boff", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int3boff");
    digitalWrite(RELE_B_interruttore3, 0 ^ Flag_inversione_RELE);                                     // Spengo il RELE 2
    delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
    SONOFFDUAL_B_OFF();
#endif
    EEPROM_write_releBinterruttore3();
  });

  //interruttore 4
  webserver.on("/int4aon", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int4aon");
    digitalWrite(RELE_A_interruttore4, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 1
    delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
    SONOFFDUAL_A_ON();
#endif
    EEPROM_write_releAinterruttore4();
  });
  webserver.on("/int4bon", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int4bon");
    digitalWrite(RELE_B_interruttore4, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 2
    delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
    SONOFFDUAL_B_ON();
#endif
    EEPROM_write_releBinterruttore4();
  });
  webserver.on("/int4aoff", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int4aoff");
    digitalWrite(RELE_A_interruttore4, 0 ^ Flag_inversione_RELE);                                     // Spengo il RELE 1
    delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
    SONOFFDUAL_A_OFF();
#endif
    EEPROM_write_releAinterruttore4();
  });
  webserver.on("/int4boff", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int4boff");
    digitalWrite(RELE_B_interruttore4, 0 ^ Flag_inversione_RELE);                                     // Spengo il RELE 2
    delay(TEMPO_RELE);                                                                  // Aspetto
#if defined(SONOFFDUAL)
    SONOFFDUAL_B_OFF();
#endif
    EEPROM_write_releBinterruttore4();
  });

  //cancello 1
  webserver.on("/can1aon", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /can1aon");
    digitalWrite(RELE_A_cancello1, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 1
#if defined(SONOFFDUAL)
    SONOFFDUAL_A_ON();
#endif
#if defined(ESP01_SERIAL_RELE)
    ESP01_SERIAL_RELE_AON();
#endif
    Rele_A_eccitato_cancello1 = true;
    ulTimeReleACancello1 = millis();
  });
  webserver.on("/can1bon", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /can1bon");
    digitalWrite(RELE_B_cancello1, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 2
#if defined(SONOFFDUAL)
    SONOFFDUAL_B_ON();
#endif
    Rele_B_eccitato_cancello1 = true;
    ulTimeReleBCancello1 = millis();
  });

  //cancello 2
  webserver.on("/can2aon", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /can2aon");
    digitalWrite(RELE_A_cancello2, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 1
#if defined(SONOFFDUAL)
    SONOFFDUAL_A_ON();
#endif
    Rele_A_eccitato_cancello2 = true;
    ulTimeReleACancello2 = millis();
  });
  webserver.on("/can2bon", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /can2bon");
    digitalWrite(RELE_B_cancello2, 1 ^ Flag_inversione_RELE);                                     // Accendo il RELE 2
#if defined(SONOFFDUAL)
    SONOFFDUAL_B_ON();
#endif
    Rele_B_eccitato_cancello2 = true;
    ulTimeReleBCancello2 = millis();
  });

  webserver.on("/reboot", []() {
    webserver.send(200, "text/html", webPage_REBOOT);
    webserver.handleClient();
    Debug_MSG_LN("Message arrived [web] /reboot");
    Debug_MSG_LN("RIAVVIO L'ESP (da web)");
    delay(500);
    ESP.restart();
  });

  webserver.begin();
  Debug_MSG_LN("HTTP server started");
}

void loop_web() {
  webserver.handleClient();
}

