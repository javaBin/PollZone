#pragma once
#include <arduino.h>
#include <ESP8266WiFi.h>

class PollButton {
  private:
    int buttonId;
    uint8_t buttonPin;
    uint8_t ledPin;
    int lastState;
  public:
    PollButton(int buttonId, uint8_t buttonPin, uint8_t ledPin);
    void setup();
    bool processButtonPushed();
    void ledOn();
    void ledOff();
    int getButtonId();
};

