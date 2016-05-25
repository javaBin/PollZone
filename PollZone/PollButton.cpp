#include <arduino.h>
#include <ESP8266WiFi.h>
#include "PollButton.h"

PollButton::PollButton(int _buttonId, uint8_t _buttonPin, uint8_t _ledPin) {
  buttonId = _buttonId;
  buttonPin = _buttonPin;
  ledPin = _ledPin;
  lastState = 0;
}

void PollButton::setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  lastState = 0;
}

bool PollButton::processButtonPushed() {
  bool isReleaseEvent = false;
  int buttonState = digitalRead(buttonPin);
  if (buttonState != lastState) {
    String msg = "Button " + String(buttonId) + " changed " + lastState + " => " + buttonState;
    Serial.println(msg);
    
    if (lastState == 0) {
      digitalWrite(ledPin, HIGH);
    } else {
      digitalWrite(ledPin, LOW); 
      isReleaseEvent = true;
    }
    lastState = buttonState;
    delay(100);
  }
  return isReleaseEvent;
}

void PollButton::ledOn() {
  digitalWrite(ledPin, HIGH);
}

void PollButton::ledOff() {
  digitalWrite(ledPin, LOW);
}

int PollButton::getButtonId() {
  return buttonId;
}

