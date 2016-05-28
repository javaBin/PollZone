#pragma once
#include <WiFiClient.h>
#include <PubSubClient.h>

class PollClient {
  private:
    String ssid;
    String pwd; 
    String mqttServer;
    String mac;
    String topic;
    WiFiClient wifiClient;
    PubSubClient client;
  public:
    PollClient(const String &ssid, const String &pwd, const String &mqttServer);
    void setup();
    void ensureConnected();
    bool send(int buttonId);
};
