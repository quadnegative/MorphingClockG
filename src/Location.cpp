#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "Config.h"
#include "Display.h"
//#include "Icons.h"

const String ifconfigme = "http://ifconfig.me/ip"; 
const String ipapi = "http://ip-api.com/json/";
String externalIp = "";
String autoGeo = "";
String geoJson = "";


bool getPublicIP (bool verbose) {
    HTTPClient http;
    http.begin(ifconfigme);
    int httpResponseCode = http.GET();
    if (httpResponseCode = 200) {
        externalIp = http.getString();
    }
    else {
        Serial.println("Location: bad response ifconfig.me");
        return false;
    }
    http.end();
    if (verbose) debugln(externalIp);
    return true;
}

bool getAutoGeo(bool verbose){
    HTTPClient http;
    http.begin(ipapi+externalIp);
    int httpResponseCode = http.GET();
    if (httpResponseCode = 200) {
        autoGeo = http.getString();
    }
    else {
        Serial.println("Location: bad response ip-api.com");
        return false;
    }
    http.end();
    if (verbose) debugln("Location: " + autoGeo);
    return true;
}

bool processautoGeo(bool verbose){
    JsonDocument doc;
    if (!autoGeo.length()) {
    debugln(F("Location: GeoAuto: unable to retrieve data"));
    return false;
    }
    else {
        DeserializationError error = deserializeJson(doc, autoGeo.c_str());
        if (error) return false;
        //if (verbose) debugln("city:"+ doc["city"].as<String>() );
        config["City"] = doc["city"].as<String>(); 
        //if (verbose) debugln("region:"+doc["region"].as<String>() );
        config["Region"] = doc["region"].as<String>(); 
        //if (verbose) debugln("country:"+ doc["countryCode"].as<String>());
        config["Country"] = doc["countryCode"].as<String>(); 
        //if (verbose) debugln("latitude:"+ doc["lat"].as<String>());
        config["Lat"] = doc["lat"].as<String>(); 
        //if (verbose) debugln("longitude:"+ doc["lon"].as<String>());
        config["Lon"] = doc["lon"].as<String>(); 
    }
    config["GeoLocationAuto"] = "q=" + config["City"].as<String>() + "," + config["Region"].as<String>() + "," + config["Country"].as<String>();
    return true;
}

void setupLocation(bool verbose) {
    if (config["GeoLocation"] && config["GeoLocation"] == "Auto") {
        if (getPublicIP(verbose)) {
            if (getAutoGeo(verbose)) {
                if (processautoGeo(verbose)) {
                    if (config["GeoLocationAuto"] && config["GeoLocationAuto"].as<String>().indexOf("q=") != -1) {
                        if (verbose) debugln("Location: GeoLocationAuto");
                        if (verbose) debugln("Location: " + config["GeoLocationAuto"].as<String>());
                        if (verbose) dma_display->drawIcon(check_ico, 4 , 7, 5, 5);
                    }
                    else
                        if (verbose) dma_display->drawIcon(x_ico, 4 , 7, 5, 5);
                }
                else
                        if (verbose) dma_display->drawIcon(x_ico, 4 , 7, 5, 5);
            }
            else
                        if (verbose) dma_display->drawIcon(x_ico, 4 , 7, 5, 5);
        }
        else
                        if (verbose) dma_display->drawIcon(x_ico, 4 , 7, 5, 5);

        
    }
    else if (
        config["GeoLocation"].as<String>().indexOf("lat=") != -1
        && config["GeoLocation"].as<String>().indexOf("lon=") != -1
        ) {
        if (verbose) debugln("Location: Geo Coordinates");
        if (verbose) debugln("Location: " + config["GeoLocation"].as<String>());
        if (verbose) dma_display->drawIcon(check_ico, 4 , 7, 5, 5);
    }
    else if (config["GeoLocation"].as<String>().indexOf("q=") != -1) {
        if (verbose) debugln("Location: City,State,Country");
        if (verbose) debugln("Location: " + config["GeoLocation"].as<String>());
        if (verbose) dma_display->drawIcon(check_ico, 4 , 7, 5, 5);
    }
    else if (config["GeoLocation"].as<String>().indexOf("id=") != -1) {
        if (verbose) debugln("Location: City ID");
        if (verbose) debugln("Location: " + config["GeoLocation"].as<String>());
        if (verbose) dma_display->drawIcon(check_ico, 4 , 7, 5, 5);
    }
    else if (config["GeoLocation"].as<String>().indexOf("zip=") != -1) {
        if (verbose) debugln("Location: ZIP Code");
        if (verbose) debugln("Location: " + config["GeoLocation"].as<String>());
        if (verbose) dma_display->drawIcon(check_ico, 4 , 7, 5, 5);
    }
    else
        if (verbose) dma_display->drawIcon(x_ico, 4 , 7, 5, 5);
}