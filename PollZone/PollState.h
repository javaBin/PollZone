#pragma once

#include "PollClient.h"
#include "Buttons.h"

class PollState {
    PollClient* client;
    Buttons& buttons;

    void processButton(PollButton* button);
  public:
    PollState(
      PollClient* client,
      Buttons& buttons);
    void setup();
    void loop();
};

