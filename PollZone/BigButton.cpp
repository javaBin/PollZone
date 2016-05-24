#include <arduino.h>
#include "BigButton.h"





BigButton::BigButton(int _pinIn, int _pinOut, const char _message[])
{
	pinIn = _pinIn;
	pinOut = _pinIn;
	message = _message;
	lastState = 0;

	pinMode(pinIn, INPUT);
	pinMode(pinOut, OUTPUT);

	digitalWrite(pinOut, HIGH);


}



BigButton::~BigButton()
{
}

void BigButton::process()
{
//	int buttonState = digitalRead(pinIn);
//	if (buttonState != lastState) {

//		Serial.print(message);
//		Serial.print(" stateChange ");
//		Serial.print(lastState);
//		Serial.print(" => ");
//		Serial.println(buttonState);
		if (lastState == 0) {
			digitalWrite(pinOut, LOW);
			lastState = 1;
		}
		else {
			digitalWrite(pinOut, HIGH);
			lastState = 0;
			// Send state to MQTT
		}
//		lastState = buttonState;
//
//	}

}
