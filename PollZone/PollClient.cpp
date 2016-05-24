#include <arduino.h>
#include <PubSubClient.h>
#include <WiFiUdp.h>
#include <WiFiServer.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFi.h>
#include "PollClient.h"

PollClient::PollClient(String _ssid, String _pwd) {
  ssid = _ssid;
  pwd = _pwd;
  client = new PubSubClient(*(new WiFiClient()));
  mac = WiFi.macAddress();
  topic = "/pollerbox/" + mac + "/vote";
};

void PollClient::setup() {
  Serial.println("Connecting to " + ssid);
  
  WiFi.begin(ssid.c_str(), pwd.c_str());

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("WiFi connected");
  Serial.println("IP address: " + WiFi.localIP());
  Serial.println("MAC address: " + mac);
  Serial.println("Publishing on topic: " + topic);
}

void PollClient::ensureConnected() {
  while (!client->connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client->connect(mac.c_str())) {
      Serial.printf("connected! state: %d\n", client->state());
    } else {
      Serial.println("failed, rc=" + String(client->state()) + " try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

bool PollClient::send(int buttonId) {
  if (client->connected()) {
    return client->publish(topic.c_str(), String(buttonId).c_str(), true);  
  } else {
    return false;
  }
  
}

