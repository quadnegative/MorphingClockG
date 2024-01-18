#include "params.h"
//#include <FS.h>
#include "LittleFS.h"
#include <WiFiManager.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <PxMatrix.h>
//#include <TimeLib.h>
#include "TinyFont.h"
#include "Digit.h"
#include "Digitsec.h"
#include "TinyIcons.h"
#include "WeatherIcons.h"
#include <Timezone.h>
#include <web.h>
#define DEBUG 1
#define debug(...) \
            do { if (DEBUG) Serial.print(__VA_ARGS__); } while (0)
#define debugln(...) \
            do { if (DEBUG) Serial.println(__VA_ARGS__); } while (0)

/* #region boards */
//ESP8266 setup
#ifdef ESP8266
  #include <ESP8266WiFi.h>
  #include <Ticker.h>
  //#include <ESP8266WiFiMulti.h>
  //#include <ESP8266mDNS.h>
  Ticker display_ticker;
  #define P_LAT 16
  #define P_A 5
  #define P_B 4
  #define P_C 15
  #define P_D 12
  #define P_E 0
  #define P_OE 2

  
#endif
#ifdef ESP32
  #include <Ticker.h>
  #include <WiFi.h>
  #include <ESPmDNS.h>
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
  Ticker display_ticker;
 // hw_timer_t * timer = NULL;
 // portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
#endif

/* #endregion */

/* #region variables */
JsonDocument config;

// needed variables
// US Eastern Time Zone (New York, Detroit)
//TimeChangeRule myDST = {"EDT", Second, Sun, Mar, 2, -240};  // Eastern Daylight Time = UTC - 4 hours
//TimeChangeRule mySTD = {"EST", First, Sun, Nov, 2, -300};   // Eastern Standard Time = UTC - 5 hours
// UTC
TimeChangeRule utcRule = {"UTC", Last, Sun, Mar, 1, 0};     // UTC
Timezone UTC(utcRule);
// US Eastern Time Zone (New York, Detroit)
TimeChangeRule usEDT = {"EDT", Second, Sun, Mar, 2, -240};  // Eastern Daylight Time = UTC - 4 hours
TimeChangeRule usEST = {"EST", First, Sun, Nov, 2, -300};   // Eastern Standard Time = UTC - 5 hours
Timezone usET(usEDT, usEST);
// // US Central Time Zone (Chicago, Houston)
// TimeChangeRule usCDT = {"CDT", Second, Sun, Mar, 2, -300};
// TimeChangeRule usCST = {"CST", First, Sun, Nov, 2, -360};
// Timezone usCT(usCDT, usCST);
// // US Mountain Time Zone (Denver, Salt Lake City)
// TimeChangeRule usMDT = {"MDT", Second, Sun, Mar, 2, -360};
// TimeChangeRule usMST = {"MST", First, Sun, Nov, 2, -420};
// Timezone usMT(usMDT, usMST);
// // Arizona is US Mountain Time Zone but does not use DST
// Timezone usAZ(usMST);
// // US Pacific Time Zone (Las Vegas, Los Angeles)
// TimeChangeRule usPDT = {"PDT", Second, Sun, Mar, 2, -420};
// TimeChangeRule usPST = {"PST", First, Sun, Nov, 2, -480};
// Timezone usPT(usPDT, usPST);
// // US Alaska Standard Time
// TimeChangeRule usADT = {"ADT", Second, Sun, Mar, 2, -420};
// TimeChangeRule usAST = {"AST", First, Sun, Nov, 2, -480};
// Timezone usAT(usADT, usAST);
// // US Hawaii-Aleutian Standard Time
// TimeChangeRule usHADT = {"HADT", Second, Sun, Mar, 2, -420};
// TimeChangeRule usHAST = {"HAST", First, Sun, Nov, 2, -480};
// Timezone usHAT(usHADT, usHAST);

byte ntpsync = 1;
//const char ntpsvr[] = "time.google.com"; //"pool.ntp.org";
//const char *WiFi_hostname = "MorphClockQ2"; //If you have more than one Morphing Clock you will need to change the hostname
const char openweather[] = "api.openweathermap.org";
const char ifconfigme[] = "ifconfig.me"; 
const char ipapi[] = "ip-api.com";
byte hh;
byte mm;
byte ss;
byte prevhh = 0;
byte prevmm = 0;
byte prevss = 0;
long tnow;
int in = -10000;
int tempMax = -10000;
int tempM = -10000;
int presM = -10000;
int humiM = -10000;
int wind_speed = -10000;
int condM = -1;  //-1 - undefined, 0 - unk, 1 - sunny, 2 - cloudy, 3 - overcast, 4 - rainy, 5 - thunders, 6 - snow
String condS = "";
String Weatherjson = "";
String externalIp = "";
int wind_nr;
int xo = 1, yo = 26;
char use_ani = 0;
WiFiUDP ntpUDP;
//WiFiUDP mdnsUDP;
NTPClient timeClient(ntpUDP);
WiFiClient client;
WiFiClient httpcli;
WiFiServer httpsvr(80);  //Initialize the server on Port 80

// Pins for LED MATRIX
PxMATRIX display(64, 32,P_LAT, P_OE,  P_A, P_B, P_C, P_D);
//PxMATRIX display(64, 64, P_LAT, P_OE, P_A, P_B, P_C, P_D, P_E);

//If you want to adjust color/brightness/position of screen objects you can do that in the following sections.
byte day_bright = 70;  //sets daytime brightness; 70 is default. values higher than this may not work.
byte dim_bright = 20;  // sets brightness for partly dimmed mode
byte nm_bright = 25;   // sets brightness for night mode

//=== SEGMENTS ===
// This section determines the position of the HH:MM ss digits onscreen with format digit#(&display, 0, x_offset, y_offset, irrelevant_color)

byte digit_offset_amount;

byte Digitsec_x = 56 + digit_offset_amount;
byte Digit_x = 62 + digit_offset_amount;

Digitsec digit0(&display, 0, Digitsec_x - 7 * 1, 14, display.color565(255, 255, 255));
Digitsec digit1(&display, 0, Digitsec_x - 7 * 2, 14, display.color565(255, 255, 255));
Digit digit2(&display, 0, Digit_x - 4 - 9 * 3, 8, display.color565(255, 255, 255));
Digit digit3(&display, 0, Digit_x - 4 - 9 * 4, 8, display.color565(255, 255, 255));
Digit digit4(&display, 0, Digit_x - 7 - 9 * 5, 8, display.color565(255, 255, 255));
Digit digit5(&display, 0, Digit_x - 7 - 9 * 6, 8, display.color565(255, 255, 255));


//=== COLORS ===
// this defines the colors used for the time, date, and wind info
// format:  display.color565 (R,G,B), with RGB values from 0 to 255
// default is set really dim (~10% of max), increase for more brightness if needed

int color_disp = display.color565(40, 40, 50);  // primary color


// some other colors
// R G B
int cc_blk = display.color565(0, 0, 0);         // black
int cc_wht = display.color565(25, 25, 25);      // white
int cc_bwht = display.color565(255, 255, 255);  // bright white
int cc_red = display.color565(50, 0, 0);        // red
int cc_bred = display.color565(255, 0, 0);      // bright red
int cc_org = display.color565(25, 10, 0);       // orange
int cc_borg = display.color565(255, 165, 0);    // bright orange
int cc_grn = display.color565(0, 45, 0);        // green
int cc_bgrn = display.color565(0, 255, 0);      // bright green
int cc_blu = display.color565(0, 0, 150);       // blue
int cc_bblu = display.color565(0, 128, 255);    // bright blue
int cc_ylw = display.color565(45, 45, 0);       // yellow
int cc_bylw = display.color565(255, 255, 0);    // bright yellow
int cc_gry = display.color565(10, 10, 10);      // gray
int cc_bgry = display.color565(128, 128, 128);  // bright gray
int cc_dgr = display.color565(3, 3, 3);         // dark grey
int cc_cyan = display.color565(0, 30, 30);      // cyan
int cc_bcyan = display.color565(0, 255, 255);   // bright cyan
int cc_ppl = display.color565(25, 0, 25);       // purple
int cc_bppl = display.color565(255, 0, 255);    // bright purple

// Colors for time, wind, date and weather text  (Temperature color varies based on actual temp)
int cc_time;
int cc_wind;
int cc_date;
int cc_wtext;

//===OTHER SETTINGS===
int ani_speed = 500;      // sets animation speed
int weather_refresh = 2;  // sets weather refresh interval in minutes; must be between 1 and 59
int morph_off = 0;        // display issue due to weather check

//=== POSITION ===
// to stop seeing data use "nosee" to move its position outside of the display range
byte nosee = 100;

// Set weather icon position; TF_cols = 4 by default
byte img_x = 7 * TF_COLS + 1;
byte img_y = 2;

// Date position
byte date_x = 2;
byte date_y = 26;

// Temperature position
byte tmp_x = 12 * TF_COLS;
byte tmp_y = 2;

// Wind position and label
// I'm not clear on why the first position is shifted by TF_COLS wasting space
//  byte wind_x = 1*TF_COLS;
byte wind_x = 0;
byte wind_y = 2;

// Pressure position and label
byte press_x = nosee;
byte press_y = nosee;
char press_label[] = "";

// Humidity postion and label
byte humi_x = nosee;
byte humi_y = nosee;
char humi_label[] = "%";

// Text weather condition
byte wtext_x = 5 * TF_COLS - 3;
byte wtext_y = 2;

String wind_lstr = "   ";
String humi_lstr = "   ";
int wind_humi;
String wind_direction[10] = { "  ", "N ", "NE", "E ", "SE", "S ", "SW", "W ", "NW", "N " };
String weather_text[12] = {"        "," SUNNY  ","P-CLOUDY","OVERCAST","  RAIN  ","T-STORMS","  SNOW  ","  HAZY  "," CLEAR  "," FOGGY  "," CLOUDY ","OVERCAST"};

// Color palette
void select_palette() {
  int x;
  x = atoi(config["ColorPalette"]);
  debug(F("Palette:"));
  debugln(x);

  switch (x) {
    default:
    case 1:
      cc_time = cc_cyan;
      cc_wind = cc_ylw;
      cc_date = cc_grn;
      cc_wtext = cc_wht;
      break;
    case 2:
      cc_time = cc_red;
      cc_wind = cc_ylw;
      cc_date = cc_blu;
      cc_wtext = cc_grn;
      break;
    case 3:
      cc_time = cc_blu;
      cc_wind = cc_grn;
      cc_date = cc_ylw;
      cc_wtext = cc_wht;
      break;
    case 4:
      cc_time = cc_ylw;
      cc_wind = cc_cyan;
      cc_date = cc_blu;
      cc_wtext = cc_grn;
      break;
    case 5:
      cc_time = cc_bblu;
      cc_wind = cc_grn;
      cc_date = cc_ylw;
      cc_wtext = cc_grn;
      break;
    case 6:
      cc_time = cc_org;
      cc_wind = cc_red;
      cc_date = cc_grn;
      cc_wtext = cc_ylw;
      break;
    case 7:
      cc_time = cc_grn;
      cc_wind = cc_ppl;
      cc_date = cc_cyan;
      cc_wtext = cc_ylw;
      break;
    case 8:
      cc_time = cc_cyan;
      cc_wind = cc_ylw;
      cc_date = cc_grn;
      cc_wtext = cc_wht;
      break;
    case 9:
      cc_time = cc_cyan;
      cc_wind = cc_ylw;
      cc_date = cc_grn;
      cc_wtext = cc_wht;
      break;
    case 10:
      cc_time = cc_cyan;
      cc_wind = cc_ylw;
      cc_date = cc_grn;
      cc_wtext = cc_wht;
      break;
    case 11:
      cc_time = cc_cyan;
      cc_wind = cc_ylw;
      cc_date = cc_grn;
      cc_wtext = cc_wht;
      break;
  }
}

#ifdef ESP8266
// ISR for display refresh
void display_updater() {
  int x;
  x = atoi(config["Brightness"]);
  if (x > 70 or x < 0)
    x = 70;

  display.display(x);  //set brightness
}
#endif
#ifdef ESP32
  void IRAM_ATTR display_updater(){
    int x;
    x = atoi(config["Brightness"]);
    if (x > 70 or x < 0)
      x = 70;
    // Increment the counter and set the time of ISR
    //portENTER_CRITICAL_ISR(&timerMux);
    display.display(1);
    //delayMicroseconds(0);
    //portEXIT_CRITICAL_ISR(&timerMux);
  }
#endif
/* #endregion */

/* #region configs */
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
  debugln(F("Loaded config...."));
  JsonObject documentRoot = config.as<JsonObject>();
  for (JsonPair keyValue : documentRoot) {
    debug(String(keyValue.key().c_str())+": ");
    debugln(keyValue.value().as<String>());
  }
}

int vars_read() {
  // //Remove file for testing when it has corrupt data
  // LittleFS.remove("/mvars.jsn");
  // File varf1 = LittleFS.open ("/mvars.jsn", "w");
  // varf1.close ();
  // return 1;

  File varf = LittleFS.open("/mvars.jsn", "r");
  if (!varf) {
    debugln(F("Failed to open config file"));
    varf.close();
    return 0;
  }
  DeserializationError error = deserializeJson(config, varf);
  if (error)  {
    debugln(F("Failed to deserialize config, using default configuration"));
    varf.close();
    return 0;
  }
  varf.close();

  String tmp = config["Hostname"];
  tmp.toLowerCase();
  tmp.replace(" ",  "-");
  tmp.replace("_",  "-");
  config["Hostname"] = tmp;
  
  return 1;
}

int vars_write() {
  File varf = LittleFS.open("/mvars.jsn", "w");
  if (!varf) {
    debugln(F("Failed to open config file"));
    varf.close();
    return 0;
  }

  String tmp = config["Hostname"];
  tmp.toLowerCase();
  tmp.replace(" ",  "-");
  tmp.replace("_",  "-");
  config["Hostname"] = tmp;

  debugln(F("Writing config file ......"));
  if (serializeJson(config, varf) == 0) {
    debugln(F("Failed to write to file"));
  }
  varf.close();
  return 1;
}

void init_config_vars() {
  debugln(F("Setting Default Config"));
  config = DefaultConfig();

  String tmp = config["Hostname"];
  tmp.toLowerCase();
  tmp.replace(" ",  "-");
  tmp.replace("_",  "-");
  config["Hostname"] = tmp;
}
/* #endregion */

/* #region Setups */
void resetclock() {
  display.fillScreen(0);
  TFDrawText(&display, String("  RESTART  "), 10, 9, cc_blu);
  TFDrawText(&display, String("MORPH CLOCK"), 10, 16, cc_blu);
  delay(2000);
  #ifdef ESP8266
    ESP.reset();
  #endif
  #ifdef ESP32
    ESP.restart();
  #endif
}


void setupDisplay(bool is_enable) {
  #ifdef ESP8266
    display.begin(8);
    display.setFastUpdate(true);
    //display.setDriverChip(FM6126A);
    //display.setMuxDelay(0,1,0,0,0);
    if (is_enable)
        display_ticker.attach(0.004, display_updater);
      else
        display_ticker.detach();

  #endif
  #ifdef ESP32
    display.begin(16,SPI_BUS_CLK, SPI_BUS_MOSI, SPI_BUS_MISO, SPI_BUS_SS);
    display.setFastUpdate(true);
    
    if (is_enable)
        display_ticker.attach(0.004, display_updater);
      else
        display_ticker.detach();
  //  if (is_enable) {
  //     timer = timerBegin(3, 240, true);
  //     timerAttachInterrupt(timer, &display_updater,false);
  //     //timerAttachInterrupt(timer, &dumb_print,false);
  //     timerAlarmWrite(timer, 2000, true);
  //     delayMicroseconds(0);
  //     timerAlarmEnable(timer);
  //   }
  //   else {
  //     timerDetachInterrupt(timer);
  //     timerAlarmDisable(timer);
  //    }
  #endif
}

int connect_wifi(String n_ssid, String n_pass) {
  int c_cnt = 0;
  debug(F("Trying WiFi Connect:"));
  debugln(n_ssid);
  WiFi.hostname(config["Hostname"].as<String>());
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  //WiFi.setTxPower(WIFI_POWER_8_5dBm);
  WiFi.begin(n_ssid, n_pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    debug(".");
    c_cnt++;
    if (c_cnt > 50) {
      debugln(F("Wifi Connect Failed"));
      return 1;
    }
  }
  debugln(F("success!"));
  debug(F("IP Address is: "));
  debugln(WiFi.localIP());  //
  return 0;
}

void setupWIFI() {
  if (connect_wifi(config["SSID"].as<String>(), config["Password"].as<String>()) == 1) {  // Try settings in config file
    debugln(F("Cannot connect to anything, RESTART ESP"));
    TFDrawText(&display, String("WIFI FAILED CONFIG"), 1, 10, cc_grn);
    JsonDocument defaultconfig = DefaultConfig();
    if (connect_wifi(defaultconfig["SSID"].as<String>(), defaultconfig["Password"].as<String>()) == 1) {  // Try settings in params.h
      debugln(F("Cannot connect to anything, RESTART ESP"));
      TFDrawText(&display, String("WIFI FAILED PARAMS.H"), 1, 10, cc_grn);
      resetclock();
    }
  }
  return;
}

void setupmDNS(bool verbose) {
  #ifdef ESP8266
  #endif

  #ifdef ESP32
  if (!MDNS.begin(config["Hostname"].as<const char*>())) {
        debugln(F("Error setting up MDNS responder!"));
        while(1) {
            delay(1000);
        }
    }
    debugln("mDNS Hostname: " + config["Hostname"].as<String>());
  #endif
}

int setupTimeOffset(bool verbose) {
  int tzOffSet = 0;
  if ((config["DST"]))
  {
    if (config["TimeZone"] == "EST") {
      if (usET.utcIsDST(now()))
      {
        tzOffSet = (usEDT.offset)/60;
        if (verbose) debug(F("EDT:"));
        if (verbose) debugln(tzOffSet);
      }
      else
      {
        tzOffSet = (usEST.offset)/60;
        if (verbose) debug(F("EST:"));
        if (verbose) debugln(tzOffSet);
      }
    }
    // else if (config["TimeZone"] == "CST") {
    //   if (usCT.utcIsDST(now()))
    //   {
    //     tzOffSet = (usCDT.offset)/60;
    //     if (verbose) debug(F("CDT:"));
    //     if (verbose) debugln(tzOffSet);
    //   }
    //   else
    //   {
    //     tzOffSet = (usCST.offset)/60;
    //     if (verbose) debug(F("CST:"));
    //     if (verbose) debugln(tzOffSet);
    //   }
    // }
    // else if (config["TimeZone"] == "MST") {
    //   if (usMT.utcIsDST(now()))
    //   {
    //     tzOffSet = (usMDT.offset)/60;
    //     if (verbose) debug(F("MDT:"));
    //     if (verbose) debugln(tzOffSet);
    //   }
    //   else
    //   {
    //     tzOffSet = (usMST.offset)/60;
    //     if (verbose) debug(F("MST:"));
    //     if (verbose) debugln(tzOffSet);
    //   }
    // }
    // else if (config["TimeZone"] == "PST") {
    //   if (usPT.utcIsDST(now()))
    //   {
    //     tzOffSet = (usPDT.offset)/60;
    //     if (verbose) debug(F("PDT:"));
    //     if (verbose) debugln(tzOffSet);
    //   }
    //   else
    //   {
    //     tzOffSet = (usPST.offset)/60;
    //     if (verbose) debug(F("PST:"));
    //     if (verbose) debugln(tzOffSet);
    //   }
    // }
    // else if (config["TimeZone"] == "AST") {
    //   if (usAT.utcIsDST(now()))
    //   {
    //     tzOffSet = (usADT.offset)/60;
    //     if (verbose) debug(F("ADT:"));
    //     if (verbose) debugln(tzOffSet);
    //   }
    //   else
    //   {
    //     tzOffSet = (usAST.offset)/60;
    //     if (verbose) debug(F("AST:"));
    //     if (verbose) debugln(tzOffSet);
    //   }
    // }
    // else if (config["TimeZone"] == "HAST") {
    //   if (usHAT.utcIsDST(now()))
    //   {
    //     tzOffSet = (usHADT.offset)/60;
    //     if (verbose) debug(F("HADT:"));
    //     if (verbose) debugln(tzOffSet);
    //   }
    //   else
    //   {
    //     tzOffSet = (usHAST.offset)/60;
    //     if (verbose) debug(F("HAST:"));
    //     if (verbose) debugln(tzOffSet);
    //   }
    // }
    else if (config["TimeZone"] == "CustomST") {
      //Future CustomST Rules from web UI
    }
  }
  else {
    tzOffSet = atoi(config["TimeZone"]);
  }
  return tzOffSet;
}

void setupNTP(bool verbose) {
  int tzOffSet = 0;
  tzOffSet = setupTimeOffset(verbose);
  int tzOffSetms = tzOffSet * 3600; //seconds 3600=1hour
  timeClient.setTimeOffset(tzOffSetms);
  timeClient.setUpdateInterval(3600000); //milliseconds. 3600000=1hour
  timeClient.setPoolServerName(config["NTPServer"]);
  timeClient.begin();
  ntpsync = 1;
  return;
}

void getWeatherjson(bool verbose) {
  if (!sizeof(config["apiKey"])) {
    debugln(F("Missing API KEY for weather data, skipping"));
    return;
  }
  if (!sizeof(config["GeoLocation"])){
    if(client.connect(ifconfigme,80)){
      client.print("GET");
      client.println("Host: ifconfig.me");
      client.println("Connection: close");
      client.println();
    }else{
    if (verbose) debugln(F("ifconfig.me unreachable"));
      return;
    }
    externalIp = client.readStringUntil('\n');
    if(client.connect(ipapi,80)){
      client.print("GET /#"+externalIp);
      client.println("Host: api.openweathermap.org");
      client.println("Connection: close");
      client.println();
    }else {
      if (verbose) debugln(F("ip-api unreacheable"));
      return;
    }
  }

  //client.setTimeout(500);  //readStringUntil has a default 5 second wait
  if (client.connect(openweather, 80)) {
    client.print("GET /data/2.5/weather?q=" + config["GeoLocation"].as<String>() + "&appid=" + config["apiKey"].as<String>() + "&cnt=1");
    if (config["Metric"])
      client.println("&units=metric");
    else
      client.println("&units=imperial");

    client.println("Host: api.openweathermap.org");
    client.println("Connection: close");
    client.println();
  } else {
    if (verbose) debugln(F("Weather:unable to connect"));
    return;
  }
  Weatherjson = client.readStringUntil('\n');
  return;
  // Sample of what the weather API sends back
  //  {"coord":{"lon":-80.1757,"lat":33.0185},"weather":[{"id":741,"main":"Fog","description":"fog","icon":"50n"},
  //  {"id":500,"main":"Rain","description":"light rain","icon":"10n"}],"base":"stations","main":{"temp":55.47,
  //  "feels_like":55.33,"temp_min":52.81,"temp_max":57.79,"pressure":1014,"humidity":98},"visibility":402,
  //  "wind":{"speed":4.61,"deg":0},"rain":{"1h":0.25},"clouds":{"all":100},
  //  "dt":1647516313,"sys":{"type":2,"id":2034311,"country":"US","sunrise":1647516506,
  //  "sunset":1647559782},"timezone":-14400,"id":4597919,"name":"Summerville","cod":200}
}

void processWeather(bool verbose) {
  String line = Weatherjson;
  if (verbose) debugln(line);
  
  // Sample of what the weather API sends back
  //  {"coord":{"lon":-80.1757,"lat":33.0185},"weather":[{"id":741,"main":"Fog","description":"fog","icon":"50n"},
  //  {"id":500,"main":"Rain","description":"light rain","icon":"10n"}],"base":"stations","main":{"temp":55.47,
  //  "feels_like":55.33,"temp_min":52.81,"temp_max":57.79,"pressure":1014,"humidity":98},"visibility":402,
  //  "wind":{"speed":4.61,"deg":0},"rain":{"1h":0.25},"clouds":{"all":100},
  //  "dt":1647516313,"sys":{"type":2,"id":2034311,"country":"US","sunrise":1647516506,
  //  "sunset":1647559782},"timezone":-14400,"id":4597919,"name":"Summerville","cod":200}
 
  String sval = "";
  
  if (!line.length())
    debugln(F("Weather:unable to retrieve data"));
  else {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, line.c_str());
    if (error) return;
    //condS
    sval = doc["weather"][0]["icon"].as<String>();
    if (verbose) debug(F("condS:"));
    if (verbose) debugln(sval);
    if (sval) {
      //0 - unk, 1 - sunny, 2 - cloudy, 3 - overcast, 4 - rainy, 5 - thunders, 6 - snow
      if (sval.equals("01d"))
        condM = 1;  //sunny
      else if (sval.equals("01n"))
        condM = 8;  //clear night
      else if (sval.equals("02d"))
        condM = 2;  //partly cloudy day
      else if (sval.equals("02n"))
        condM = 10;  //partly cloudy night
      else if (sval.equals("03d"))
        condM = 3;  //overcast day
      else if (sval.equals("03n"))
        condM = 11;  //overcast night
      else if (sval.equals("04d"))
        condM = 3;  //overcast day
      else if (sval.equals("04n"))
        condM = 11;  //overcast night
      else if (sval.equals("09d"))
        condM = 4;  //rain
      else if (sval.equals("09n"))
        condM = 4;
      else if (sval.equals("10d"))
        condM = 4;
      else if (sval.equals("10n"))
        condM = 4;
      else if (sval.equals("11d"))
        condM = 5;  //thunder
      else if (sval.equals("11n"))
        condM = 5;
      else if (sval.equals("13d"))
        condM = 6;  //snow
      else if (sval.equals("13n"))
        condM = 6;
      else if (sval.equals("50d"))
        condM = 7;  //haze (day)
      else if (sval.equals("50n"))
        condM = 9;  //fog (night)
       
      condS = sval;
      if (verbose) debug(F("condM:"));
      if (verbose) debugln(condM);
    }
    //tempM
    sval = doc["main"]["temp"].as<String>();
    if (verbose) debug(F("tempM:"));
    if (verbose) debugln(sval);
    if (sval) {
      tempM = sval.toInt();
    } else
      debugln(F("temp NOT found!"));

    //humiM
    sval = doc["main"]["humidity"].as<String>();
    if (verbose) debug(F("humiM:"));
    if (verbose) debugln(sval);
    if (humiM) {
      humiM = sval.toInt();
    } else
      debugln(F("humidity NOT found!"));

    //wind speed
    sval = doc["wind"]["speed"].as<String>();
    if (verbose) debug(F("wind speed:"));
    if (verbose) debugln(sval);
    if (sval) {
      wind_speed = sval.toInt();
    } else {
      wind_speed = -10000;
      debugln(F("wind speed NOT found"));
    }

    // Timezone offset
    //   bT = line.indexOf ("\"timezone\":");  
    //   if (bT > 0)
    //   {
    //     int tz;
    //     bT2 = line.indexOf (",\"", bT + 11);
    //     sval = line.substring (bT + 11, bT2);
    //     tz = sval.toInt()/3600;
    //   }
    //   else
    //     Serial.println ("timezone offset NOT found!");

    //wind direction
    sval = doc["wind"]["deg"].as<String>();
    if (verbose) debug(F("wind direction:"));
    if (verbose) debugln(sval);
    if (sval) {
      wind_nr = round(((sval.toInt() % 360)) / 45.0) + 1;
    } else {
      wind_nr = 0;
      debugln(F("wind direction NOT found"));
    }
  }
}
/* #endregion */

/* #region Draws */
void draw_wind() {
  if (wind_speed < 0)
    return;
  wind_lstr = String(wind_speed);
  if (wind_speed != 0) {
    switch (wind_lstr.length()) {  //We have to pad the string to exactly 4 characters
      case 1:
        wind_lstr = String(wind_lstr) + String(" ");
        break;
    }
    TFDrawText(&display, wind_direction[wind_nr], wind_x, wind_y, cc_wht);  //Change Wind Direction color for readability
    TFDrawText(&display, wind_lstr, wind_x + 8, wind_y, cc_wind);
  } else {
    wind_lstr = String("CALM");
    TFDrawText(&display, wind_lstr, wind_x, wind_y, cc_wind);
  }
  wind_humi = 1;  //Reset switch for toggling wind or humidity display
}

void draw_weather_conditions() {
  //0 - unk, 1 - sunny, 2 - cloudy, 3 - overcast, 4 - rainy, 5 - thunders, 6 - snow, 7, hazy, 9 - fog

  xo = img_x;
  yo = img_y;

  if (condM == 0) {
    debug(F("!weather condition icon unknown, show: "));
    debugln(condS);
    int cc_dgr = display.color565(30, 30, 30);
    //draw the first 5 letters from the unknown weather condition
    String lstr = condS.substring(0, (condS.length() > 5 ? 5 : condS.length()));
    lstr.toUpperCase();
    TFDrawText(&display, lstr, xo, yo, cc_dgr);
  } else {
    //  TFDrawText (&display, String("     "), xo, yo, 0);
  }
  xo = img_x;
  yo = img_y;
  switch (condM) {
    case 0:  //unk
      break;
    case 1:  //sunny
      DrawIcon(&display, sunny_ico, xo, yo, 10, 5);
      use_ani = 1;
      break;
    case 2:  //cloudy
      DrawIcon(&display, cloudy_ico, xo, yo, 10, 5);
      use_ani = 1;
      break;
    case 3:  //overcast
      DrawIcon(&display, ovrcst_ico, xo, yo, 10, 5);
      use_ani = 1;
      break;
    case 4:  //rainy
      DrawIcon(&display, rain_ico, xo, yo, 10, 5);
      use_ani = 1;
      break;
    case 5:  //thunders
      DrawIcon(&display, thndr_ico, xo, yo, 10, 5);
      use_ani = 1;
      break;
    case 6:  //snow
      DrawIcon(&display, snow_ico, xo, yo, 10, 5);
      use_ani = 1;
      break;
    case 7:  //mist
      DrawIcon(&display, mist_ico, xo, yo, 10, 5);
      use_ani = 1;
      break;
    case 8:  //clear night
      DrawIcon(&display, moony_ico, xo, yo, 10, 5);
      use_ani = 1;
      break;
    case 9:  //fog night
      DrawIcon(&display, mistn_ico, xo, yo, 10, 5);
      use_ani = 1;
      break;
    case 10:  //partly cloudy night
      DrawIcon(&display, cloudyn_ico, xo, yo, 10, 5);
      use_ani = 1;
      break;
    case 11:  //cloudy night
      DrawIcon(&display, ovrcstn_ico, xo, yo, 10, 5);
      use_ani = 1;
      break;
  }
}

void draw_weather() {
  //int value = 0;

  if (tempM == -10000 && humiM == -10000 && presM == -10000) {
    TFDrawText (&display, String("NO WEATHER DATA"), 1, 1, cc_dgr);
    debugln(F("No weather data available"));
  } else {

    //-temperature
    int lcc = cc_red;
    char tmp_Unit;
    if (config["Metric"]) {
      tmp_Unit = 'C';
      lcc = cc_red;
      if (tempM < 30)
        lcc = cc_org;
      if (tempM < 25)
        lcc = cc_ylw;
      if (tempM < 20)
        lcc = cc_grn;
      if (tempM < 15)
        lcc = cc_blu;
      if (tempM < 10)
        lcc = cc_cyan;
      if (tempM < 1)
        lcc = cc_wht;
    } else {
      tmp_Unit = 'F';
      //F
      if (tempM < 90)
        lcc = cc_grn;
      if (tempM < 75)
        lcc = cc_blu;
      if (tempM < 33)
        lcc = cc_wht;
    }

    String lstr = String(tempM) + String(tmp_Unit);

    //Padding Temp with spaces to keep position the same
    switch (lstr.length()) {
      case 2:
        lstr = String("  ") + String(lstr);
        break;
      case 3:
        lstr = String(" ") + String(lstr);
        break;
    }

    TFDrawText(&display, lstr, tmp_x, tmp_y, lcc);  // draw temperature

    //weather conditions
    //-humidity
    lcc = cc_red;
    if (humiM < 80)
      lcc = cc_org;
    if (humiM < 60)
      lcc = cc_grn;
    if (humiM < 40)
      lcc = cc_blu;
    if (humiM < 20)
      lcc = cc_wht;

    // Pad humi to exactly 4 characters
    humi_lstr = String(humiM) + String(humi_label);
    switch (humi_lstr.length()) {
      case 2:
        humi_lstr = String(humi_lstr) + String("  ");
        break;
      case 3:
        humi_lstr = String(humi_lstr) + String(" ");
        break;
    }
    TFDrawText(&display, humi_lstr, humi_x, humi_y, lcc);  // humidity

    //int cc = color_disp;
    //cc = color_disp;

    //Draw wind speed and direction
    draw_wind();
    wind_humi = 1;  //Reset switch for toggling wind or humidity display

    if (!config["WeatherAnimation"]) {
     TFDrawText(&display, weather_text[condM], wtext_x, wtext_y, cc_wtext);
    } else {
      draw_weather_conditions();
    }
  }
}

void draw_date() {
  //date below the clock
  //long tnow = now();
  long tnow = timeClient.getEpochTime();
  String lstr = "";

  //sval = doc["wind"]["deg"].as<String>();
  for (int i = 0; i < 5; i += 2) {
    switch (config["DateFormat"].as<String>()[i]) {
      case 'D':
        lstr += (day(tnow) < 10 ? " " + String(day(tnow)) : String(day(tnow)));
        if (i < 4)
          lstr += config["DateFormat"].as<String>()[i + 1];
        break;
      case 'M':  //Replaced leading 0 with space where double quotes are
        lstr += (month(tnow) < 10 ? " " + String(month(tnow)) : String(month(tnow)));
        if (i < 4)
          lstr += config["DateFormat"].as<String>()[i + 1];
        break;
      case 'Y':
        lstr += String(year(tnow));
        if (i < 4)
          lstr += config["DateFormat"].as<String>()[i + 1];
        break;
    }
  }
  //
  String DayofWeek = "  ";
  switch (weekday(tnow)) {
    case 1:
      DayofWeek = " SUN";
      break;
    case 2:
      DayofWeek = " MON";
      break;
    case 3:
      DayofWeek = " TUE";
      break;
    case 4:
      DayofWeek = " WED";
      break;
    case 5:
      DayofWeek = " THR";
      break;
    case 6:
      DayofWeek = " FRI";
      break;
    case 7:
      DayofWeek = " SAT";
      break;
  }

  lstr += String(DayofWeek);

  if (lstr.length())
    TFDrawText(&display, lstr, date_x, date_y, cc_date);
}

void draw_am_pm() {
  // this sets AM/PM display and is disabled when military time is used
  if (!config["Military"]) {
    if (hh >= 12)
      TFDrawText(&display, String(" PM"), 42, 19, cc_time);
    else
      TFDrawText(&display, String(" AM"), 42, 19, cc_time);
  }
}

void draw_animations(int stp) {
  //weather icon animation
  String lstr = "";
  xo = img_x;
  yo = img_y;
  //0 - unk, 1 - sunny, 2 - cloudy, 3 - overcast, 4 - rainy, 5 - thunders, 6 - snow
  if (use_ani) {
    int *af = NULL;
    switch (condM) {
      case 1:
        af = suny_ani[stp % 5];
        break;
      case 2:
        af = clod_ani[stp % 10];
        break;
      case 3:
        af = ovct_ani[stp % 5];
        break;
      case 4:
        af = rain_ani[stp % 5];
        break;
      case 5:
        af = thun_ani[stp % 5];
        break;
      case 6:
        af = snow_ani[stp % 5];
        break;
      case 7:
        af = mist_ani[stp % 4];
        break;
      case 8:
        af = mony_ani[stp % 17];
        break;
      case 9:
        af = mistn_ani[stp % 4];
        break;
      case 10:
        af = clodn_ani[stp % 10];
        break;
      case 11:
        af = ovctn_ani[stp % 1];
        break;
    }
    //draw animation
    if (af)
      DrawIcon(&display, af, xo, yo, 10, 5);
  }
}

void set_digit_color() {
  digit0.SetColor(cc_time);
  digit1.SetColor(cc_time);
  digit2.SetColor(cc_time);
  digit3.SetColor(cc_time);
  digit4.SetColor(cc_time);

  // Don't print leading zero if not Military
  if (!config["Military"] && hh < 10)
    digit5.SetColor(cc_blk);
  else
    digit5.SetColor(cc_time);
}
/* #endregion */

/* #region WebServer */
//To find the values they are sandwiched between search and it always ends before "HTTP /"
//Pidx + ? is length of string searching for ie "?geoloc=" = length 8, pidx + 8
//pidx2 is end of string location for HTTP /
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

      //Check and Test new WiFi info
      if (connect_wifi(postcfg["SSID"], postcfg["Password"]) == 1) {
        debugln(F("Wifi Connect failed, will try prior SSID and Password"));
        if (connect_wifi(config["SSID"], config["Password"]) == 1)
          ESP.restart();  //Give up reboot
      }
      
      config = postcfg;

      vars_write();
      getWeatherjson(false);
      processWeather(false);
      draw_weather_conditions();
      //Write DST code in setupNTP
      setupNTP(false);
      select_palette();
      //display_updater();
      prevhh = -1;
      ntpsync = 1;  //force full redraw
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

        //it_config_vars();
        //vars_write();
        //vars_read();
        //resetclock
      }
      else {
        httpcli.println(F("HTTP/1.0 401 Unauthorized"));
        httpcli.println(F("Content-Type: application/json"));
        httpcli.println();
        delay(1);
        httpcli.stop();
      }
    }
    //Get /morph.js
    else if ((pidx = httprq.indexOf("GET /morph.js")) != -1) {
      morphJS2(httpcli);
    }
    //Get index
    else {
      index2(httpcli);
    }
  }
}
/* #endregion */

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial){ 
    delay(10);
  }
  //Begin
  debugln(F(""));
  debugln(F("Begin Setup"));

  // Read the config file
  #define FORMAT_LittleFS_IF_FAILED true
  if (LittleFS.begin(FORMAT_LittleFS_IF_FAILED)) {
    debugln(F("LittleFS Initialize....ok"));
    if (!vars_read()) {
      init_config_vars();  //Copy from params.h
    }
  } else {
    debugln(F("LittleFS Initialization...failed"));
  }
  
  show_config_vars();

  //start display
  setupDisplay(true);

  TFDrawText(&display, String("  MORPH CLOCK  "), 0, 1, cc_blu);
  TFDrawText(&display, String("  STARTING UP  "), 0, 10, cc_blu);

  String lstr = String("  TIMEZONE:") + config["TimeZone"].as<String>();
  TFDrawText(&display, lstr, 0, 24, cc_cyan);

  //connect to wifi network
  setupWIFI();
  TFDrawText(&display, String("WIFI CONNECTED "), 3, 10, cc_grn);
  TFDrawText(&display, String(WiFi.localIP().toString()), 4, 17, cc_grn);

  //start mDNS
  setupmDNS(true);

  //start NTP
  setupNTP(true);

  //get Weather
  getWeatherjson(true);
  processWeather(true);

  //start http
  httpsvr.begin();  // Start the HTTP Server

  // //prep screen for clock display
  select_palette();
  display.fillScreen(0);

  debugln(F("Setup Complete"));
  vars_write();
  //resetclock();
}

void loop() {
  // put your main code here, to run repeatedly:
  timeClient.update();
  // debugln(timeClient.getFormattedTime());
  // delay(1000);
  
  //handle web server requests
  web_server();

  digit1.DrawColon(cc_time);
  digit3.DrawColon(cc_time);

  static int i = 0;
  static int last = 0;
  static int cm;

  //animations?
  cm = millis();
  if ((cm - last) > ani_speed)  // animation speed
  {
    last = cm;
    i++;
  }
  //time changes every miliseconds, we only want to draw when digits actually change.
  //tnow = now();
  //hh = hour(tnow);
  //mm = minute(tnow);
  //ss = second(tnow);
  hh = timeClient.getHours();
  mm = timeClient.getMinutes();
  ss = timeClient.getSeconds();
  //


  //GQGQ    if (ntpsync or (hh != prevhh))   Fixed morphing bug that required refreshing the screen for hh change
  if (ntpsync) {

    ntpsync = 0;
    //
    prevss = ss;
    prevmm = mm;
    prevhh = hh;

    //we had a sync so draw without morphing

    //clear screen
    display_updater();
    display.fillScreen(0);
    debugln("Display cleared");

    //date and weather
    draw_date();
    draw_am_pm();
    draw_weather();
    //

    //military time?
    if (hh > 12 && !config["Military"])  // when not using military time
      hh -= 12;
    if (hh == 0 && !config["Military"])  // this makes the first hour of the day 12a when military time isn't used.
      hh += 12;

    set_digit_color();

    digit0.Draw(ss % 10);
    digit1.Draw(ss / 10);
    digit2.Draw(mm % 10);
    digit3.Draw(mm / 10);
    digit4.Draw(hh % 10);
    digit5.Draw(hh / 10);
  } else {
    //seconds
    if (ss != prevss) {

      int s0 = ss % 10;
      int s1 = ss / 10;
      set_digit_color();

      //There is a delay problem when checking the weather site which causes the morph to scramble the digit
      //We must force no morphing to make sure the digit is displayed correctly

      if (morph_off == 0) {
        if (s0 != digit0.Value()) digit0.Morph(s0);
      } else {
        digit0.SetColor(cc_blk);
        digit0.Draw(8);  //Turn off all segments to black
        morph_off = 0;
        digit0.SetColor(cc_time);
        digit0.Draw(s0);
      }
      if (s1 != digit1.Value()) digit1.Morph(s1);

      prevss = ss;
      //refresh weather at 31sec in the minute
      if (ss == 21 && ((mm % weather_refresh) == 0)) getWeatherjson(false);
      if (ss == 31 && ((mm % weather_refresh) == 0)) {
        processWeather(false);
        //morph_off = 0;  //Currently not using, set to 0 this was to address weather delay
      } else if ((ss % 10) == 0) {  // Toggle display every 10 seconds between wind and humidity
        if (wind_humi == 1) {
          TFDrawText(&display, humi_lstr, wind_x, wind_y, cc_wind);
          wind_humi = 0;
        } else {
          draw_wind();
          wind_humi = 1;
        }
      }
    }
    //minutes
    if (mm != prevmm) {
      int m0 = mm % 10;
      int m1 = mm / 10;
      if (m0 != digit2.Value()) digit2.Morph(m0);
      if (m1 != digit3.Value()) digit3.Morph(m1);
      prevmm = mm;
      draw_weather();
    }
    //hours

    if (hh != prevhh) {
      display_updater();
      prevhh = hh;

      draw_date();
      draw_am_pm();

      //military time?
      if (hh > 12 && !config["Military"])
        hh -= 12;
      //
      if (hh == 0 && !config["Military"])  // this makes the first hour of the day 12a when military time isn't used.
        hh += 12;


      int h0 = hh % 10;
      int h1 = hh / 10;
      set_digit_color();

      digit4.Morph(h0);

      if (!config["Military"] && hh < 10)  //We have to clear leading zero to black
        digit5.Draw(h1);

      if (h1 != digit5.Value()) digit5.Morph(h1);
      timeClient.update();
      ntpsync = 1;
    }  //hh changed

    if (config["WeatherAnimation"])
      draw_animations(i);
  }
}
