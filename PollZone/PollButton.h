#pragma once
#include <arduino.h>
#include <ESP8266WiFi.h>
#include "PollClient.h"

class PollButton {
  private:
    int buttonId;
    uint8_t buttonPin;
    uint8_t ledPin;
    int lastState;
    PollClient* client;
  public:
    PollButton(int buttonId, uint8_t buttonPin, uint8_t ledPin, PollClient* client);
    void setup();
    void process();
    void ledOn();
    void ledOff();
};

