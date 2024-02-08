#ifndef CONFIG_H
#define CONFIG_H

#include <ArduinoJson.h>
#include "Params.h"
#include "Icons.h"

#define DEBUG 1
#define debug(...) \
            do { if (DEBUG) Serial.print(__VA_ARGS__); } while (0)
#define debugln(...) \
            do { if (DEBUG) Serial.println(__VA_ARGS__); } while (0)

extern JsonDocument config;

void rebootclock();
void init_config();
String serializeConfig();
JsonDocument deserializeConfig(String json);
bool vars_write();
void init_default_config();

#endif