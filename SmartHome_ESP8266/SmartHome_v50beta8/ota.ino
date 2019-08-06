#ifdef ArduinoIDE_OTA

#include <ArduinoOTA.h>

void setup_OTA() {
  String tmp = "SmartHome_V";
  tmp += Versione;
  tmp += "_";
  tmp += macToStr(mac);
  if ((String)Hostname_OTA != "") tmp = Hostname_OTA;
  Debug_MSG_LN("Setup OTA: " + tmp );
  beginOTA((char*) tmp.c_str());
}

void beginOTA(const char* hostname) {
  ArduinoOTA.setHostname(hostname);
  //ArduinoOTA.setPassword("admin");
  ArduinoOTA.onStart([]() {
    Debug_MSG_LN("Start OTA");
  });
  ArduinoOTA.onEnd([]() {
    Debug_MSG_LN("\nEnd OTA");
    unblockingDelay(1000);
    Debug_MSG_LN("Restart ESP");
    unblockingDelay(1000);
    ESP.restart();
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Debug_MSG_LN("Progress: " + (String)(progress / (total / 100)) + " %");
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Debug_MSG_LN("Error: ");
    if (error == OTA_AUTH_ERROR) Debug_MSG_LN("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Debug_MSG_LN("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Debug_MSG_LN("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Debug_MSG_LN("Receive Failed");
    else if (error == OTA_END_ERROR) Debug_MSG_LN("End Failed");
  });
  ArduinoOTA.begin();
  Debug_MSG_LN(" OTA is up");
}

void loop_OTA() {
  ArduinoOTA.handle();
}

#endif
