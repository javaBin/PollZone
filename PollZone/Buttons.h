#pragma once

#include "PollButton.h"

class Buttons {
    bool idle_on;
    fixed_interval_timer<1000> idle_blink;
    bool ota_mode, ota_on;
    fixed_interval_timer<500> ota_blink;

  public:
    PollButton* button1;
    PollButton* button2;
    PollButton* button3;

    Buttons() : idle_on(false), ota_mode(false), ota_on(false) {
      // const int board = 1; // protoboard
      const int board = 2; // prototype rev B

      if (board == 1) {
        button1 = new PollButton(1, D0, D1);
        button2 = new PollButton(2, D2, D3);
        button3 = new PollButton(3, D7, D8);
      } else if (board == 2) {
        button1 = new PollButton(1, D1, D0);
        button2 = new PollButton(2, D2, D3);
        button3 = new PollButton(3, D6, D4);
      }
    }

    bool anyPressed() {
      return button1->pressed() || button2->pressed() || button3->pressed();
    }

    void set_ota_mode(bool mode) {
      ota_mode = mode;
    }

    void setup() {
      button1->setup();
      button2->setup();
      button3->setup();
    }

    void loop() {
      if (ota_mode) {
        if (ota_blink.expired()) {
          ota_on = !ota_on;
        }
        button1->led(ota_on);
        button2->led(ota_on);
        button3->led(ota_on);
        return;
      }

      if (idle_blink.expired()) {
        idle_on = !idle_on;
      }

      button1->loop();
      button2->loop();
      button3->loop();

      if (anyPressed()) {
        button1->led(button1->pressed());
        button2->led(button2->pressed());
        button3->led(button3->pressed());
      } else {
        button1->led(idle_on);
        button2->led(idle_on);
        button3->led(idle_on);
      }
    }
};

