#pragma once
#include <arduino.h>
#include <PubSubClient.h>

class PollClient {
  private:
    String ssid;
    String pwd; 
    String mqttServer;
    String mac;
    String topic;
    PubSubClient* client;
  public:
    PollClient(String ssid, String pwd, String mqttServer);
    void setup();
    void ensureConnected();
    bool send(int buttonId);
};

