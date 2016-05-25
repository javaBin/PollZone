#include "config.h"
#include "BigButton.h"
#include "PollButton.h"
#include "PollClient.h"

PollButton* buttonOne;
PollButton* buttonTwo;
PollButton* buttonThree;

PollClient* pollClient;

int rotateSequence = 0;

void rotateLEDs(boolean reset) {
  rotateSequence++;
  int active = rotateSequence % 3;
  if (reset) {
    buttonOne->ledOn();
    buttonTwo->ledOn();
    buttonThree->ledOn();
  } else {
    switch (active) {
      case 0:
        buttonOne->ledOn();
        buttonTwo->ledOff();
        buttonThree->ledOff();
        break;
      case 1:
        buttonOne->ledOff();
        buttonTwo->ledOn();
        buttonThree->ledOff();      
        break;
      case 2:
        buttonOne->ledOff();
        buttonTwo->ledOff();
        buttonThree->ledOn();
        break;
      default:
        break;
    }
  }
}

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  delay(100);

  pollClient = new PollClient(wifi_ssid, wifi_password, mqtt_server);

  buttonOne = new PollButton(1, D0, D1, pollClient);
  buttonTwo = new PollButton(2, D2, D3, pollClient);
  buttonThree = new PollButton(3, D7, D5, pollClient);
  
  buttonOne->setup();
  buttonTwo->setup();
  buttonThree->setup();

  pollClient->setup();
}

// the loop function runs over and over again until power down or reset
void loop() {
  buttonOne->process();
  buttonTwo->process();
  buttonThree->process();
}

