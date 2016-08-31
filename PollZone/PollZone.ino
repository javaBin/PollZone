#include "PollButton.h"
#include "PollClient.h"
#include "PollState.h"
#include "Cfg.h"
#include "utils.h"
#include <Arduino.h>
#include <ArduinoOTA.h>

PollButton* buttonOne;
PollButton* buttonTwo;
PollButton* buttonThree;

Cfg cfg;
PollClient* pollClient;
PollState* pollState;

int rotateSequence = 0;

void rotateLEDs(boolean reset) {
  rotateSequence++;
  int active = rotateSequence % 3;
  if (reset) {
    buttonOne->ledOn();
    buttonTwo->ledOn();
    buttonThree->ledOn();
  } else {
    switch (active) {
      case 0:
        buttonOne->ledOn();
        buttonTwo->ledOff();
        buttonThree->ledOff();
        break;
      case 1:
        buttonOne->ledOff();
        buttonTwo->ledOn();
        buttonThree->ledOff();
        break;
      case 2:
        buttonOne->ledOff();
        buttonTwo->ledOff();
        buttonThree->ledOn();
        break;
      default:
        break;
    }
  }
}

const String build_time(__TIME__);
const String build_date(__DATE__);

static fixed_interval_timer<1000> ota_blink;

static bool in_ota = false;

void setupOta() {
  ArduinoOTA.onStart([]() {
    Serial.println("Starting OTA upgrade");
    in_ota = true;
    ota_blink.reset();
  });
  ArduinoOTA.onEnd([]() {
    in_ota = false;
    Serial.println("\nOTA upgrade complete");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
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
  Serial.print(build_date);
  Serial.print(" ");
  Serial.println(build_time);

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

  // const int board = 1; // protoboard
  const int board = 2; // prototype rev B

  if (board == 1) {
    buttonOne = new PollButton(1, D0, D1);
    buttonTwo = new PollButton(2, D2, D3);
    buttonThree = new PollButton(3, D7, D8);
  } else if (board == 2) {
    buttonOne = new PollButton(1, D1, D0);
    buttonTwo = new PollButton(2, D2, D3);
    buttonThree = new PollButton(3, D6, D4);
  }

  pollState = new PollState(pollClient, buttonOne, buttonTwo, buttonThree);

  pollState->setup();

  setupOta();
}

// the loop function runs over and over again until power down or reset
void loop() {
  ArduinoOTA.handle();

  if (in_ota) {
    static bool ledState = true;
    if (ota_blink.expired()) {
      buttonOne->led(ledState);
      buttonTwo->led(ledState);
      buttonThree->led(ledState);

      ledState = !ledState;
    }
  } else {
    pollState->processButtons();
  }
}

