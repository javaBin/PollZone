#pragma once

#include "Cfg.h"
#include <WiFiClient.h>
#include <PubSubClient.h>

class PollClient {
  private:
    Cfg &cfg;
    String mac;
    String topic;
    WiFiClient wifiClient;
    PubSubClient client;

    static PollClient* instance;
    static void onMqttMsg(char* topic, byte* payload, unsigned int length);

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

