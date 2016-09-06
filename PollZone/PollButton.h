#pragma once

#include "utils.h"
#include <stdint.h>

class PollButton {
  private:
    const int buttonId;
    const uint8_t buttonPin;
    const uint8_t ledPin;
    bool state, recently_changed;

    fixed_interval_timer<100> debounce_timer;
  public:
    PollButton(int buttonId, uint8_t buttonPin, uint8_t ledPin);
    void setup();
    void loop();
    void ledOn();
    void ledOff();
    void led(bool on);
    int getButtonId();

    bool pressed() const {
      return state;
    }

    bool has_recently_changed() {
      bool x = recently_changed;
      recently_changed = false;
      return x;
    }
};

