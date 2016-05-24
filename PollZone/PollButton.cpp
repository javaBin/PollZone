#include <arduino.h>
#include <ESP8266WiFi.h>
#include "PollButton.h"

PollButton::PollButton(int _buttonId, uint8_t _buttonPin, uint8_t _ledPin, PollClient* _client) {
  buttonId = _buttonId;
  buttonPin = _buttonPin;
  ledPin = _ledPin;
  client = _client;
  lastState = 0;
}

void PollButton::setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
//  digitalWrite(outPin, HIGH);
  lastState = 0;
}

void PollButton::process() {
  int buttonState = digitalRead(buttonPin);
  if (buttonState != lastState) {
    String msg = "Button " + String(buttonId) + " changed " + lastState + " => " + buttonState;
    Serial.println(msg);
    
    if (lastState == 0) {
      digitalWrite(ledPin, LOW);
    }  else {
      digitalWrite(ledPin, HIGH);
    }

    bool result = client->send(buttonId);
    if (result) {
      Serial.println("Button " + String(buttonId) + " push sendt to server");  
    }

    lastState = buttonState;
    delay(100);
  }
}

void PollButton::ledOn() {
  digitalWrite(ledPin, HIGH);
}

void PollButton::ledOff() {
  digitalWrite(ledPin, LOW);
}

