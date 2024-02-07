#include <Arduino.h>
#include <ArduinoJson.h>
#include "LittleFS.h"
#include "Params.h"

#define DEBUG 1
#define debug(...) \
            do { if (DEBUG) Serial.print(__VA_ARGS__); } while (0)
#define debugln(...) \
            do { if (DEBUG) Serial.println(__VA_ARGS__); } while (0)

JsonDocument config;

String serializeConfig() {
  String configJSON;
  serializeJson(config, configJSON);
  return configJSON;
}

JsonDocument deserializeConfig(String json) {
  JsonDocument doc;
  deserializeJson(doc, json);
  return doc;
}

void show_config_vars() {
  debugln(F("Config: Loaded config...."));
  JsonObject documentRoot = config.as<JsonObject>();
  for (JsonPair keyValue : documentRoot) {
    debug(String(keyValue.key().c_str())+": ");
    debugln(keyValue.value().as<String>());
  }
}

bool vars_read() {
  // //Remove file for testing when it has corrupt data
  // LittleFS.remove("/mvars.jsn");
  // File varf1 = LittleFS.open ("/mvars.jsn", "w");
  // varf1.close ();
  // return 1;

  File varf = LittleFS.open("/mvars.jsn", "r");
  if (!varf) {
    debugln(F("Config: Failed to open config file"));
    varf.close();
    return false;
  }
  DeserializationError error = deserializeJson(config, varf);
  if (error)  {
    debugln(F("Config: Failed to deserialize config, using default configuration"));
    varf.close();
    return false;
  }
  varf.close();

  String tmp = config["Hostname"];
  tmp.toLowerCase();
  tmp.replace(" ",  "-");
  tmp.replace("_",  "-");
  config["Hostname"] = tmp;
  
  return true;
}

bool vars_write() {
  File varf = LittleFS.open("/mvars.jsn", "w");
  if (!varf) {
    debugln(F("Config: Failed to open config file"));
    varf.close();
    return false;
  }

  String tmp = config["Hostname"];
  tmp.toLowerCase();
  tmp.replace(" ",  "-");
  tmp.replace("_",  "-");
  config["Hostname"] = tmp;

  debugln(F("Config: Writing config file ......"));
  if (serializeJson(config, varf) == 0) {
    debugln(F("Config: Failed to write to file"));
  }
  varf.close();
  return true;
}

void init_default_config() {
  debugln(F("Config: Setting Default Config"));
  config = DefaultConfig();

  String tmp = "";

  tmp = config["GeoLocation"].as<String>();
  tmp.toLowerCase();
  tmp.replace(" ",  "");
  tmp = "q=" + tmp;
  config["GeoLocation"] = tmp;

  tmp = config["Hostname"].as<String>();
  tmp.toLowerCase();
  tmp.replace(" ",  "-");
  tmp.replace("_",  "-");
  config["Hostname"] = tmp;
}

void init_config() {
  // Read the config file
  #define FORMAT_LittleFS_IF_FAILED true
  if (LittleFS.begin(FORMAT_LittleFS_IF_FAILED)) {
    debugln(F("LittleFS Initialize....ok"));
    if (!vars_read()) {
      init_default_config();  //Copy from params.h
    }
  } else {
    debugln(F("LittleFS Initialization...failed"));
  }
  show_config_vars();
}

