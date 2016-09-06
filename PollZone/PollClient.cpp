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
  client(wifiClient),
  mac(WiFi.macAddress()),
  clientId("pollbox-" + mac),
  voteTopic("pollerbox/" + mac + "/vote"),
  onlineTopic("pollerbox/" + mac + "/online"),
  serialTopic("pollerbox/" + mac + "/serial"),
  nameTopic("pollerbox/" + mac + "/name"),
  wifiTopic("pollerbox/" + mac + "/wifi"),
  brokerTopic("pollerbox/" + mac + "/mqtt-broker"),
  ipTopic("pollerbox/" + mac + "/ip"),
  otaPasswordTopic("pollerbox/" + mac + "/ota-password"),
  buildTimestampTopic("pollerbox/" + mac + "/build-timestamp") {
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
  Serial.print("MAC address: ");
  Serial.println(mac);

  client.setServer(cfg.mqttBroker.c_str(), 1883);
  instance = this;
  client.setCallback(onMqttMsg);
  ensureConnected();
}

void PollClient::onMqttMsg(char* _topic, byte* payload, unsigned int length) {
  String topic(_topic);
  String value;

  value.reserve(length + 1);
  for (unsigned int i = 0; i < length; i++) {
    value += static_cast<char>(payload[i]);
  }

  instance->onMsg(topic, value);
}

void PollClient::onMsg(const String& topic, const String& payload) {
  if (topic.endsWith("/serial")) {
    if (payload.length()) {
      Serial.print("Setting serial: ");
      Serial.println(payload);
      cfg.setSerial(payload);
    }
  } else if (topic.endsWith("/name")) {
    if (payload.length()) {
      Serial.print("Setting name: ");
      Serial.println(payload);
      cfg.setName(payload);
    }
  } else if (topic.endsWith("/wifi")) {
    auto separator = payload.indexOf(':');
    if (separator == -1) {
      return;
    }

    String ssid = payload.substring(0, separator);
    String password = payload.substring(separator + 1);

    cfg.setWifi(ssid, password);
    setup();
  } else if (topic.endsWith("/mqtt-broker")) {
    cfg.setMqttBroker(payload);
    setup();
  } else if (topic.endsWith("/ota-password")) {
    Serial.print("Setting OTA password: ");
    Serial.println(payload);
    cfg.setOtaPassword(payload);
  }
}

void PollClient::ensureConnected() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker...");
    if (client.connect(clientId.c_str(), onlineTopic.c_str(), 0, true, "false")) {
      Serial.println("connected");

      client.publish(onlineTopic.c_str(), "true", true);
      client.subscribe(wifiTopic.c_str());
      client.subscribe(brokerTopic.c_str());
      client.subscribe(otaPasswordTopic.c_str());

      if (cfg.serial.length()) {
        Serial.println("Publishing serial.");
        client.publish(serialTopic.c_str(), cfg.serial.c_str(), true);
        // If you want to reset the ID, uncomment this line
        // client.subscribe(serialTopic.c_str());
      } else {
        client.subscribe(serialTopic.c_str());
      }

      if (cfg.name.length()) {
        Serial.println("Publishing name.");
        client.publish(nameTopic.c_str(), cfg.name.c_str(), true);
      } else {
        client.subscribe(nameTopic.c_str());
      }

      Serial.println("Publishing IP");
      client.publish(ipTopic.c_str(), WiFi.localIP().toString().c_str(), true);

      client.publish(buildTimestampTopic.c_str(), cfg.buildTimestamp.c_str(), true);
    } else {
      Serial.printf("failed! rc=");
      Serial.println(client.state());
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

bool PollClient::send(int buttonId) {
  ensureConnected();
  if (client.connected()) {
    return client.publish(voteTopic.c_str(), String(buttonId).c_str(), false);
  } else {
    return false;
  }
}

