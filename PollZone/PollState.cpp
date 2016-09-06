#include "PollState.h"
#include "PollClient.h"
#include "PollButton.h"

PollState::PollState(
  PollClient* _client,
  Buttons& buttons): client(_client), buttons(buttons) {
}

void PollState::setup() {
  client->setup();
}

void PollState::processButton(PollButton* button) {
  if (button->has_recently_changed() && button->pressed()) {
    int buttonId = button->getButtonId();
    bool result = client->send(buttonId);
    if (result) {
      Serial.println("Button " + String(buttonId) + " push sendt to server");
    }
  }
}

void PollState::loop() {
  processButton(buttons.button1);
  processButton(buttons.button2);
  processButton(buttons.button3);
}

