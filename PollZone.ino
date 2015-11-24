/*
 Name:		PollZone.ino
 Created:	11/23/2015 8:58:43 PM
 Author:	kristian
*/

#include <WiFiUdp.h>
#include <WiFiServer.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFi.h>
#include "config.h"
#include "BigButton.h"

int pinInRed = D0;
int pinOutRed = D1;
const char* messageRed = "Red";
int lastStateRed = 0;
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
	setUpButton(pinInRed, pinOutRed, &lastStateRed);

}



// the loop function runs over and over again until power down or reset
void loop() {
	process(pinInRed, pinOutRed, &lastStateRed, messageRed);
}

void setUpButton(uint8_t inPin, uint8_t outPin, int* oldState) {
	pinMode(inPin, INPUT);
	pinMode(outPin, OUTPUT);
	digitalWrite(outPin, HIGH);
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