#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
//#include <ESP8266mDNS.h>

//MDNSResponder mdns;
ESP8266WebServer webserver(80);

String webPage = "";
String webPage_REBOOT = "";

void setup_web() {
#if defined(WEB_SERVER)
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
  for (int i = 0; i < Tapparella_nodi; i++) {
    webPage += "<p>" + Tapparella[i].topic + "  <a href=\"tap" + (String) (i+1) + "su\"><button>SU</button></a>&nbsp;<a href=\"tap" + (String) (i+1) + "giu\"><button>GIU</button></a>&nbsp;<a href=\"tap" + (String) (i+1) + "stop\"><button>STOP</button></a></p>";
  }
  for (int i = 0; i < Interruttore_nodi; i++) {
    webPage += "<p>" + Interruttore[i].topic + "  <a href=\"int" + (String) (i+1) + "on\"><button>ON</button></a>&nbsp;<a href=\"int" + (String) (i+1) + "off\"><button>OFF</button></a></p>";
  }
  for (int i = 0; i < Impulso_nodi; i++) {
    webPage += "<p>" + Impulso[i].topic + "  <a href=\"imp" + (String) (i+1) + "on\"><button>ON</button></a></p>";
  }
  for (int i = 0; i < Temporizzatore_nodi; i++) {
    webPage += "<p>" + Temporizzatore[i].topic + "  <a href=\"tem" + (String) (i+1) + "on\"><button>ON</button></a></p>";
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

  //Tapparella 1
  webserver.on("/tap1su", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /tap1su");
    String payload = "su";
    client.publish((char*) Tapparella[0].topic.c_str(), (char*) payload.c_str());
  });
  webserver.on("/tap1giu", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /tap1giu");
    String payload = "giu";
    client.publish((char*) Tapparella[0].topic.c_str(), (char*) payload.c_str());
  });
  webserver.on("/tap1stop", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /tap1stop");
    String payload = "stop";
    client.publish((char*) Tapparella[0].topic.c_str(), (char*) payload.c_str());
  });
    //Tapparella 2
  webserver.on("/tap2su", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /tap2su");
    String payload = "su";
    client.publish((char*) Tapparella[1].topic.c_str(), (char*) payload.c_str());
  });
  webserver.on("/tap2giu", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /tap2giu");
    String payload = "giu";
    client.publish((char*) Tapparella[1].topic.c_str(), (char*) payload.c_str());
  });
  webserver.on("/tap2stop", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /tap2stop");
    String payload = "stop";
    client.publish((char*) Tapparella[1].topic.c_str(), (char*) payload.c_str());
  });
    //Tapparella 3
  webserver.on("/tap3su", []() {
    webserver.send(300, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /tap3su");
    String payload = "su";
    client.publish((char*) Tapparella[2].topic.c_str(), (char*) payload.c_str());
  });
  webserver.on("/tap3giu", []() {
    webserver.send(300, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /tap3giu");
    String payload = "giu";
    client.publish((char*) Tapparella[2].topic.c_str(), (char*) payload.c_str());
  });
  webserver.on("/tap3stop", []() {
    webserver.send(300, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /tap3stop");
    String payload = "stop";
    client.publish((char*) Tapparella[2].topic.c_str(), (char*) payload.c_str());
  });
    //Tapparella 4
  webserver.on("/tap4su", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /tap4su");
    String payload = "su";
    client.publish((char*) Tapparella[3].topic.c_str(), (char*) payload.c_str());
  });
  webserver.on("/tap4giu", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /tap4giu");
    String payload = "giu";
    client.publish((char*) Tapparella[3].topic.c_str(), (char*) payload.c_str());
  });
  webserver.on("/tap4stop", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /tap4stop");
    String payload = "stop";
    client.publish((char*) Tapparella[3].topic.c_str(), (char*) payload.c_str());
  });
    //Tapparella 5
  webserver.on("/tap5su", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /tap5su");
    String payload = "su";
    client.publish((char*) Tapparella[4].topic.c_str(), (char*) payload.c_str());
  });
  webserver.on("/tap5giu", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /tap5giu");
    String payload = "giu";
    client.publish((char*) Tapparella[4].topic.c_str(), (char*) payload.c_str());
  });
  webserver.on("/tap5stop", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /tap5stop");
    String payload = "stop";
    client.publish((char*) Tapparella[4].topic.c_str(), (char*) payload.c_str());
  });
  //Interruttore 1
  webserver.on("/int1on", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int1on");
    String payload = "on";
    client.publish((char*) Interruttore[0].topic.c_str(), (char*) payload.c_str());
  });
  webserver.on("/int1off", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int1off");
    String payload = "off";
    client.publish((char*) Interruttore[0].topic.c_str(), (char*) payload.c_str());
  });
    //Interruttore 2
  webserver.on("/int2on", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int2on");
    String payload = "on";
    client.publish((char*) Interruttore[1].topic.c_str(), (char*) payload.c_str());
  });
  webserver.on("/int2off", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int2off");
    String payload = "off";
    client.publish((char*) Interruttore[1].topic.c_str(), (char*) payload.c_str());
  });
    //Interruttore 3
  webserver.on("/int3on", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int3on");
    String payload = "on";
    client.publish((char*) Interruttore[2].topic.c_str(), (char*) payload.c_str());
  });
  webserver.on("/int3off", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int3off");
    String payload = "off";
    client.publish((char*) Interruttore[2].topic.c_str(), (char*) payload.c_str());
  });
    //Interruttore 4
  webserver.on("/int4on", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int4on");
    String payload = "on";
    client.publish((char*) Interruttore[3].topic.c_str(), (char*) payload.c_str());
  });
  webserver.on("/int4off", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int4off");
    String payload = "off";
    client.publish((char*) Interruttore[3].topic.c_str(), (char*) payload.c_str());
  });
    //Interruttore 5
  webserver.on("/int5on", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int5on");
    String payload = "on";
    client.publish((char*) Interruttore[4].topic.c_str(), (char*) payload.c_str());
  });
  webserver.on("/int5off", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int5off");
    String payload = "off";
    client.publish((char*) Interruttore[4].topic.c_str(), (char*) payload.c_str());
  });
    //Interruttore 6
  webserver.on("/int6on", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int6on");
    String payload = "on";
    client.publish((char*) Interruttore[5].topic.c_str(), (char*) payload.c_str());
  });
  webserver.on("/int6off", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int6off");
    String payload = "off";
    client.publish((char*) Interruttore[5].topic.c_str(), (char*) payload.c_str());
  });
    //Interruttore 7
  webserver.on("/int7on", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int7on");
    String payload = "on";
    client.publish((char*) Interruttore[6].topic.c_str(), (char*) payload.c_str());
  });
  webserver.on("/int7off", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int7off");
    String payload = "off";
    client.publish((char*) Interruttore[6].topic.c_str(), (char*) payload.c_str());
  });
    //Interruttore 8
  webserver.on("/int8on", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int8on");
    String payload = "on";
    client.publish((char*) Interruttore[7].topic.c_str(), (char*) payload.c_str());
  });
  webserver.on("/int8off", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int8off");
    String payload = "off";
    client.publish((char*) Interruttore[7].topic.c_str(), (char*) payload.c_str());
  });
    //Interruttore 9
  webserver.on("/int9on", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int9on");
    String payload = "on";
    client.publish((char*) Interruttore[8].topic.c_str(), (char*) payload.c_str());
  });
  webserver.on("/int9off", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int9off");
    String payload = "off";
    client.publish((char*) Interruttore[8].topic.c_str(), (char*) payload.c_str());
  });
    //Interruttore 10
  webserver.on("/int10on", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int10on");
    String payload = "on";
    client.publish((char*) Interruttore[9].topic.c_str(), (char*) payload.c_str());
  });
  webserver.on("/int10off", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /int10off");
    String payload = "off";
    client.publish((char*) Interruttore[9].topic.c_str(), (char*) payload.c_str());
  });
  
  //Impulso 1
  webserver.on("/imp1on", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /can1on");
    String payload = "on";
    client.publish((char*) Impulso[0].topic.c_str(), (char*) payload.c_str());
  });
  //Impulso 2
  webserver.on("/imp2on", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /can2on");
    String payload = "on";
    client.publish((char*) Impulso[1].topic.c_str(), (char*) payload.c_str());
  });
    //Impulso 3
  webserver.on("/imp3on", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /can3on");
    String payload = "on";
    client.publish((char*) Impulso[2].topic.c_str(), (char*) payload.c_str());
  });
    //Impulso 4
  webserver.on("/imp4on", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /can4on");
    String payload = "on";
    client.publish((char*) Impulso[3].topic.c_str(), (char*) payload.c_str());
  });
    //Impulso 5
  webserver.on("/imp5on", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /can5on");
    String payload = "on";
    client.publish((char*) Impulso[4].topic.c_str(), (char*) payload.c_str());
  });
    //Impulso 6
  webserver.on("/imp6on", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /can6on");
    String payload = "on";
    client.publish((char*) Impulso[5].topic.c_str(), (char*) payload.c_str());
  });
    //Impulso 7
  webserver.on("/imp7on", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /can7on");
    String payload = "on";
    client.publish((char*) Impulso[6].topic.c_str(), (char*) payload.c_str());
  });
    //Impulso 8
  webserver.on("/imp8on", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /can8on");
    String payload = "on";
    client.publish((char*) Impulso[7].topic.c_str(), (char*) payload.c_str());
  });
    //Impulso 9
  webserver.on("/imp9on", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /can9on");
    String payload = "on";
    client.publish((char*) Impulso[8].topic.c_str(), (char*) payload.c_str());
  });
    //Impulso 10
  webserver.on("/imp10on", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /can10on");
    String payload = "on";
    client.publish((char*) Impulso[9].topic.c_str(), (char*) payload.c_str());
  });
  //Temporizzatore 1
  webserver.on("/tem1on", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /tim1on");
    String payload = "on";
    client.publish((char*) Temporizzatore[0].topic.c_str(), (char*) payload.c_str());
  });
  //Temporizzatore 2
  webserver.on("/tem2on", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /tim2on");
    String payload = "on";
    client.publish((char*) Temporizzatore[1].topic.c_str(), (char*) payload.c_str());
  });
  //Temporizzatore 3
  webserver.on("/tem3on", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /tim3on");
    String payload = "on";
    client.publish((char*) Temporizzatore[2].topic.c_str(), (char*) payload.c_str());
  });
  //Temporizzatore 4
  webserver.on("/tem4on", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /tim4on");
    String payload = "on";
    client.publish((char*) Temporizzatore[3].topic.c_str(), (char*) payload.c_str());
  });
  //Temporizzatore 5
  webserver.on("/tem5on", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /tim5on");
    String payload = "on";
    client.publish((char*) Temporizzatore[4].topic.c_str(), (char*) payload.c_str());
  });
  //Temporizzatore 6
  webserver.on("/tem6on", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /tim6on");
    String payload = "on";
    client.publish((char*) Temporizzatore[5].topic.c_str(), (char*) payload.c_str());
  });
  //Temporizzatore 7
  webserver.on("/tem7on", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /tim7on");
    String payload = "on";
    client.publish((char*) Temporizzatore[6].topic.c_str(), (char*) payload.c_str());
  });
  //Temporizzatore 8
  webserver.on("/tem8on", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /tim8on");
    String payload = "on";
    client.publish((char*) Temporizzatore[7].topic.c_str(), (char*) payload.c_str());
  });
  //Temporizzatore 9
  webserver.on("/tem9on", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /tim9on");
    String payload = "on";
    client.publish((char*) Temporizzatore[8].topic.c_str(), (char*) payload.c_str());
  });
  //Temporizzatore 10
  webserver.on("/tem10on", []() {
    webserver.send(200, "text/html", webPage);
    Debug_MSG_LN("Message arrived [web] /tim10on");
    String payload = "on";
    client.publish((char*) Temporizzatore[9].topic.c_str(), (char*) payload.c_str());
  });
  //REBOOT
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
#endif
}

void loop_web() {
#if defined(WEB_SERVER)
  webserver.handleClient();
#endif
}
