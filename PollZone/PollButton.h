#pragma once

#include <stdint.h>

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
    void led(bool on);
    int getButtonId();
};

