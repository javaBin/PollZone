#include "Cfg.h"
#include "config.h"
#include <FS.h>

Cfg::Cfg(const char *buildTimestamp) : buildTimestamp(buildTimestamp) {
}

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

void Cfg::setSerial(const String &serial) {
  this->serial = serial;

  writeConfig("serial", serial);
}

void Cfg::setName(const String &name) {
  this->name = name;

  writeConfig("name", name);
}

void Cfg::setWifi(const String &ssid, const String &password) {
  wifiSsid = ssid;
  wifiPassword = password;

  writeConfig("wifiSsid", wifiSsid);
  writeConfig("wifiPassword", wifiPassword);
}

void Cfg::setMqttBroker(const String &mqttBroker) {
  this->mqttBroker = mqttBroker;

  writeConfig("mqttBroker", mqttBroker);
}

void Cfg::setOtaPassword(const String &otaPassword) {
  this->otaPassword = otaPassword;

  writeConfig("otaPassword", otaPassword);
}

void Cfg::loadConfig() {
  // On the first run begin() will take a long time. I assume it is formatting
  // the filesystem or similar initialization.
  Serial.println("SPIFFS begin");
  SPIFFS.begin();

  readConfig("wifiSsid", wifiSsid, defaultWifiSsid);
  readConfig("wifiPassword", wifiPassword, defaultWifiPassword);
  readConfig("mqttBroker", mqttBroker, defaultMqttBroker);
  readConfig("serial", serial, "");
  readConfig("name", name, "");
  readConfig("otaPassword", otaPassword, "");

  SPIFFS.end();
  Serial.println("SPIFFS end");
}

