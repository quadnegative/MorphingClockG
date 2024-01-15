#include <Arduino.h>
#include <ArduinoJson.h>

JsonDocument DefaultConfig() {
    JsonDocument doc;

    //open weather map api key 
    //To Get your API Key - https://openweathermap.org/
    doc["apiKey"] = ""; //e.g a hex string like "abcdef0123456789abcdef0123456789"
    doc["GeoLocation"] = ""; //e.g. "Paris,FR" //the city you want the weather for 
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
