#include <Arduino.h>
#include <ArduinoJson.h>


#ifdef ESP32
  #define P_A 19
  #define P_B 21
  #define P_C 4
  #define P_D 5
  #define P_E 15
  #define P_OE 2

  #define P_LAT 22
  #define SPI_BUS_MOSI 23
  #define SPI_BUS_CLK 18
  #define PxMATRIX_COLOR_DEPTH 1
  #define SPI_BUS_MISO 22
  #define SPI_BUS_SS 7
#endif

JsonDocument DefaultConfig() {
    JsonDocument doc;

    //open weather map api key 
    //To Get your API Key - https://openweathermap.org/
    doc["apiKey"] = ""; //e.g a hex string like "abcdef0123456789abcdef0123456789"
    doc["GeoLocation"] = ""; //e.g. "Miami,FL,US" //the city you want the weather for 
    doc["TimeZone"] = "EST"; // "-5", "EST", "CST", "MST", "PST", "AST", "HAST"
    doc["DST"] = true;
    doc["Military"] = false;
    doc["Metric"] = false;
    doc["WeatherAnimation"] = false;
    doc["ColorPalette"] = "0";
    doc["Brightness"] = "70";
    doc["SSID"] = ""; //wifi network
    doc["Password"] = ""; //wifi password
    doc["Hostname"] = "MorphClockG"; //wifi password
    doc["NTPServer"] = "time.google.com"; //"pool.ntp.org";
    doc["DateFormat"] = "M.D.Y"; // date format: D.M.Y or M.D.Y or M.D or D.M or D/M/Y.. looking for trouble

    return doc;
}
