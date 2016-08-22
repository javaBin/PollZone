#include "PollClient.h"
#include "Cfg.h"
#include <PubSubClient.h>
#include <WiFiUdp.h>
#include <WiFiServer.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFi.h>

PollClient* PollClient::instance;

PollClient::PollClient(
  Cfg &cfg) :
  cfg(cfg),
  wifiClient(),
  client(wifiClient) {

  mac = WiFi.macAddress();
  topic = "pollerbox/" + mac + "/vote";
};

// This is called both by the main setup(), but also when reconfigured.
void PollClient::setup() {
  if (client.connected()) {
    Serial.print("Disconnecting from MQTT broker.");
    client.disconnect();
  }

  Serial.print("Connecting to AP ");
  Serial.println(cfg.wifiSsid);

  WiFi.begin(cfg.wifiSsid.c_str(), cfg.wifiPassword.c_str());

  const int default_reset_countdown = 25;
  int reset_countdown = default_reset_countdown;

  const int default_status_countdown = 10;
  int status_countdown = default_status_countdown;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (--reset_countdown == 0) {
      reset_countdown = default_reset_countdown;
      status_countdown = default_status_countdown;
      Serial.println();
      Serial.println("Reconnecting WiFi");
      WiFi.reconnect();
    } else if (--status_countdown == 0) {
      status_countdown = default_status_countdown;

      Serial.print(" Status: ");
      Serial.println(WiFi.status());
    } else {
      Serial.print(".");
    }
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.printf("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("MAC address: " + mac);

  client.setServer(cfg.mqttBroker.c_str(), 1883);
  instance = this;
  client.setCallback(onMqttMsg);
  ensureConnected();

  Serial.println("Publishing on topic: " + topic);
}

void PollClient::onMqttMsg(char* _topic, byte* payload, unsigned int length) {
  String topic(_topic);
  String value;

  value.reserve(length + 1);
  for (unsigned int i = 0; i < length; i++) {
    value += static_cast<char>(payload[i]);
  }

  if (topic.endsWith("/wifi")) {
    auto separator = value.indexOf(':');
    if (separator == -1) {
      return;
    }

    String ssid = value.substring(0, separator);
    String password = value.substring(separator + 1);

    instance->cfg.setWifi(ssid, password);
    instance->setup();
  } else if (topic.endsWith("/mqtt-broker")) {
    instance->cfg.setMqttBroker(value);
    instance->setup();
  }
}

void PollClient::ensureConnected() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker...");
    if (client.connect(mac.c_str(), ("pollerbox/" + mac + "/online").c_str(), 0, true, "false")) {
      Serial.print("connected! state: ");
      Serial.println(client.state());
    } else {
      Serial.printf("failed! rc=");
      Serial.println(client.state());
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }

  client.publish(("pollerbox/" + mac + "/online").c_str(), "true", true);
  client.subscribe(("pollerbox/" + mac + "/wifi").c_str());
  client.subscribe(("pollerbox/" + mac + "/mqtt-broker").c_str());
}

bool PollClient::send(int buttonId) {
  ensureConnected();
  if (client.connected()) {
    return client.publish(topic.c_str(), String(buttonId).c_str(), false);
  } else {
    return false;
  }
}

