#ifndef WEATHER_H
#define WEATHER_H

#include <Arduino.h>

extern JsonDocument Weatherjson;
extern bool validateAPIkey(String key);
extern void getWeatherjson(bool verbose, int trys);
extern void processWeather(bool verbose);

extern void displayWeather();

#endif