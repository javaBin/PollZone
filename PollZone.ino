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

// network-stuff
String mac;
String topic;
WiFiClient espClient;
PubSubClient client(espClient);
int clientLoop = 0;


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
	if (!client.connected()) {
		reconnect();
	}
	else {
		clientLoop += 1;
		if (clientLoop % 100 == 0) {
			if (!client.loop())
				Serial.println("PubSubClient loop failed");
			clientLoop = 0;
		}
	}

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
			if (client.connected()) {
				Serial.println("Publishing to MQTT topic " + topic + " value: " + message);
				bool result = client.publish(topic.c_str(), message, true);
				Serial.printf("Result: %s\n", result ? "true": "false");
       
			}
			else {
				Serial.println("Not able to publish, client not connected");
			}

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
	mac = WiFi.macAddress();
	Serial.println(mac);
	topic = "/pollerbox/" + mac + "/vote";
	Serial.println("Publishing on topic: " + topic);

	client.setServer(mqtt_server, 1883);

}

void reconnect() {
	// Loop until we're reconnected
	while (!client.connected()) {
		Serial.print("Attempting MQTT connection...");
		// Attempt to connect
		if (client.connect(mac.c_str())) {
			Serial.printf("connected! state: %d\n", client.state());
		}
		else {
			Serial.print("failed, rc=");
			Serial.print(client.state());
			Serial.println(" try again in 5 seconds");
			// Wait 5 seconds before retrying
			delay(5000);
		}
	}
}
