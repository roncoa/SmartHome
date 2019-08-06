#ifdef WEB_SERVER

#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>

ESP8266WebServer HttpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

const char* firmware_user = "smarthome";
const char* firmware_password = "";

void setup_web() {
  httpUpdater.setup(&HttpServer, "/firmware", firmware_user, firmware_password);
  HttpServer.on("/", handle_OnConnect);
  HttpServer.on("/reboot", []() {
    String webPage_REBOOT = "<html>";
    webPage_REBOOT += "<head><meta http-equiv=""refresh"" content=""10;url=/"" />";
    webPage_REBOOT += "<title>SmartHome v" + Versione +  "</title></head>";
    webPage_REBOOT += "<body><h1>REBOOTING in 10 sec.</h1>";
    webPage_REBOOT += "</body></html>";
    HttpServer.send(200, "text/html", webPage_REBOOT);
    HttpServer.handleClient();
    Debug_MSG_LN("WEB message arrived /reboot");
    Debug_MSG_LN("RIAVVIO L'ESP (da web)");
    cmd_reboot();
  });
  HttpServer.on("/reset", []() {
    String webPage_RESET = "<html>";
    webPage_RESET += "<head><meta http-equiv=""refresh"" content=""10;url=/"" />";
    webPage_RESET += "<title>SmartHome v" + Versione +  "</title></head>";
    webPage_RESET += "<body><h1>RESET+REBOOT in 10 sec.</h1>";
    webPage_RESET += "</body></html>";
    HttpServer.send(200, "text/html", webPage_RESET);
    HttpServer.handleClient();
    Debug_MSG_LN("WEB message arrived /reset");
    Debug_MSG_LN("RIAVVIO L'ESP (da web)");
    EEPROM_clear_reset();
  });

#if Tapparella_nodi>0
  for (int i = 0; i < Tapparella_nodi; i++)
  {
    HttpServer.on("/tap" + (String)i + "su", [i]() {
      Debug_MSG_LN("WEB message arrived /tap" + (String)i + "su");
      Tapparella_cmd_SU(i, Tapparella[i].tempo_max);
      handle_IndexRedir();
    });
    HttpServer.on("/tap" + (String)i + "giu", [i]() {
      Debug_MSG_LN("WEB message arrived /tap" + (String)i + "giu");
      Tapparella_cmd_GIU(i, Tapparella[i].tempo_max);
      handle_IndexRedir();
    });
    HttpServer.on("/tap" + (String)i + "stop", [i]() {
      Debug_MSG_LN("WEB message arrived /tap" + (String)i + "stop");
      Tapparella_cmd_STOP(i);
      handle_IndexRedir();
    });
    HttpServer.on("/tap" + (String)i + "piu", [i]() {
      Debug_MSG_LN("WEB message arrived /tap" + (String)i +  "piu");
      Tapparella[i].tempo_max = Tapparella[i].tempo_max + 1000;
      if (Tapparella[i].tempo_max > 120000) Tapparella[i].tempo_max = 120000;
      EEPROM_WRITE_Tapparella_tempo_max(i);
      Stato_TAP (i);
      handle_IndexRedir();
    });
    HttpServer.on("/tap" + (String)i + "meno", [i]() {
      Debug_MSG_LN("WEB message arrived /tap" + (String)i +  "meno");
      Tapparella[i].tempo_max = Tapparella[i].tempo_max - 1000;
      if (Tapparella[i].tempo_max < 5000) Tapparella[i].tempo_max = 5000;
      EEPROM_WRITE_Tapparella_tempo_max(i);
      Stato_TAP (i);
      handle_IndexRedir();
    });
  }
#endif

#if Interruttore_nodi>0
  for (int i = 0; i < Interruttore_nodi; i++) {
    HttpServer.on("/int" + (String)i + "on", [i]() {
      Debug_MSG_LN("WEB message arrived /int" + (String)i + "on");
      digitalWrite(Interruttore[i].rele, 1 ^ Flag_inversione_RELE);
#ifdef SONOFFDUAL
      if (i = 0) SONOFFDUAL_A_ON();
      if (i = 1) SONOFFDUAL_B_ON();
#endif
#ifdef ESP01_SERIAL_RELE
      ESP01_SERIAL_RELE_A_ON();
#endif
      EEPROM_WRITE_Interruttore_stato_rele(i);
      Stato_INT(i);
      handle_IndexRedir();
    });
    HttpServer.on("/int" + (String)i + "off", [i]() {
      Debug_MSG_LN("WEB message arrived /int" + (String)i + "off");
      digitalWrite(Interruttore[i].rele, 0 ^ Flag_inversione_RELE);
#ifdef SONOFFDUAL
      if (i = 0) SONOFFDUAL_A_OFF();
      if (i = 1) SONOFFDUAL_B_OFF();
#endif
#ifdef ESP01_SERIAL_RELE
      ESP01_SERIAL_RELE_A_OFF();
#endif
      EEPROM_WRITE_Interruttore_stato_rele(i);
      Stato_INT(i);
      handle_IndexRedir();
    });
  }
#endif

#if Impulso_nodi>0
  for (int i = 0; i < Impulso_nodi; i++) {
    HttpServer.on("/imp" + (String)i + "on", [i]() {
      Debug_MSG_LN("WEB message arrived /imp" + (String)i + "on");
      digitalWrite(Impulso[i].rele, 1 ^ Flag_inversione_RELE);
#ifdef SONOFFDUAL
      if (i = 0) SONOFFDUAL_A_ON();
      if (i = 1) SONOFFDUAL_B_ON();
#endif
#ifdef ESP01_SERIAL_RELE
      ESP01_SERIAL_RELE_A_ON();
#endif
      Impulso[i].rele_eccitato = true;
      Impulso[i].ulTime = millis();
      Stato_IMP(i);
      handle_IndexRedir();
    });
#ifdef Impulso1_NFC
    HttpServer.on("/imp" + (String)i + "piukey", [i]() {
      Debug_MSG_LN("WEB message arrived /imp" + (String)i + "piukey");
      piukey();
      handle_IndexRedir();
    });
    HttpServer.on("/imp" + (String)i + "menokey", [i]() {
      Debug_MSG_LN("WEB message arrived /imp" + (String)i + "menokey");
      menokey();
      handle_IndexRedir();
    });
    HttpServer.on("/imp" + (String)i + "clearkey", [i]() {
      Debug_MSG_LN("WEB message arrived /imp" + (String)i + "clearkey");
      clearkey();
      handle_IndexRedir();
    });
    for (int ii = 1; ii < 25; ii++) {
      HttpServer.on("/imp" + (String)i + "delkey" + (String)ii, [i, ii]() {
        Debug_MSG_LN("WEB message arrived /imp" + (String)i +  "delkey" + (String)ii);
        String tmp = "";
        int i = EEPROM_NFC + ((ii - 1) * 4);
        tmp += String(EEPROM.read(i + 0) < 0x10 ? "0" : "");
        tmp += String(EEPROM.read(i + 0), HEX);
        tmp += String(EEPROM.read(i + 1) < 0x10 ? "0" : "");
        tmp += String(EEPROM.read(i + 1), HEX);
        tmp += String(EEPROM.read(i + 2) < 0x10 ? "0" : "");
        tmp += String(EEPROM.read(i + 2), HEX);
        tmp += String(EEPROM.read(i + 3) < 0x10 ? "0" : "");
        tmp += String(EEPROM.read(i + 3), HEX);
        eraseKey(tmp);
        handle_IndexRedir();
      });
    }
#endif
  }
#endif

#if Temporizzatore_nodi>0
  for (int i = 0; i < Temporizzatore_nodi; i++) {
    HttpServer.on("/tem" + (String)i + "on", [i]() {
      Debug_MSG_LN("WEB message arrived /tem" + (String)i + "on");
      digitalWrite(Temporizzatore[i].rele, 1 ^ Flag_inversione_RELE);
#ifdef SONOFFDUAL
      if (i = 0) SONOFFDUAL_A_ON();
      if (i = 1) SONOFFDUAL_B_ON();
#endif
#ifdef ESP01_SERIAL_RELE
      ESP01_SERIAL_RELE_A_ON();
#endif
      Temporizzatore[i].rele_eccitato = true;
      Temporizzatore[i].ulTime = millis();
      Stato_TEM(i);
      handle_IndexRedir();
    });
    HttpServer.on("/tem" + (String)i + "piu", [i]() {
      Debug_MSG_LN("WEB message arrived /tem" + (String)i +  "piu");
      Temporizzatore[i].tempo = Temporizzatore[i].tempo + 1000;
      if (Temporizzatore[i].tempo > 3600000) Temporizzatore[i].tempo = 3600000;
      EEPROM_WRITE_Temporizzatore_tempo(i);
      Stato_TEM(i);
      handle_IndexRedir();
    });
    HttpServer.on("/tem" + (String)i + "meno", [i]() {
      Debug_MSG_LN("WEB message arrived /tem" + (String)i +  "meno");
      Temporizzatore[i].tempo = Temporizzatore[i].tempo - 1000;
      if (Temporizzatore[i].tempo < 1000) Temporizzatore[i].tempo = 1000;
      EEPROM_WRITE_Temporizzatore_tempo(i);
      Stato_TEM(i);
      handle_IndexRedir();
    });
    HttpServer.on("/tem" + (String)i + "piupiu", [i]() {
      Debug_MSG_LN("WEB message arrived /tem" + (String)i +  "piupiu");
      Temporizzatore[i].tempo = Temporizzatore[i].tempo + 30000;
      if (Temporizzatore[i].tempo > 3600000) Temporizzatore[i].tempo = 3600000;
      EEPROM_WRITE_Temporizzatore_tempo(i);
      Stato_TEM(i);
      handle_IndexRedir();
    });
    HttpServer.on("/tem" + (String)i + "menomeno", [i]() {
      Debug_MSG_LN("WEB message arrived /tem" + (String)i +  "menomeno");
      Temporizzatore[i].tempo = Temporizzatore[i].tempo - 30000;
      if (Temporizzatore[i].tempo < 1000) Temporizzatore[i].tempo = 1000;
      EEPROM_WRITE_Temporizzatore_tempo(i);
      Stato_TEM(i);
      handle_IndexRedir();
    });
  }
#endif

#if Termostato_nodi>0
  for (int i = 0; i < Termostato_nodi; i++) {
    HttpServer.on("/ter" + (String)i + "on", [i]() {
      Debug_MSG_LN("WEB message arrived /ter" + (String)i + "on");
      digitalWrite(Termostato[i].rele, 1 ^ Flag_inversione_RELE);
      Stato_TER(i);
      EEPROM_Termostato_write(i);
      handle_IndexRedir();
    });
    HttpServer.on("/ter" + (String)i + "off", [i]() {
      Debug_MSG_LN("WEB message arrived /ter" + (String)i + "off");
      digitalWrite(Termostato[i].rele, 0 ^ Flag_inversione_RELE);
      Stato_TER(i);
      EEPROM_Termostato_write(i);
      handle_IndexRedir();
    });
    HttpServer.on("/ter" + (String)i + "man", [i]() {
      Debug_MSG_LN("WEB message arrived /ter" + (String)i + "man");
      Termostato[i].AUTO = false;
      Termostato[i].CRONO = false;
      Stato_TER(i);
      EEPROM_Termostato_write(i);
      handle_IndexRedir();
    });
    HttpServer.on("/ter" + (String)i + "auto", [i]() {
      Debug_MSG_LN("WEB message arrived /ter" + (String)i + "auto");
      Termostato[i].AUTO = true;
      Termostato[i].CRONO = false;
      Stato_TER(i);
      EEPROM_Termostato_write(i);
      handle_IndexRedir();
    });
    HttpServer.on("/ter" + (String)i + "crono", [i]() {
      Debug_MSG_LN("WEB message arrived /ter" + (String)i + "crono");
      Termostato[i].AUTO = false;
      Termostato[i].CRONO = true;
      Stato_TER(i);
      EEPROM_Termostato_write(i);
      handle_IndexRedir();
    });
    for (int ii = 0; ii <= 23; ii++) {
      HttpServer.on("/ter" + (String)i + "piuora" + (String)ii, [i, ii]() {
        Debug_MSG_LN("WEB message arrived /ter" + (String)i +  "piuora" + (String)ii);
        Termostato[i].crono[ii] += 1;
        if (Termostato[i].crono[ii] > 3) Termostato[i].crono[ii] = 3;
        Stato_TER(i);
        EEPROM_Termostato_write(i);
        handle_IndexRedir();
      });
      HttpServer.on("/ter" + (String)i + "menoora" + (String)ii, [i, ii]() {
        Debug_MSG_LN("WEB message arrived /ter" + (String)i +  "menoora" + (String)ii);
        Termostato[i].crono[ii] -= 1;
        if (Termostato[i].crono[ii] < 1) Termostato[i].crono[ii] = 1;
        Stato_TER(i);
        EEPROM_Termostato_write(i);
        handle_IndexRedir();
      });
    }
    HttpServer.on("/ter" + (String)i + "piu", [i]() {
      Debug_MSG_LN("WEB message arrived /ter" + (String)i +  "piu");
      Termostato[i].ta = Termostato[i].ta + 0.1;
      if (Termostato[i].ta > 30) Termostato[i].ta = 30;
      Stato_TER(i);
      EEPROM_Termostato_write(i);
      handle_IndexRedir();
    });
    HttpServer.on("/ter" + (String)i + "meno", [i]() {
      Debug_MSG_LN("WEB message arrived /ter" + (String)i +  "meno");
      Termostato[i].ta = Termostato[i].ta - 0.1;
      if (Termostato[i].ta < 2) Termostato[i].ta = 2;
      Stato_TER(i);
      EEPROM_Termostato_write(i);
      handle_IndexRedir();
    });

    HttpServer.on("/ter" + (String)i + "piut3", [i]() {
      Debug_MSG_LN("WEB message arrived /ter" + (String)i +  "piut3");
      Termostato[i].t3 += 0.1;
      if (Termostato[i].t3 > 30) Termostato[i].t3 = 30;
      Stato_TER(i);
      EEPROM_Termostato_write(i);
      handle_IndexRedir();
    });
    HttpServer.on("/ter" + (String)i + "menot3", [i]() {
      Debug_MSG_LN("WEB message arrived /ter" + (String)i +  "menot3");
      Termostato[i].t3 -= 0.1;
      if (Termostato[i].t3 < 2) Termostato[i].t3 = 2;
      Stato_TER(i);
      EEPROM_Termostato_write(i);
      handle_IndexRedir();
    });

    HttpServer.on("/ter" + (String)i + "piut2", [i]() {
      Debug_MSG_LN("WEB message arrived /ter" + (String)i +  "piut2");
      Termostato[i].t2 += 0.1;
      if (Termostato[i].t2 > 30) Termostato[i].t2 = 30;
      Stato_TER(i);
      EEPROM_Termostato_write(i);
      handle_IndexRedir();
    });
    HttpServer.on("/ter" + (String)i + "menot2", [i]() {
      Debug_MSG_LN("WEB message arrived /ter" + (String)i +  "menot2");
      Termostato[i].t2 -= 0.1;
      if (Termostato[i].t2 < 2) Termostato[i].t2 = 2;
      Stato_TER(i);
      EEPROM_Termostato_write(i);
      handle_IndexRedir();
    });
    HttpServer.on("/ter" + (String)i + "piut1", [i]() {
      Debug_MSG_LN("WEB message arrived /ter" + (String)i +  "piut1");
      Termostato[i].t1 += 0.1;
      if (Termostato[i].t1 > 30) Termostato[i].t1 = 30;
      Stato_TER(i);
      EEPROM_Termostato_write(i);
      handle_IndexRedir();
    });
    HttpServer.on("/ter" + (String)i + "menot1", [i]() {
      Debug_MSG_LN("WEB message arrived /ter" + (String)i +  "menot1");
      Termostato[i].t1 -= 0.1;
      if (Termostato[i].t1 < 2) Termostato[i].t1 = 2;
      Stato_TER(i);
      EEPROM_Termostato_write(i);
      handle_IndexRedir();
    });
  }
#endif

#if Sensore_nodi>0
#endif

#if Display_nodi>0
#endif

#if Power_nodi>0
  for (int i = 0; i < Power_nodi; i++) {
    HttpServer.on("/pow" + (String)i + "zero", [i]() {
      Debug_MSG_LN("WEB message arrived /pow" + (String)i +  "zero");
      Power[0].Wh_zero_parziale = Power[0].Wh;
      EEPROM_WRITE_Power_zero(0);
      EEPROM_READ_Power_zero(0);
      handle_IndexRedir();
    });
    HttpServer.on("/pow" + (String)i + "KWh", [i]() {
      Debug_MSG_LN("WEB message arrived /pow" + (String)i +  "KWh");
      KWh = true;
      EEPROM_WRITE_Power_Wh_KWh();
      handle_IndexRedir();
    });
    HttpServer.on("/pow" + (String)i + "Wh", [i]() {
      Debug_MSG_LN("WEB message arrived /pow" + (String)i +  "Wh");
      KWh = false;
      EEPROM_WRITE_Power_Wh_KWh();
      handle_IndexRedir();
    });
  }
#endif
  HttpServer.onNotFound(handle_NotFound);
  HttpServer.begin();
  Debug_MSG_LN("WEB server started");
}

void loop_web() {
  HttpServer.handleClient();
}

void handle_OnConnect() {
  // Debug_MSG_LN("WEB handle_OnConnect");
  HttpServer.send(200, "text/html", SendHTML() );
}

void handle_NotFound() {
  // Debug_MSG_LN("WEB handle_NotFound");
  HttpServer.send(404, "text/plain", "Not found");
}

void handle_IndexRedir() {
  // Debug_MSG_LN("WEB handle_IndexRedir");
  HttpServer.sendHeader("Location", String("/"), true);
  HttpServer.send ( 302, "text/plain", "");
}

void handle_IndexRedir_fw() {
  // Debug_MSG_LN("WEB handle_IndexRedir_fw");
  HttpServer.sendHeader("Location", "/firmware", true);
  HttpServer.send(302, "text/plane", "");
}

String SendHTML() {
  String ptr = "<!DOCTYPE html><html>";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">";
  ptr += "<title>SmartHome</title>";
  ptr += "<meta http-equiv='refresh' content='2'>";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; text-align: left;}";
  ptr += "h1 {color: #444444;}";
  ptr += "p {font-size: 24px;color: #444444;}";
  ptr += "th, td {border: 1px solid black;}";
  ptr += ".buttonGRE {background-color: green; color: white;}"; /* Green */
  ptr += ".buttonRED {background-color: red; color: white;}"; /* Red */
  ptr += ".buttonBLU {background-color: blue; color: white;}"; /* Blu */
  ptr += ".dotGRE {height: 15px; width: 15px; background-color: green; border-radius: 50%; display: inline-block; border: 1px solid black;}";
  ptr += ".dotRED {height: 15px; width: 15px; background-color: red; border-radius: 50%; display: inline-block; border: 1px solid black;}";
  ptr += ".quadGREY {height: 15px; width: 15px; background-color: grey; display: inline-block;}";
  ptr += ".quadRED {height: 15px; width: 15px; background-color: red; display: inline-block;}";
  ptr += "</style>";
  ptr += "</head>";
  ptr += "<body>";
  ptr += "<h1>SmartHome</h1> ";

  // Tapparella
#if Tapparella_nodi>0
  ptr += "<p><h2>Nodi Tapparella:</h2>";
  for (int i = 0; i < Tapparella_nodi; i++) {
    ptr += Tapparella[i].topic + "<br><br>";
    if (digitalRead(Tapparella[i].rele_GIU) == 1 ^ Flag_inversione_RELE) {
      ptr += "  <a href=\"tap" + (String)i + "giu\"><button class=\"button buttonGRE\">GIU</button></a>&nbsp;";
    }
    else {
      ptr += "  <a href=\"tap" + (String)i + "giu\"><button>GIU</button></a>&nbsp;";
    }
    if (digitalRead(Tapparella[i].rele_SU) == 1 ^ Flag_inversione_RELE) {
      ptr += "<a href = \"tap" + (String)i + "su\"><button class=\"button buttonGRE\">SU</button></a>&nbsp;";
    }
    else {
      ptr += "<a href = \"tap" + (String)i + "su\"><button>SU</button></a>&nbsp;";
    }
    ptr += "<a href=\"tap" + (String)i + "stop\"><button>STOP</button></a><br>";
    ptr += "<br>";
    float tmp = Tapparella[i].tempo_max / 1000;
    ptr += "<table>";
    ptr += "<tr>";
    ptr += "<td>Percentuale</td>";
    ptr += "<td>" + (String)Tapparella[i].percentuale + " %</td>";
    ptr += "</tr>";
    ptr += "</table>";
    ptr += "<table>";
    ptr += "<tr>";
    ptr += "<td>Tempo MAX.</td>";
    ptr += "<td>" + (String)tmp + " sec.</td>";
    ptr += "<td style=\"border:none\"><a href=\"tap" + (String)i + "meno\"><button>-1</button></a>&nbsp;<a href=\"tap" + (String)i + "piu\"><button>+1</button></a></td>";
    ptr += "</tr>";
    ptr += "</table>";
    ptr += "<br>";
  }
  ptr += "</p>";
#endif

  // Interruttore
#if Interruttore_nodi>0
  ptr += "<p><h2>Nodi Interruttore:</h2>";
  for (int i = 0; i < Interruttore_nodi; i++) {
    ptr += Interruttore[i].topic + "<br>";
    ptr += "<br>";
    ptr += "<a href=\"int" + (String)i + "on\"><button>ON</button></a>&nbsp;<a href=\"int" + (String)i + "off\"><button>OFF</button></a>";
    if (digitalRead(Interruttore[i].rele) == 1 ^ Flag_inversione_RELE) {
      ptr += "  Rele:<span class=\"dotGRE\"></span>";
    }
    else {
      ptr += "  Rele:<span class=\"dotRED\"></span>";
    }
    if (Interruttore[i].rele != Interruttore[i].stato) {
      if (digitalRead(Interruttore[i].stato) == 1 ^ Flag_inversione_RELE) {
        ptr += "  Stato:<span class=\"dotGRE\"></span>";
      }
      else {
        ptr += "  Stato:<span class=\"dotRED\"></span>";
      }
    }
    ptr += "<br>";
    ptr += "<br>";
  }
  ptr += "</p>";
#endif

  // Impulso
#if Impulso_nodi>0
  ptr += "<p><h2>Nodi Impulso:</h2>";
  for (int i = 0; i < Impulso_nodi; i++) {
    ptr += Impulso[i].topic + "<br><br>";
    if (digitalRead(Impulso[i].rele) == 1 ^ Flag_inversione_RELE) {
      ptr += "  <a href=\"imp" + (String)i + "on\"><button class=\"button buttonGRE\">ON</button></a><br>";
    }
    else {
      ptr += "  <a href=\"imp" + (String)i + "on\"><button>ON</button></a><br>";
    }
    ptr += "<br>";
#ifdef Impulso1_NFC
    ptr += "NFC<br>";
    ptr += "<a href=\"imp" + (String)i + "piukey\"><button>Aggiungi tag</button></a>&nbsp;";
    ptr += "<a href=\"imp" + (String)i + "menokey\"><button>Rimuovi tag</button></a>&nbsp;";
    ptr += "<a href=\"imp" + (String)i + "clearkey\"><button>Cancella tutti i tag</button></a><br>";

    ptr += "<table>";
    for (int ii = EEPROM_NFC; ii < EEPROM_NFC + 100; ii = ii + 4) {
      String tmp = "";
      tmp += String(EEPROM.read(ii + 0) < 0x10 ? "0" : "");
      tmp += String(EEPROM.read(ii + 0), HEX);
      tmp += String(EEPROM.read(ii + 1) < 0x10 ? "0" : "");
      tmp += String(EEPROM.read(ii + 1), HEX);
      tmp += String(EEPROM.read(ii + 2) < 0x10 ? "0" : "");
      tmp += String(EEPROM.read(ii + 2), HEX);
      tmp += String(EEPROM.read(ii + 3) < 0x10 ? "0" : "");
      tmp += String(EEPROM.read(ii + 3), HEX);

      if (tmp != "ffffffff" && tmp != "00000000") {
        ptr += "<tr>";
        ptr += "<td>";
        ptr += "Key " + String(((ii - EEPROM_NFC) / 4) + 1);
        ptr += "</td>";
        ptr += "<td>";
        ptr += tmp;
        ptr += "</td>";
        ptr += "<td style=\"border:none\">";
        ptr += "<a href=\"imp" + (String)i + "delkey" + String(((ii - EEPROM_NFC) / 4) + 1) + "\"><button>-</button></a>";
        ptr += "</td>";
        ptr += "</tr>";
      }
    }
    ptr += "</table>";
    ptr += "<br>";
#endif
  }
  ptr += "</p>";
#endif

  // Temporizzatore
#if Temporizzatore_nodi>0
  ptr += "<p><h2>Nodi Temporizzatore: </h2>";
  for (int i = 0; i < Temporizzatore_nodi; i++) {
    ptr += Temporizzatore[i].topic + "<br>";
    ptr += "<br>";
    if (digitalRead(Temporizzatore[i].rele) == 1 ^ Flag_inversione_RELE) {
      long tmp = millis() - Temporizzatore[i].ulTime;
      tmp -= Temporizzatore[i].tempo;
      tmp = tmp / 1000;
      ptr += "  <a href = \"tem" + (String)i + "on\"><button class=\"button buttonGRE\">ON</button></a>   " + (String)tmp + " sec.<br>";
    }
    else {
      ptr +=  "  <a href=\"tem" + (String)i + "on\"><button>ON</button></a><br>";
    }
    long tmp = Temporizzatore[i].tempo / 1000;
    ptr += "  Tempo: <a href=\"tem" + (String)i + "menomeno\"><button>-30</button></a>&nbsp;<a href=\"tem" + (String)i + "meno\"><button>-1</button></a> " + (String)tmp + " sec. " + "<a href=\"tem" + (String)i + "piu\"><button>+1</button></a>&nbsp;<a href=\"tem" + (String)i + "piupiu\"><button>+30</button></a><br>";
    ptr += "<br>";
  }
  ptr += "</p>";
#endif

  // Termostato
#if Termostato_nodi>0
  now = dstAdjusted.time(nullptr);
  char H[80], M[80];
  strftime (H, 80, "%H", localtime(&now));
  strftime (M, 80, "%M", localtime(&now));
  ptr += "<p><h2>Nodi Termostato:</h2>";
  for (int i = 0; i < Termostato_nodi; i++) {
    ptr += Termostato[i].topic + "<br><br>";
    ptr += "<table>";
    ptr += "<tr>";
    ptr += "<td>Temperatura</td>";
    ptr += "<td>" + (String)Termostato[i].t + "&#186;C</td>";
    ptr += "</tr>";
    ptr += "<tr>";
    ptr += "<td>Umidita'</td>";
    ptr += "<td>" + (String)Termostato[i].h + "%</td>";
    ptr += "</tr>";
    ptr += "</table>";
    ptr += "<br>";
    if (digitalRead(Termostato[i].rele) == 1 ^ Flag_inversione_RELE) {
      ptr += "<a href=\"ter" + (String)i + "off\"><button>OFF</button></a>&nbsp;<a href=\"ter" + (String)i + "on\"><button class=\"button buttonGRE\">ON</button></a>&nbsp;";
    }
    else {
      ptr += "<a href=\"ter" + (String)i + "off\"><button class=\"button buttonRED\">OFF</button></a>&nbsp;<a href=\"ter" + (String)i + "on\"><button>ON</button></a>&nbsp;";
    }
    ptr += "&nbsp;&nbsp;";
    if (!Termostato[i].CRONO && Termostato[i].AUTO) {
      ptr += "<a href=\"ter" + (String)i + "man\"><button>MAN</button></a>&nbsp;";
      ptr += "<a href=\"ter" + (String)i + "auto\"><button class=\"button buttonBLU\">AUTO</button></a>&nbsp;";
      ptr += "<a href=\"ter" + (String)i + "crono\"><button>CRONO</button></a>&nbsp;";
    }
    if (!Termostato[i].CRONO && !Termostato[i].AUTO) {
      ptr += "<a href=\"ter" + (String)i + "man\"><button class=\"button buttonBLU\">MAN</button></a>&nbsp;";
      ptr += "<a href=\"ter" + (String)i + "auto\"><button>AUTO</button></a>&nbsp;";
      ptr += "<a href=\"ter" + (String)i + "crono\"><button>CRONO</button></a>&nbsp;";
    }
    if (Termostato[i].CRONO) {
      ptr += "<a href=\"ter" + (String)i + "man\"><button>MAN</button></a>&nbsp;";
      ptr += "<a href=\"ter" + (String)i + "auto\"><button>AUTO</button></a>&nbsp;";
      ptr += "<a href=\"ter" + (String)i + "crono\"><button class=\"button buttonBLU\">CRONO</button></a>&nbsp;";
    }
    ptr += "<br><br>";
    ptr += "<table><td>";
    ptr += "TA=" + (String)Termostato[i].ta + "&#186;C  <a href=\"ter" + (String)i + "meno\"><button>-0.1</button></a>&nbsp;<a href=\"ter" + (String)i + "piu\"><button>+0.1</button></a><br>";
    ptr += "</td></table><br>";
    ptr += "<table>";
    ptr += "<tr>";
    ptr += "<td>T3=" + (String)Termostato[i].t3 + "&#186;C  <a href=\"ter" + (String)i + "menot3\"><button>-0.1</button></a>&nbsp;<a href=\"ter" + (String)i + "piut3\"><button>+0.1</button></a></td>";
    for (int ii = 0; ii <= 23; ii++) {
      ptr += "<td>";
      String HH = "0" + (String)ii;
      HH = HH.substring(HH.length() - 2);
      if (Termostato[i].crono[ii] == 3) {
        if ((String)H == HH) ptr += "<span class=\"quadRED\"></span>";
        else ptr += "<span class=\"quadGREY\"></span>";
      }
      ptr += "</td>";
    }
    ptr += "</tr>";

    ptr += "<tr>";
    ptr += "<td>T2=" + (String)Termostato[i].t2 + "&#186;C  <a href=\"ter" + (String)i + "menot2\"><button>-0.1</button></a>&nbsp;<a href=\"ter" + (String)i + "piut2\"><button>+0.1</button></a></td>";
    for (int ii = 0; ii <= 23; ii++) {
      ptr += "<td>";
      String HH = "0" + (String)ii;
      HH = HH.substring(HH.length() - 2);
      if (Termostato[i].crono[ii] >= 2) {
        if ((String)H == HH) ptr += "<span class=\"quadRED\"></span>";
        else ptr += "<span class=\"quadGREY\"></span>";
      }
      ptr += "</td>";
    }
    ptr += "</tr>";

    ptr += "<tr>";
    ptr += "<td>T1=" + (String)Termostato[i].t1 + "&#186;C  <a href=\"ter" + (String)i + "menot1\"><button>-0.1</button></a>&nbsp;<a href=\"ter" + (String)i + "piut1\"><button>+0.1</button></a></td>";
    for (int ii = 0; ii <= 23; ii++) {
      ptr += "<td>";
      String HH = "0" + (String)ii;
      HH = HH.substring(HH.length() - 2);
      if (Termostato[i].crono[ii] >= 1) {
        if ((String)H == HH) ptr += "<span class=\"quadRED\"></span>";
        else ptr += "<span class=\"quadGREY\"></span>";
      }
      ptr += "</td>";
    }
    ptr += "</tr>";

    ptr += "<tr>";
    ptr += "<th>" + (String)H + ":" + (String)M; + "</th>";
    for (int ii = 0; ii <= 23; ii++) {
      String HH = "0" + (String)ii;
      HH = HH.substring(HH.length() - 2);
      if ((String)H == HH) ptr += "<th>" + (String)ii + "</th>";
      else ptr += "<td>" + (String)ii + "</td>";
    }
    ptr += "</tr>";

    ptr += "<tr>";
    ptr += "<td style=\"border:none\"></td>";
    for (int ii = 0; ii <= 23; ii++) {
      ptr += "<td><a href=\"ter" + (String)i + "piuora" + (String)ii + "\"><button>+</button></a></td>";
    }
    ptr += "</tr>";

    ptr += "<tr>";
    ptr += "<td style=\"border:none\"></td>";
    for (int ii = 0; ii <= 23; ii++) {
      ptr += "<td><a href=\"ter" + (String)i + "menoora" + (String)ii + "\"><button>-</button></a></td>";
    }
    ptr += "</tr>";
    ptr += "</table>";

    ptr += "<br>";
  }
  ptr += "</p>";
#endif

  // Sensore
#if Sensore_nodi>0
  ptr += "<p><h2>Nodi Sensore:</h2>";
  for (int i = 0; i < Sensore_nodi; i++) {
    ptr += Sensore[i].topic + "<br>";
    ptr += "<br>";
    if (Sensore[i].stato == Sensore_ACK_ON) {
      ptr += " <span class=\"dotGRE\"></span><br>";
    }
    else {
      ptr += " <span class=\"dotRED\"></span><br>";
    }
    ptr += "<br>";
  }
  ptr += "</p>";
#endif

  // Display
#if Display_nodi>0
  for (int i = 0; i < Display_nodi; i++) {
    ptr += Display[i].topic + "<br>";
    ptr += "<br>";
    ptr += "<table>";
    ptr += "";
    ptr += "<tr><td>Riga 1</td><td style=\"width:150px\">" + (String)Display[0].riga_1 + "</td></tr>";
    ptr += "<tr><td>Riga 2</td><td>" + (String)Display[0].riga_2 + "</td></tr>";
    ptr += "<tr><td>Riga 3</td><td>" + (String)Display[0].riga_3 + "</td></tr>";
    ptr += "<tr><td>Riga 4</td><td>" + (String)Display[0].riga_4 + "</td></tr>";
    ptr += "</table>";
    ptr += "<br>";
  }
  ptr += "</p>";
#endif

  // Power
#if Power_nodi>0
  ptr += "<p><h2>Nodi Power:</h2>";
  for (int i = 0; i < Power_nodi; i++) {
    ptr += Power[i].topic + "<br><br>";
    ptr += "<table>";
    ptr += "<tr><td style=\"width:150px\">V</td><td style=\"width:150px\">" + (String)Power[0].V  + " V</td></tr>";
    ptr += "<tr><td>A</td><td>" + (String)Power[0].A + " A</td></tr>";
    ptr += "<tr><td>W</td><td>" + (String)Power[0].W + " W</td></tr>";
    ptr += "<tr><td>VA</td><td>" + (String)Power[0].VA + " VA</td></tr>";
    ptr += "<tr><td>VAr</td><td>" + (String)Power[0].VAr + " VAr</td></tr>";
    ptr += "<tr><td>cos&#x3C6</td><td>" + (String)Power[0].PF + "</td></tr>";
    if (KWh) {
      ptr += "<tr><td>KWh totali</td><td>" + (String)(Power[i].Wh / 1000) + " KWh</td><td style=\"border:none\"><a href=\"pow" + (String)i + "KWh\"><button>KWh</button></a><a href=\"pow" + (String)i + "Wh\"><button>Wh</button></a></td></tr>";
      ptr += "<tr><td>KWh parziali</td><td>" + (String)(Power[i].Wh_parziale / 1000) + " KWh</td><td style=\"border:none\"><a href=\"pow" + (String)i + "zero\"><button>Azzera</button></a></td></tr>";
    }
    else {
      ptr += "<tr><td>Wh totali</td><td>" + (String)Power[i].Wh + " Wh</td><td style=\"border:none\"><a href=\"pow" + (String)i + "KWh\"><button>KWh</button></a><a href=\"pow" + (String)i + "Wh\"><button>Wh</button></a></td></tr>";
      ptr += "<tr><td>Wh parziali</td><td>" + (String)Power[i].Wh_parziale + " Wh</td><td style=\"border:none\"><a href=\"pow" + (String)i + "zero\"><button>Azzera</button></a></td></tr>";
    }
    ptr += "</table>";
    ptr += "<br>";
    ptr += "<table>";
    if (KWh) {
      ptr += "<tr><td style=\"width:150px\">KWh oggi</td><td style=\"width:150px\">" + (String)(Power[0].Wh_oggi / 1000) + " KWh</td></tr>";
      ptr += "<tr><td>KWh settimana</td><td>" + (String)(Power[0].Wh_settimanale / 1000)  + " KWh</td></tr>";
      ptr += "<tr><td>KWh mese</td><td>" + (String)(Power[0].Wh_mensile / 1000) + " KWh</td></tr>";
    }
    else {
      ptr += "<tr><td style=\"width:150px\">Wh oggi</td><td style=\"width:150px\">" + (String)Power[0].Wh_oggi  + " Wh</td></tr>";
      ptr += "<tr><td>Wh settimana</td><td>" + (String)Power[0].Wh_settimanale  + " Wh</td></tr>";
      ptr += "<tr><td>Wh mese</td><td>" + (String)Power[0].Wh_mensile  + " Wh</td></tr>";
    }
    ptr += "</table>";
    ptr += "<br>";
    ptr += "<table>";
    if (KWh) {
      ptr += "<tr><td style=\"width:150px\">KWh ieri</td><td style=\"width:150px\">" + (String)(Power[0].Wh_ieri / 1000) + "KWh</td></tr>";
      ptr += "<tr><td>KWh sett. scorsa</td><td>" + (String)(Power[0].Wh_settimana_scorsa / 1000) + "KWh</td></tr>";
      ptr += "<tr><td>KWh mese scorso</td><td>" + (String)(Power[0].Wh_mese_scorso / 1000) + "KWh</td></tr>";
    }
    else {
      ptr += "<tr><td style=\"width:150px\">Wh ieri</td><td style=\"width:150px\">" + (String)Power[0].Wh_ieri  + "Wh</td></tr>";
      ptr += "<tr><td>Wh sett. scorsa</td><td>" + (String)Power[0].Wh_settimana_scorsa  + "Wh</td></tr>";
      ptr += "<tr><td>Wh mese scorso</td><td>" + (String)Power[0].Wh_mese_scorso  + "Wh</td></tr>";
    }
    ptr += "</table>";
    ptr += "<br>";
  }
  ptr += "</p>";
#endif

  ptr += "<p><h2>Utility:</h2>";
  ptr += "<a href=\"/firmware\"\"><button>Firmware update</button></a>&nbsp;<a href=\"/reset\"\"><button>Reset device</button></a>&nbsp;<a href=\"/reboot\"\"><button>Reboot device</button></a><br></p>";
  ptr += "<h4>SmartHome v" + Versione + " (C)2019 - by roncoa@gmail.com</h4>";
  ptr += "</body>";
  ptr += "</html>";
  return ptr;
}

#endif
