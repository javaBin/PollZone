#include "PollClient.h"
#include "PollState.h"
#include "Buttons.h"
#include "Cfg.h"
#include "utils.h"
#include <Arduino.h>
#include <ArduinoOTA.h>

Cfg cfg(__DATE__ " " __TIME__);
PollClient* pollClient;
PollState* pollState;
Buttons buttons;

int rotateSequence = 0;

void setupOta() {
  static fixed_interval_timer<1000> ota_blink;

  ArduinoOTA.onStart([]() {
    Serial.println("Starting OTA upgrade");
    ota_blink.reset();
    buttons.set_ota_mode(true);
  });
  ArduinoOTA.onEnd([]() {
    buttons.set_ota_mode(false);
    Serial.println("\nOTA upgrade complete");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    buttons.loop();
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });

  Serial.print("OTA Password: ");
  if (cfg.otaPassword.length()) {
    Serial.println(cfg.otaPassword);
    ArduinoOTA.setPassword(cfg.otaPassword.c_str());
  } else {
    Serial.println("Not set");
  }

  ArduinoOTA.begin();
}

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.print("Build timestamp: ");
  Serial.println(cfg.buildTimestamp);

  cfg.loadConfig();
  Serial.println("Configuration:");
  Serial.print("Wlan SSID:     ");
  Serial.println(cfg.wifiSsid);
  Serial.print("Wlan Password: ");
  Serial.println(cfg.wifiPassword);
  Serial.print("MQTT Broker:   ");
  Serial.println(cfg.mqttBroker);
  Serial.print("Serial:        ");
  if (cfg.serial.length()) {
    Serial.println(cfg.serial);
  } else {
    Serial.println("Not set");
  }
  Serial.print("Name:          ");
  if (cfg.name.length()) {
    Serial.println(cfg.name);
  } else {
    Serial.println("Not set");
  }

  pollClient = new PollClient(cfg);
  buttons.setup();
  pollState = new PollState(pollClient, buttons);
  pollState->setup();

  setupOta();
}

// the loop function runs over and over again until power down or reset
void loop() {
  ArduinoOTA.handle();

  buttons.loop();
  pollClient->loop();
  pollState->loop();
}

