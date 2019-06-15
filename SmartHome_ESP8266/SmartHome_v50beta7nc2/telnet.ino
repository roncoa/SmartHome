#ifdef TELNET_DEBUG

WiFiServer telnetServer(23);
WiFiClient serverClient;

void setup_telnet() {
  telnetServer.begin();
  telnetServer.setNoDelay(true);
  Debug_MSG_LN("TELNET started");
}

void loop_telnet() {
  if (telnetServer.hasClient()) {
    if (!serverClient || !serverClient.connected()) {
      if (serverClient) {
        serverClient.stop();
        Serial.println("Telnet Client Stop");
      }
      serverClient = telnetServer.available();
      Serial.println("New Telnet client");
      serverClient.flush();  // clear input buffer, else you get strange characters
    }
  }
}

#endif
