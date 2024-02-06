#include <Arduino.h>
#include <NTPClient.h>
#include <Timezone.h>
#include "Config.h"
#include "Main.h"
#include "TimezoneRules.h"
#include "Display.h"
//#include "Icons.h"

NTPClient timeClient(ntpUDP);

bool validateNTPServer(String NTPServer) {
  bool valid = false;
  timeClient.end();
  timeClient.setPoolServerName(NTPServer.c_str());
  timeClient.begin();
  if (timeClient.forceUpdate()) valid = true;
  else debugln("NTP: could not connect to " + NTPServer);
  return valid;
}

int setupTimeOffset(bool verbose) {
  int tzOffSet = 0;
  if (config["TimeZone"] == "EST") {
    if (config["DST"] && usET.utcIsDST(now()))
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
    return tzOffSet;
  }
  else if (config["TimeZone"] == "CST") {
    if (config["DST"] && usCT.utcIsDST(now()))
    {
      tzOffSet = (usCDT.offset)/60;
      if (verbose) debug(F("CDT:"));
      if (verbose) debugln(tzOffSet);
    }
    else
    {
      tzOffSet = (usCST.offset)/60;
      if (verbose) debug(F("CST:"));
      if (verbose) debugln(tzOffSet);
    }
    return tzOffSet;
  }
  else if (config["TimeZone"] == "CST") {
    if (config["DST"] && usCT.utcIsDST(now()))
    {
      tzOffSet = (usCDT.offset)/60;
      if (verbose) debug(F("CDT:"));
      if (verbose) debugln(tzOffSet);
    }
    else
    {
      tzOffSet = (usCST.offset)/60;
      if (verbose) debug(F("CST:"));
      if (verbose) debugln(tzOffSet);
    }
    return tzOffSet;
  }
  else if (config["TimeZone"] == "MST") {
    if (config["DST"] && usMT.utcIsDST(now()))
    {
      tzOffSet = (usMDT.offset)/60;
      if (verbose) debug(F("MDT:"));
      if (verbose) debugln(tzOffSet);
    }
    else
    {
      tzOffSet = (usMST.offset)/60;
      if (verbose) debug(F("MST:"));
      if (verbose) debugln(tzOffSet);
    }
    return tzOffSet;
  }
  else if (config["TimeZone"] == "PST") {
    if (config["DST"] && usPT.utcIsDST(now()))
    {
      tzOffSet = (usPDT.offset)/60;
      if (verbose) debug(F("PDT:"));
      if (verbose) debugln(tzOffSet);
    }
    else
    {
      tzOffSet = (usPST.offset)/60;
      if (verbose) debug(F("PST:"));
      if (verbose) debugln(tzOffSet);
    }
    return tzOffSet;
  }
  else if (config["TimeZone"] == "AST") {
    if (config["DST"] && usAT.utcIsDST(now()))
    {
      tzOffSet = (usADT.offset)/60;
      if (verbose) debug(F("ADT:"));
      if (verbose) debugln(tzOffSet);
    }
    else
    {
      tzOffSet = (usAST.offset)/60;
      if (verbose) debug(F("AST:"));
      if (verbose) debugln(tzOffSet);
    }
    return tzOffSet;
  }
  else if (config["TimeZone"] == "HAST") {
    if (config["DST"] && usHAT.utcIsDST(now()))
    {
      tzOffSet = (usHADT.offset)/60;
      if (verbose) debug(F("HADT:"));
      if (verbose) debugln(tzOffSet);
    }
    else
    {
      tzOffSet = (usHAST.offset)/60;
      if (verbose) debug(F("HAST:"));
      if (verbose) debugln(tzOffSet);
    }
    return tzOffSet;
  }
  else if (config["TimeZone"] == "CustomST") {
    //Future CustomST Rules from web UI
    return tzOffSet;
  }
  else {
    tzOffSet = atoi(config["TimeZone"]);
    return tzOffSet;
  }
  return tzOffSet;
}

void setupNTP(bool verbose) {
  timeClient.setTimeOffset((setupTimeOffset(verbose)) * 3600); //seconds 3600=1hour
  timeClient.setUpdateInterval(3600000); //milliseconds. 3600000=1hour
  timeClient.setPoolServerName(config["NTPServer"]);
  timeClient.begin();
  timeClient.forceUpdate();
  while (!timeClient.isTimeSet()) {
    if (verbose) debugln(F("NTP: Waiting for time"));
    if (verbose) dma_display->drawIcon(x_ico, 4 , 13, 5, 5);
    timeClient.forceUpdate();
  }
  if (verbose) dma_display->drawIcon(check_ico, 4 , 13, 5, 5);
  if (verbose) dma_display->drawIcon(check_ico, 4 , 19, 5, 5);
  return;
}
