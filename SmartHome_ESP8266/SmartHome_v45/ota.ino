// Arduino OTA
#include <ArduinoOTA.h>

void setup_OTA() {
  beginOTA(OTA_hostname);
}
void loop_OTA() {
  ArduinoOTA.handle();
}

void beginOTA(const char* hostname) {
  ArduinoOTA.setHostname(hostname);
  //ArduinoOTA.setPassword("admin");
  ArduinoOTA.onStart([]() {
    Debug_MSG_LN("Start OTA ");
  });
  ArduinoOTA.onEnd([]() {
    Debug_MSG_LN("\nEnd OTA");
    delay(1000);
    Debug_MSG_LN("Restart ESP");
    delay(500);
    ESP.restart();
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Debug_MSG_LN("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Debug_MSG_LN("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Debug_MSG_LN("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Debug_MSG_LN("Receive Failed");
    else if (error == OTA_END_ERROR) Debug_MSG_LN("End Failed");
  });
  ArduinoOTA.begin();
  Debug_MSG_LN("OTA is up");
}


