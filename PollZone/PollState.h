#pragma once

#include "PollClient.h"
#include "PollButton.h"

class PollState {

  private:
    PollClient* client;
    PollButton* button1;
    PollButton* button2;
    PollButton* button3;
    void processButton(PollButton* button);

  public:
    PollState(
        PollClient* client,
        PollButton* button1,
        PollButton* button2,
        PollButton* button3);
    void setup();
    void processButtons();
};
