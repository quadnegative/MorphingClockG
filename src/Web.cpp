#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>
#include "Config.h"
#include "Display.h"
#include "Colors.h"
#include "Main.h"
#include "Location.h"
#include "Weather.h"
#include "NTP.h"
#include "html.h"

WiFiClient httpcli;
WiFiServer httpsvr(80);

void web_server() {
  httpcli = httpsvr.accept();
  if (httpcli) {
    //Read what the browser has sent into a String class and print the request to the monitor
    String httprq = httpcli.readString();
    // Looking under the hood
    //debugln(httprq);
    int pidx = -1;

    //Get /Config
    if ((pidx = httprq.indexOf("GET /config")) != -1) {
      //getConfig(httpcli);
      httpcli.println(F("HTTP/1.0 200 OK"));
      httpcli.println(F("Content-Type: application/json"));
      httpcli.println(F("Access-Control-Allow-Origin: *"));
      httpcli.println(F("Connection: close"));  // the connection will be closed after completion of the response
      httpcli.println(F(""));

      httpcli.println(serializeConfig());
      
      // give the web browser time to receive the data
      delay(1);
      // close the connection:
      httpcli.stop();
    }
    //Post /Config
    else if ((pidx = httprq.indexOf("POST /config")) != -1) {
      httpcli.println(F("HTTP/1.0 200 OK"));
      httpcli.println(F("Content-Type: application/json"));
      httpcli.println();
      delay(1);
      httpcli.stop();

      int index = httprq.indexOf("\r\n\r\n");
      String body = "";
      body = httprq.substring(index+1);
      debugln(body);

      JsonDocument postcfg = deserializeConfig(body);

      //Validate API Key
      if (postcfg["apiKey"] != config["apiKey"]) {
        if (!validateAPIkey(postcfg["apiKey"].as<String>())) {
          debugln(F("OpenWeatherMap: new key failed, keep existing key"));
          postcfg["apiKey"] = config["apiKey"];
        }
      }

      //Validate WiFi Creds
      if ((postcfg["SSID"] != config["SSID"]) || (postcfg["Password"] != config["Password"])) {
        if (!connect_wifi(postcfg["SSID"], postcfg["Password"])) {
          debugln(F("Wifi Connect failed, will try prior SSID and Password"));
          if (!connect_wifi(config["SSID"], config["Password"])) {
            ESP.restart();  //Give up reboot
          }
          else {
            postcfg["SSID"] = config["SSID"];
            postcfg["Password"] = config["Password"];
          }
        }
      }

      //Add Validate NTP Server
      if (postcfg["NTPServer"] != config["NTPServer"]) {
        if (!validateNTPServer(postcfg["NTPServer"])) {
          debugln(F("NTPServer: new server failed, keep existing server"));
          postcfg["NTPServer"] = config["NTPServer"];
        }
      }

      bool reloadWeather = false;
      if (postcfg["Metric"] != config["Metric"]) reloadWeather = true;
      if (postcfg["GeoLocation"] != config["GeoLocation"]) reloadWeather = true;

      bool reloadNTP = false;
      if (postcfg["TimeZone"] != config["TimeZone"]) reloadNTP = true;
      if (postcfg["DST"] != config["DST"]) reloadNTP = true;
      if (postcfg["Military"] != config["Military"]) reloadNTP = true;
      if (postcfg["NTPServer"] != config["NTPServer"]) reloadNTP = true;
      

      config = postcfg;
      vars_write();
      if (reloadWeather) {
        setupLocation(false);
        getWeatherjson(false, 3);
      }
      if (reloadNTP) {
        setupNTP(false);
      }
      dma_display->setBrightness8(config["Brightness"]);
      select_palette();
    }
    else if ((pidx = httprq.indexOf("POST /reset")) != -1) {
      int index = httprq.indexOf("\r\n\r\n");
      String body = "";
      body = httprq.substring(index+1);
      debugln(body);

      JsonDocument doc = deserializeConfig(body);
      debugln(doc["ConfigReset"].as<String>());

      //Add Global var for valid reset window from last page load
      if (doc["ConfigReset"] == "true") {
        httpcli.println(F("HTTP/1.0 200 OK"));
        httpcli.println(F("Content-Type: application/json"));
        httpcli.println();
        delay(1);
        httpcli.stop();

        init_default_config();
        vars_write();
        resetclock;
      }
      else {
        httpcli.println(F("HTTP/1.0 401 Unauthorized"));
        httpcli.println(F("Content-Type: application/json"));
        httpcli.println();
        delay(1);
        httpcli.stop();
      }
    }
    else if ((pidx = httprq.indexOf("POST /reboot")) != -1) {
      httpcli.println(F("HTTP/1.1 200 OK"));
      httpcli.println(F("Content-Type: text/javascript"));
      httpcli.println(F("Access-Control-Allow-Origin: *"));
      httpcli.println(F("Connection: close"));  // the connection will be closed after completion of the response
      httpcli.println(F(""));
      // give the web browser time to receive the data
      delay(1);
      // close the connection:
      httpcli.stop();
      //resetclock();
    }
    //Get /morph.js
    else if ((pidx = httprq.indexOf("GET /morph.js")) != -1) {
      httpcli.println(F("HTTP/1.1 200 OK"));
      httpcli.println(F("Content-Type: text/javascript"));
      httpcli.println(F("Access-Control-Allow-Origin: *"));
      httpcli.println(F("Connection: close"));  // the connection will be closed after completion of the response
      httpcli.println(F(""));
      httpcli.println(script_js);
      // give the web browser time to receive the data
      delay(1);
      // close the connection:
      httpcli.stop();
    }
    //Get index
    else {
      httpcli.println(F("HTTP/1.1 200 OK"));
      httpcli.println(F("Content-type: text/html"));
      httpcli.println(F("Access-Control-Allow-Origin: *"));
      httpcli.println(F("Connection: close"));  // the connection will be closed after completion of the response
      httpcli.println(F(""));
      httpcli.println(index_html);
      // give the web browser time to receive the data
      delay(1);
      // close the connection:
      httpcli.stop();
    }
    clockStartingUp = true;
  }
}