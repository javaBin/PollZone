#include "PollState.h"
#include "PollClient.h"
#include "PollButton.h"

PollState::PollState(
    PollClient* _client,
    PollButton* _b1,
    PollButton* _b2,
    PollButton* _b3) {
  client = _client;
  button1 = _b1;
  button2 = _b2;
  button3 = _b3;
};

void PollState::setup() {
  button1->setup();
  button2->setup();
  button3->setup();
  client->setup();
}

void PollState::processButton(PollButton* button) {
  if (button->processButtonPushed()) {
      int buttonId = button->getButtonId();
      bool result = client->send(buttonId);
      if (result) {
        Serial.println("Button " + String(buttonId) + " push sendt to server");  
      }
  }
}

void PollState::processButtons() {
  processButton(button1);
  processButton(button2);
  processButton(button3);
}

