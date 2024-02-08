#ifndef TIMEZONERULES_H
#define TIMEZONERULES_H

#include <Timezone.h>
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
// US Central Time Zone (Chicago, Houston)
TimeChangeRule usCDT = {"CDT", Second, Sun, Mar, 2, -300};
TimeChangeRule usCST = {"CST", First, Sun, Nov, 2, -360};
Timezone usCT(usCDT, usCST);
// US Mountain Time Zone (Denver, Salt Lake City)
TimeChangeRule usMDT = {"MDT", Second, Sun, Mar, 2, -360};
TimeChangeRule usMST = {"MST", First, Sun, Nov, 2, -420};
Timezone usMT(usMDT, usMST);
// Arizona is US Mountain Time Zone but does not use DST
Timezone usAZ(usMST);
// US Pacific Time Zone (Las Vegas, Los Angeles)
TimeChangeRule usPDT = {"PDT", Second, Sun, Mar, 2, -420};
TimeChangeRule usPST = {"PST", First, Sun, Nov, 2, -480};
Timezone usPT(usPDT, usPST);
// US Alaska Standard Time
TimeChangeRule usADT = {"ADT", Second, Sun, Mar, 2, -420};
TimeChangeRule usAST = {"AST", First, Sun, Nov, 2, -480};
Timezone usAT(usADT, usAST);
// US Hawaii-Aleutian Standard Time
TimeChangeRule usHADT = {"HADT", Second, Sun, Mar, 2, -420};
TimeChangeRule usHAST = {"HAST", First, Sun, Nov, 2, -480};
Timezone usHAT(usHADT, usHAST);

#endif