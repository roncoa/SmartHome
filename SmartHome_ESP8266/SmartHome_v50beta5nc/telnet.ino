#ifdef TELNET_DEBUG

#include <ESP8266WiFi.h>

#define MAX_SRV_CLIENTS 1

WiFiServer server(23);
WiFiClient serverClients[MAX_SRV_CLIENTS];

void setup_telnet() {
  server.begin();
  server.setNoDelay(true);
#ifdef DEBUG
  Debug_MSG_LN("TELNET started");
#endif
}

void loop_telnet() {
  uint8_t i;
  //check if there are any new clients
  if (server.hasClient()) {
    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
      //find free/disconnected spot
      if (!serverClients[i] || !serverClients[i].connected()) {
        if (serverClients[i]) serverClients[i].stop();
        serverClients[i] = server.available();
        Debug_MSG("New client: ");   Debug_MSG_LN((String) i);
        continue;
      }
    }
    //no free/disconnected spot so reject
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }
}

#endif
