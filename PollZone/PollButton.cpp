#include <Arduino.h>
#include "PollButton.h"

PollButton::PollButton(int _buttonId, uint8_t _buttonPin, uint8_t _ledPin) :
  buttonId(_buttonId),
  buttonPin(_buttonPin),
  ledPin(_ledPin),
  state(false),
  recently_changed(false) {
}

void PollButton::setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  state = false;
}

void PollButton::loop() {
  if (!debounce_timer.expired()) {
    return;
  }

  bool currentState = digitalRead(buttonPin);
  if (currentState == state) {
    return;
  }

  state = currentState;
  recently_changed = true;
  Serial.println("Button " + String(buttonId) + " changed: " + state + " => " + currentState);
}

void PollButton::ledOn() {
  digitalWrite(ledPin, HIGH);
}

void PollButton::ledOff() {
  digitalWrite(ledPin, LOW);
}

void PollButton::led(bool on) {
  digitalWrite(ledPin, on ? HIGH : LOW);
}

int PollButton::getButtonId() {
  return buttonId;
}
