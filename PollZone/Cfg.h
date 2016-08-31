#pragma once

#include <Arduino.h>

class Cfg {
    void readConfig(const String &key, String &dest, const char* defaultValue);
    void writeConfig(const String& key, const String &value);

  public:
    void loadConfig();

    void setSerial(const String &serial);
    void setName(const String &name);
    void setWifi(const String &ssid, const String &password);
    void setMqttBroker(const String &broker);
    void setOtaPassword(const String &otaPassword);

    String serial;
    String name;
    String mqttBroker;
    String wifiSsid;
    String wifiPassword;
    String otaPassword;
};

