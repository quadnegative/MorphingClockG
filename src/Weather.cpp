#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "Config.h"
#include "NTP.h"
#include "Main.h"
#include "Display.h"
#include "Colors.h"
#include "TinyFont.h"
#include "TinyIcons.h"
//#include "WeatherIcons.h"
#include "Display.h"
#include "Icons.h"

JsonDocument Weatherjson;
String weather_text[12] = {"        ","  SUNNY  "," P-CLOUDY"," OVERCAST","   RAIN  "," T-STORMS","   SNOW  ","   HAZY  ","  CLEAR  ","  FOGGY  ","  CLOUDY "," OVERCAST"};
String wind_direction[10] = { "  ", "N ", "NE", "E ", "SE", "S ", "SW", "W ", "NW", "N " };
bool callingAPI = false;
unsigned long previousMillis = 0;
const long interval = 1000;
int OWM_refresh = 5;  // sets weather refresh interval in minutes; must be between 1 and 59
long pss;
int ani_speed = 500;
int last_cm = 0;
int i_frame = 0;
int ani_x = 1;
int ani_y = 24;
int nani_x = 1;
int nani_y = 8;
//int wtext_x = 5 * TF_COLS - 3;
int wtext_x = 13;
int wtext_y = 0;
int temp_x = 14 * TF_COLS - 2;
//int temp_x = -1;
int temp_y = 0;
int wind_x = -1;
int wind_y = 24;
int humi_x = -1;
//int humi_y = 24;
int humi_y = 0;

bool validateAPIkey(String key){
  bool valid = false;
  if (!sizeof(config["apiKey"])) {
    debugln(F("OpenWeatherMap: Missing API KEY for weather data, skipping"));
    return valid;
  }
  const char apiServer[] = "https://api.openweathermap.org/data/2.5/weather?lat=41.4902&lon=-91.5754&appid=";
  HTTPClient http;
  http.begin(apiServer+key);
  int httpResponseCode = http.GET();
  if (httpResponseCode == 200) {
    valid = true;
  } 
  else if (httpResponseCode == 401) {
    debugln(F("OpenWeatherMap: Invalid API key"));
  }
  else if (httpResponseCode == 429) {
    debugln(F("OpenWeatherMap: Exceeded API call limit"));
  }
  else {
    debugln(F("OpenWeatherMap: bad response"));
  }
  http.end();
  return valid;
}

void getWeatherjson(bool verbose, int trys) {
  if (!sizeof(config["apiKey"])) {
    debugln(F("OpenWeatherMap: Missing API KEY for weather data, skipping"));
    return;
  }
  if (verbose) debugln(F("OpenWeatherMap: Calling API"));
  int cnt = 0;
  int httpResponseCode = 0;
  String apiServer = "";
  if (config["GeoLocation"] == "Auto") {
    if (config["GeoLocationAuto"]) {
      apiServer = "https://api.openweathermap.org/data/2.5/weather?" + config["GeoLocationAuto"].as<String>() + "&appid=" + config["apiKey"].as<String>() + "&cnt=1";
    }
  }
  else 
    apiServer = "https://api.openweathermap.org/data/2.5/weather?" + config["GeoLocation"].as<String>() + "&appid=" + config["apiKey"].as<String>() + "&cnt=1";
  if (config["Metric"])
    apiServer += "&units=metric";
  else
    apiServer += "&units=imperial";
  if (verbose) debugln("OpenWeatherMap: " + apiServer);
  HTTPClient http;
  http.begin(apiServer);
  while (cnt < trys) {
    httpResponseCode = http.GET();
    if (httpResponseCode == 200) {
      DeserializationError error = deserializeJson(Weatherjson, http.getString());
      if (error) {
        if (verbose) dma_display->drawIcon(x_ico, 4 , 25, 5, 5);
        return;
      }
      if (verbose) debugln(Weatherjson.as<String>());
      cnt = 9;
    }
    else {
      if (verbose) debugln("OpenWeatherMap: " + String(httpResponseCode) + ": " + http.getString());
      if (verbose) debugln(F("OpenWeatherMap: Retrying API Call"));
      cnt++;
      delay(500);
    }
  }
  http.end();
  clockStartingUp = true;
  if (cnt > 3 && cnt != 9) {
    if (verbose) dma_display->drawIcon(x_ico, 4 , 25, 5, 5);
  }
  else if (verbose && cnt == 9) if (verbose) dma_display->drawIcon(check_ico, 4 , 25, 5, 5);
  //clockStartingUp = true;
  // Sample of what the weather API sends back
  //  {"coord":{"lon":-80.1757,"lat":33.0185},"weather":[{"id":741,"main":"Fog","description":"fog","icon":"50n"},
  //  {"id":500,"main":"Rain","description":"light rain","icon":"10n"}],"base":"stations","main":{"temp":55.47,
  //  "feels_like":55.33,"temp_min":52.81,"temp_max":57.79,"pressure":1014,"humidity":98},"visibility":402,
  //  "wind":{"speed":4.61,"deg":0},"rain":{"1h":0.25},"clouds":{"all":100},
  //  "dt":1647516313,"sys":{"type":2,"id":2034311,"country":"US","sunrise":1647516506,
  //  "sunset":1647559782},"timezone":-14400,"id":4597919,"name":"Summerville","cod":200}
}

String adjustTempSpacing(int temp) {
  String str = "";
  if (temp < 10) str = " " + String(temp);
  else str = String(temp);
  return str;
}

void displayTemp() {
  float tf = Weatherjson["main"]["temp"];
  int tempM  = tf + 0.5;
  int lcc = cc_red;
  char tmp_Unit;
  if (config["Metric"]) {
    tmp_Unit = 'C';
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
  }
  else {
    tmp_Unit = 'F';
    if (tempM < 90)
      lcc = cc_org;
    if (tempM < 80)
      lcc = cc_grn;
    if (tempM < 70)
      lcc = cc_cyan;
    if (tempM < 33)
      lcc = cc_wht;
  }

  if (tempM <= 99) {
    TFDrawTextDMA(dma_display, String(adjustTempSpacing(tempM)), temp_x, temp_y, lcc);
    // dma_display->drawPixel(8, 0, lcc);
  }
  else if (tempM > 99) {
    TFDrawTextDMA(dma_display, String(adjustTempSpacing(tempM)), temp_x - 3, temp_y, lcc);
    // dma_display->drawPixel(10, 0, lcc);
  }
  else if (tempM < 0) {
    TFDrawTextDMA(dma_display, String(adjustTempSpacing(tempM)), 0, temp_y, lcc);
    // dma_display->drawPixel(12, 0, lcc);
  }
  dma_display->drawPixel(63, 0, lcc);
  //TFDrawTextDMA(dma_display, String(adjustTempSpacing(Weatherjson["main"]["temp_max"], tmp_Unit)), temp_x, temp_y + 6, cc_dorg);
  //TFDrawTextDMA(dma_display, String(adjustTempSpacing(Weatherjson["main"]["temp_min"], tmp_Unit)), temp_x, temp_y + 12, cc_blu);
}

// void displayConditionTexts(String text) {
//   text.toUpperCase();
//   if ((text.length() * TF_COLS) > (63 - wtext_x))
//     TFScrollTextDMA(dma_display, text, wtext_x, wtext_y, (63 - wtext_x), cc_wtext);
//   else { 
//     TFDrawTextDMA(dma_display, text, wtext_x, wtext_y, cc_wtext);
//   }
// }

template<typename T, size_t size> 
size_t GetArrLength(T(&)[size]){return size;}

void displayCondition(int condM, int *ico, int **ani, int frames) {
  // int ss = timeClient.getSeconds();
  // // int wss = (ss % 30);
  // if (0 <= ss && ss <= 29) {
  //   displayConditionTexts(Weatherjson["weather"][0]["main"].as<String>());
  // }
  // if (30 <= ss && ss <= 59 ) {
  //   displayConditionTexts(Weatherjson["weather"][0]["description"].as<String>());
  // }
  TFDrawTextDMA(dma_display, weather_text[condM], wtext_x, wtext_y, cc_wtext);
  if (config["WeatherAnimation"]) {
    int cm = millis();    
    if ((cm - last_cm) > ani_speed)  // animation speed
    {
      last_cm = cm;
      DrawIconDMA(dma_display, ani[i_frame], nani_x, nani_y, 12, 12);
      i_frame++;
      if (i_frame == frames) i_frame = 0;
    }
  }
  else {
    DrawIconDMA(dma_display, ico, nani_x, nani_y, 12, 12);
  }
}

void displayConditions() {
  String sval = Weatherjson["weather"][0]["icon"].as<String>();
  
  if (sval) {
    //0 - unk, 1 - sunny, 2 - cloudy, 3 - overcast, 4 - rainy, 5 - thunders, 6 - snow
    if (sval == "01d") {
      // condM = 1;  //sunny
      // DrawIconDMA(dma_display, sunny_ico, suny_ani[], xo, yo, 10, 5);
      displayCondition(1, sun_ico, sun_ani, GetArrLength(sun_ani));

      // DrawIconDMA(dma_display, sun_ico, nani_x, nani_y, 12, 12);
    }
    else if (sval == "01n") {
      // condM = 8;  //clear night
      // DrawIconDMA(dma_display, moony_ico, xo, yo, 10, 5);
      displayCondition(8, moon_ico, moon_ani, GetArrLength(moon_ani));

      // DrawIconDMA(dma_display, moon_ico, nani_x, nani_y, 12, 12);
    }
    else if (sval == "02d") {
      // condM = 2;  //partly cloudy day
      // DrawIconDMA(dma_display, cloudy_ico, xo, yo, 10, 5);
      displayCondition(2, few_clouds_d_ico, few_clouds_d_ani, GetArrLength(few_clouds_d_ani));
    }
    else if (sval == "02n") {
      // condM = 10;  //partly cloudy night
      // DrawIconDMA(dma_display, cloudyn_ico, xo, yo, 10, 5);
      displayCondition(10, few_clouds_n_ico, few_clouds_n_ani, GetArrLength(few_clouds_n_ani));
    }
    else if (sval == "03d") {
      // condM = 3;  //overcast day
      // DrawIconDMA(dma_display, ovrcst_ico, xo, yo, 10, 5);
      displayCondition(3, scattered_clouds_ico, scattered_clouds_ani, GetArrLength(scattered_clouds_ani));

      // DrawIconDMA(dma_display, scattered_clouds_ico, nani_x, nani_y, 12, 12);
    }
    else if (sval == "03n") {
      // condM = 11;  //overcast night
      // DrawIconDMA(dma_display, ovrcstn_ico, xo, yo, 10, 5);
      displayCondition(11, scattered_clouds_ico, scattered_clouds_ani, GetArrLength(scattered_clouds_ani));

      // DrawIconDMA(dma_display, scattered_clouds_ico, nani_x, nani_y, 12, 12);
    }
    else if (sval == "04d") {
      // condM = 3;  //overcast day
      // DrawIconDMA(dma_display, ovrcst_ico, xo, yo, 10, 5);
      displayCondition(3, broken_clouds_ico, broken_clouds_ani, GetArrLength(broken_clouds_ani));

      // DrawIconDMA(dma_display, broken_clouds_ico, nani_x, nani_y, 12, 12);
    }
    else if (sval == "04n") {
      // condM = 11;  //overcast night
      // DrawIconDMA(dma_display, ovrcstn_ico, xo, yo, 10, 5);
      displayCondition(11, broken_clouds_ico, broken_clouds_ani, GetArrLength(broken_clouds_ani));

      // DrawIconDMA(dma_display, broken_clouds_ico, nani_x, nani_y, 12, 12);
    }
    else if (sval == "09d") {
      // condM = 4;  //rain
      // DrawIconDMA(dma_display, rain_ico, rain_ani, xo, yo, 10, 5);
      displayCondition(4, shower_rain_ico, shower_rain_ani, GetArrLength(shower_rain_ani));
    }
    else if (sval == "09n") {
      // condM = 4;  //rain
      // DrawIconDMA(dma_display, rain_ico, rain_ani, xo, yo, 10, 5);
      displayCondition(4, shower_rain_ico, shower_rain_ani, GetArrLength(shower_rain_ani));
    }
    else if (sval == "10d") {
      // condM = 4;  //rain
      // DrawIconDMA(dma_display, rain_ico, rain_ani, xo, yo, 10, 5);
      displayCondition(4, rain_d_ico, rain_d_ani, GetArrLength(rain_d_ani));
    }
    else if (sval == "10n") {
      // condM = 4;  //rain
      // DrawIconDMA(dma_display, rain_ico, rain_ani, xo, yo, 10, 5);
      displayCondition(4, rain_n_ico, rain_n_ani, GetArrLength(rain_n_ani));
    }
    else if (sval == "11d") {
      // condM = 5;  //thunder
      // DrawIconDMA(dma_display, thndr_ico, xo, yo, 10, 5);
      displayCondition(5, thunderstorm_ico, thunderstorm_ani, GetArrLength(thunderstorm_ani));
    }
    else if (sval == "11n") {
      // condM = 5;  //thunder
      // DrawIconDMA(dma_display, thndr_ico, xo, yo, 10, 5);
      displayCondition(5, thunderstorm_ico, thunderstorm_ani, GetArrLength(thunderstorm_ani));
    }
    else if (sval == "13d") {
      // condM = 6;  //snow
      // DrawIconDMA(dma_display, snow_ico, xo, yo, 10, 5);
      displayCondition(6, snow_ico, snow_ani, GetArrLength(snow_ani));
    }
    else if (sval == "13n") {
      // condM = 6;  //snow
      // DrawIconDMA(dma_display, snow_ico, xo, yo, 10, 5);
      displayCondition(6, snow_ico, snow_ani, GetArrLength(snow_ani));
    }
    else if (sval == "50d") {
      // condM = 7;  //haze (day)
      // DrawIconDMA(dma_display, mist_ico, xo, yo, 10, 5);
      displayCondition(7, mist_ico, mist_ani, GetArrLength(mist_ani));
    }
    else if (sval == "50n") {
      // condM = 9;  //fog (night)
      // DrawIconDMA(dma_display, mistn_ico, xo, yo, 10, 5);
      displayCondition(9, mist_ico, mist_ani, GetArrLength(mist_ani));
    }
  }
}

void displayWind() {
  int winds = Weatherjson["wind"]["speed"];
  int windd = Weatherjson["wind"]["deg"];
  int wind_nr = round(((windd % 360)) / 45.0) + 1;
  String wind_lstr = String(winds);
  switch (wind_lstr.length()) {  //We have to pad the string to exactly 4 characters
    case 1:
      wind_lstr = String(wind_lstr) + String(" ");
      break;
  }
  TFDrawTextDMA(dma_display, wind_direction[wind_nr], wind_x, wind_y, cc_wind);  //Change Wind Direction color for readability
  TFDrawTextDMA(dma_display, wind_lstr, wind_x + 8, wind_y, cc_wind);
}

void displayHumidity() {
  int lcc = cc_red;
  if (Weatherjson["main"]["humidity"] < 80)
    lcc = cc_org;
  if (Weatherjson["main"]["humidity"] < 60)
    lcc = cc_grn;
  if (Weatherjson["main"]["humidity"] < 40)
    lcc = cc_cyan;
  if (Weatherjson["main"]["humidity"] < 20)
    lcc = cc_wht;

  String humi_lstr = Weatherjson["main"]["humidity"].as<String>()+"%";
  switch (humi_lstr.length()) {
    case 2:
      humi_lstr = String(humi_lstr) + String("  ");
      break;
    case 3:
      humi_lstr = String(humi_lstr) + String(" ");
      break;
  }
  TFDrawTextDMA(dma_display, humi_lstr, humi_x, humi_y, lcc);
}

void displayWeather() {
  long mm = timeClient.getMinutes();
  long ss = timeClient.getSeconds();
  if (ss == 15 && ((mm % OWM_refresh) == 0)) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis; 
      getWeatherjson(false, 1);
    }
  } 
  if (Weatherjson["main"]["temp"]) {
    // int wss = (ss % 20);
    // if (0 <= wss && wss <= 9) {
    //   displayWind();
    // }
    // if (10 <= wss && wss <= 19 ) {
    //   displayHumidity();
    // }
    displayWind();
    displayHumidity();
    displayTemp();
    displayConditions();
  }
}