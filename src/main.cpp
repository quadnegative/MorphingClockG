#include <Arduino.h>
#include <WiFiManager.h> 
#include <ESPmDNS.h>
#include "Config.h"
#include "Display.h"
#include "Colors.h"
#include "Main.h"
#include "Icons.h"
#include "Location.h"
#include "NTP.h"
#include "TinyFont.h"
#include "Clock.h"
#include "Weather.h"
#include "Web.h"

/* #region variables */
bool clockStartingUp = true;
WiFiUDP ntpUDP;

/* #endregion */

/* #region Setups */
bool connect_wifi(String n_ssid, String n_pass) {
  int c_cnt = 0;
  debug(F("WIFI: Trying Connection: "));
  debugln(n_ssid);
  WiFi.hostname(config["Hostname"].as<String>());
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(true);
  WiFi.disconnect();
  WiFi.begin(n_ssid, n_pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    debug(".");
    c_cnt++;
    if (c_cnt > 50) {
      debugln(F("WIFI: Connect Failed"));
      return false;
    }
  }
  debugln(F("success!"));
  debug(F("IP Address is: "));
  debugln(WiFi.localIP());
  return true;
}

void setupWIFI(bool verbose) {
  if (!connect_wifi(config["SSID"].as<String>(), config["Password"].as<String>())) {  // Try settings in config file
    JsonDocument defaultconfig = DefaultConfig();
    if (!connect_wifi(defaultconfig["SSID"].as<String>(), defaultconfig["Password"].as<String>())) {  // Try settings in params.h
      if (verbose) debugln(F("WIFI: Cannot connect to anything, RESTART ESP"));
      if (verbose) dma_display->drawIcon(x_ico, 4 , 1, 5, 5);
      rebootclock();
    }
  }
  dma_display->drawIcon(check_ico, 4 , 1, 5, 5);
  return;
}

void setupmDNS(bool verbose) {
  if (!MDNS.begin(config["Hostname"].as<const char*>())) {
    if (verbose)debugln(F("mDNS: Error setting up responder!"));
    // while(1) {
    //   delay(1000);
    // }
  }
  if (verbose)debugln("mDNS: Hostname: " + config["Hostname"].as<String>());
}
/* #endregion */

void setup() {
  if (Serial) Serial.end();
  Serial.begin(9600);
  while (!Serial){ 
    delay(10);
  }

  //Begin
  debugln(F(""));
  debugln(F("Begin Setup"));

  //init configs
  init_config();

  //start dma_display
  setupDMADisplay();
  select_palette();
  
  dma_display->clearScreen();
  TFDrawTextDMA(dma_display, String("WIFI"), 11, 1, cc_bwht);
  TFDrawTextDMA(dma_display, String("LOCATION"), 11, 7, cc_bwht);
  TFDrawTextDMA(dma_display, String("TIMEZONE"), 11, 13, cc_bwht);
  TFDrawTextDMA(dma_display, String("NTP"), 11, 19, cc_bwht);
  TFDrawTextDMA(dma_display, String("WEATHER"), 11, 25, cc_bwht);

  //start Wifi
  setupWIFI(true);
  
  //start mDNS
  setupmDNS(true);

  //Start Location
  setupLocation(true);

  //start NTP
  setupNTP(true);

  //get Weather
  getWeatherjson(true, 3);

  //start web
  httpsvr.begin();
  
  delay(2000);
  dma_display->clearScreen();
  debugln(F("Setup Complete"));
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    setupWIFI(false);
  }

  timeClient.update();  

  web_server();

  displayWeather();
    
  displayClock();
}