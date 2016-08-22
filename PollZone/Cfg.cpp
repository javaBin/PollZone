#include "Cfg.h"
#include "config.h"
#include <FS.h>

void Cfg::readConfig(const String &key, String &dest, const char* defaultValue) {
  String file = "/" + key + ".txt";
  
  File f = SPIFFS.open(file, "r");
  if (!f) {
    dest = defaultValue;
  } else {
    dest = f.readString();
    f.close();
  }  
}

void Cfg::writeConfig(const String &key, const String &value) {
  String file = "/" + key + ".txt";

  SPIFFS.begin();

  File f = SPIFFS.open(file, "w");
  f.write(reinterpret_cast<const uint8_t*>(value.c_str()), value.length() + 1);
  f.close();

  SPIFFS.end();
}

void Cfg::setWifi(const String &ssid, const String &password) {
  wifiSsid = ssid;
  wifiPassword = password;

  writeConfig("wifiSsid", wifiSsid);
  writeConfig("wifiPassword", wifiPassword);
}

void Cfg::setMqttBroker(const String &broker) {
  mqttBroker = broker;

  writeConfig("mqttBroker", mqttBroker);
}

void Cfg::loadConfig() {
  SPIFFS.begin();

  readConfig("wifiSsid", wifiSsid, defaultWifiSsid);
  readConfig("wifiPassword", wifiPassword, defaultWifiPassword);
  readConfig("mqttBroker", mqttBroker, defaultMqttBroker);

  SPIFFS.end();
}

