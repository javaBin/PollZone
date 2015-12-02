/*
 Name:		PollZone.ino
 Created:	11/23/2015 8:58:43 PM
 Author:	kristian
*/

#include <PubSubClient.h>
#include <WiFiUdp.h>
#include <WiFiServer.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFi.h>
#include "config.h"
#include "BigButton.h"

// Green button config
int pinInGreen = D0;
int pinOutGreen = D1;
const char* messageGreen = "Green";
int lastStateGreen = 0;

// Yellow button config
int pinInYellow = D2;
int pinOutYellow = D3;
const char* messageYellow = "Yellow";
int lastStateYellow = 0;

// Red button config
int pinInRed = D4;
int pinOutRed = D5;
const char* messageRed = "Red";
int lastStateRed = 0;

// reset button config
int pinInReset = D6;
int lastStateReset = 0;

//BigButton red(D2, D3, "Red");
//BigButton* red;


// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);
	delay(100);

	setupWifi();
	//red = new BigButton(D2, D3, "Red");
//	auto func = []() {Serial.println("From within func"); };
//	func();
	setUpButton(pinInGreen, pinOutGreen, &lastStateGreen);
	setUpButton(pinInYellow, pinOutYellow, &lastStateYellow);
	setUpButton(pinInRed, pinOutRed, &lastStateRed);
	setUpResetButton(pinInReset, &lastStateRed);
}



// the loop function runs over and over again until power down or reset
void loop() {
	process(pinInGreen, pinOutGreen, &lastStateGreen, messageGreen);
	process(pinInYellow, pinOutYellow, &lastStateYellow, messageYellow);
	process(pinInRed, pinOutRed, &lastStateRed, messageRed);
	processReset(pinInReset, &lastStateReset);
}

void setUpButton(uint8_t inPin, uint8_t outPin, int* oldState) {
	pinMode(inPin, INPUT);
	pinMode(outPin, OUTPUT);
	digitalWrite(outPin, HIGH);
	*oldState = 0;
}

void setUpResetButton(uint8_t inPin, int* oldState) {
	pinMode(inPin, INPUT);
	*oldState = 0;
}

void process(uint8_t inPin, uint8_t outPin, int* oldState, const char message[]) {
	int buttonState = digitalRead(inPin);
	if (buttonState != *oldState) {

	//		Serial.printf("last: %d now: %d\n", lastState, buttonState);
	//		delay(500);
		Serial.print(message);
		Serial.print(" stateChange ");
		Serial.print(*oldState);
		Serial.print(" => ");
		Serial.println(buttonState);

	//Serial.printf("pinin: %d pinout: %d oldVal: %d newVal: %d message: %s\n", inPin, outPin, *oldState, buttonState, message);
		if (*oldState == 0) {
			digitalWrite(outPin, LOW);
		}
		else {
			digitalWrite(outPin, HIGH);
		// Send state to MQTT
		}
		*oldState = buttonState;
		delay(100);
	}

}

void processReset(uint8_t inPin, int* oldState) {
	int buttonState = digitalRead(inPin);
	if (buttonState != *oldState) {

		//		Serial.printf("last: %d now: %d\n", lastState, buttonState);
		//		delay(500);
		Serial.print("Reset stateChange ");
		Serial.print(*oldState);
		Serial.print(" => ");
		Serial.println(buttonState);

		*oldState = buttonState;
	}

}

void setupWifi() {
	// We start by connecting to a WiFi network

	Serial.println();
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(wifi_ssid);
	WiFi.begin(wifi_ssid, wifi_password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
	Serial.println("MAC address: ");
	Serial.println(WiFi.macAddress());

}
