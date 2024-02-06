#include <Arduino.h>
#include <Timezone.h>
#include <TimeLib.h>
#include "Config.h"
#include "Main.h"
#include "NTP.h"
#include "Display.h"
#include "Colors.h"
#include "TinyFont.h"
#include "Digit.h"
#include "Weather.h"

int prevss = 0;
int prevmm = 0;
int prevhh = 0;

byte Digitsec_x = 65;
byte Digit_x = 53;

Digit digit0(0, Digitsec_x - 6 * 1, 16, 3, 3, cc_time);
Digit digit1(0, Digitsec_x - 6 * 2, 16, 3, 3, cc_time);
Digit digit2(0, Digit_x - 9 * 1, 10, 6, 6, cc_time);
Digit digit3(0, Digit_x - 9 * 2, 10, 6, 6, cc_time);
Digit digit4(0, Digit_x - 3 - 9 * 3, 10, 6, 6, cc_time);
Digit digit5(0, Digit_x - 3 - 9 * 4, 10, 6, 6, cc_time);


void displayDate() {
  String lstr = "";

  // String D = (day(timeClient.getEpochTime()) < 10 ? " " + String(day(timeClient.getEpochTime())) : String(day(timeClient.getEpochTime())));
  // String M = (month(timeClient.getEpochTime()) < 10 ? " " + String(month(timeClient.getEpochTime())) : String(month(timeClient.getEpochTime())));
  // String Y = String(year(timeClient.getEpochTime()));

  String D = String(day(timeClient.getEpochTime()));
  String M = String(month(timeClient.getEpochTime()));
  String Y = String(year(timeClient.getEpochTime()));

  // for (int i = 0; i < 5; i += 2) {
  //   switch (config["DateFormat"].as<String>()[i]) {
  //     case 'D':
  //       lstr += (day(timeClient.getEpochTime()) < 10 ? " " + String(day(timeClient.getEpochTime())) : String(day(timeClient.getEpochTime())));
  //       if (i < 4)
  //         lstr += config["DateFormat"].as<String>()[i + 1];
  //       break;
  //     case 'M':  //Replaced leading 0 with space where double quotes are
  //       lstr += (month(timeClient.getEpochTime()) < 10 ? " " + String(month(timeClient.getEpochTime())) : String(month(timeClient.getEpochTime())));
  //       if (i < 4)
  //         lstr += config["DateFormat"].as<String>()[i + 1];
  //       break;
  //     case 'Y':
  //       lstr += String(year(timeClient.getEpochTime()));
  //       if (i < 4)
  //         lstr += config["DateFormat"].as<String>()[i + 1];
  //       break;
  //   }
  // }

  if (config["DateFormat"].as<String>() == "M.D.Y") {
    TFDrawTextDMA(dma_display, Y, 35, 24, cc_date);
    dma_display->drawPixel(34, 26, cc_date);
    if (day(timeClient.getEpochTime()) < 10 ) {
      TFDrawTextDMA(dma_display, D, 25 + TF_COLS, 24, cc_date);
      dma_display->drawPixel(24 + TF_COLS, 26, cc_date);
      if (month(timeClient.getEpochTime()) < 10) {
        TFDrawTextDMA(dma_display, M, 15 + TF_COLS + TF_COLS, 24, cc_date);
      }
      else {
        TFDrawTextDMA(dma_display, M, 15 + TF_COLS, 24, cc_date);
      }
    }
    else {
      TFDrawTextDMA(dma_display, D, 25, 24, cc_date);
      dma_display->drawPixel(24, 26, cc_date);
      if (month(timeClient.getEpochTime()) < 10) {
        TFDrawTextDMA(dma_display, M, 15 + TF_COLS, 24, cc_date);
      }
    }
  }
  else if (config["DateFormat"].as<String>() == "D.M.Y") {
    TFDrawTextDMA(dma_display, D, 15, 24, cc_date);
    dma_display->drawPixel(24, 26, cc_date);
    TFDrawTextDMA(dma_display, M, 25, 24, cc_date);
    dma_display->drawPixel(34, 26, cc_date);
    TFDrawTextDMA(dma_display, Y, 35, 24, cc_date);
  }

  String DayofWeek = "";
  switch (weekday(timeClient.getEpochTime())) {
    case 1:
      DayofWeek = "SUN";
      break;
    case 2:
      DayofWeek = "MON";
      break;
    case 3:
      DayofWeek = "TUE";
      break;
    case 4:
      DayofWeek = "WED";
      break;
    case 5:
      DayofWeek = "THR";
      break;
    case 6:
      DayofWeek = "FRI";
      break;
    case 7:
      DayofWeek = "SAT";
      break;
  }

  //lstr += String(DayofWeek);

  // if (lstr.length())
  //   TFDrawTextDMA(dma_display, lstr, 11, 24, cc_date);
  TFDrawTextDMA(dma_display, DayofWeek, 52, 24, cc_date);
}

int getTmePixel(long timestamp) {
  int pixel = ((timestamp%86400L) / 60) * ((float)64 / (float)1440);
  return pixel;
}

void displayDayProgessBar() {
  //day progress bar
  dma_display->drawFastHLine(0, 30, 63, cc_blk);
  dma_display->drawFastHLine(0, 30, getTmePixel(timeClient.getEpochTime()), cc_blu);
  if (Weatherjson["sys"]["sunrise"]) {
    //sunrise and sunset markers
    long tsunrise = long(Weatherjson["sys"]["sunrise"]) + long(Weatherjson["timezone"]);
    long tsunset = long(Weatherjson["sys"]["sunset"]) + long(Weatherjson["timezone"]);
    dma_display->drawPixel(getTmePixel(tsunrise), 30, cc_bblu); //cc_ylw
    dma_display->drawPixel(getTmePixel(tsunset), 30, cc_bblu); //cc_dgr
  }
}

int adjustPixel(int pixel) {
  if (pixel > 0 ) {
        pixel = 31 - pixel;
        if (pixel < 0) pixel = pixel + 64;
    }
    else {
        pixel = pixel * -1;
        pixel = 32 + pixel;
        if (pixel > 63) pixel = pixel - 64;
    }
    return pixel;
}

void displayDayLightProgessBar() {
  //Progress bar/line? with sun up and sun down markers
  if (Weatherjson["sys"]["sunrise"]) {
    dma_display->drawFastHLine(0, 31, 64, cc_blu);

    long tnow = long(timeClient.getEpochTime());
    long tsunrise = long(Weatherjson["sys"]["sunrise"]) + long(Weatherjson["timezone"]);
    long tsunset = long(Weatherjson["sys"]["sunset"]) + long(Weatherjson["timezone"]);

    int sunrisePixel = ((tnow - tsunrise) / 60) * ((float)64 / (float)1440);
    int sunsetPixel = ((tnow - tsunset) / 60) * ((float)64 / (float)1440);
    
    sunrisePixel = adjustPixel(sunrisePixel);
    sunsetPixel = adjustPixel(sunsetPixel);

    if (sunrisePixel < sunsetPixel)  {
      int length = sunsetPixel - sunrisePixel;
      dma_display->drawFastHLine(sunrisePixel, 31, length, cc_ylw);
    }
    else {
      int length = 64 - sunrisePixel;
      dma_display->drawFastHLine(0, 31, sunsetPixel, cc_ylw);
      dma_display->drawFastHLine(sunrisePixel, 31, length, cc_ylw);
    }

    dma_display->drawPixel(sunrisePixel, 31, cc_bylw); //cc_ylw
    dma_display->drawPixel(sunsetPixel, 31, cc_bblu); //cc_dgr
    dma_display->drawFastHLine(31, 31, 2, cc_wht);
  }
}

void displayClock() {
    int hh = timeClient.getHours();
    int mm = timeClient.getMinutes();
    int ss = timeClient.getSeconds();
    
    digit0.SetColor(cc_time);
    digit1.SetColor(cc_time);
    digit2.SetColor(cc_time);
    digit3.SetColor(cc_time);
    digit4.SetColor(cc_time);
    if (!config["Military"]) {
      if (hh > 12) {
        hh -= 12;
        TFDrawTextDMA(dma_display, String("PM"), 56, 17, cc_time);
      }
      else TFDrawTextDMA(dma_display, String("AM"), 56, 17, cc_time);
      if (hh == 0) hh += 12;
      if (hh < 10) digit5.SetColor(cc_blk);
      else digit5.SetColor(cc_time);
    }
    else digit5.SetColor(cc_time);

    displayDayProgessBar();
    displayDayLightProgessBar();

    if (clockStartingUp) { // If we didn't have a previous time. Just draw it without morphing.
      dma_display->clearScreen();
      digit0.Draw(ss % 10);
      digit1.Draw(ss / 10);
      digit2.Draw(mm % 10);
      digit3.Draw(mm / 10);
      digit4.Draw(hh % 10);
      digit5.Draw(hh / 10);
      digit3.DrawColon(cc_time);
      displayDate();
      clockStartingUp = false;
    }
    else {
      if (ss!=prevss) { 
        int s0 = ss % 10;
        int s1 = ss / 10;
        if (s0!=digit0.Value()) digit0.Morph(s0);
        if (s1!=digit1.Value()) digit1.Morph(s1);
        prevss = ss;
      }

      if (mm!=prevmm) {
        int m0 = mm % 10;
        int m1 = mm / 10;
        if (m0!=digit2.Value()) digit2.Morph(m0);
        if (m1!=digit3.Value()) digit3.Morph(m1);
        displayDate();
        prevmm = mm;
      }
      
      if (hh!=prevhh) {
        int h0 = hh % 10;
        int h1 = hh / 10;
        if (h0!=digit4.Value()) digit4.Morph(h0);
        if (h1!=digit5.Value()) digit5.Morph(h1);
        prevhh = hh;
      }
    }
}
