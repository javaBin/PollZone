#pragma once

#include "Cfg.h"
#include <WiFiClient.h>
#include <PubSubClient.h>

class PollClient {
  private:
    Cfg &cfg;
    String mac;
    String clientId;
    String voteTopic;
    String onlineTopic;
    String serialTopic;
    String nameTopic;
    String wifiTopic;
    String brokerTopic;
    String ipTopic;
    String otaPasswordTopic;
    String buildTimestampTopic;
    WiFiClient wifiClient;
    PubSubClient client;

    static PollClient* instance;
    static void onMqttMsg(char* topic, byte* payload, unsigned int length);
    void onMsg(const String& topic, const String& payload);

    void connectWifi();

  public:
    PollClient(Cfg &cfg);
    void setup();
    void ensureConnected();
    bool send(int buttonId);
    void loop() {
      client.loop();
    }
};

