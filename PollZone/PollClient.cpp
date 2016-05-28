#include <arduino.h>
#include <PubSubClient.h>
#include <WiFiUdp.h>
#include <WiFiServer.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFi.h>
#include "PollClient.h"

PollClient::PollClient(String _ssid, String _pwd, String _mqttServer) {
  ssid = _ssid;
  pwd = _pwd;
  mqttServer = _mqttServer;
  client = new PubSubClient(*(new WiFiClient()));
  mac = WiFi.macAddress();
  topic = "pollerbox/" + mac + "/vote";
};

void PollClient::setup() {
  Serial.println("Connecting to " + ssid);
  
  WiFi.begin(ssid.c_str(), pwd.c_str());

  int iter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (iter++ == 10) {
      Serial.println(" Status: " + String(WiFi.status()));
      iter = 0;
    }
    Serial.print(".");
  }
  client->setServer(mqttServer.c_str(), 1883);
  ensureConnected();
  
  Serial.println("\nWiFi connected");
  Serial.printf("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("MAC address: " + mac);
  Serial.println("Publishing on topic: " + topic);
}

void PollClient::ensureConnected() {
  while (!client->connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client->connect(mac.c_str(), ("pollerbox/" + mac + "/online").c_str(), 0, true, "false")) {
      client->publish(("pollerbox/" + mac + "/online").c_str(), "true", false);  
      Serial.printf("connected! state: %d\n", client->state());
    } else {
      Serial.printf("failed, rc=%d try again in 5 seconds\n", client->state());
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

bool PollClient::send(int buttonId) {
  ensureConnected();
  if (client->connected()) {
    return client->publish(topic.c_str(), String(buttonId).c_str(), false);  
  } else {
    return false;
  }
  
}

